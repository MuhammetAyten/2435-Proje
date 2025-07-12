#include "UITooltip.h"

CUITooltip::CUITooltip()
{
	vector<int>offsets;
	offsets.push_back(0x1BC);
	offsets.push_back(0x124);
	offsets.push_back(0x1A4);
	offsets.push_back(0x0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);

	ParseUIElements();
}

CUITooltip::~CUITooltip()
{
}

void CUITooltip::ParseUIElements()
{
	string0 = g_pMain->GetChildByID(m_dVTableAddr, "string_0");
}