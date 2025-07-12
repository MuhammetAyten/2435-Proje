#include "stdafx.h"
#include "UITaskbarMain.h"

CUITaskbarMainPlug::CUITaskbarMainPlug()
{
	vector<int>offsets;
	offsets.push_back(0x3D8);
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);
	m_btnPPCard = 0;
	m_mobdrop = 0;
	m_dailyquest = 0;
	btn_wof = 0;
	base_menu = 0;
	ParseUIElements();
	InitReceiveMessage();
	UpdatePosition();
}
extern DWORD KO_UI_SCREEN_SIZE;
void CUITaskbarMainPlug::UpdatePosition()
{
#if (HOOK_SOURCE_VERSION == 1098)

	POINT koScreen = *(POINT*)KO_UI_SCREEN_SIZE;
	RECT region = g_pMain->GetUiRegion(m_dVTableAddr);
	POINT orta;
	LONG w = (region.right - region.left);
	orta.x = (koScreen.x / 2) - 513;
	orta.y = region.top;
	g_pMain->SetUIPos(m_dVTableAddr, orta);
#endif
}

CUITaskbarMainPlug::~CUITaskbarMainPlug()
{
}

void CUITaskbarMainPlug::ParseUIElements()
{
	DWORD parent = 0;

	base_menu = g_pMain->GetChildByID(m_dVTableAddr, "base_menu");
	str_exp = g_pMain->GetChildByID(m_dVTableAddr, "str_exp");
	m_btnMenu = g_pMain->GetChildByID(base_menu, "btn_menu");
	m_btnRank = g_pMain->GetChildByID(base_menu, "btn_rank");

#if (HOOK_SOURCE_VERSION != 1098)	
	m_mobdrop = g_pMain->GetChildByID(base_menu, "btn_mobsearch");
	btn_wof = g_pMain->GetChildByID(base_menu, "btn_wof");
	m_dailyquest = g_pMain->GetChildByID(base_menu, "btn_daily_quest");
#endif
	base_exp = g_pMain->GetChildByID(m_dVTableAddr, "base_exp");
	base_TaskBar = g_pMain->GetChildByID(m_dVTableAddr, "base_TaskBar");
	m_btn00Stand = g_pMain->GetChildByID(base_TaskBar, "btn_00");
	m_btn01Sit = g_pMain->GetChildByID(base_TaskBar, "btn_01");
	m_btn02Seek = g_pMain->GetChildByID(base_TaskBar, "btn_02");
	m_btn03Trade = g_pMain->GetChildByID(base_TaskBar, "btn_03");
	m_btn04SkillK = g_pMain->GetChildByID(base_TaskBar, "btn_04");
	m_btn05Character = g_pMain->GetChildByID(base_TaskBar, "btn_05");
	m_btn06inventory = g_pMain->GetChildByID(base_TaskBar, "btn_06");
#if (HOOK_SOURCE_VERSION == 1098)	
	base_oldbuttons = g_pMain->GetChildByID(m_dVTableAddr, "base_oldbuttons");
	m_btninventory = g_pMain->GetChildByID(base_oldbuttons, "btn_inventory");
	m_btncharacter = g_pMain->GetChildByID(base_oldbuttons, "btn_character");
	m_btnskill = g_pMain->GetChildByID(base_oldbuttons, "btn_skill");
	m_btntrade = g_pMain->GetChildByID(base_oldbuttons, "btn_trade");
	m_btncommand = g_pMain->GetChildByID(base_oldbuttons, "btn_command");
	m_btninvite = g_pMain->GetChildByID(base_oldbuttons, "btn_invite");
	g_pMain->SetVisible(m_btninvite, true);
	g_pMain->SetState(m_btninvite, UI_STATE_BUTTON_NORMAL);

	m_btnattack = g_pMain->GetChildByID(base_oldbuttons, "btn_attack");
	m_btnwalk = g_pMain->GetChildByID(base_oldbuttons, "btn_walk");
	m_btnrun = g_pMain->GetChildByID(base_oldbuttons, "btn_run");
	m_btnsit = g_pMain->GetChildByID(base_oldbuttons, "btn_sit");

	g_pMain->SetVisible(m_btnsit, true);
	g_pMain->SetState(m_btnsit, UI_STATE_BUTTON_NORMAL);

	m_btnstand = g_pMain->GetChildByID(base_oldbuttons, "btn_stand");

	g_pMain->SetVisible(m_btnstand, false);
	g_pMain->SetState(m_btnstand, UI_STATE_BUTTON_DISABLE);

	m_btncamera = g_pMain->GetChildByID(base_oldbuttons, "btn_camera");
	m_btndisband = g_pMain->GetChildByID(base_oldbuttons, "btn_disband");

	g_pMain->SetVisible(m_btndisband, false);
	g_pMain->SetState(m_btndisband, UI_STATE_BUTTON_DISABLE);

	m_btnexit = g_pMain->GetChildByID(base_oldbuttons, "btn_exit");
	m_btnmap = g_pMain->GetChildByID(base_oldbuttons, "btn_map");
#endif
}


DWORD uiTaskbarMainVTable;
DWORD Func_Taskbar = 0;
void __stdcall UITaskbarMainReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->uiTaskbarMain->ReceiveMessage(pSender, dwMsg);
	__asm
	{
		MOV ECX, uiTaskbarMainVTable
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_Taskbar
		CALL EAX
	}
}

void CUITaskbarMainPlug::OpenSeed()
{
	UITaskbarMainReceiveMessage_Hook((DWORD*)m_btnRank, UIMSG_BUTTON_CLICK);
}

