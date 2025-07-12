#include "stdafx.h"
#include "UIMiniMenu.h"

CUIMiniMenuPlug::CUIMiniMenuPlug()
{
	vector<int>offsets;
	offsets.push_back(0x398);
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);
	m_btngiftpus = 0;
	m_btnViewInfo = 0;

	ParseUIElements();
	InitReceiveMessage();
}

CUIMiniMenuPlug::~CUIMiniMenuPlug()
{
}

void CUIMiniMenuPlug::ParseUIElements()
{
	base_default = g_pMain->GetChildByID(m_dVTableAddr, "base_default");
	m_btngiftpus = g_pMain->GetChildByID(base_default, "btn_giftpus");
	m_btnViewInfo = g_pMain->GetChildByID(base_default, "btn_viewinfo");
}

DWORD uiMiniMenuVTable;
DWORD miniMenuSetVisibleFalse;
void __stdcall SetVisibleViaOGFuncAsm()
{
	_asm
	{
		MOV ECX, uiMiniMenuVTable
		MOV EAX, miniMenuSetVisibleFalse
		PUSH EAX
		CALL KO_SET_VISIBLE_MINIMENU_FUNC
		//RET
	}
}

bool CUIMiniMenuPlug::ReceiveMessage(DWORD * pSender, uint32_t dwMsg)
{
	uiMiniMenuVTable = m_dVTableAddr;
	miniMenuSetVisibleFalse = 0;
	if (!pSender || pSender == 0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;

	if (pSender == (DWORD*)m_btnViewInfo)
	{
		int16 target = -1;
		DWORD ko_adr = 0;

		ReadProcessMemory(GetCurrentProcess(), (LPVOID)KO_PTR_CHR, &ko_adr, sizeof(&ko_adr), 0);
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)(ko_adr + KO_OFF_MOB), &target, sizeof(&target), 0);

		Packet result(WIZ_USER_INFO, uint8(0x05));
		result << target;
		g_pMain->Send(&result);
		SetVisibleViaOGFuncAsm();
	}
	else if (pSender == (DWORD*)m_btngiftpus) //14.11.2020 User Sag Tik Pus
	{
		if (g_pMain->uiPowerUpStore  == NULL)
		{
			g_pMain->uiPowerUpStore ->OpenPowerUpStore();
		}
		g_pMain->uiPowerUpStore ->UpdateItemList(g_pMain->pClientHookManager->item_list, g_pMain->pClientHookManager->cat_list);
		g_pMain->uiPowerUpStore ->UpdateRefundItemList(g_pMain->pClientHookManager->pusrefund_itemlist);
		g_pMain->uiPowerUpStore ->Open();
		SetVisibleViaOGFuncAsm();
	}

	return true;
}

void __stdcall UiMiniMenuReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->uiMiniMenuPlug->ReceiveMessage(pSender, dwMsg);

	__asm
	{
		MOV ECX, uiMiniMenuVTable
		PUSH dwMsg
		PUSH pSender
		MOV EAX, KO_UI_MINIMENU_RECEIVE_MESSAGE_FUNC
		CALL EAX
	}
}

void CUIMiniMenuPlug::InitReceiveMessage()
{
	*(DWORD*)KO_UI_MINIMENU_RECEIVE_MESSAGE_PTR = (DWORD)UiMiniMenuReceiveMessage_Hook;
}