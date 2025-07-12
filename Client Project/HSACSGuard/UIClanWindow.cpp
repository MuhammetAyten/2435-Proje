#include "stdafx.h"
#include "UIClanWindow.h"

CUIClanWindowPlug::CUIClanWindowPlug()
{
	vector<int>offsets;
	offsets.push_back(0x30);
	offsets.push_back(0);
	offsets.push_back(0x574);
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);
	m_btnClanBank = ClanPremStatus = 0;

	ParseUIElements();
	InitReceiveMessage();
}

CUIClanWindowPlug::~CUIClanWindowPlug()
{

}

void CUIClanWindowPlug::ParseUIElements()
{
	DWORD parent;
	parent = g_pMain->GetChildByID(m_dVTableAddr, "group");
	m_btnClanBank = g_pMain->GetChildByID(m_dVTableAddr, "btn_bank");
}

DWORD uiClanWindowVTable;
bool CUIClanWindowPlug::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	uiClanWindowVTable = m_dVTableAddr;
	if (!pSender || pSender == 0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;

	std::string msg = "";
	if (pSender == (DWORD*)m_btnClanBank) { Packet result(WIZ_HSACS_CLAN, uint8(0x01));		g_pMain->Send(&result); }
	return true;
}

DWORD Func_ClanWindow = 0;
void __stdcall UiClanWindowReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->uiClanWindowPlug->ReceiveMessage(pSender, dwMsg);

	__asm
	{
		MOV ECX, uiClanWindowVTable
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_ClanWindow
		CALL EAX
	}
}

void CUIClanWindowPlug::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_ClanWindow = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)UiClanWindowReceiveMessage_Hook;
}