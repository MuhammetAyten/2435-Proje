#include "CastleSiegeWar.h"

CUICastleSiegeWar::CUICastleSiegeWar()
{
	m_iRemainingTime = 0;
	minimalize = false;
	set = false;
#if (HOOK_SOURCE_VERSION == 2369)
	txt_castle_owner = NULL;
	txt_csw_remaining_time = NULL;
	m_Timer = NULL;
	timeLapsProgress = 0;
	base_max = NULL;
	btn_max = NULL;
	txt_time = NULL;
	txt_clan = NULL;
	base_time = NULL;
	time = NULL;
#else
	txt_castle_owner = NULL;
	txt_csw_remaining_time = NULL;
#endif
	SRect = RECT();
}

CUICastleSiegeWar::~CUICastleSiegeWar()
{

}

bool CUICastleSiegeWar::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false)
		return false;
#if (HOOK_SOURCE_VERSION == 2369)
	base_max = (CN3UIBase*)GetChildByID(xorstr("base_max"));
	btn_max = (CN3UIButton*)GetChildByID(xorstr("btn_max"));
	txt_time = (CN3UIString*)base_max->GetChildByID(xorstr("txt_time"));
	txt_clan = (CN3UIString*)base_max->GetChildByID(xorstr("txt_clan"));
	base_time = (CN3UIBase*)base_max->GetChildByID(xorstr("base_time"));
	time = (CN3UIProgress*)base_time->GetChildByID(xorstr("time"));
	time->SetRange(0, 60);
	txt_time->SetString("");
#else
	txt_csw_remaining_time = (CN3UIString*)GetChildByID(xorstr("txt_csw_remaining_time"));
	txt_castle_owner = (CN3UIString*)GetChildByID(xorstr("txt_castle_owner"));
#endif
	RECT rc;
	SetRect(&rc, m_rcRegion.left, m_rcRegion.top, m_rcRegion.left + GetWidth(), m_rcRegion.top + GetHeight());
	SetRegion(rc);
	SetMoveRect(GetRegion());
	Close();
	return true;
}

bool CUICastleSiegeWar::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
#if (HOOK_SOURCE_VERSION == 2369)
	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == btn_max)
		{
			minimalize = !minimalize;
			base_max->SetVisible(minimalize ? false : true);
			return true;
		}
	}
#endif
	return true;
}

bool CheckChangeClientSize(RECT CRect, RECT SRect)
{
	return CRect.right != SRect.right
		|| CRect.left != SRect.left
		|| CRect.bottom != SRect.bottom
		|| CRect.top != SRect.top;
}

void CUICastleSiegeWar::Tick()
{
	if (!IsVisible())
		return;

#if (HOOK_SOURCE_VERSION == 1098)
	POINT koScreen = *(POINT*)0x00DE297C; RECT rct{};
	rct.left = 0;
	rct.top = 0;
	rct.right = koScreen.x;
	rct.bottom = koScreen.y;

	if (CheckChangeClientSize(rct, SRect)) {
		SRect = rct;
		POINT pos{};
		pos.x = (rct.right - rct.left) - 400;
		pos.y = (((rct.bottom - rct.top) / 2) / 2 / 2) - 50;
		SetPos(pos.x, pos.y);
	}
#endif

	uint64 remVal = 0;
	if (m_iRemainingTime > getMSTime())
		remVal = m_iRemainingTime - getMSTime();

	if (!remVal)
	{
		SetVisible(false);
		return;
	}

	int seconds = remVal / 1000;
	remVal %= 1000;
	int minutes = seconds / 60;
	seconds %= 60;

	if ((seconds + minutes <= 0))
	{
		SetVisible(false);
		return;
	}

	std::string remtime = "";
	if (minutes < 10 && seconds < 10)
		remtime = string_format(xorstr("0%d : 0%d"), minutes, seconds);
	else if (minutes < 10)
		remtime = string_format(xorstr("0%d : %d"), minutes, seconds);
	else if (seconds < 10)
		remtime = string_format(xorstr("%d : 0%d"), minutes, seconds);
	else
		remtime = string_format(xorstr("%d : %d"), minutes, seconds);
#if (HOOK_SOURCE_VERSION == 2369)
	txt_time->SetString(string_format("%s", remtime.c_str()));

	if (!set
		&& getMSTime() > timeLapsProgress
		&& IsVisible())
	{
		float n = 0.0f;
		if (m_iRemainingTime > getMSTime())
			n = (float)(m_iRemainingTime - getMSTime()) / 1000.0f;

		if (n >= 0.0f)
			time->SetCurValue(n, 0);
		else {
			set = true;
			time->SetCurValue(0, 0);
		}
		timeLapsProgress = getMSTime() + 500;
	}
#else
	txt_csw_remaining_time->SetString(string_format("Remaining Time : %s", remtime.c_str()));
#endif
}

void CUICastleSiegeWar::setOptions(uint32 remtime, std::string knights_name, uint8 OpStatus, uint32 war_time)
{
	if (!remtime) {
		m_iRemainingTime = 0;
		return;
	}

	if (knights_name.empty())
		knights_name = "''";

#if (HOOK_SOURCE_VERSION == 2369)
	set = false;
	time->SetRange(0, war_time * 60);
	m_iRemainingTime = (1000 * remtime) + getMSTime();

	float n = 0.0f;
	if (m_iRemainingTime > getMSTime())
		n = (float)(m_iRemainingTime - getMSTime()) / 1000.0f;
	if (n >= 0.0f) time->SetCurValue(n, 0);
	else time->SetCurValue(0, 0);

	if (txt_clan)
		txt_clan->SetString(knights_name);
#else
	set = false;
	m_iRemainingTime = (1000 * remtime) + getMSTime();
	if (txt_castle_owner)
		txt_castle_owner->SetString(knights_name);
#endif
	SetPos(g_pMain->pClientHookManager->GetScreenCenter(this).x, g_pMain->pClientHookManager->GetScreenCenter(this).y - 260);
	Open();
}

void CUICastleSiegeWar::Open()
{
#if (HOOK_SOURCE_VERSION == 2369)
	base_max->SetVisible(true);
#endif
	SetVisible(true);
}

void CUICastleSiegeWar::Close()
{
	m_iRemainingTime = 0;
#if (HOOK_SOURCE_VERSION == 2369)
	if (txt_clan) txt_clan->SetString("");
#else
	if (txt_castle_owner) txt_castle_owner->SetString("");
#endif
	SetVisible(false);
}