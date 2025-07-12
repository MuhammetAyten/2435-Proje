﻿#include "stdafx.h"
#include "DBAgent.h"

#pragma region CUser::ClanWarehouseProcess(Packet & pkt)

void CUser::ClanWareHouseProcess(Packet& pkt)
{
	if (pCindWar.isEventUser()
		&& g_pMain->isCindirellaZone(GetZoneID()))
		return;

	auto OpCode = pkt.read<uint8_t>();
	if (isInGenie())
	{
		if (OpCode == ClanBankOpen)
			return WarehouseSendError(WAREHOUSE_OPEN, WarehouseError::NoAccess);
		else
			return WarehouseSendError(OpCode, WarehouseError::NoAccess);
	}

	switch (OpCode)
	{
	case ClanBankOpcodes::ClanBankOpen:
		ClanWarehouseOpen(pkt);
		break;
	case ClanBankOpcodes::ClanBankInput:
		ClanWarehouseItemInput(pkt);
		break;
	case ClanBankOpcodes::ClanBankOutPut:
		ClanWarehouseItemOutput(pkt);
		break;
	case ClanBankOpcodes::ClanBankMove:
		ClanWarehouseItemMove(pkt);
		break;
	case ClanBankOpcodes::ClanBankInventoryMove:
		ClanWarehouseInventoryItemMove(pkt);
		break;
	}
}

void CUser::ClanWarehouseOpen(Packet& pkt)
{
	Packet result(WIZ_HSACS_CLAN);
	result << uint8_t(ClanBankOpcodes::ClanBankOpen);

	uint8_t ReturnValue = 1;

	if (!isInGame()
		|| isDead()
		|| isTrading()
		|| isMerchanting()
		|| isMining()
		|| isFishing()
		|| isInAutoClan())
	{
		ReturnValue = 0;
		goto fail_return;
	}

	uint16_t sNpcID;
	pkt >> sNpcID;
	{
		if (g_pMain->pServerSetting.ClanBankWithPremium)
		{
			if (m_bIsLoggingOut 
				|| !isInClan() 
				|| !sClanPremStatus
				|| !g_pMain->ClanBankStatus)
			{
				printf("HSACS - Dominik  | 1: %s: m_bIsLoggingOut=%d,sClanPremStatus=%d,g_pMain->ClanBankStatus=%d\n", __FUNCTION__, m_bIsLoggingOut, sClanPremStatus, g_pMain->ClanBankStatus);
				ReturnValue = 0;
				goto fail_return;
			}
		}
		else
		{
			if (m_bIsLoggingOut 
				|| !isInClan() 
				|| !g_pMain->ClanBankStatus)
			{
				printf("HSACS - Dominik  | 2: %s: m_bIsLoggingOut=%d,sClanPremStatus=%d,g_pMain->ClanBankStatus=%d\n", __FUNCTION__, m_bIsLoggingOut, sClanPremStatus, g_pMain->ClanBankStatus);
				ReturnValue = 0;
				goto fail_return;
			}
		}
	}

	auto* pKnights = g_pMain->GetClanPtr(GetClanID());
	if (pKnights == nullptr)
	{
		g_pMain->SendHelpDescription(this, "Clan Bank : Couldn't Identify Clan.");
		ReturnValue = 0;
		goto fail_return;
	}

	result << ReturnValue << pKnights->GetClanInnCoins();

	for (int32_t i = 0; i < WAREHOUSE_MAX; i++)
	{
		auto* pItem = &pKnights->m_sClanWarehouseArray[i];
		if (pItem == nullptr)
			continue;

		if (pItem->nExpirationTime != 0 && (pItem->nExpirationTime < (uint32_t)UNIXTIME))
			memset(pItem, 0, sizeof(_ITEM_DATA));

		result << pItem->nNum << pItem->sDuration << pItem->sCount << pItem->bFlag << uint32_t(0x00) << pItem->nExpirationTime;
	}

	Send(&result);
	g_pMain->SendHelpDescription(this, "Clan Bank : Your clan bank is opened.");
	return;

fail_return:
	result << ReturnValue;
	Send(&result);
}

