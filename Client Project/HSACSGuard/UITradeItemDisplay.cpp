#include "stdafx.h"
#include "UITradeItemDisplay.h"

CUITradeItemDiplayPlug::CUITradeItemDiplayPlug()
{
	vector<int>offsets;
	offsets.push_back(0x2C8);
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);

	ParseUIElements();
}

CUITradeItemDiplayPlug::~CUITradeItemDiplayPlug()
{
}

void __stdcall CUITradeItemDiplayPlug::ParseUIElements()
{
	DWORD curVTable = 0;

	for (int i = 0; i < 4; i++)
	{
		curVTable = 0;
		__asm pushad
		g_pMain->GetChildByID(m_dVTableAddr, std::to_string(i), curVTable);
		__asm popad

		if (curVTable != 0)
		{
			CMerchantInvSlot* newSlot = new CMerchantInvSlot(i, curVTable);
			m_Slots.push_back(newSlot);
		}
	}
}

DWORD CUITradeItemDiplayPlug::GetSlotVTable(int id)
{
	for (MerchantInvSlotListItor itor = m_Slots.begin(); m_Slots.end() != itor; ++itor)
	{
		CMerchantInvSlot* slot = (*itor);

		if (slot->m_iID == id)
			return slot->m_dVTableAddr;
	}

	return 0;
}