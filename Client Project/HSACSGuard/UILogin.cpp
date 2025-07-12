#include "UILogin.h"

uint32 idLastState, pwLastState;

CUILogin::CUILogin()
{
	if (g_pMain->m_SettingsMgr == NULL) {
		g_pMain->m_SettingsMgr = new CSettingsManager();
		g_pMain->m_SettingsMgr->Init();
	}

	vector<int>offsets;
	offsets.push_back(0x30);
	offsets.push_back(0); //offsettt

	m_dVTableAddr = g_pMain->rdword(0x00F368F4, offsets);

	m_bGroupLogin = 0;
	m_btnRememberID = 0;
	m_EditPW = 0;
	m_EditUID = 0;
	m_btnLogin = 0;

	ParseUIElements();
}

CUILogin::~CUILogin()
{
	m_bGroupLogin = 0;
	m_btnRememberID = 0;
	m_EditPW = 0;
	m_EditUID = 0;
	m_btnLogin = 0;
}

DWORD uiLoginVTable;
DWORD Func_UILogin = 0;
void __stdcall UiLoginReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->uiLogin->ReceiveMessage(pSender, dwMsg);
	__asm
	{
		MOV ECX, uiLoginVTable
		PUSH dwMsg
		PUSH pSender
		CALL Func_UILogin
	}
}
extern DWORD KO_UI_SCREEN_SIZE;
void CUILogin::ParseUIElements()
{
	m_bGroupLogin = g_pMain->GetChildByID(m_dVTableAddr, "Group_Login");

	if (m_bGroupLogin == NULL) {
		return;
	}

	m_btnRememberID = g_pMain->GetChildByID(m_bGroupLogin, "btn_remember");
	idLastState = g_pMain->GetState(m_btnRememberID);

	m_btnLogin = g_pMain->GetChildByID(m_bGroupLogin, "btn_ok");
	m_EditUID = g_pMain->GetChildByID(m_bGroupLogin, "Edit_ID");
	m_EditPW = g_pMain->GetChildByID(m_bGroupLogin, "Edit_PW");
	Group_ServerList_01 = g_pMain->GetChildByID(m_dVTableAddr, "Group_ServerList_01");
	Group_introduction_el = g_pMain->GetChildByID(m_dVTableAddr, "Group_introduction_el");
	Group_introduction_ka = g_pMain->GetChildByID(m_dVTableAddr, "Group_introduction_ka");
	premium = g_pMain->GetChildByID(m_dVTableAddr, "premium");

	if (g_pMain->m_SettingsMgr->m_uID.length() > 0 && m_EditUID)
	{
		g_pMain->SetEditString(m_EditUID, g_pMain->m_SettingsMgr->m_uID);
	}

	if (g_pMain->m_SettingsMgr->m_uPW.length() > 0 && m_EditPW)
		g_pMain->SetEditString(m_EditPW, g_pMain->m_SettingsMgr->m_uPW);

	if (g_pMain->m_SettingsMgr->m_uID.length() > 0 || g_pMain->m_SettingsMgr->m_uPW.length() > 0)
		g_pMain->SetState(m_btnRememberID, UI_STATE_BUTTON_DOWN);

	InitReceiveMessage();

	POINT koScreen = *(POINT*)KO_UI_SCREEN_SIZE;

#if (HOOK_SOURCE_VERSION == 1098)
	if (true)
	{
		POINT ret;
		POINT org = g_pMain->GetUiPos(m_bGroupLogin);
		ret.x = org.x;
		ret.y = koScreen.y - g_pMain->GetUiHeight(m_bGroupLogin);
		g_pMain->SetUIPos(m_bGroupLogin, ret);
	}
	if (true)
	{
		POINT ret;
		POINT org = g_pMain->GetUiPos(Group_ServerList_01);
		ret.x = 0;
		ret.y = 0;
		g_pMain->SetUIPos(Group_ServerList_01, ret);
		ret.y = 0;
		ret.x = koScreen.x - g_pMain->GetUiWidth(Group_introduction_el);
		g_pMain->SetUIPos(Group_introduction_el, ret);
		
		ret.y = g_pMain->GetUiHeight(Group_introduction_el) + 25 ;
		g_pMain->SetUIPos(Group_introduction_ka, ret);
		
		RECT region = g_pMain->GetUiRegion(premium);
	
		ret.x = (koScreen.x / 2) - 513;
		ret.y = 0;
		g_pMain->SetUIPos(premium, ret);
	}
#elif (HOOK_SOURCE_VERSION == 1534)
	if (true)
	{
		POINT ret;
		POINT org = g_pMain->GetUiPos(m_bGroupLogin);
		ret.x = 710;
		ret.y = 150;
		g_pMain->SetUIPos(m_bGroupLogin, ret);
	}
	if (true)
	{
			POINT ret;
			POINT org = g_pMain->GetUiPos(Group_ServerList_01);
			ret.x = 710;
			ret.y = 200;
			g_pMain->SetUIPos(Group_ServerList_01, ret);
		
	}
#endif
}

bool CUILogin::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	uiLoginVTable = m_dVTableAddr;
	if (!pSender || pSender == 0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;

	if (pSender == (DWORD*)m_btnLogin)
	{
		if (g_pMain->GetState(m_btnRememberID) == UI_STATE_BUTTON_DOWN)
		{
			g_pMain->m_SettingsMgr->m_uID = g_pMain->GetEditString(m_EditUID);
			g_pMain->m_SettingsMgr->m_uPW = g_pMain->GetEditString(m_EditPW);
			g_pMain->m_SettingsMgr->Save();
		}
		else {
			g_pMain->m_SettingsMgr->m_uID = "";
			g_pMain->m_SettingsMgr->m_uPW = "";
			g_pMain->m_SettingsMgr->Save();
		}

		/*std::string accountID = g_pMain->GetEditString(m_EditUID);
		if (accountID.size())
		{
			g_pMain->accountID = accountID;
			STRTOUPPER(g_pMain->accountID);
		}*/
	}

	return true;
}

void CUILogin::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_UILogin = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)UiLoginReceiveMessage_Hook;
}