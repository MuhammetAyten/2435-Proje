#include "UISchedular.h"

CUISchedular::CUISchedular()
{
	m_dVTableAddr = NULL;
	m_btnsituation = NULL;
	m_btndropresult = NULL;
	m_btnbug = NULL;
	btn_lottery = NULL;

	BasladimiBaslamadimi = false;
	vector<int>offsets;
	offsets.push_back(0x50);
	offsets.push_back(0);
	offsets.push_back(0x510);
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);

	ParseUIElements();
	InitReceiveMessage();
}

CUISchedular::~CUISchedular()
{
}

void CUISchedular::ParseUIElements()
{

	std::string find = xorstr("btn_dropresult");
	g_pMain->GetChildByID(m_dVTableAddr, find, m_btndropresult);
	find = xorstr("btn_bug");
	g_pMain->GetChildByID(m_dVTableAddr, find, m_btnbug);
	find = xorstr("btn_lottery");
	g_pMain->GetChildByID(m_dVTableAddr, find, btn_lottery);

	g_pMain->SetVisible(m_btndropresult, false);
	g_pMain->SetState(m_btndropresult, UI_STATE_BUTTON_DISABLE);

	/*g_pMain->SetVisible(btn_lottery, false);
	g_pMain->SetState(btn_lottery, UI_STATE_BUTTON_DISABLE);*/
}

void CUISchedular::DropResultStatus(bool status)
{
	if (this)
	{
		g_pMain->SetVisible(m_btndropresult, status);
		g_pMain->SetState(m_btndropresult, status ? UI_STATE_BUTTON_NORMAL : UI_STATE_BUTTON_DISABLE);
	}
}

DWORD uiSchedularT;
bool CUISchedular::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	uiSchedularT = m_dVTableAddr;
	if (!pSender || pSender == 0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;

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
	else if (pSender == (DWORD*)m_btndropresult)
	{
	
			if (g_pMain->IsVisible(g_pMain->pClientDropResult->m_dVTableAddr))
				g_pMain->pClientDropResult->Close();
			else
				g_pMain->pClientDropResult->Open();
		
	}
	else if (pSender == (DWORD*)btn_lottery)
	{
		if (g_pMain->pClientLottyEvent != NULL)
		{
			g_pMain->SetVisible(g_pMain->pClientLottyEvent->m_dVTableAddr,true);
			g_pMain->SetVisible(btn_lottery, false);
			g_pMain->SetState(btn_lottery, UI_STATE_BUTTON_DISABLE);
		}
	}
	
	return true;
}


DWORD Func_Schedular = 0;
void __stdcall uiSchedular_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->uiSchedularPlug->ReceiveMessage(pSender, dwMsg);
	__asm
	{
		MOV ECX, uiSchedularT
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_Schedular
		CALL EAX
	}
}

void CUISchedular::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_Schedular = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)uiSchedular_Hook;
}
