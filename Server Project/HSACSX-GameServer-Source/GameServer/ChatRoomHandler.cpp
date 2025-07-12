﻿#include "stdafx.h"

enum Chatroomopcodes
{
	CHATROOM_LIST = 0x04,
	CHATROOM_CREATE = 0x05,
	CHATROOM_JOIN = 0x06,
	CHATROOM_LEAVE = 0x07,
	CHATROOM_SEARCH = 0x08,
	CHATROOM_UPDATECONFIG = 0x09,
	CHATROOM_MEMBEROPTION = 0x0B,
	CHATROOM_ADMIN = 0x0C,

	CHATROOM_MANUEL = 0x0B
};

void CUser::ChatRoomHandle(Packet & pkt)
{
	uint8 OpCode = pkt.read<uint8>();
	if (OpCode != CHATROOM_MANUEL)
		return;

	uint8 SubOpCode = pkt.read<uint8>();
	switch (SubOpCode)
	{
	case CHATROOM_LIST:
		ChatRoomList(pkt);
		break;
	case CHATROOM_CREATE:
		ChatRoomCreate(pkt);
		break;
	case CHATROOM_JOIN:
		ChatRoomJoin(pkt);
		break;
	case CHATROOM_LEAVE:
		ChatRoomLeave(pkt);
		break;
	case CHATROOM_ADMIN:
		ChatRoomAdmin(pkt);
		break;
	case CHATROOM_MEMBEROPTION:
		ChatRoomMemberoption(pkt);
		break;
	default:
		TRACE("Chatroom Handle Unknow : %d\n", SubOpCode);
		printf("Chatroom Handle Unknow : %d\n", SubOpCode);
		break;
	}
}

void CUser::ChatRoomList(Packet & pkt)
{
	Packet result(WIZ_NATION_CHAT,uint8(0x0B));

	uint16 count = 0;
	result << uint8(CHATROOM_LIST) << count;
	result.DByte();

	g_pMain->m_ChatRoomArray.m_lock.lock();
	auto m_sChatRoomArray = g_pMain->m_ChatRoomArray.m_UserTypeMap;
	g_pMain->m_ChatRoomArray.m_lock.unlock();

	foreach(itr, m_sChatRoomArray)
	{
		_CHAT_ROOM * pChatRoom = itr->second;
		if (pChatRoom == nullptr)
			continue;

		result << uint16(++count) 
			<< pChatRoom->strRoomName
			<< uint8(pChatRoom->isPassword())
			<< pChatRoom->m_bRoomNation
			<< uint8(0)
			<< pChatRoom->m_sCurrentUser
			<< pChatRoom->m_sMaxUser;
	}

	result.put(2, count);
	Send(&result);
}

void CUser::ChatRoomCreate(Packet & pkt )
{
	if (m_ChatRoomIndex < 1)
		return;
	
	Packet result(WIZ_NATION_CHAT, uint8(CHATROOM_MEMBEROPTION));

	uint8 isPassword = 0, m_bResult = 0; 
	uint16 MaxAdmissions = 0;
	std::string strRoomName, strPassword;

	pkt.DByte();
	pkt >> strRoomName >> isPassword;

	if (isPassword == 0x01)
	{
		pkt.DByte();
		pkt >> strPassword;
	}

	pkt >> MaxAdmissions;

	if (MaxAdmissions > 200)
		goto return_fail;

	_CHAT_ROOM * m_pRoom = new _CHAT_ROOM();

	m_pRoom->strRoomName = strRoomName;
	m_pRoom->m_bRoomNation = GetNation();
	m_pRoom->strAdministrator = m_strUserID;
	m_pRoom->m_sMaxUser = MaxAdmissions;
	m_pRoom->strPassword = strPassword;
	m_pRoom->nIndex = g_pMain->m_ChatRoomArray.GetSize() + 1;
	m_pRoom->m_sCurrentUser = 0;

	if (!m_pRoom->AddUser(m_strUserID))
	{
		TRACE("Room creation fail 1\n");
		delete m_pRoom;
		goto return_fail;
	}

	{
		bool check = false;
		g_pMain->m_ChatRoomArray.m_lock.lock();
		auto m_sChatRoomArray = g_pMain->m_ChatRoomArray.m_UserTypeMap;
		g_pMain->m_ChatRoomArray.m_lock.unlock();

		foreach(itr, m_sChatRoomArray)
		{
			if (itr->second == nullptr
				|| itr->second->strRoomName == strRoomName)
			{
				check = true;
				break;
			}
		}

		if (!g_pMain->m_ChatRoomArray.PutData(m_pRoom->nIndex, m_pRoom) || check)
		{
			TRACE("Room creation fail 2\n");
			delete m_pRoom;
			goto return_fail;
		}
	}

	m_ChatRoomIndex = m_pRoom->nIndex;
	result << uint8(0x05) << uint8(1) << uint16(m_pRoom->nIndex) << m_pRoom->strRoomName << uint8(isMeChatRoom(m_pRoom->nIndex)) << m_pRoom->m_sCurrentUser << m_pRoom->m_sMaxUser << uint32(0);
	Send(&result);
	return;
return_fail:
	result << uint8(0x05) << uint8(0);
	Send(&result);
}

