#include "TopRightReWarIcon.h"

CUIClientTopRightNewPlug::CUIClientTopRightNewPlug()
{
	m_dVTableAddr = NULL;
	m_btnsituation = NULL;
	m_lottery = NULL;
	m_collectionrace = NULL;
	m_cindirella = NULL;
	m_btndropresult = NULL;
	m_btnlive = NULL;
	m_btnbaselive = NULL;
	BasladimiBaslamadimi = false;
	vector<int>offsets;
	offsets.push_back(0x3B4);
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);

	ParseUIElements();
	InitReceiveMessage();
}

CUIClientTopRightNewPlug::~CUIClientTopRightNewPlug()
{
}

void CUIClientTopRightNewPlug::ParseUIElements()
{
	m_btndropresult = g_pMain->GetChildByID(m_dVTableAddr, "btn_dropresult");
	m_lottery = g_pMain->GetChildByID(m_dVTableAddr, "btn_lottery");
	m_collectionrace = g_pMain->GetChildByID(m_dVTableAddr, "btn_collectionrace");
	m_cindirella = g_pMain->GetChildByID(m_dVTableAddr, "btn_funclass");
	m_btnbaselive = g_pMain->GetChildByID(m_dVTableAddr, "Base_Live");
	m_btnlive = g_pMain->GetChildByID(m_dVTableAddr, "btn_live");
	
	g_pMain->SetVisible(m_btndropresult, false);
	g_pMain->SetState(m_btndropresult, UI_STATE_BUTTON_DISABLE);
	g_pMain->SetVisible(m_lottery, false);
	g_pMain->SetState(m_lottery, UI_STATE_BUTTON_DISABLE);
	g_pMain->SetVisible(m_collectionrace, false);
	g_pMain->SetState(m_collectionrace, UI_STATE_BUTTON_DISABLE);
	g_pMain->SetVisible(m_cindirella, false);
	g_pMain->SetState(m_cindirella, UI_STATE_BUTTON_DISABLE);
	g_pMain->SetVisible(m_btnbaselive, false);
	g_pMain->SetState(m_btnbaselive, UI_STATE_BUTTON_DISABLE);
	g_pMain->SetVisible(m_btnlive, false);
	g_pMain->SetState(m_btnlive, UI_STATE_BUTTON_DISABLE);
}

void CUIClientTopRightNewPlug::DropResultStatus(bool status)
{
	if (this)
	{
		g_pMain->SetVisible(m_btndropresult, status);
		g_pMain->SetState(m_btndropresult, status ? UI_STATE_BUTTON_NORMAL : UI_STATE_BUTTON_DISABLE);
	}
}

DWORD uiTopRightNewVTable;
bool CUIClientTopRightNewPlug::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	uiTopRightNewVTable = m_dVTableAddr;
	if (!pSender || pSender == 0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;

	else if (pSender == (DWORD*)m_btndropresult)
	{
		if (g_pMain->pClientDropResult == NULL)
			g_pMain->pClientHookManager->ShowDropResult();
		else 
		{
			if (g_pMain->IsVisible(g_pMain->pClientDropResult->m_dVTableAddr))
				g_pMain->pClientDropResult->Close();
			else
				g_pMain->pClientDropResult->Open();
		}
	}
	else if (pSender == (DWORD*)m_btnlive)
		ShellExecute(NULL, "open", LiveURL.c_str(), NULL, NULL, SW_SHOWNORMAL);
	else if (pSender == (DWORD*)m_lottery)
	{
		HideLotteryButton();

		if (g_pMain->pClientLottyEvent != NULL)
			g_pMain->pClientLottyEvent->Open();
	}	
	else if (pSender == (DWORD*)m_collectionrace)
	{
		HideCollectionRaceButton();

		if (g_pMain->pClientCollection != NULL)
			g_pMain->pClientCollection->Open();
	}
	else if (pSender == (DWORD*)m_cindirella)
	{
		HideCindirellaButton();

		if (g_pMain->uiCindirella != NULL)
			g_pMain->uiCindirella->Open();
	}
	return true;
}

void CUIClientTopRightNewPlug::UpdateTopLeftVisible(uint8_t Live, std::string LiveURLv)
{
	LiveURL = LiveURLv;
	g_pMain->SetVisible(m_btnlive, Live);
	g_pMain->SetState(m_btnlive, Live ? eUI_STATE::UI_STATE_BUTTON_NORMAL : eUI_STATE::UI_STATE_BUTTON_DISABLE);
	g_pMain->SetVisible(m_btnbaselive, Live);
	g_pMain->SetState(m_btnbaselive, Live ? eUI_STATE::UI_STATE_BUTTON_NORMAL : eUI_STATE::UI_STATE_BUTTON_DISABLE);
}

void CUIClientTopRightNewPlug::OpenLotteryButton()
{
	g_pMain->SetVisible(m_lottery, true);
	g_pMain->SetState(m_lottery, UI_STATE_BUTTON_NORMAL);
}

void CUIClientTopRightNewPlug::OpenCollectionRaceButton()
{
	g_pMain->SetVisible(m_collectionrace, true);
	g_pMain->SetState(m_collectionrace, UI_STATE_BUTTON_NORMAL);
}

void CUIClientTopRightNewPlug::OpenCindirellaButton()
{
	g_pMain->SetVisible(m_cindirella, true);
	g_pMain->SetState(m_cindirella, UI_STATE_BUTTON_NORMAL);
}

void CUIClientTopRightNewPlug::HideCollectionRaceButton()
{
	g_pMain->SetVisible(m_collectionrace, false);
	g_pMain->SetState(m_collectionrace, UI_STATE_BUTTON_DISABLE);
}

void CUIClientTopRightNewPlug::HideCindirellaButton()
{
	g_pMain->SetVisible(m_cindirella, false);
	g_pMain->SetState(m_cindirella, UI_STATE_BUTTON_DISABLE);
}

void CUIClientTopRightNewPlug::HideLotteryButton()
{
	g_pMain->SetVisible(m_lottery, false);
	g_pMain->SetState(m_lottery, UI_STATE_BUTTON_DISABLE);
}

DWORD Func_RIcon = 0;
void __stdcall uiTopRightNewMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->pClientTopRightNewPlug->ReceiveMessage(pSender, dwMsg);
	__asm
	{
		MOV ECX, uiTopRightNewVTable
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_RIcon
		CALL EAX
	}
}

void CUIClientTopRightNewPlug::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_RIcon = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)uiTopRightNewMessage_Hook;
}