void CUser::ClanWarehouseItemInput(Packet& pkt)
{
	_ITEM_TABLE pTable = _ITEM_TABLE();
	Packet result(WIZ_HSACS_CLAN);
	result << uint8_t(ClanBankOpcodes::ClanBankInput);

	Packet DBSave(WIZ_DB_SAVE_USER, uint8(ProcDbType::ClanBankSave));
	uint8_t ReturnValue = 1;

	if (!isInGame() 
		|| isDead() 
		|| isTrading() 
		|| isStoreOpen() 
		|| isMerchanting() 
		|| isMining() 
		|| isFishing())
	{
		ReturnValue = 0;
		goto fail_return;
	}

	uint16_t sNpcID;
	uint32_t nItemID, nCount;
	uint8_t Page, bSrcPos, bDstPos;

	pkt >> sNpcID;
	{
		if (g_pMain->pServerSetting.ClanBankWithPremium)
		{
			if (m_bIsLoggingOut
				|| !isInClan() 
				|| !sClanPremStatus 
				|| !g_pMain->ClanBankStatus)
			{
				if (m_bIsLoggingOut)
					printf("HSACS - Dominik  | cb_error input:0-1\n");
				else if (!isInClan())
					printf("HSACS - Dominik  | cb_error input:0-2\n");
				else if (!sClanPremStatus)
					printf("HSACS - Dominik  | cb_error input:0-3\n");
				else if (!g_pMain->ClanBankStatus)
					printf("HSACS - Dominik  | cb_error input:0-4\n");

				ReturnValue = 0;
				goto fail_return;
			}
		}
		else
		{
			if (m_bIsLoggingOut || !isInClan() || !g_pMain->ClanBankStatus)
			{
				ReturnValue = 0;
				goto fail_return;
			}
		}
	}

	auto* pKnights = g_pMain->GetClanPtr(GetClanID());
	if (pKnights == nullptr)
	{
		g_pMain->SendHelpDescription(this, "Clan Bank : Couldn't Identify Clan.");
		ReturnValue = 0;
		goto fail_return;
	}

	pkt >> nItemID >> Page >> bSrcPos >> bDstPos >> nCount;
	pTable = g_pMain->GetItemPtr(nItemID);
	if (pTable.isnull())
	{
		ReturnValue = 0;
		goto fail_return;
	}

	if (nItemID == ITEM_GOLD)
	{
		if (!hasCoins(nCount) || pKnights->GetClanInnCoins() + nCount > COIN_MAX)
		{
			ReturnValue = 0;
			goto fail_return;
		}

		pKnights->m_nMoney += nCount;
		m_iGold -= nCount;

		result << ReturnValue;
		Send(&result);

		ClanBankInsertLog(pKnights, 0, 0, nCount, true);
		return;
	}

	uint16_t reference_pos = 24 * Page;

	if (bSrcPos > HAVE_MAX
		|| reference_pos + bDstPos > WAREHOUSE_MAX
		|| (nItemID >= ITEM_NO_TRADE_MIN && nItemID <= ITEM_NO_TRADE_MAX))
	{
		ReturnValue = 0;
		goto fail_return;
	}

	auto* pSrcItem = GetItem(SLOT_MAX + bSrcPos);
	if (pSrcItem == nullptr || pSrcItem->nNum != nItemID || pSrcItem->sCount < nCount || pSrcItem->isRented() || pSrcItem->isExpirationTime() || pSrcItem->isSealed()) // Clan bankasına Süreli item veya kilitli item atması engellendi
	{
		ReturnValue = 0;
		goto fail_return;
	}

	if (pSrcItem->isDuplicate())
	{
		ReturnValue = 2;
		goto fail_return;
	}

	auto* pDstItem = &pKnights->m_sClanWarehouseArray[reference_pos + bDstPos];
	if (pDstItem == nullptr)
	{
		ReturnValue = 0;
		goto fail_return;
	}

	if ((!pTable.isStackable() && pDstItem->nNum != 0)
		|| (pTable.isStackable() && pDstItem->nNum != 0 && pDstItem->nNum != pSrcItem->nNum)
		|| pSrcItem->sCount < nCount)
	{
		ReturnValue = 0;
		goto fail_return;
	}

	if (pTable.isStackable())
		pDstItem->sCount += (uint16)nCount;
	else
		pDstItem->sCount = (uint16)nCount;

	if (pTable.isStackable())
		pSrcItem->sCount -= nCount;
	else
		pSrcItem->sCount = 0;

	uint64 serial = pSrcItem->nSerialNum;
	if (!serial) serial = g_pMain->GenerateItemSerial();

	if (pTable.isStackable())
	{
		if (!pSrcItem->sCount && !pDstItem->nNum)
			pDstItem->nSerialNum = serial;
		else if (!pDstItem->nNum)
			pDstItem->nSerialNum = g_pMain->GenerateItemSerial();
	}
	else pDstItem->nSerialNum = serial;

	pDstItem->sDuration = pSrcItem->sDuration;
	pDstItem->bFlag = pSrcItem->bFlag;
	pDstItem->sRemainingRentalTime = pSrcItem->sRemainingRentalTime;
	pDstItem->nExpirationTime = pSrcItem->nExpirationTime;
	pDstItem->nNum = pSrcItem->nNum;

	if (pDstItem->sCount > MAX_ITEM_COUNT)
		pDstItem->sCount = MAX_ITEM_COUNT;

	if ((!pSrcItem->sCount) || (!pTable.m_bCountable) || (pTable.m_bKind == 255 && !pTable.m_bCountable))
		memset(pSrcItem, 0, sizeof(_ITEM_DATA));

	SetUserAbility(false);
	SendItemWeight();

#if(GAME_SOURCE_VERSION == 2369)
	HandleHShieldSoftwareNewItemMoveLoadderHandler();
#endif

	result << ReturnValue;
	Send(&result);

	if (pDstItem != nullptr)
		ClanBankInsertLog(pKnights, nItemID, nCount, 0, true);

	DBSave << pKnights->GetName();
	g_pMain->AddDatabaseRequest(DBSave, this);

	{
		if (g_pMain->pServerSetting.ClanBankWithPremium)
		{
			if (sClanPremStatus)
			{
				if (pKnights->GetID() == GetClanID())
				{
					Packet ClanNotices;
					std::string buffer;

					if (pDstItem->sCount > 1)
						buffer = string_format("### %s Clan Bankasına %d adet %s bıraktı. ###", GetName().c_str(), pDstItem->sCount, pTable.m_sName.c_str()); // is stored 
					else
						buffer = string_format("### %s Clan Bankasına %s Bıraktı. ###", GetName().c_str(), pTable.m_sName.c_str()); // is stored 

					ChatPacket::Construct(&ClanNotices, (uint8)ChatType::GM_CHAT, &buffer);
					pKnights->Send(&ClanNotices);
				}
				else
					goDisconnect("1", __FUNCTION__);
			}
		}
		else
		{
			if (pKnights->GetID() == GetClanID())
			{
				Packet ClanNotices;
				std::string buffer;

				if (pDstItem->sCount > 1)
					buffer = string_format("### %s Clan Bankasina %d adet %s birakti. ###", GetName().c_str(), pDstItem->sCount, pTable.m_sName.c_str()); // is stored 
				else
					buffer = string_format("### %s Clan Bankasına %s Birakti. ###", GetName().c_str(), pTable.m_sName.c_str()); // is stored 

				ChatPacket::Construct(&ClanNotices, (uint8)ChatType::GM_CHAT, &buffer);
				pKnights->Send(&ClanNotices);
			}
			else
				goDisconnect("2", __FUNCTION__);
		}
	}
	return;

fail_return:
	result << ReturnValue;
	Send(&result);
}