void CUser::ChatRoomJoin(Packet & pkt)
{
	/*Result opcedes 
	 * 0 = Successfully
	 * 1 = Already in room
	 * 2 = Room does not exit ! 
	 * 3 = Password lenght is correct
	 * 4 = Password does not match
	 * 5 = Nation do not match */

	uint16 RoomID;
	uint8 isPassword, nResult = 0;
	std::string strPassword;

	pkt >> RoomID >> isPassword >> strPassword;

	if (isMeChatRoom(RoomID))
		nResult = 1;

	_CHAT_ROOM * pRoom = g_pMain->m_ChatRoomArray.GetData(RoomID);
	if (pRoom == nullptr || pRoom->m_sCurrentUser + 1 > pRoom->m_sMaxUser)
		nResult = 0x02;
	else if (pRoom->isPassword() && STRCASECMP(strPassword.c_str(), pRoom->strPassword.c_str()) != 0)
		nResult = 0x04;
	else if (!pRoom->AddUser(GetName()))
		nResult = 0x02;
	else if (pRoom->m_bRoomNation != GetNation())
		nResult = 0x05;

	Packet result(WIZ_NATION_CHAT, uint8(CHATROOM_MANUEL));
	result << uint8(CHATROOM_JOIN) << nResult << RoomID;

	Send(&result);

	if (nResult == 0)
		m_ChatRoomIndex = pRoom->nIndex;
}                    

void CUser::ChatRoomLeave(Packet & pkt)
{
	uint16 RoomID = pkt.read<uint16>();
	_CHAT_ROOM * pRoom = g_pMain->m_ChatRoomArray.GetData(RoomID);
	if (pRoom == nullptr)
		return;

	// Kullanıcıları odadan atma 

	if (pRoom->isAdministrator(GetName()) == 2)
		g_pMain->m_ChatRoomArray.DeleteData(RoomID);

	Packet result(WIZ_NATION_CHAT, uint8(CHATROOM_MANUEL));
	result << uint8(CHATROOM_LEAVE) << uint8(0);
	Send(&result);

	pRoom->m_sCurrentUser--;
	m_ChatRoomIndex = -1;
}

void CUser::ChatRoomChat(std::string * strMessage, std::string strSender)
{
	CUser * pUser = g_pMain->GetUserPtr(strSender, NameType::TYPE_CHARACTER);
	if (pUser == nullptr || !pUser->isInGame())
		return;

	Packet result(WIZ_CHAT, uint8(ChatType::CHATROM_CHAT));
	result.DByte();
	result << uint8(0) << (int32)pUser->GetSocketID() << pUser->GetName() << *strMessage << pUser->GetZoneID();
	//10 21 00 0701 0B00 7370656369616C6C697374 0600 74656B726172 15

	SendChatRoom(result);
}

void CUser::ChatRoomAdmin(Packet & pkt)
{
	//19 0B 0C  00 0300 0500 726573756C 1802 0A00 0100 1802 0B00 4D617844616D616765336B

	Packet result(WIZ_NATION_CHAT,uint8(CHATROOM_MANUEL));
		
	uint16 count = 0 , subpkt = pkt.read<uint8>();

	result << uint8(CHATROOM_ADMIN) << uint8(0) << subpkt;

	_CHAT_ROOM * pRoom = g_pMain->m_ChatRoomArray.GetData(m_ChatRoomIndex);
	if (pRoom == nullptr)
		return;

	result.DByte();

	result << pRoom->strRoomName << uint8(0x18) << uint8(0x02) << pRoom->m_sMaxUser << pRoom->m_sCurrentUser;

	foreach(itr, pRoom->m_UserList)
	{
		CUser * pSendUser = g_pMain->GetUserPtr(itr->second, NameType::TYPE_CHARACTER);
		if (pSendUser == nullptr)
			continue;

		result << uint8(0x18) << uint8(pRoom->isAdministrator(pSendUser->GetName())) << pSendUser->GetName();		
	}
	Send(&result);
}

void CUser::ChatRoomMemberoption(Packet & pkt)
{
	uint8 subPkt = pkt.read<uint8>();
	if (subPkt == 1)
		ChatRoomKickOut(pkt.read<uint16>());
}

void CUser::ChatRoomKickOut(uint16 userID)
{
	_CHAT_ROOM * pRoom = g_pMain->m_ChatRoomArray.GetData(m_ChatRoomIndex);
	if (pRoom == nullptr)
		return;

	Packet result(WIZ_NATION_CHAT, uint8(CHATROOM_MANUEL));

	result << uint8(CHATROOM_MEMBEROPTION);

	foreach(itr, pRoom->m_UserList)
	{
		if (itr->first == userID)
		{
			result << uint8(2);
			Send(&result);
			return;
		}
	}

	pRoom->m_UserList.erase(userID);
	pRoom->m_sCurrentUser--;
	m_ChatRoomIndex = -1;

	result << uint8(1) << userID;
	SendChatRoom(result);
}

void CUser::SendChatRoom(Packet & result)
{
	_CHAT_ROOM * pRoom = g_pMain->m_ChatRoomArray.GetData(m_ChatRoomIndex);
	if (pRoom == nullptr)
		return;

	foreach(itr, pRoom->m_UserList)
	{
		CUser * pSendUser = g_pMain->GetUserPtr(itr->second, NameType::TYPE_CHARACTER);
		if (pSendUser == nullptr || !pSendUser->isInGame())
			continue;

		pSendUser->Send(&result);
	}
}