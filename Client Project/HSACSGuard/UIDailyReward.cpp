#include "stdafx.h"
#include "../DateTime.h"
#include "UIDailyReward.h"

//#if(HOOK_SOURCE_VERSION == 2369)
CUIDailyRewardPlug::CUIDailyRewardPlug()
{
	vector<int>offsets;
	offsets.push_back(0x248);
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);

	kills = 1;
	TotalLogging = 0;

	ParseUIElements();
	InitReceiveMessage();
}

CUIDailyRewardPlug::~CUIDailyRewardPlug()
{
}

void CUIDailyRewardPlug::ParseUIElements()
{
	DWORD group_daily_rewards;
	DWORD group_cumulative_rewards;

	std::string find = xorstr("btn_close");
	btn_close = g_pMain->GetChildByID(m_dVTableAddr, find);

	find = xorstr("str_remaining_time");
	str_remaining_time = g_pMain->GetChildByID(m_dVTableAddr, find);

	find = xorstr("group_daily_rewards");
	group_daily_rewards = g_pMain->GetChildByID(m_dVTableAddr, find);

	find = xorstr("group_cumulative_rewards");
	group_cumulative_rewards = g_pMain->GetChildByID(m_dVTableAddr, find);

	find = xorstr("group_select");
	group_select = g_pMain->GetChildByID(m_dVTableAddr, find);

	find = xorstr("img_select");
	img_select = g_pMain->GetChildByID(group_select, find);
	float fUVAspect = (float)45.0f / (float)64.0f;

	for (int i = 0; i < 25; i++)
	{
		std::string find = string_format(xorstr("group_daily_reward_%d"), i);
		group_daily_reward[i] = g_pMain->GetChildByID(group_daily_rewards, find);

		find = xorstr("str_item_reward_count");
		str_item_reward_count[i] = g_pMain->GetChildByID(group_daily_reward[i], find);
		find = xorstr("str_days");
		str_days[i] = g_pMain->GetChildByID(group_daily_reward[i], find);
		find = xorstr("str_item_reward");
		str_item_reward[i] = g_pMain->GetChildByID(group_daily_reward[i], find);

		find = xorstr("img_complete");
		img_complete[i] = g_pMain->GetChildByID(group_daily_reward[i], find);
		find = xorstr("img_accept");
		img_accept[i] = g_pMain->GetChildByID(group_daily_reward[i], find);
		find = xorstr("img_item_reward");
		img_item_reward[i] = g_pMain->GetChildByID(group_daily_reward[i], find);
		DWORD bCreateIcon = 0x00;

		if (!g_pMain->_InitIcon(bCreateIcon, img_item_reward[i], 910014000))
			continue;

		ItemInfo* inf = new ItemInfo();
		inf->icon = bCreateIcon;
		inf->nItemID = 0;
		g_pMain->SetRegion(inf->icon, img_item_reward[i]);
		items.push_back(inf);
	}

	for (int i = 0; i < 3; i++)
	{
		std::string find = string_format(xorstr("group_cumulative_reward_%d"), i);
		group_cumulative_reward[i] = g_pMain->GetChildByID(group_cumulative_rewards, find);

		find = xorstr("img_item_reward");
		img_item_cumulative_reward[i] = g_pMain->GetChildByID(group_cumulative_reward[i],(find));
		find = xorstr("str_item_reward");
		str_item_reward_cumulative[i] = g_pMain->GetChildByID(group_cumulative_reward[i], (find));

		DWORD bCreateIcon = 0x00;

		if (!g_pMain->_InitIcon(bCreateIcon, img_item_cumulative_reward[i], 910014000))
			continue;

		ItemInfo* inf = new ItemInfo();
		inf->icon = bCreateIcon;
		inf->nItemID = 0;
		g_pMain->SetRegion(inf->icon, img_item_cumulative_reward[i]);
		Cumulative.push_back(inf);
	}

	for (int i = 0; i < 25; i++)
	{
		g_pMain->SetVisible(img_complete[i], false);
		g_pMain->SetString(str_item_reward_count[i], xorstr("1"));
	}

	g_pMain->UIScreenCenter(m_dVTableAddr);
	m_Timer = new CTimer(false);
}

DWORD pClientDailyReward;
DWORD DailyReward;

void CUIDailyRewardPlug::Open()
{
	g_pMain->SetVisible(m_dVTableAddr, true);
}

void CUIDailyRewardPlug::Close()
{
	g_pMain->SetVisible(m_dVTableAddr, false);
}

void CUIDailyRewardPlug::OnKeyPress(int iKey)
{
	if (!g_pMain->IsVisible(m_dVTableAddr))
		return;

	switch (iKey)
	{
	case DIK_ESCAPE:
		Close();
	}
}

