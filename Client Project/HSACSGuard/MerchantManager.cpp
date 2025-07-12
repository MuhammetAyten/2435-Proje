#include "stdafx.h"
#include "MerchantManager.h"

CMerchantManager::CMerchantManager()
{
	ResetMerchant();
}

CMerchantManager::~CMerchantManager()
{
}

bool CMerchantManager::NeedToCheckTooltip()
{
	return (status == MERCHANT_OPEN || status == MERCHANT_ITEM_LIST)
		&& g_pMain->IsVisible(g_pMain->pClientTradeInventory->m_dVTableAddr);
}

bool CMerchantManager::NeedToCheckDisplayTooltip()
{
	if (status != MERCHANT_CREATED && status != MERCHANT_SPECIAL_CREATED)
		return false;

	return g_pMain->IsVisible(g_pMain->uiTradeItemDisplay->m_dVTableAddr)
		|| g_pMain->IsVisible(g_pMain->uiTradeItemDisplaySpecial->m_dVTableAddr);
}

bool CMerchantManager::UpdateTooltipString(std::string & str)
{
	POINT cp = g_pMain->pClientHookManager->pClientLocalInput->MouseGetPos();

	for (MerchantSlotListItor itor = m_sMerchantListSlot.begin(); m_sMerchantListSlot.end() != itor; ++itor)
	{
		CMerchantSlot * slot = (*itor);
		DWORD slotVTable = 0;

		if (status == MERCHANT_OPEN || status == MERCHANT_ITEM_LIST)
			slotVTable = g_pMain->pClientTradeInventory->GetSlotVTable(slot->m_iPos);
		else if (status == MERCHANT_CREATED)
			slotVTable = g_pMain->uiTradeItemDisplay->GetSlotVTable(slot->m_iPos);
		else if (status == MERCHANT_SPECIAL_CREATED)
			slotVTable = g_pMain->uiTradeItemDisplaySpecial->GetSlotVTable(slot->m_iPos);

		if (slotVTable != 0)
		{
			bool isIn = false;
			POINT pt = g_pMain->GetUiPos(slotVTable);

			RECT rp;
			rp.left = pt.x;
			rp.right = pt.x + (status == MERCHANT_CREATED ? 32 : 45);
			rp.top = pt.y;
			rp.bottom = pt.y + (status == MERCHANT_CREATED ? 32 : 45);

			isIn = cp.x >= rp.left && cp.x <= rp.right && cp.y >= rp.top && cp.y <= rp.bottom;

			if (isIn)
			{
				std::string strPrice = g_pMain->StringHelper->NumberFormat(slot->m_iPrice, ',');
				if (slot->m_bIsKC)
				{
					str = string_format("  Purchasing Price : %s Knight Cash", strPrice.c_str());
					return true;
				}

				str = string_format("  Purchasing Price : %s Coin", strPrice.c_str());
				return false;
			}
		}
	}

	return false;
}

void CMerchantManager::SetRecentItemAddReq(Packet& pkt)
{
	uint32 price, itemID;
	uint16 count;
	uint8 OpCode, subCode, srcPos, dstPos, mode, isKC;
	pkt >> OpCode >> subCode >> itemID >> count >> price >> srcPos >> dstPos >> mode >> isKC;
	m_sMerchantReqSlot = new CMerchantSlot(dstPos, price, isKC ? 1 == true : false, count);	
}

bool CMerchantManager::UpdateSlot(CMerchantSlot* item)
{
	for (MerchantSlotListItor itor = m_sMerchantListSlot.begin(); m_sMerchantListSlot.end() != itor; ++itor)
	{
		CMerchantSlot * slot = (*itor);
		if (slot->m_iPos == m_sMerchantReqSlot->m_iPos)
		{
			*itor = item;
			return true;
		}
	}
	return false;
}

