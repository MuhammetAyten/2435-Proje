#include "stdafx.h"
#include "DBAgent.h"
#include "KnightsManager.h"

using std::string;
extern CDBAgent g_DBAgent;

#pragma region CUser::SendNameChange()
void CUser::SendNameChange()
{
	if (!isInGame()
		|| isDead()
		|| isTrading()
		|| isMerchanting()
		|| isStoreOpen()
		|| isSellingMerchant()
		|| isBuyingMerchant()
		|| isMining()
		|| isFishing())
		return;

	Packet result(WIZ_NAME_CHANGE, uint8(NameChangeShowDialog));
	Send(&result);
}
#pragma endregion

#pragma region CUser::SendClanNameChange()
void CUser::SendClanNameChange()
{
	if (!isInGame()
		|| isDead()
		|| isTrading()
		|| isMerchanting()
		|| isStoreOpen()
		|| isSellingMerchant()
		|| isBuyingMerchant()
		|| isMining()
		|| isFishing())
		return;

	Packet result(WIZ_NAME_CHANGE, uint8(ClanNameChange));
	result << uint8(ShowDialog);
	Send(&result);
}
#pragma endregion

#pragma region CUser::HandlePlayerClanNameChange(Packet & pkt)
void CUser::HandleSelectCharacterNameChange(Packet & pkt)
{
	Packet result(WIZ_NAME_CHANGE, uint8(CharSelectNameChange));
	result << uint16(2) << uint8(16);
	Send(&result);
}
#pragma endregion

#pragma region CUser::HandleNameChange(Packet & pkt)
void CUser::HandleNameChange(Packet & pkt)
{
	uint8 opcode;
	pkt >> opcode;

	if (!isInGame()
		|| isDead()
		|| isTrading()
		|| isMerchanting()
		|| isStoreOpen()
		|| isSellingMerchant()
		|| isBuyingMerchant()
		|| isMining()
		|| isFishing())
		return;

	switch (opcode)
	{
	case CharNameChange:
		HandlePlayerNameChange(pkt);
		break;
	case CharSelectNameChange:
		HandleSelectCharacterNameChange(pkt);
		break;
	case ClanNameChange:
		HandlePlayerClanNameChange(pkt);
		break;
	default:
		printf("NameChange Unhandled packets opcode (%u) \n", opcode);
		TRACE("NameChange Unhandled packets opcode (%u) \n", opcode);
		break;
	}
}
#pragma endregion

#pragma region CUser::HandlePlayerClanNameChange(Packet & pkt)
void CUser::HandlePlayerClanNameChange(Packet & pkt)
{
	Packet result(WIZ_NAME_CHANGE, uint8(ClanNameChange));
	string strClanID;
	pkt >> strClanID;

	// Ensure we have the scroll before handling this request.
	if (!CheckExistItem(ITEM_CLAN_NAME_SCROLL))
	{
		result << uint8(ShowDialog);
		Send(&result);
		return;
	}

	if (strClanID.empty()
		|| strClanID.length() > MAX_ID_SIZE)
	{
		result << uint8(InvalidName);
		Send(&result);
		return;
	}

	CKnights * pKnights = g_pMain->GetClanPtr(GetClanID());
	if (pKnights == nullptr
		|| !isClanLeader()
		|| !isInClan())
	{
		result << uint8(NotClan);
		Send(&result);
		return;
	}

	g_pMain->m_KnightsArray.m_lock.lock();
	auto m_sKnightsArray = g_pMain->m_KnightsArray.m_UserTypeMap;
	g_pMain->m_KnightsArray.m_lock.unlock();

	foreach(itr, m_sKnightsArray)
	{
		CKnights * pKnights = itr->second;
		if (pKnights == nullptr)
			continue;

		if (strClanID == pKnights->GetName())
		{
			result << uint8(InvalidName);
			Send(&result);
			return;
		}
	}

	result << strClanID;
	g_pMain->AddDatabaseRequest(result, this);
}
#pragma endregion


#pragma region CUser::HandlePlayerNameChange(Packet & pkt)
void CUser::HandlePlayerNameChange(Packet & pkt)
{
	Packet result(WIZ_NAME_CHANGE);
	string strUserID;
	pkt >> strUserID;

	// Ensure we have the scroll before handling this request.
	if (!CheckExistItem(ITEM_SCROLL_OF_IDENTITY) && !isGM() && !isGMUser())
	{
		result << uint8(NameChangeShowDialog);
		Send(&result);
		return;
	}

	if (strUserID.empty()
		|| strUserID.length() > MAX_ID_SIZE)
	{
		result << uint8(NameChangeInvalidName);
		Send(&result);
		return;
	}

	if (isKing())
	{
		result << uint8(NameChangeKing);
		Send(&result);
		return;
	}

	/*if (isInClan()) // NameChange Basarken Clan Var Ise Basm�yordu Kald�r�ld� 07.05.2020
	{
		result << uint8(NameChangeInClan);
		Send(&result);
		return;
	}*/

	result << uint8(CharNameChange) << strUserID;
	g_pMain->AddDatabaseRequest(result, this);
}
#pragma endregion


