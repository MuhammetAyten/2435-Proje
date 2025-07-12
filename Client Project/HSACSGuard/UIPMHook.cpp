#include "UIPMHook.h"
extern std::string name;
extern DWORD rdwordExt(DWORD ulBase);

DWORD verifyBase;

HookPM::HookPM(DWORD base, bool bOpenWindow)
{
	el_Base = base;
	m_bOpenWindow = bOpenWindow;
	exit_id = 0;
	btn_close = 0;
	user_id = "";
	m_canTick = true;
	m_LastTick = GetTickCount();
	m_LastSeenSend = GetTickCount();
	m_bcolored = false;

	ParseUIElements();
}

HookPM::~HookPM()
{
	user_id = "";
	exit_id = 0;
	btn_close = 0;
	m_LastTick = 0;
	m_LastSeenSend = 0;
	m_canTick = false;
	m_bcolored = false;
}

void HookPM::Tick()
{
	if (GetTickCount() - m_LastTick <= 20)
		return;

	m_LastTick = GetTickCount();

	_CHAT_USER_MAP * pPmMap = g_pMain->pClientChatUser.GetData(user_id);
	if (pPmMap == nullptr)
	{
		exit_id = 0;
		m_bcolored = false;
		return;
	}

	if (exit_id == 0) 
		return;
	
	if (!m_bcolored)
		return;

	name = g_pMain->GetString(exit_id);
	if (name.size() <= 0)
	{
		exit_id = 0;
		m_bcolored = false;
		return;
	}

	if (pPmMap->btype == 20)
		g_pMain->SetStringColor(exit_id, D3DCOLOR_XRGB(223, 52, 52));
	else if (pPmMap->btype == 21)
		g_pMain->SetStringColor(exit_id, D3DCOLOR_XRGB(255, 0, 0));
}

void HookPM::ParseUIElements()
{
	std::string find = xorstr("exit_id");
	g_pMain->GetChildByID(el_Base, find, exit_id);

	find = xorstr("str_mcount");
	g_pMain->GetChildByID(el_Base, find, str_msgcount);
}