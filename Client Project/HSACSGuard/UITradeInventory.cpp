#include "stdafx.h"
#include "UITradeInventory.h"

CUITradeInventoryPlug::CUITradeInventoryPlug()
{
	vector<int>offsets;
	offsets.push_back(0x2C4);
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);

	m_txtTotal = 0;
	val = 0;
	lastState = 0;
	base_mid = 0;
	base_bot = 0;
	base_mid = 0;
	lastVisible = false;

	ParseUIElements();
}

CUITradeInventoryPlug::~CUITradeInventoryPlug()
{
}

void CUITradeInventoryPlug::ParseUIElements()
{
	base_top = g_pMain->GetChildByID(m_dVTableAddr, "base_top");
	base_bot = g_pMain->GetChildByID(m_dVTableAddr, "base_bot");
	base_mid = g_pMain->GetChildByID(m_dVTableAddr, "base_mid");
	text_sell_KnightCash = g_pMain->GetChildByID(base_mid, "text_sell_KnightCash");
	text_sell_money = g_pMain->GetChildByID(base_mid, "text_sell_money");

	if (base_top) {
		for (int i = 0; i < 12; i++)
		{
			DWORD curVTable = g_pMain->GetChildByID(base_top, string_format(xorstr("at%d"), i));
			if (curVTable != 0)
			{
				CMerchantInvSlot* newSlot = new CMerchantInvSlot(i, curVTable);
				m_Slots.push_back(newSlot);
			}
		}
	}
}

void CUITradeInventoryPlug::OnOpen()
{
	if (lastState == 0) val = g_pMain->Player.KnightCash;
	UpdateTotal(val, lastState == 0 ? true : false, false);
}

void CUITradeInventoryPlug::OnClose()
{
	val = 0;
}

void CUITradeInventoryPlug::UpdateTotal(int32 value, bool iskc, bool add)
{
	if ((lastState == 0 && !iskc) || (lastState == 1 && iskc)) {
		val = 0;
		lastState = !lastState;
	}

	if (add && iskc)
		val += value;
	else if (iskc)
		val = value;

	std::string strValue = g_pMain->StringHelper->NumberFormat(val);
	g_pMain->SetString(m_txtTotal, strValue);
}

void CUITradeInventoryPlug::Tick()
{
	if (m_dVTableAddr != 0) {
		if (lastVisible != g_pMain->IsVisible(m_dVTableAddr))
		{
			lastVisible = !lastVisible;
			if (lastVisible)
				OnOpen();
			else
				OnClose();
		}
	}
}

DWORD CUITradeInventoryPlug::GetSlotVTable(int id)
{
	for (MerchantInvSlotListItor itor = m_Slots.begin(); m_Slots.end() != itor; ++itor)
	{
		CMerchantInvSlot* slot = (*itor);

		if (slot->m_iID == id)
			return slot->m_dVTableAddr;
	}

	return 0;
}