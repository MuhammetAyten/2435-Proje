#include "stdafx.h"
#include "UIGenieSub.h"

CUIGenieSubPlug::CUIGenieSubPlug()
{
	vector<int>offsets;
	offsets.push_back(0x30);
	offsets.push_back(0);
	offsets.push_back(0x53C);
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);
	ParseUIElements();
}

CUIGenieSubPlug::~CUIGenieSubPlug()
{

}
void CUIGenieSubPlug::ParseUIElements()
{
	m_textGenieTime = g_pMain->GetChildByID(m_dVTableAddr, "str_progress");
	TargetSam = g_pMain->GetChildByID(m_dVTableAddr, "TargetSam");
	TargetTwo = g_pMain->GetChildByID(m_dVTableAddr, "TargetTwo");
	
	RECT ASD = g_pMain->GetUiRegion2(m_dVTableAddr);
	ASD.bottom = ASD.bottom - 25;
	g_pMain->SetUiRegion(m_dVTableAddr, ASD);
}