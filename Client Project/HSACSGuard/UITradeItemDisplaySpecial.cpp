#include "stdafx.h"
#include "UITradeItemDisplaySpecial.h"

CUITradeItemDisplaySpecialPlug::CUITradeItemDisplaySpecialPlug()
{
	vector<int>offsets;
	offsets.push_back(0x2CC);
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);

	ParseUIElements();
}

CUITradeItemDisplaySpecialPlug::~CUITradeItemDisplaySpecialPlug()
{
}

void CUITradeItemDisplaySpecialPlug::ParseUIElements()
{
	for (int i = 0; i < 8; i++)
	{
		DWORD curVTable = 0;
		g_pMain->GetChildByID(m_dVTableAddr, std::to_string(i), curVTable);

		if (curVTable != 0)
		{
			CMerchantInvSlot* newSlot = new CMerchantInvSlot(i, curVTable);
			m_Slots.push_back(newSlot);
		}
	}
}

DWORD CUITradeItemDisplaySpecialPlug::GetSlotVTable(int id)
{
	for (MerchantInvSlotListItor itor = m_Slots.begin(); m_Slots.end() != itor; ++itor)
	{
		CMerchantInvSlot* slot = (*itor);

		if (slot->m_iID == id)
			return slot->m_dVTableAddr;
	}

	return 0;
}