bool CUITaskbarMainPlug::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	uiTaskbarMainVTable = m_dVTableAddr;
	if (!pSender || pSender == 0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;

#if (HOOK_SOURCE_VERSION == 1098)	
	if (pSender == (DWORD*)m_btnmap)
	{
		if (g_pMain->uiTaskbarSub != NULL)
		{
			if (g_pMain->uiMinimapPlug) 
			{
				if(DWORD map = g_pMain->uiMinimapPlug->m_dVTableAddr) 
				{
					if (*(bool*)(map + 0xF9))
					{
						__asm 
						{
							pushad
							pushfd
							mov ecx, map
							mov eax, 0x846C40
							call eax
							popfd
							popad
						}
					}
					else 
					{
						__asm 
						{
							pushad
							pushfd
							mov ecx, map
							mov eax, 0x846E00
							call eax
							popfd
							popad
						}
					}
				}
			}
		}
	}
	else if (pSender == (DWORD*)m_btninventory)
	{
		if (g_pMain->uiTaskbarMain != NULL)
			g_pMain->uiTaskbarMain->OpenInventory();
	}
	else if (pSender == (DWORD*)m_btncharacter)
	{
		if (g_pMain->uiTaskbarMain != NULL)
			g_pMain->uiTaskbarMain->OpenCharacterU();
	}
	else if (pSender == (DWORD*)m_btnskill)
	{
		if (g_pMain->uiTaskbarMain != NULL)
			g_pMain->uiTaskbarMain->OpenSkill();
	}
	else if (pSender == (DWORD*)m_btnstand)
	{
		if (g_pMain->uiTaskbarMain != NULL)
			g_pMain->uiTaskbarMain->OpenStand();
	}
	else if (pSender == (DWORD*)m_btnsit)
	{
		if (g_pMain->uiTaskbarMain != NULL)
			g_pMain->uiTaskbarMain->OpenSit();
	}
	else if (pSender == (DWORD*)m_btntrade)
	{
		if (g_pMain->uiTaskbarMain != NULL)
			g_pMain->uiTaskbarMain->OpenTrade();
	}
	else if (pSender == (DWORD*)m_btncommand)
	{
		if (g_pMain->uiTaskbarSub != NULL)
			g_pMain->uiTaskbarSub->OpenHotkeyH();
	}
	else if (pSender == (DWORD*)m_btninvite)
	{
		if (g_pMain->uiTaskbarSub != NULL)
			g_pMain->uiTaskbarSub->OpenInviteParty();
	}
	else if (pSender == (DWORD*)m_btnexit)
	{
		if (g_pMain->uiTaskbarSub != NULL)
			g_pMain->uiTaskbarSub->OpenExitGame();
	}
	else if (pSender == (DWORD*)m_btndisband)
	{
		if (g_pMain->uiPartyBBS != NULL)
			g_pMain->uiPartyBBS->OpenPartyDisband();
	}
	else if (pSender == (DWORD*)m_btnrun)
	{
		g_pMain->ChangeRunState();
		g_pMain->SetVisible(m_btnrun, false);
		g_pMain->SetVisible(m_btnwalk, true);
	}
	else if (pSender == (DWORD*)m_btnwalk)
	{
		g_pMain->ChangeRunState();
		g_pMain->SetVisible(m_btnrun, true);
		g_pMain->SetVisible(m_btnwalk, false);
	}
	else if (pSender == (DWORD*)m_btncamera)
		g_pMain->CommandCameraChange();
	else if (pSender == (DWORD*)m_btnattack)   
	{
		if (DWORD target = g_pMain->GetTarget()) 
		{
			__asm {	
				push target
				push 1
				mov ecx, [KO_DLG]
				mov ecx, [ecx]
				mov eax, 0x00535550
				call eax
			}
		}
	}
#else
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
	else if (pSender == (DWORD*)btn_wof)
		g_pMain->pClientWheelOfFunPlug->OpenWheel();
#endif
	return true;
}

void CUITaskbarMainPlug::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_Taskbar = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)UITaskbarMainReceiveMessage_Hook;
}

void CUITaskbarMainPlug::OpenPPCard()
{
	UITaskbarMainReceiveMessage_Hook((DWORD*)m_btnPPCard, UIMSG_BUTTON_CLICK);
}

#if (HOOK_SOURCE_VERSION == 1098)	
void CUITaskbarSubPlug::OpenSeedHelper()
{
	UITaskbarMainReceiveMessage_Hook((DWORD*)m_btnQuestion, UIMSG_BUTTON_CLICK);
}

void CUITaskbarMainPlug::OpenMapButton()
{
	UITaskbarMainReceiveMessage_Hook((DWORD*)m_btnMenu, UIMSG_BUTTON_CLICK);
}
void CUITaskbarMainPlug::OpenStand()
{
	UITaskbarMainReceiveMessage_Hook((DWORD*)m_btn00Stand, UIMSG_BUTTON_CLICK);
}
void CUITaskbarMainPlug::OpenSit()
{
	UITaskbarMainReceiveMessage_Hook((DWORD*)m_btn01Sit, UIMSG_BUTTON_CLICK);
}
void CUITaskbarMainPlug::OpenTrade()
{
	UITaskbarMainReceiveMessage_Hook((DWORD*)m_btn03Trade, UIMSG_BUTTON_CLICK);
}
void CUITaskbarMainPlug::OpenSkill()
{
	UITaskbarMainReceiveMessage_Hook((DWORD*)m_btn04SkillK, UIMSG_BUTTON_CLICK);
}
void CUITaskbarMainPlug::OpenCharacterU()
{
	UITaskbarMainReceiveMessage_Hook((DWORD*)m_btn05Character, UIMSG_BUTTON_CLICK);
}
void CUITaskbarMainPlug::OpenInventory()
{
	UITaskbarMainReceiveMessage_Hook((DWORD*)m_btn06inventory, UIMSG_BUTTON_CLICK);
}
#endif