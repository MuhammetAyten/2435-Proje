#include "stdafx.h"
#include "hLotteryEvent.h"

CUILotteryEventPlug::CUILotteryEventPlug()
{
	vector<int>offsets;
	offsets.push_back(0x5A0);   // co_report_questionnaire Satýr : 276
	offsets.push_back(0);
	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);
	for (int i = 0; i < 9; i++)
		itemIDs[i] = 0;
	m_Timer = NULL;
	str_remaining = NULL;
	ticket_buy_count = NULL;
	str_participant = NULL;
	ParseUIElements();
	InitReceiveMessage();
}

CUILotteryEventPlug::~CUILotteryEventPlug()
{
}

void CUILotteryEventPlug::ParseUIElements()
{
	base_default = g_pMain->GetChildByID(m_dVTableAddr, "base_default");
	group_reqs = g_pMain->GetChildByID(m_dVTableAddr, "group_reqs");
	group_rewards = g_pMain->GetChildByID(m_dVTableAddr, "group_rewards");
	group_details = g_pMain->GetChildByID(m_dVTableAddr, "group_details");
	btn_close = g_pMain->GetChildByID(m_dVTableAddr, "btn_exit");
	ticket_buy_count = g_pMain->GetChildByID(group_reqs, "text_ticket_buy_count");
	reqSlots = g_pMain->GetChildByID(group_reqs, "slot1");

	for (int i = 0; i < 4; i++) {
		rewardSlots[i]=g_pMain->GetChildByID(group_rewards, string_format("icon%d", i + 1));
		g_pMain->SetVisible(rewardSlots[i], false);
	}

	str_participant = g_pMain->GetChildByID(group_details, "str_participant");
	str_ticket = g_pMain->GetChildByID(group_details, "str_ticket");
#if (HOOK_SOURCE_VERSION == 1098 || HOOK_SOURCE_VERSION == 1534)
	str_remaining = g_pMain->GetChildByID(m_dVTableAddr, "str_remaining");
#else
	str_remaining = g_pMain->GetChildByID(group_details, "str_remaining");
#endif
	btn_join = g_pMain->GetChildByID(group_reqs, "btn_join");
	for (int i = 0; i < 9; i++)
		itemTbl[i] = nullptr;

	g_pMain->UIScreenCenter(m_dVTableAddr);
}

void CUILotteryEventPlug::Update(uint32 reqs[5], uint32 reqsCount[5], uint32 rewards[4], bool reset)
{
	if (reset)
	{
		g_pMain->SetVisible(reqSlots, false);
		
		for (int i = 0; i < 5; i++) {
			itemIDs[i] = 0;
			itemTbl[i] = nullptr;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		itemIDs[i] = reqs[i];
		if (reqs[i] == 0)
			continue;

		TABLE_ITEM_BASIC* tbl = g_pMain->pClientTBLManager->GetItemData(reqs[i]);
		if (!tbl)
		{
			itemTbl[i] = nullptr;
			continue;
		}

		auto pItemExt = g_pMain->pClientTBLManager->GetExtData(tbl->extNum, reqs[i]);
		if (TABLE_ITEM_EXT* ext = g_pMain->pClientTBLManager->GetExtData(tbl->extNum, rewards[i]))
		{
			DWORD dwIconID = tbl->dxtID;
			if (ext->dxtID > 0)
				dwIconID = ext->dxtID;

			std::string dxt = string_format(xorstr("UI\\ItemIcon_%.1d_%.4d_%.2d_%.1d.dxt"), dwIconID / 10000000u, dwIconID / 1000u % 10000, dwIconID / 10u % 100, dwIconID % 10);

			g_pMain->SetTexImage(reqSlots,dxt);
		}
		else {
			std::vector<char> buffer(256, NULL);

			sprintf(&buffer[0], "ui\\itemicon_%.1d_%.4d_%.2d_%.1d.dxt",
				(tbl->dxtID / 10000000),
				(tbl->dxtID / 1000) % 10000,
				(tbl->dxtID / 10) % 100,
				tbl->dxtID % 10);

			std::string szIconFN = &buffer[0];


				g_pMain->SetTexImage(reqSlots, szIconFN);
		}
		g_pMain->SetVisible(reqSlots,true);
		itemTbl[i] = tbl;
	}

	for (int i = 0; i < 4; i++)
	{
		itemIDs[i + 5] = rewards[i];
		if (rewards[i] == 0)
			continue;

		TABLE_ITEM_BASIC* tbl = g_pMain->pClientTBLManager->GetItemData(rewards[i]);
		if (!tbl)
		{
			itemTbl[i + 5] = nullptr;
			continue;
		}


		if (TABLE_ITEM_EXT* ext = g_pMain->pClientTBLManager->GetExtData(tbl->extNum, rewards[i]))
		{
			DWORD dwIconID = tbl->dxtID;
			if (ext->dxtID > 0)
				dwIconID = ext->dxtID;

			std::string dxt = string_format(xorstr("UI\\ItemIcon_%.1d_%.4d_%.2d_%.1d.dxt"), dwIconID / 10000000u, dwIconID / 1000u % 10000, dwIconID / 10u % 100, dwIconID % 10);

			g_pMain->SetTexImage(rewardSlots[i], dxt);
		
		}
		else {
			std::vector<char> buffer(256, NULL);

			sprintf(&buffer[0], "ui\\itemicon_%.1d_%.4d_%.2d_%.1d.dxt",
				(tbl->dxtID / 10000000),
				(tbl->dxtID / 1000) % 10000,
				(tbl->dxtID / 10) % 100,
				tbl->dxtID % 10);

			std::string szIconFN = &buffer[0];

	
			g_pMain->SetTexImage(rewardSlots[i], szIconFN);
		}

		g_pMain->SetVisible(rewardSlots[i],true);
		itemTbl[i + 5] = tbl;
	}
}

DWORD pClientLottyEvent;
DWORD Func_Lottery;

bool CUILotteryEventPlug::ReceiveMessage(DWORD * pSender, uint32_t dwMsg)
{
	pClientLottyEvent = m_dVTableAddr;

	if (!pSender || pSender == 0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;
	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender ==(DWORD*) btn_close)
		{
			Close();

			if (g_pMain->pClientTopRightNewPlug != NULL)
				g_pMain->pClientTopRightNewPlug->OpenLotteryButton();
		}
		else if (pSender == (DWORD*)btn_join)
		{
			Packet pkt(WIZ_HSACS_HOOK, uint8(HSACSOpCodes::LOTTERY));
			pkt << uint8(3);
			g_pMain->Send(&pkt);
		}
	}
	return true;
}