uint32_t CUIDailyRewardPlug::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;
	if (g_pMain->IsVisible(g_pMain->pClientDailyReward->m_dVTableAddr))
	{
		if (dwFlags == UI_MOUSE_LBCLICKED)
		{
			for (int i = 0; i < 25; i++)
			{
				if (g_pMain->IsIn(group_daily_reward[i], ptCur.x, ptCur.y))
				{
					Packet result(WIZ_HSACS_HOOK, uint8(HSACSOpCodes::WIZ_DAILY_REWARD));
					result << uint8(1) << items[i]->nItemID;
					g_pMain->Send(&result);
				}
			}

			for (int i = 0; i < 3; i++)
			{
				if (g_pMain->IsIn(img_item_cumulative_reward[i],ptCur.x, ptCur.y))
				{
					if (TotalLogging < 15 && i == 0)
						return 0;
					else if (TotalLogging < 20 && i == 1)
						return 0;
					else if (TotalLogging < 25 && i == 2)
						return 0;

					/*	Packet pkt(CyberACS, uint8(PL_DAILY_REWARD));
						pkt << uint8(3) << Cumulative[i]->nItemID;
						g_pMain->Send(&pkt);*/
				}
			}
		}
		else
		{
			for (int i = 0; i < 25; i++)
			{
				if (g_pMain->IsIn(group_daily_reward[i], ptCur.x, ptCur.y))
				{
					POINT setui, getui, getui2;
					getui = g_pMain->GetUiPos(group_daily_reward[i]);
					getui2 = g_pMain->GetUiPos(group_daily_reward[i]);
					setui.x = getui2.x;
					setui.y = getui.y;
					g_pMain->SetUIPos(img_select, setui);
				}
			}
		}

		bool showTooltip = false;
		for (auto it : items)
		{
			if (g_pMain->IsIn(it->icon, ptCur.x, ptCur.y) && it->tbl != nullptr)
			{
				g_pMain->ShowToolTipEx(it->nItemID, ptCur.x, ptCur.y);
				break;
			}
		}

		for (auto it : Cumulative)
		{
			if (g_pMain->IsIn(it->icon, ptCur.x, ptCur.y) && it->tbl != nullptr)
			{
				g_pMain->ShowToolTipEx(it->nItemID, ptCur.x, ptCur.y);
				break;
			}
		}
	}
	return dwRet;
}

void CUIDailyRewardPlug::Update(uint32 Itemid[], uint8 isUsed[], uint8 sDays[], uint32 sCumulative[])
{
	int b = 0;
	DateTime sTime;
	uint8 sGetDay = sTime.GetDay();

	for (int i = 0; i < 3; i++)
	{
		TABLE_ITEM_BASIC* tbl = g_pMain->pClientTBLManager->GetItemData(sCumulative[i]);
		if (tbl == nullptr)
		{
			if (sCumulative[i] > 0 && sCumulative[i] < 10000) // item group
			{
				g_pMain->SetTexImage(Cumulative[i]->icon, xorstr("UI\\itemicon_group.dxt"));
				Cumulative[i]->tbl = nullptr;
				Cumulative[i]->nItemID = 0;
				g_pMain->SetString(str_item_reward_cumulative[i],xorstr("No Item"));
			}
			else
				continue;
		}
		else
		{
			std::vector<char> buffer(256, NULL);
			sprintf(&buffer[0], "ui\\itemicon_%.1d_%.4d_%.2d_%.1d.dxt",
				(tbl->dxtID / 10000000),
				(tbl->dxtID / 1000) % 10000,
				(tbl->dxtID / 10) % 100,
				tbl->dxtID % 10);

			std::string szIconFN = &buffer[0];
			g_pMain->SetTexImage(Cumulative[i]->icon, szIconFN);
			Cumulative[i]->tbl = tbl;
			Cumulative[i]->nItemID = sCumulative[i];
			g_pMain->SetString(str_item_reward_cumulative[i], tbl->Description.c_str());
		}
	}

	for (int i = 0; i < 25; i++)
	{
		if (isUsed[i] == 1)
		{
			g_pMain->SetVisible(img_complete[i], true);
			g_pMain->SetVisible(img_accept[i], false);
			TotalLogging++;
		}
		else if (isUsed[i] == 0)
		{
			g_pMain->SetVisible(img_complete[i], false);
			g_pMain->SetVisible(img_accept[i], true);
		}

		if (sDays[i] <= sGetDay && sDays[i] != 0 && isUsed[i] == 1)
			g_pMain->SetString(str_days[i], xorstr("Acquired"));
		else if (sDays[i] <= sGetDay && sDays[i] != 0)
			g_pMain->SetString(str_days[i],xorstr("Avabile")); 
		else
		{
			b++;
			if (b == 1)
				g_pMain->SetString(str_days[i], xorstr("1 day"));
			else
			{
				string asd = string_format(("%d day(s)"), b);
				g_pMain->SetString(str_days[i], asd.c_str());
			}
		}

		TABLE_ITEM_BASIC* tbl = g_pMain->pClientTBLManager->GetItemData(Itemid[i]);
		if (tbl == nullptr)
		{
			if (Itemid[i] > 0 && Itemid[i] < 10000) // item group
			{
				g_pMain->SetTexImage(items[i]->icon, xorstr("UI\\itemicon_group.dxt"));
				items[i]->tbl = nullptr;
				items[i]->nItemID = 0;
				g_pMain->SetString(str_item_reward[i],xorstr("No Item"));
			}
			else
				continue;
		}
		else
		{
			std::vector<char> buffer(256, NULL);
			sprintf(&buffer[0], "ui\\itemicon_%.1d_%.4d_%.2d_%.1d.dxt",
				(tbl->dxtID / 10000000),
				(tbl->dxtID / 1000) % 10000,
				(tbl->dxtID / 10) % 100,
				tbl->dxtID % 10);

			std::string szIconFN = &buffer[0];
			g_pMain->SetTexImage(items[i]->icon, szIconFN);
			items[i]->tbl = tbl;
			items[i]->nItemID = Itemid[i];
			g_pMain->SetString(str_item_reward[i], tbl->Description.c_str());
		}
	}
}