void CUser::ClanWarehouseItemOutput(Packet& pkt)
{
	_ITEM_TABLE pTable = _ITEM_TABLE();
	Packet result(WIZ_HSACS_CLAN);
	result << uint8_t(ClanBankOpcodes::ClanBankOutPut);

	Packet DBSave(WIZ_DB_SAVE_USER, uint8(ProcDbType::ClanBankSave));
	uint8_t ReturnValue = 1;

	if (!isInGame() || isDead() || isTrading() || isStoreOpen() || isMerchanting() || isMining() || isFishing())
	{
		ReturnValue = 0;
		goto fail_return;
	}

	uint16_t sNpcID;
	uint32_t nItemID, nCount;
	uint8_t Page, bSrcPos, bDstPos;

	pkt >> sNpcID;
	{
		if (g_pMain->pServerSetting.ClanBankWithPremium)
		{
			if (m_bIsLoggingOut || !isInClan() || !sClanPremStatus || !g_pMain->ClanBankStatus || (!isClanLeader() && !isClanAssistant()))
			{
				if (m_bIsLoggingOut)
					printf("HSACS - Dominik  | cb_error output:1-1\n");
				else if (!isInClan())
					printf("HSACS - Dominik  | cb_error output:1-2\n");
				else if (!sClanPremStatus)
					printf("HSACS - Dominik  | cb_error output:1-3\n");
				else if (!g_pMain->ClanBankStatus)
					printf("HSACS - Dominik  | cb_error output:1-4\n");
				else if (!isClanLeader() && !isClanAssistant())
					printf("HSACS - Dominik  | cb_error output:1-5\n");

				ReturnValue = 0;
				goto fail_return;
			}
		}
		else
		{
			if (m_bIsLoggingOut || !isInClan() || !g_pMain->ClanBankStatus || (!isClanLeader() && !isClanAssistant()))
			{
				ReturnValue = 0;
				goto fail_return;
			}
		}
	}

	auto* pKnights = g_pMain->GetClanPtr(GetClanID());
	if (pKnights == nullptr)
	{
		g_pMain->SendHelpDescription(this, "Clan Bank : Couldn't Identify Clan.");
		ReturnValue = 0;
		goto fail_return;
	}

	pkt >> nItemID >> Page >> bSrcPos >> bDstPos >> nCount;
	pTable = g_pMain->GetItemPtr(nItemID);
	if (pTable.isnull())
	{
		ReturnValue = 0;
		goto fail_return;
	}

	if (nItemID == ITEM_GOLD)
	{
		if (!pKnights->hasClanInnCoins(nCount) || GetCoins() + nCount > COIN_MAX)
		{
			ReturnValue = 0;
			goto fail_return;
		}

		m_iGold += nCount;
		pKnights->m_nMoney -= nCount;

		result << ReturnValue;
		Send(&result);

		ClanBankInsertLog(pKnights, 0, 0, nCount, false);
		return;
	}

	if (pTable.m_bCountable)
	{
		if (((pTable.m_sWeight * nCount) + m_sItemWeight) > m_sMaxWeight)
		{
			ReturnValue = 3;
			goto fail_return;
		}
	}
	else
	{
		if ((pTable.m_sWeight + m_sItemWeight) > m_sMaxWeight)
		{
			ReturnValue = 3;
			goto fail_return;
		}
	}

	uint16_t reference_pos = 24 * Page;

	if (reference_pos + bSrcPos > WAREHOUSE_MAX || bDstPos > HAVE_MAX)
	{
		ReturnValue = 0;
		goto fail_return;
	}

	auto* pSrcItem = &pKnights->m_sClanWarehouseArray[reference_pos + bSrcPos];
	if (pSrcItem == nullptr || pSrcItem->nNum != nItemID || pSrcItem->sCount < nCount)
	{
		ReturnValue = 0;
		goto fail_return;
	}

	if (pSrcItem->isDuplicate())
	{
		ReturnValue = 2;
		goto fail_return;
	}

	auto* pDstItem = GetItem(SLOT_MAX + bDstPos);
	if (pDstItem == nullptr)
	{
		ReturnValue = 0;
		goto fail_return;
	}

	if ((!pTable.isStackable() && pDstItem->nNum != 0)
		|| (pTable.isStackable() && pDstItem->nNum != 0 && pDstItem->nNum != pSrcItem->nNum)
		|| pSrcItem->sCount < nCount)
	{
		ReturnValue = 0;
		goto fail_return;
	}

	if (pTable.isStackable())
		pDstItem->sCount += (uint16)nCount;
	else
		pDstItem->sCount = (uint16)nCount;

	if (pTable.isStackable())
		pSrcItem->sCount -= nCount;
	else
		pSrcItem->sCount = 0;

	uint64 serial = pSrcItem->nSerialNum;
	if (!serial) serial = g_pMain->GenerateItemSerial();

	if (pTable.isStackable())
	{
		if (!pSrcItem->sCount && !pDstItem->nNum)
			pDstItem->nSerialNum = serial;
		else if (!pDstItem->nNum)
			pDstItem->nSerialNum = g_pMain->GenerateItemSerial();
	}
	else pDstItem->nSerialNum = serial;

	pDstItem->sDuration = pSrcItem->sDuration;
	pDstItem->bFlag = pSrcItem->bFlag;
	pDstItem->sRemainingRentalTime = pSrcItem->sRemainingRentalTime;
	pDstItem->nExpirationTime = pSrcItem->nExpirationTime;
	pDstItem->nNum = pSrcItem->nNum;

	if (pDstItem->sCount > MAX_ITEM_COUNT)
		pDstItem->sCount = MAX_ITEM_COUNT;

	if (!pSrcItem->sCount || !pTable.m_bCountable || (pTable.m_bKind == 255 && !pTable.m_bCountable))
		memset(pSrcItem, 0, sizeof(_ITEM_DATA));

	SetUserAbility(false);
	SendItemWeight();

#if(GAME_SOURCE_VERSION == 2369)
	HandleHShieldSoftwareNewItemMoveLoadderHandler();
#endif

	result << ReturnValue;
	Send(&result);

	if (pDstItem != nullptr)
		ClanBankInsertLog(pKnights, nItemID, nCount, 0, false);

	DBSave << pKnights->GetName();
	g_pMain->AddDatabaseRequest(DBSave, this);

	{
		if (g_pMain->pServerSetting.ClanBankWithPremium)
		{
			if (sClanPremStatus)
			{
				if (pKnights->GetID() == GetClanID())
				{
					Packet ClanNotices;
					std::string buffer;

					if (pDstItem->sCount > 1)
						buffer = string_format("### %s Clan Bankasından %d adet %s aldı. ###", GetName().c_str(), pDstItem->sCount, pTable.m_sName.c_str()); // is picked 
					else
						buffer = string_format("### %s Clan Bankasından %s aldı. ###", GetName().c_str(), pTable.m_sName.c_str()); // is picked 

					ChatPacket::Construct(&ClanNotices, (uint8)ChatType::GM_CHAT, &buffer);
					pKnights->Send(&ClanNotices);
				}
				else
					goDisconnect("3", __FUNCTION__);
			}
		}
		else
		{
			if (pKnights->GetID() == GetClanID())
			{
				Packet ClanNotices;
				std::string buffer;

				if (pDstItem->sCount > 1)
					buffer = string_format("### %s Clan Bankasından %d adet %s aldı. ###", GetName().c_str(), pDstItem->sCount, pTable.m_sName.c_str()); // is picked 
				else
					buffer = string_format("### %s Clan Bankasından %s aldı. ###", GetName().c_str(), pTable.m_sName.c_str()); // is picked 

				ChatPacket::Construct(&ClanNotices, (uint8)ChatType::GM_CHAT, &buffer);
				pKnights->Send(&ClanNotices);
			}
			else
				goDisconnect("4", __FUNCTION__);
		}
	}
	return;

fail_return:
	result << ReturnValue;
	Send(&result);
}