void CUILotteryEventPlug::Tick()
{
	if (m_Timer == NULL)
		m_Timer = new CTimer();

	if (g_pMain->pLotteryEvent.sRemainingTime > 0) 
	{
		if (m_Timer->IsElapsedSecond() && g_pMain->pLotteryEvent.sRemainingTime > 0)
			g_pMain->pLotteryEvent.sRemainingTime--;

		uint16_t remainingMinutes = (uint16_t)ceil(g_pMain->pLotteryEvent.sRemainingTime / 60);
		uint16_t remainingSeconds = g_pMain->pLotteryEvent.sRemainingTime - (remainingMinutes * 60);

		std::string remainingTime;
		if (remainingMinutes < 10 && remainingSeconds < 10)
			remainingTime = string_format(xorstr("0%d : 0%d"), remainingMinutes, remainingSeconds);
		else if (remainingMinutes < 10)
			remainingTime = string_format(xorstr("0%d : %d"), remainingMinutes, remainingSeconds);
		else if (remainingSeconds < 10)
			remainingTime = string_format(xorstr("%d : 0%d"), remainingMinutes, remainingSeconds);
		else
			remainingTime = string_format(xorstr("%d : %d"), remainingMinutes, remainingSeconds);

		g_pMain->SetString(str_remaining, string_format(xorstr("%s"), remainingTime.c_str()));
	}
}

void CUILotteryEventPlug::OpenLottery()
{
	g_pMain->SetVisible(m_dVTableAddr, true);
	g_pMain->UIScreenCenter(m_dVTableAddr);

}
uint32_t CUILotteryEventPlug::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;
	if (g_pMain->IsVisible(m_dVTableAddr))
	{
		bool showTooltip = false;

		for (int i = 0; i < 4; i++)
		{
			if (g_pMain->IsVisible(rewardSlots[i]) && g_pMain->IsIn(rewardSlots[i],ptCur.x, ptCur.y) && itemTbl[i+5]) {
				g_pMain->ShowToolTipEx(itemIDs[i + 5], ptCur.x, ptCur.y);
				showTooltip = true;
			}
		}
		if (!showTooltip)
		{
			if (g_pMain->IsVisible(reqSlots) && g_pMain->IsIn(reqSlots, ptCur.x, ptCur.y) && itemTbl[0]) {
				g_pMain->ShowToolTipEx(itemIDs[0], ptCur.x, ptCur.y);
				showTooltip = true;
			}
		}
	}

	return dwRet;
}

void CUILotteryEventPlug::Open()
{
	g_pMain->SetVisible(m_dVTableAddr,true);
}

void CUILotteryEventPlug::Close()
{
	g_pMain->SetVisible(m_dVTableAddr,false);
}
void __stdcall pClientLottyEventReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->pClientLottyEvent->ReceiveMessage(pSender, dwMsg);

	__asm
	{
		MOV ECX, pClientLottyEvent
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_Lottery
		CALL EAX
	}
}

void CUILotteryEventPlug::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_Lottery = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)pClientLottyEventReceiveMessage_Hook;
}