bool CUIDailyRewardPlug::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	pClientDailyReward = m_dVTableAddr;

	if (!pSender 
		|| pSender == 0 
		|| dwMsg != UIMSG_BUTTON_CLICK)
		return false;

	if (pSender == (DWORD*)btn_close)
		Close();
}

uint32 UpdateTime()
{
	DateTime sTime;
	return ((sTime.GetHour() * 60 * 60) + (sTime.GetMinute() * 60) + sTime.GetSecond());
}
void CUIDailyRewardPlug::Tick()
{
	if (!m_Timer)
		return;

	if (m_Timer->IsElapsedSecond())
	{
		uint32 sOldSeconds = 0;
		sOldSeconds = uint32(86400 - UpdateTime());
		time_t m_iRemainingTime = sOldSeconds;

		if (m_iRemainingTime == 0)
		{
			Close();
			return;
		}

		uint16_t sHourse = (uint16_t)ceil((m_iRemainingTime / 60) / 60);
		uint16_t sMinutes = (uint16_t)ceil((m_iRemainingTime / 60));
		uint16_t sSeconds = m_iRemainingTime - (sMinutes * 60);
		uint16_t m_sMinutes;
		m_sMinutes = (m_iRemainingTime - (sHourse * 3600)) / 60;
		std::string remainingTime;

		if (m_sMinutes < 10 && sSeconds < 10 && sHourse < 10)
			remainingTime = string_format(xorstr("0%d : 0%d : 0%d"), sHourse, m_sMinutes, sSeconds);
		else if (sHourse < 10 && m_sMinutes < 10)
			remainingTime = string_format(xorstr("0%d : 0%d : %d"), sHourse, m_sMinutes, sSeconds);
		else if (sHourse < 10 && sSeconds < 10)
			remainingTime = string_format(xorstr("0%d : %d : 0%d"), sHourse, m_sMinutes, sSeconds);
		else if (sHourse < 10)
			remainingTime = string_format(xorstr("0%d : %d : %d"), sHourse, m_sMinutes, sSeconds);
		else if (m_sMinutes < 10 && sSeconds < 10)
			remainingTime = string_format(xorstr("%d : 0%d : 0%d"), sHourse, m_sMinutes, sSeconds);
		else if (sSeconds < 10)
			remainingTime = string_format(xorstr("%d : %d : 0%d"), sHourse, m_sMinutes, sSeconds);
		else if (m_iRemainingTime <= 0)
			remainingTime = string_format(xorstr("00 : 00 : 00"));
		else
			remainingTime = string_format(xorstr("%d : %d : %d"), sHourse, m_sMinutes, sSeconds);

		g_pMain->SetString(str_remaining_time, string_format(xorstr("Remaining Time %s"), remainingTime.c_str()));
	}
}

void __stdcall UIDailyRewardReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->pClientDailyReward->ReceiveMessage(pSender, dwMsg);

	__asm
	{
		MOV ECX, pClientDailyReward
		PUSH dwMsg
		PUSH pSender
		MOV EAX, DailyReward
		CALL EAX
	}
}

void CUIDailyRewardPlug::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	DailyReward = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)UIDailyRewardReceiveMessage_Hook;
}
//#endif