void CUser::ClanWarehouseItemMove(Packet& pkt)
{
	_ITEM_TABLE pTable = _ITEM_TABLE();
	Packet result(WIZ_HSACS_CLAN);
	result << uint8_t(ClanBankOpcodes::ClanBankMove);

	Packet DBSave(WIZ_DB_SAVE_USER, uint8(ProcDbType::ClanBankSave));
	uint8_t ReturnValue = 1;

	if (!isInGame()
		|| isDead()
		|| isTrading()
		|| isStoreOpen()
		|| isMerchanting()
		|| isMining()
		|| isFishing())
	{
		ReturnValue = 0;
		goto fail_return;
	}

	uint16_t sNpcID;
	uint32_t nItemID;
	uint8_t Page, bSrcPos, bDstPos;

	pkt >> sNpcID;
	{
		if (g_pMain->pServerSetting.ClanBankWithPremium)
		{
			if (m_bIsLoggingOut || !isInClan() || !sClanPremStatus || !g_pMain->ClanBankStatus || (!isClanLeader() && !isClanAssistant()))
			{
				ReturnValue = 0;
				goto fail_return;
			}
		}
		else
		{
			if (m_bIsLoggingOut || !isInClan() || !g_pMain->ClanBankStatus || (!isClanLeader() && !isClanAssistant()))
			{
				ReturnValue = 0;
				goto fail_return;
			}
		}
	}

	auto* pKnights = g_pMain->GetClanPtr(GetClanID());
	if (pKnights == nullptr)
	{
		g_pMain->SendHelpDescription(this, "Clan Bank : Couldn't Identify Clan.");
		ReturnValue = 0;
		goto fail_return;
	}

	pkt >> nItemID >> Page >> bSrcPos >> bDstPos;
	pTable = g_pMain->GetItemPtr(nItemID);
	if (pTable.isnull())
	{
		ReturnValue = 0;
		goto fail_return;
	}

	uint16_t reference_pos = 24 * Page;

	if (reference_pos + bSrcPos > WAREHOUSE_MAX || reference_pos + bDstPos > WAREHOUSE_MAX)
	{
		ReturnValue = 0;
		goto fail_return;
	}

	auto* pSrcItem = &pKnights->m_sClanWarehouseArray[reference_pos + bSrcPos];
	auto* pDstItem = &pKnights->m_sClanWarehouseArray[reference_pos + bDstPos];
	if (pSrcItem == nullptr || pDstItem == nullptr)
	{
		ReturnValue = 0;
		goto fail_return;
	}

	if (pSrcItem->nNum != nItemID || pDstItem->nNum != 0)
	{
		ReturnValue = 0;
		goto fail_return;
	}

	if (pSrcItem->isDuplicate() || pDstItem->isDuplicate())
	{
		ReturnValue = 2;
		goto fail_return;
	}

	memcpy(pDstItem, pSrcItem, sizeof(_ITEM_DATA));
	memset(pSrcItem, 0, sizeof(_ITEM_DATA));

	result << ReturnValue;
	Send(&result);

	DBSave << pKnights->GetName();
	g_pMain->AddDatabaseRequest(DBSave, this);
	return;

fail_return:
	result << ReturnValue;
	Send(&result);
}

