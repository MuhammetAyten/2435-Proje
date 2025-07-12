#include "stdafx.h"
#include "UIInformationWind.h"
#include "HSACSEngine.h"

void UIMerchantWindTick()
{
	while (true)
	{
		if (g_pMain)
		{
			if (g_pMain->m_bGameStart)
			{
				if (g_pMain->uiInformationWind)
				{
					if (g_pMain->uiInformationWind->merchant_state)
						g_pMain->uiInformationWind->MerchantBtnSetState();
				}
			}
		}
		Sleep(500);
	}
}

void UINoticeWindTick()
{
	while (true)
	{
		if (g_pMain)
		{
			if (g_pMain->m_bGameStart)
			{
				if (g_pMain->uiInformationWind)
				{
					if (g_pMain->uiInformationWind->merchant_state)
						g_pMain->uiInformationWind->NoticeBtnSetState();
				}
			}
		}
		Sleep(500);
	}
}

CUIInformationWindPlug::CUIInformationWindPlug()
{
	vector<int>offsets;
	offsets.push_back(0x214);
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);

	if (m_dVTableAddr == 0)
		return;

	merchant_state = notice_state = false;
	ParseUIElements();
	InitReceiveMessage();
	CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(UINoticeWindTick), NULL, 0, 0);
	CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(UIMerchantWindTick), NULL, 0, 0);
}

CUIInformationWindPlug::~CUIInformationWindPlug()
{

}

void CUIInformationWindPlug::ParseUIElements()
{
	DWORD parent = 0;
	parent = g_pMain->GetChildByID(m_dVTableAddr, "base_msgwnd");
	btn_merchant = g_pMain->GetChildByID(parent, "btn_Merchant");
	btn_Notice = g_pMain->GetChildByID(parent, "btn_notice");
}

DWORD uiInformationWindVTable;
bool CUIInformationWindPlug::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	uiInformationWindVTable = m_dVTableAddr;
	if (!pSender || pSender == 0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;

	if (pSender == (DWORD*)btn_merchant)
	{
		merchant_state = false;
		g_pMain->uiNoticeWind->Open(true);
		g_pMain->SetState(btn_merchant, UI_STATE_BUTTON_NORMAL);
	}
	else if (pSender == (DWORD*)btn_Notice)
	{
		notice_state = false;
		g_pMain->uiNoticeWind->Open();
		g_pMain->SetState(btn_Notice, UI_STATE_BUTTON_NORMAL);
	}

	return true;
}

DWORD Func_InfoBox = 0;

void __stdcall UIInformationWindReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->uiInformationWind->ReceiveMessage(pSender, dwMsg);
	__asm
	{
		MOV ECX, uiInformationWindVTable
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_InfoBox
		CALL EAX
	}
}

void CUIInformationWindPlug::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_InfoBox = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)UIInformationWindReceiveMessage_Hook;
}

void CUIInformationWindPlug::MerchantBtnSetState()
{
	g_pMain->SetState(btn_merchant, UI_STATE_BUTTON_DOWN);
	Sleep(1 * SECOND);
	g_pMain->SetState(btn_merchant, UI_STATE_BUTTON_NORMAL);
}

void CUIInformationWindPlug::NoticeBtnSetState()
{
	g_pMain->SetState(btn_Notice, UI_STATE_BUTTON_DOWN);
	Sleep(1 * SECOND);
	g_pMain->SetState(btn_Notice, UI_STATE_BUTTON_NORMAL);
}