#pragma region CUser::NameChangeSystem(Packet & pkt)
void CUser::NameChangeSystem(Packet & pkt)
{
	Packet result(WIZ_NAME_CHANGE);
	uint8 opcode;
	string strName, strClanName, oldname = GetName();;

	pkt >> opcode;

	switch (opcode)
	{
	case CharNameChange:
	{
		pkt >> strName;
		uint8 bResult = 0;

		bResult = g_DBAgent.UpdateCharacterName(GetAccountName(), GetName(), strName);
		if (bResult == 3)
		{
			std::string userid = GetName();
			std::string useridcopy = strName;

			STRTOUPPER(userid);
			STRTOUPPER(useridcopy);

			if (isInClan()) {
				CKnights* pKnights = g_pMain->GetClanPtr(GetClanID());
				if (pKnights != nullptr) {
					_KNIGHTS_USER * pKnightUser = pKnights->m_arKnightsUser.GetData(userid);
					if (pKnightUser != nullptr) {
						_KNIGHTS_USER * pKnightUserCopy = new _KNIGHTS_USER();
						pKnightUserCopy->strUserName = strName;
						pKnightUserCopy->bLevel = pKnightUser->bLevel;
						pKnightUserCopy->sClass = pKnightUser->sClass;
						pKnightUserCopy->nLastLogin = pKnightUser->nLastLogin;
						pKnightUserCopy->bFame = pKnightUser->bFame;
						pKnightUserCopy->nLoyalty = pKnightUser->nLoyalty;
						pKnightUserCopy->LoyaltyMonthly = pKnightUser->LoyaltyMonthly;
						pKnightUserCopy->nDonatedNP = pKnightUser->nDonatedNP;
						pKnightUserCopy->strMemo = pKnightUser->strMemo;
						m_pKnightsUser = pKnightUserCopy;

						if (pKnights->m_arKnightsUser.PutData(useridcopy, pKnightUserCopy))
							pKnights->m_arKnightsUser.DeleteData(userid);
					}
				}
			}

			auto* pDaily = g_pMain->m_DailyRank.GetData(userid);
			if (pDaily != nullptr) {
				_DAILY_RANK* pDailyUserCopy = new _DAILY_RANK();
				pDailyUserCopy->strUserID = strName;
				for (int i = 0; i < 3; i++) {
					pDailyUserCopy->GmRank[i] = pDaily->GmRank[i];
					pDailyUserCopy->MhRank[i] = pDaily->MhRank[i];
					pDailyUserCopy->ShRank[i] = pDaily->ShRank[i];
					pDailyUserCopy->AkRank[i] = pDaily->AkRank[i];
					pDailyUserCopy->CwRank[i] = pDaily->CwRank[i];
					pDailyUserCopy->UpRank[i] = pDaily->UpRank[i];
				}
				pDailyUserCopy->GMTotalSold = pDaily->GMTotalSold;
				pDailyUserCopy->MHTotalKill = pDaily->MHTotalKill;
				pDailyUserCopy->SHTotalExchange = pDaily->SHTotalExchange;
				pDailyUserCopy->AKLoyaltyMonthly = pDaily->AKLoyaltyMonthly;
				pDailyUserCopy->CWCounterWin = pDaily->CWCounterWin;
				pDailyUserCopy->UPCounterBles = pDaily->UPCounterBles;
				g_pMain->m_DailyRank.DeleteData(GetName());
				if (!g_pMain->m_DailyRank.PutData(useridcopy, pDailyUserCopy)) delete pDailyUserCopy;
			}

			// Replace the character's name (in both the session and the character lookup hashmap).
			g_pMain->ReplaceCharacterName(this, strName);

			if (isInClan() && m_pKnightsUser != nullptr)
				m_pKnightsUser->strUserName = strName;

			// Remove user from others' view & make them reappear again so 
			// the name can be updated for those currently in range.
			UserInOut(INOUT_OUT);
			UserInOut(INOUT_IN);

			result.DByte();
			result << uint8(NameChangeSuccess) << strName;
			Send(&result);

			// Take the scroll...
			RobItem(ITEM_SCROLL_OF_IDENTITY);

			Packet bottom(WIZ_USER_INFORMATIN, uint8(BottomUserListOpcode::RegionDelete));
			bottom.SByte(); bottom << oldname;
			SendToRegion(&bottom, nullptr, GetEventRoom());

			UserNameChangeInsertLog(oldname, strName);
		}
		else
		{
			result << uint8(bResult);
			Send(&result);
			return;
		}
	}
	case CharSelectNameChange:
		break;
	case ClanNameChange:
	{
		pkt >> strClanName;

		CKnights * pKnights = g_pMain->GetClanPtr(GetClanID());
		if (pKnights == nullptr)
		{
			result << uint8(ClanNameChange) << uint8(NotClan);
			Send(&result);
			return;
		}

		std::string oldClanName = pKnights->m_strName;

		uint8 bResult = g_DBAgent.UpdateCharacterClanName(pKnights->GetName(), strClanName, this);
		if (bResult == 3)
		{
			pKnights->m_strName = strClanName;
			// Remove user from others' view & make them reappear again so 
			// the Clan Name can be updated for those currently in range.
			UserInOut(INOUT_OUT);
			UserInOut(INOUT_IN);

			result.DByte();
			result << uint8(ClanNameChange) << uint8(Succes) << strClanName;
			Send(&result);

			g_pMain->Send_KnightsMember(pKnights->GetID(), &result);

			// Take the scroll...
			RobItem(ITEM_CLAN_NAME_SCROLL);
			ClanNameChangeInsertLog(oldClanName, strClanName);
		}
		else
		{
			result << uint8(ClanNameChange) << uint8(bResult);
			Send(&result);
		}
	}
	break;
	default:
		printf("NameChangeSystem Unhandled opcode packets (%x) \n", opcode);
		TRACE("NameChangeSystem Unhandled opcode packets (%x) \n", opcode);
		break;
	}
}
#pragma endregion