void CUser::ClanWarehouseInventoryItemMove(Packet& pkt)
{
	_ITEM_TABLE pTable = _ITEM_TABLE();
	Packet result(WIZ_HSACS_CLAN);
	result << uint8_t(ClanBankOpcodes::ClanBankInventoryMove);

	uint8_t ReturnValue = 1;

	if (!isInGame()
		|| isDead()
		|| isTrading()
		|| isStoreOpen()
		|| isMerchanting()
		|| isMining()
		|| isFishing())
	{
		ReturnValue = 0;
		goto fail_return;
	}

	uint16_t sNpcID;
	uint32_t nItemID;
	uint8_t Page, bSrcPos, bDstPos;

	pkt >> sNpcID;
	{
		if (g_pMain->pServerSetting.ClanBankWithPremium)
		{
			if (m_bIsLoggingOut || !isInClan() || !sClanPremStatus || !g_pMain->ClanBankStatus || (!isClanLeader() && !isClanAssistant()))
			{
				ReturnValue = 0;
				goto fail_return;
			}
		}
		else
		{
			if (m_bIsLoggingOut || !isInClan() || !g_pMain->ClanBankStatus || (!isClanLeader() && !isClanAssistant()))
			{
				ReturnValue = 0;
				goto fail_return;
			}
		}
	}

	auto* pKnights = g_pMain->GetClanPtr(GetClanID());
	if (pKnights == nullptr)
	{
		g_pMain->SendHelpDescription(this, "Clan Bank : Couldn't Identify Clan.");
		ReturnValue = 0;
		goto fail_return;
	}

	pkt >> nItemID >> Page >> bSrcPos >> bDstPos;
	pTable = g_pMain->GetItemPtr(nItemID);
	if (pTable.isnull())
	{
		ReturnValue = 0;
		goto fail_return;
	}

	if (bSrcPos > HAVE_MAX || bDstPos > HAVE_MAX)
		goto fail_return;

	auto* pSrcItem = GetItem(SLOT_MAX + bSrcPos);
	auto* pDstItem = GetItem(SLOT_MAX + bDstPos);
	if (pSrcItem == nullptr || pDstItem == nullptr)
	{
		ReturnValue = 0;
		goto fail_return;
	}

	if (pSrcItem->nNum != nItemID)
	{
		ReturnValue = 0;
		goto fail_return;
	}

	if (pSrcItem->isDuplicate() || pDstItem->isDuplicate())
	{
		ReturnValue = 2;
		goto fail_return;
	}

	_ITEM_DATA PositionItem;
	memcpy(&PositionItem, pDstItem, sizeof(_ITEM_DATA));
	memcpy(pDstItem, pSrcItem, sizeof(_ITEM_DATA));
	memcpy(pSrcItem, &PositionItem, sizeof(_ITEM_DATA));

#if(GAME_SOURCE_VERSION == 2369)
	HandleHShieldSoftwareNewItemMoveLoadderHandler();
#endif

	result << ReturnValue;
	Send(&result);
	return;

fail_return:
	result << ReturnValue;
	Send(&result);
}
#pragma endregion
