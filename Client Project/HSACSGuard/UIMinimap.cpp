#include "stdafx.h"
#include "UIMinimap.h"

CUIMinimapPlug::CUIMinimapPlug()
{
	vector<int>offsets;
	offsets.push_back(0x3E4);
	offsets.push_back(0);
	str_zoneid = 0;
	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);

	str_zoneid = g_pMain->GetChildByID(m_dVTableAddr, "str_zoneid");
	text_xy = g_pMain->GetChildByID(m_dVTableAddr, "Text_Position");
	btn_StateBar = g_pMain->GetChildByID(m_dVTableAddr, "btn_StateBar");
}

CUIMinimapPlug::~CUIMinimapPlug()
{
}

void CUIMinimapPlug::Tick()
{
	if (m_dVTableAddr != NULL)
	{ 
		g_pMain->SetVisible(m_dVTableAddr, false);
		g_pMain->SetVisible(text_xy, false);
		g_pMain->SetVisible(str_zoneid, false);
		g_pMain->SetVisible(btn_StateBar, false);
	}
}