void CMerchantManager::UpdateRecentItemAddReq(Packet& pkt)
{
	uint32 price, itemID;
	uint16 count, result;
	int16 durat;
	uint8 srcPos, dstPos;

	if(status != MERCHANT_CREATED) status = MERCHANT_OPEN;

	pkt >> result >> itemID >> count >> durat >> price >> srcPos >> dstPos;

	if (result == 1 && m_sMerchantReqSlot != NULL)
	{
		if (m_sMerchantReqSlot->m_iPos == dstPos && m_sMerchantReqSlot->m_iPrice == price)
		{
			bool isEmpty = true;
			for (MerchantSlotListItor itor = m_sMerchantListSlot.begin(); m_sMerchantListSlot.end() != itor; ++itor)
			{
				CMerchantSlot * slot = (*itor);
				if (slot->m_iPos == m_sMerchantReqSlot->m_iPos)
				{
					isEmpty = false;
					break;
				}
			}

			CMerchantSlot * newSlot = new CMerchantSlot(m_sMerchantReqSlot->m_iPos, m_sMerchantReqSlot->m_iPrice, m_sMerchantReqSlot->m_bIsKC, m_sMerchantReqSlot->m_iCount);

			if (isEmpty)
				m_sMerchantListSlot.push_back(newSlot);
			else
				UpdateSlot(newSlot);

			m_sMerchantReqSlot = NULL;

			g_pMain->pClientTradeInventory->UpdateTotal(price * count, false, true);
		}
	}
	else if (result == 0)
		m_sMerchantReqSlot = NULL;
}

void CMerchantManager::RemoveItemFromSlot(Packet& pkt)
{
	uint16 res;
	pkt >> res;

	if (res < 1 && m_sMerchantReqSlot != NULL) // son eklenen item onaylanmadýysa
	{
		m_sMerchantReqSlot = NULL;
		return;
	}

	uint8 pos;
	pkt >> pos;

	for (MerchantSlotListItor itor = m_sMerchantListSlot.begin(); m_sMerchantListSlot.end() != itor; ++itor)
	{
		CMerchantSlot * slot = (*itor);
		if (slot->m_iPos == pos)
		{
			g_pMain->pClientTradeInventory->UpdateTotal(slot->m_iPrice * slot->m_iCount * -1, false, true);
			itor = m_sMerchantListSlot.erase(itor);
			break;
		}
	}
}

void CMerchantManager::MerchantCreated(Packet& pkt)
{
	uint16 result;
	pkt >> result;

	if (result == 1) // pazar oluþtu
	{
		std::string advertMessage;
		uint16 socketID;
		uint8 m_bPremiumMerchant;

		pkt.DByte();
		pkt >> advertMessage >> socketID >> m_bPremiumMerchant;
		if (socketID == g_pMain->m_PlayerBase->m_iSocketID) // pazarý kuran bensem
		{
			if (m_bPremiumMerchant == 1)
				status = MERCHANT_SPECIAL_CREATED; // special merchant
			else 
				status = MERCHANT_CREATED; // normal merchant
		}
	}
}

void CMerchantManager::UpdateItemDisplaySlots(Packet& pkt)
{
	uint8 isSpecial, itemCount;
	pkt >> isSpecial >> itemCount;

	m_sMerchantListSlot.clear();
	for (int i = 0; i < itemCount; i++)
	{
		uint8 pos, isKC;
		uint32 price;

		pkt >> pos >> price >> isKC;

		CMerchantSlot * newSlot = new CMerchantSlot(pos, price, true);
		m_sMerchantListSlot.push_back(newSlot);
	}
}

void CMerchantManager::SetTheirMerchantSlots(Packet & pkt)
{
	m_sMerchantListSlot.clear();
	g_pMain->pClientMerchant->status = MERCHANT_ITEM_LIST;

	g_pMain->pClientTradeInventory->UpdateTotal(g_pMain->Player.KnightCash);

	uint16 result, uid;
	pkt >> result >> uid;

	uint32 items[12];
	uint32 prices[12];

	for (int i = 0; i < 12; i++)
	{
		uint16 count, durat;
		uint32 itemID, price, ring;

		pkt >> itemID >> count >> durat >> price >> ring;
		items[i] = itemID;
		prices[i] = price;

		if (ring != 0)
		{
			string charName;
			uint8 charClass, charLevel, unk1, charExp, charRace, unk2;

			pkt.DByte();
			pkt >> charName >> charClass >> charLevel >> unk1 >> charExp >> charRace >> unk2;
		}
	}

	for (int i = 0; i < 12; i++)
	{
		uint8 isKC;
		pkt >> isKC;

		if (items[i] > 0) // slot boþ deðilse
		{
			CMerchantSlot * newSlot = new CMerchantSlot(i, prices[i], isKC == 1 ? true : false);
			m_sMerchantListSlot.push_back(newSlot);
		}
	}
}

void CMerchantManager::ResetMerchant()
{
	m_sMerchantReqSlot = NULL;
	m_sMerchantListSlot.clear();
	status = MERCHANT_CLOSE;
}

bool CMerchantManager::IsMerchanting()
{
	return status == MERCHANT_CREATED;
}