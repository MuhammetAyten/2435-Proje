#include "UIHpBarMenu.h"

CUIHPBarMenuPlug::CUIHPBarMenuPlug()
{
	m_dVTableAddr = NULL;


	vector<int>offsets;
	offsets.push_back(0x54C);   // co_drop_off Satýr : 247
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);

	btn_daily_event = 0;
	btn_daily_quest = 0;
	btn_facebook = 0;
	btn_discord = 0; 
	btn_wof = 0;
	btn_searchdrop = 0;
	btn_ticket = 0;
	btn_rank = 0;
	btn_scheduler = 0;
	btn_pus = 0;
	text_online_time = 0;
	ParseUIElements();
	InitReceiveMessage();
}
DWORD Func_HpMenu;
CUIHPBarMenuPlug::~CUIHPBarMenuPlug()
{
}

void CUIHPBarMenuPlug::ParseUIElements()
{
#if (HOOK_SOURCE_VERSION == 1098)
	btn_wof = g_pMain->GetChildByID(m_dVTableAddr, "btn_wof");
	btn_pus = g_pMain->GetChildByID(m_dVTableAddr, "btn_pus");
	btn_searchdrop = g_pMain->GetChildByID(m_dVTableAddr, "btn_searchdrop");
	btn_ticket = g_pMain->GetChildByID(m_dVTableAddr, "btn_ticket");
	btn_scheduler = g_pMain->GetChildByID(m_dVTableAddr, "btn_scheduler");
	btn_rank = g_pMain->GetChildByID(m_dVTableAddr, "btn_dailyrank");
#else
	btn_facebook = g_pMain->GetChildByID(m_dVTableAddr, "btn_facebook");
	btn_discord = g_pMain->GetChildByID(m_dVTableAddr, "btn_discord");
	btn_ticket = g_pMain->GetChildByID(m_dVTableAddr, "btn_ticket");
	btn_daily_event = g_pMain->GetChildByID(m_dVTableAddr, "btn_daily_event");
#endif
}

DWORD uiHpMenu;
bool CUIHPBarMenuPlug::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	uiHpMenu = m_dVTableAddr;
	if (!pSender || pSender == 0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;

	if (pSender == (DWORD*)btn_pus)
	{
		if (g_pMain->uiPowerUpStore == NULL)
			g_pMain->uiPowerUpStore->OpenPowerUpStore();

		g_pMain->uiPowerUpStore->UpdateItemList(g_pMain->pClientHookManager->item_list, g_pMain->pClientHookManager->cat_list);
		g_pMain->uiPowerUpStore->Open();
	}
	else if (pSender == (DWORD*)btn_ticket)
	{
		if (g_pMain->uiSupport != NULL)
			g_pMain->uiSupport->OpenSupport();
	}
	else if (pSender == (DWORD*)btn_searchdrop)
	{
		if (g_pMain->uiSearchMonster != NULL)
			g_pMain->uiSearchMonster->OpenWithAnimation();
	}
	else if (pSender == (DWORD*)btn_wof)
		g_pMain->pClientWheelOfFunPlug->OpenWheel();
	else if (pSender == (DWORD*)btn_scheduler) 
	{
		if (g_pMain->pClientEvetShowList)
		{
			g_pMain->pClientEvetShowList->InitList();
			g_pMain->pClientEvetShowList->Open();
		}
	}
	else if (pSender == (DWORD*)btn_daily_event) {

		if (g_pMain->pClientEvetShowList)
		{
			g_pMain->pClientEvetShowList->InitList();
			g_pMain->pClientEvetShowList->Open();
		}
	}
	else if (pSender == (DWORD*)btn_discord)
		ShellExecute(NULL, "open", DiscordURL.c_str(), NULL, NULL, SW_SHOWNORMAL);
	else if (pSender == (DWORD*)btn_facebook)
		ShellExecute(NULL, "open", FacebookURL.c_str(), NULL, NULL, SW_SHOWNORMAL);
#if (HOOK_SOURCE_VERSION == 1098)
	else if (pSender == (DWORD*)btn_rank)
		if (g_pMain->uiTaskbarMain)g_pMain->uiTaskbarMain->OpenSeed();
#endif


	return true;
}

void CUIHPBarMenuPlug::UpdateHpMenuVisible(Packet& pkt)
{
	uint8_t Facebook = 0, Discord = 0, Live = 0, DiscordToken = 0;

	pkt.DByte();
	pkt >> Facebook >> FacebookURL >> Discord >> DiscordURL >> Live >> LiveURL >> DiscordToken >> DiscordTokenID >> ResellerURL;

	if (g_pMain->dc != nullptr)
	{
		if (DiscordToken)
			g_pMain->dc->Initialize(DiscordTokenID);
	}

	g_pMain->SetVisible(btn_discord,Discord);
	g_pMain->SetVisible(btn_facebook,Facebook);

	if(g_pMain->pClientTopRightNewPlug!=NULL)
		g_pMain->pClientTopRightNewPlug->UpdateTopLeftVisible(Live, LiveURL);
}

void CUIHPBarMenuPlug::UpdatePosition()
{
	if (g_pMain->pClientUIBarPlug != NULL)
	{
		POINT eventNoticePos;
		g_pMain->GetUiPos(g_pMain->pClientUIBarPlug->m_dVTableAddr, eventNoticePos);

		RECT reg = g_pMain->GetUiRegion(g_pMain->pClientUIBarPlug->m_dVTableAddr);

		POINT myPos;
#if (HOOK_SOURCE_VERSION == 1098)
		myPos.x = reg.left + (reg.right - reg.left) - 70;
		myPos.y = reg.top;
#else
		myPos.x = reg.left + (reg.right - reg.left) - 32;
		myPos.y = reg.top + 15;
#endif
		g_pMain->SetUIPos(m_dVTableAddr, myPos);
	}
}

void __stdcall UIHpMenuReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->uiHpMenuPlug->ReceiveMessage(pSender, dwMsg);
	__asm
	{
		MOV ECX, uiHpMenu
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_HpMenu
		CALL EAX
	}
}

void CUIHPBarMenuPlug::OpenHpBarMenu()
{
	g_pMain->SetVisible(m_dVTableAddr, true);
	UpdatePosition();
}

void CUIHPBarMenuPlug::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_HpMenu = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)UIHpMenuReceiveMessage_Hook;
}