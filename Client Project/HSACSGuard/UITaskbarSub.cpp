#include "stdafx.h"
#include "UITaskbarSub.h"

CUITaskbarSubPlug::CUITaskbarSubPlug()
{
	vector<int>offsets;
	offsets.push_back(0x3DC);
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);
	m_btnPowerUPStore = 0;
	m_mobdrop = 0;
	btn_wof = 0;
	m_dailyquest = 0;
	m_btnbug = NULL;
	m_btnQuestion = 0;

	ParseUIElements();
	InitReceiveMessage();
}

CUITaskbarSubPlug::~CUITaskbarSubPlug()
{

}

void CUITaskbarSubPlug::ParseUIElements()
{
	m_btnPowerUPStore = g_pMain->GetChildByID(m_dVTableAddr, "btns_pus");
	m_btnHotkeyH = g_pMain->GetChildByID(m_dVTableAddr, "btn_hotkey");
	m_btnGlobalMap = g_pMain->GetChildByID(m_dVTableAddr, "btn_globalmap");
	m_btnInviteParty = g_pMain->GetChildByID(m_dVTableAddr, "btn_party");
	m_btnExitGame = g_pMain->GetChildByID(m_dVTableAddr, "btn_exit");	
}

DWORD uiTaskbarSubVTable;
bool CUITaskbarSubPlug::ReceiveMessage(DWORD * pSender, uint32_t dwMsg)
{
	uiTaskbarSubVTable = m_dVTableAddr;
	if (!pSender || pSender == 0)
		return false;

	if (pSender == (DWORD*)m_mobdrop)
	{
		if (g_pMain->uiSearchMonster != NULL)
			g_pMain->pClientHookManager->OpenMonsterSearch();
	}
	else if (pSender == (DWORD*)m_dailyquest) 
	{
		if (g_pMain->uiQuestPage != NULL)
		{
			g_pMain->uiQuestPage->InitQuests(g_pMain->uiQuestPage->page);
			g_pMain->SetVisible(g_pMain->uiQuestPage->m_dVTableAddr, true);
			g_pMain->UIScreenCenter(g_pMain->uiQuestPage->m_dVTableAddr);
		}
	}
	if (pSender == (DWORD*)m_btnbug)
	{
		if (g_pMain->pClientUIState == NULL)
		{
			g_pMain->pClientHookManager->ShowMessageBox(xorstr("Error"), xorstr("Support is available only after the game has started."), Ok);
			return true;
		}
		
		if (g_pMain->uiSupport)
			g_pMain->uiSupport->OpenSupport();
	}
	else if (pSender == (DWORD*)btn_wof)
		g_pMain->pClientWheelOfFunPlug->OpenWheel();

	return true;
}

void __stdcall UITaskbarSubReceiveMessage_Hook(DWORD * pSender, uint32_t dwMsg)
{
	g_pMain->uiTaskbarSub->ReceiveMessage(pSender, dwMsg);
	__asm
	{
		MOV ECX, uiTaskbarSubVTable
		PUSH dwMsg
		PUSH pSender
		MOV EAX, KO_UI_TASKBAR_SUB_RECEIVE_MESSAGE_FUNC
		CALL EAX
	}
}

void CUITaskbarSubPlug::InitReceiveMessage()
{
	
	*(DWORD*)KO_UI_TASKBAR_SUB_RECEIVE_MESSAGE_PTR = (DWORD)UITaskbarSubReceiveMessage_Hook;
}
void CUITaskbarSubPlug::OpenGlobalMap()
{
	UITaskbarSubReceiveMessage_Hook((DWORD*)m_btnGlobalMap, UIMSG_BUTTON_CLICK);
}
void CUITaskbarSubPlug::OpenInviteParty()
{
	UITaskbarSubReceiveMessage_Hook((DWORD*)m_btnInviteParty, UIMSG_BUTTON_CLICK);
}
void CUITaskbarSubPlug::OpenHotkeyH()
{
	UITaskbarSubReceiveMessage_Hook((DWORD*)m_btnHotkeyH, UIMSG_BUTTON_CLICK);
}
void CUITaskbarSubPlug::OpenExitGame()
{
	UITaskbarSubReceiveMessage_Hook((DWORD*)m_btnExitGame, UIMSG_BUTTON_CLICK);
}