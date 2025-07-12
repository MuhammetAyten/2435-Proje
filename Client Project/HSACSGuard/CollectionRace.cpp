#include "CollectionRace.h"
extern std::string GetZoneName(uint16 zoneID);
CUICollectionRacePlug::CUICollectionRacePlug()
{
	m_dVTableAddr = NULL;
	minimaliaze = false;
	vector<int>offsets;
	offsets.push_back(0x558);  // co_battle_score Satýr : 250
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);
	m_Timer = NULL;
	m_iRemainingTime = m_iCompleteUserCount = TotalParticipantUser = 0;
	EventName = "";
	group_top = NULL;
	btn_min = NULL;
	btn_max = NULL;
	btn_close = NULL;
	calibreCount = 0;
	text_event_zone = NULL;
	group_time = NULL;
	text_winners = text_time = text_winners_label = text_time_label = NULL;
	text_event_name = NULL;
	for (int32_t i = 0; i < 3; i++)
	{
		requital[i] = NULL;
		group[i] = NULL;
	}

	for (int i = 0; i < 2; i++)
		group_middle[i] = NULL;

	m_bIsStarted = false;
	ParseUIElements();
	InitReceiveMessage();
}

DWORD Func_cr;
CUICollectionRacePlug::~CUICollectionRacePlug()
{

}

void CUICollectionRacePlug::ParseUIElements()
{
	group_top = g_pMain->GetChildByID(m_dVTableAddr, "group_top");
	btn_min = g_pMain->GetChildByID(group_top, "btn_min");
	btn_max = g_pMain->GetChildByID(group_top, "btn_max");
	btn_close = g_pMain->GetChildByID(group_top, "btn_close");

	group_time = g_pMain->GetChildByID(m_dVTableAddr, "group_time");
	text_winners_label = g_pMain->GetChildByID(group_time, "text_winners_label");
	text_time_label = g_pMain->GetChildByID(group_time, "text_time_label");
	text_time = g_pMain->GetChildByID(group_time, "text_time");
	text_event_name = g_pMain->GetChildByID(group_time, "text_event_name");
	text_event_zone = g_pMain->GetChildByID(group_time, "text_event_zone");
	text_winners = g_pMain->GetChildByID(group_time, "text_winners");

	group_middle[0] = g_pMain->GetChildByID(m_dVTableAddr, "group_middle_0");
	items = g_pMain->GetChildByID(m_dVTableAddr, "items");
	for (int i = 0; i < 3; i++)
	{
		requital[i] = g_pMain->GetChildByID(items, string_format(xorstr("requital_%d"), i));
		m_Items[i].txt_return_first = g_pMain->GetChildByID(requital[i], "txt_return_first");
		m_Items[i].txt_return_second = g_pMain->GetChildByID(requital[i], "txt_return_second");
		m_Items[i].txt_rate = g_pMain->GetChildByID(requital[i], "txt_rate");
		m_Items[i].area_return = g_pMain->GetChildByID(requital[i], "icon");
		m_Items[i].btn_return = g_pMain->GetChildByID(requital[i], "btn_return");
		group[i] = g_pMain->GetChildByID(m_dVTableAddr, string_format(xorstr("group_%d"), i));
		CollectionRaceEvent[i].txt_completion = g_pMain->GetChildByID(group[i], "txt_needs_first");
		CollectionRaceEvent[i].txt_target = g_pMain->GetChildByID(group[i], "txt_needs_second");
		CollectionRaceEvent[i].area_needs = g_pMain->GetChildByID(group[i], "icon");
		CollectionRaceEvent[i].btn_needs = g_pMain->GetChildByID(group[i], "btn_needs");
	}

	group_middle[1] = g_pMain->GetChildByID(items, "group_middle_1");
	bases = g_pMain->GetChildByID(m_dVTableAddr, "bases");
	for (uint8 i = 0; i < 3; i++)
	{
		base[i] = g_pMain->GetChildByID(bases, string_format(xorstr("base_%d"), i + 1));
	}
}

DWORD CollectionRace;

void CUICollectionRacePlug::setMin() {
	g_pMain->SetVisible(btn_min, false);
	g_pMain->SetVisible(btn_max, true);

	RECT a = g_pMain->GetUiRegion(group_top);
	RECT b = g_pMain->GetUiRegion(group_time);
	a.bottom = b.bottom;
	g_pMain->SetUiRegion(m_dVTableAddr, a);

	for (int i = 0; i < 3; i++)
	{
		g_pMain->SetVisible(requital[i], false);
		g_pMain->SetVisible(group[i], false);
		g_pMain->SetVisible(CollectionRaceEvent[i].img_needs, false);
		g_pMain->SetVisible(m_Items[i].img_return, false);
	}

	for (int i = 0; i < 2; i++)
		g_pMain->SetVisible(group_middle[i], false);
}

void CUICollectionRacePlug::setMax() {
	g_pMain->SetVisible(btn_min, true);
	g_pMain->SetVisible(btn_max, false);

	RECT a = g_pMain->GetUiRegion(group_top);
	RECT b = g_pMain->GetUiRegion(group_time);
	a.bottom = b.bottom;
	g_pMain->SetUiRegion(m_dVTableAddr, a);

	for (int i = 0; i < calibreCount + 1; i++)
	{
		if (i > 2)
			continue;

		g_pMain->SetVisible(group[i], true);
		g_pMain->SetVisible(CollectionRaceEvent[i].img_needs, true);
		g_pMain->SetVisible(m_Items[i].img_return, true);
	}

	for (int i = 0; i < 3; i++)
	{
		if (m_Items[i].ItemID > 0)
		{
			g_pMain->SetVisible(requital[i], true);
			g_pMain->SetVisible(m_Items[i].img_return, true);
		}
	}

	for (int i = 0; i < 2; i++)
		g_pMain->SetVisible(group_middle[i], true);
}

void CUICollectionRacePlug::Open()
{
	g_pMain->SetVisible(m_dVTableAddr, true);
}

bool CUICollectionRacePlug::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	CollectionRace = m_dVTableAddr;
	if (!pSender || pSender == (DWORD*)0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;

	if (pSender == (DWORD*)btn_min)
	{
		minimaliaze = true;
		setMin();
	}
	else if (pSender == (DWORD*)btn_max)
	{
		minimaliaze = false;
		setMax();
		Calibrate();
	}
	else if (pSender == (DWORD*)btn_close)
	{
		Close();

		if (g_pMain->pClientTopRightNewPlug != NULL)
			g_pMain->pClientTopRightNewPlug->OpenCollectionRaceButton();
	}
	else {

		for (int i = 0; i < 3; i++)
		{
			if (pSender == (DWORD*)m_Items[i].btn_return)
			{
				if (m_Items[i].ItemID == 900004000)
				{
					Packet pkt(WIZ_HSACS_HOOK, uint8(HSACSOpCodes::DROP_REQUEST));
					pkt << uint8(4) << uint8(i);
					g_pMain->Send(&pkt);
				}
			}
		}
	}

	return true;
}

void __stdcall CollectionRaceReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->pClientCollection->ReceiveMessage(pSender, dwMsg);
	__asm
	{
		MOV ECX, CollectionRace
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_cr
		CALL EAX
	}
}

void CUICollectionRacePlug::Tick()
{
	if (g_pMain->m_cCollettionStatus)
	{
		if (m_Timer->IsElapsedSecond())
			m_iRemainingTime--;

		uint16_t remainingMinutes = (uint16_t)ceil(m_iRemainingTime / 60);
		uint16_t remainingSeconds = m_iRemainingTime - (remainingMinutes * 60);

		std::string remainingTime;
		if (remainingMinutes < 10 && remainingSeconds < 10)
			remainingTime = string_format(xorstr("0%d : 0%d"), remainingMinutes, remainingSeconds);
		else if (remainingMinutes < 10)
			remainingTime = string_format(xorstr("0%d : %d"), remainingMinutes, remainingSeconds);
		else if (remainingSeconds < 10)
			remainingTime = string_format(xorstr("%d : 0%d"), remainingMinutes, remainingSeconds);
		else
			remainingTime = string_format(xorstr("%d : %d"), remainingMinutes, remainingSeconds);

		g_pMain->SetString(text_time, string_format(xorstr("%s"), remainingTime.c_str()));
	}
}

void CUICollectionRacePlug::Start()
{
	minimaliaze = false;
	uint32_t OriginItemID = 0;

	m_iRemainingTime = g_pMain->pCollectionRaceEvent.EventTimer;
	m_iCompleteUserCount = g_pMain->pCollectionRaceEvent.m_iCompleteUserCount;
	TotalParticipantUser = g_pMain->pCollectionRaceEvent.TotalParticipantUser;
	EventName = g_pMain->pCollectionRaceEvent.EventName;

	g_pMain->SetString(text_winners, string_format("%d of %d", m_iCompleteUserCount, TotalParticipantUser));
	g_pMain->SetString(text_event_name, string_format("%s", EventName.c_str()));
	g_pMain->SetString(text_event_zone, string_format("Zone : %s", GetZoneName(g_pMain->pCollectionRaceEvent.sZone).c_str()));

	uint8 baseCount = 0;

	for (int i = 0; i < 3; i++)
	{
		CollectionRaceEvent[i].nTargetID = g_pMain->pCollectionRaceEvent.sSid[i];
		CollectionRaceEvent[i].targetCount = g_pMain->pCollectionRaceEvent.killTarget[i];
		CollectionRaceEvent[i].Nation = g_pMain->pCollectionRaceEvent.Nation;

		TABLE_MOB* mobData = g_pMain->pClientTBLManager->GetMobData(CollectionRaceEvent[i].nTargetID);
		if (mobData != nullptr)
		{
			g_pMain->SetString(CollectionRaceEvent[i].txt_completion, string_format(xorstr("Hunt - 0 / %d"), CollectionRaceEvent[i].targetCount));
			g_pMain->SetString(CollectionRaceEvent[i].txt_target, mobData->strName);
			g_pMain->SetState(CollectionRaceEvent[i].btn_needs, eUI_STATE::UI_STATE_BUTTON_ON);

			OriginItemID = 900005000;

			TABLE_ITEM_BASIC* tbl = g_pMain->pClientTBLManager->GetItemData(OriginItemID);
			if (TABLE_ITEM_EXT* ext = g_pMain->pClientTBLManager->GetExtData(tbl->extNum, OriginItemID))
			{
				DWORD dwIconID = tbl->dxtID;
				if (ext->dxtID > 0)
					dwIconID = ext->dxtID;

				std::string dxt = string_format(xorstr("UI\\ItemIcon_%.1d_%.4d_%.2d_%.1d.dxt"), dwIconID / 10000000u, dwIconID / 1000u % 10000, dwIconID / 10u % 100, dwIconID % 10);

				g_pMain->SetTexImage(CollectionRaceEvent[i].area_needs, dxt);
				baseCount++;
			}

			else {
				std::vector<char> buffer(256, NULL);

				sprintf(&buffer[0], "ui\\itemicon_%.1d_%.4d_%.2d_%.1d.dxt",
					(tbl->dxtID / 10000000),
					(tbl->dxtID / 1000) % 10000,
					(tbl->dxtID / 10) % 100,
					tbl->dxtID % 10);

				std::string szIconFN = &buffer[0];


				g_pMain->SetTexImage(CollectionRaceEvent[i].area_needs, szIconFN);
				baseCount++;
			}

		}
		else if (CollectionRaceEvent[i].nTargetID == 1 && CollectionRaceEvent[i].Nation != 0)
		{
			OriginItemID = 900008000;
			TABLE_ITEM_BASIC* tbl = g_pMain->pClientTBLManager->GetItemData(OriginItemID);
			if (TABLE_ITEM_EXT* ext = g_pMain->pClientTBLManager->GetExtData(tbl->extNum, OriginItemID))
			{
				DWORD dwIconID = tbl->dxtID;
				if (ext->dxtID > 0)
					dwIconID = ext->dxtID;

				std::string dxt = string_format(xorstr("UI\\ItemIcon_%.1d_%.4d_%.2d_%.1d.dxt"), dwIconID / 10000000u, dwIconID / 1000u % 10000, dwIconID / 10u % 100, dwIconID % 10);


				g_pMain->SetTexImage(CollectionRaceEvent[i].area_needs, dxt);
				baseCount++;
			}
			else {
				std::vector<char> buffer(256, NULL);

				sprintf(&buffer[0], "ui\\itemicon_%.1d_%.4d_%.2d_%.1d.dxt",
					(tbl->dxtID / 10000000),
					(tbl->dxtID / 1000) % 10000,
					(tbl->dxtID / 10) % 100,
					tbl->dxtID % 10);

				std::string szIconFN = &buffer[0];

				g_pMain->SetTexImage(CollectionRaceEvent[i].area_needs, szIconFN);

				baseCount++;
			}

			g_pMain->SetString(CollectionRaceEvent[i].txt_completion, string_format(xorstr("0 / %d"), CollectionRaceEvent[i].targetCount));
			g_pMain->SetString(CollectionRaceEvent[i].txt_target, CollectionRaceEvent[i].Nation == 1 ? xorstr("<El Morad>") : xorstr("<Karus>"));
			g_pMain->SetState(CollectionRaceEvent[i].btn_needs, eUI_STATE::UI_STATE_BUTTON_ON);

		}
		else
		{
			g_pMain->SetString(CollectionRaceEvent[i].txt_completion, string_format(xorstr(""), CollectionRaceEvent[i].targetCount));
			g_pMain->SetString(CollectionRaceEvent[i].txt_target, xorstr(""));
		}
	}

	LoadItems(g_pMain->pCollectionRaceEvent.RewardsItemID,
		g_pMain->pCollectionRaceEvent.RewardsItemCount, g_pMain->pCollectionRaceEvent.RewardsItemRate);

	calibreCount = baseCount;

	Calibrate();

	m_Timer = new CTimer(false);
	g_pMain->SetVisible(m_dVTableAddr, true);
	
	/*if(g_pMain->pClientTopRightNewPlug)
		g_pMain->pClientTopRightNewPlug->OpenCollectionRaceButton();*/

	if (minimaliaze)
		setMin();
	else
	{
		setMax();
		Calibrate();
	}
}

uint32_t CUICollectionRacePlug::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;
	bool showTooltip = false;

	for (auto it : m_Items)
	{
		if (g_pMain->IsVisible(btn_min) && g_pMain->IsIn(it.img_return, ptCur.x, ptCur.y) && it.tbl != nullptr)
		{
			g_pMain->ShowToolTipEx(it.ItemID, ptCur.x, ptCur.y);
			break;
		}
	}

	return dwRet;
}

void CUICollectionRacePlug::Calibrate()
{
	POINT ret = g_pMain->GetUiPos(base[calibreCount - 1]);
	g_pMain->SetUIPos(items, ret);

	for (uint8 i = 0; i < 3; i++)
		g_pMain->SetVisible(group[i], false);

	for (uint8 i = 0; i < calibreCount; i++)
		g_pMain->SetVisible(group[i], true);

	for (uint8 i = calibreCount; i < 3; i++)
		if (i < 3) 	g_pMain->SetVisible(group[i], false);

	RECT minReg = g_pMain->GetUiRegion(group_top);
	RECT group_times = g_pMain->GetUiRegion(group_time);
	minReg.bottom += group_times.bottom - group_times.top;
	RECT baseSize = g_pMain->GetUiRegion(group[0]);
	RECT itemSize = g_pMain->GetUiRegion(items);
	minReg.bottom += itemSize.bottom - itemSize.top;
	minReg.bottom += calibreCount * (baseSize.bottom - baseSize.top);
	g_pMain->SetUiRegion(m_dVTableAddr, minReg);
}

void CUICollectionRacePlug::Update()
{
	int completionCount = 0;
	for (int i = 0; i < 3; i++)
	{
		TABLE_MOB* mobData = g_pMain->pClientTBLManager->GetMobData(CollectionRaceEvent[i].nTargetID);
		if (mobData != nullptr)
			g_pMain->SetString(CollectionRaceEvent[i].txt_completion, string_format(xorstr("Hunt - %d / %d"), g_pMain->pCollectionRaceEvent.killCount[i], CollectionRaceEvent[i].targetCount));
		else if (CollectionRaceEvent[i].nTargetID == 1 && CollectionRaceEvent[i].Nation != 0)
		{
			g_pMain->SetString(CollectionRaceEvent[i].txt_completion, string_format(xorstr("%d / %d"), g_pMain->pCollectionRaceEvent.killCount[i], CollectionRaceEvent[i].targetCount));
			g_pMain->SetString(CollectionRaceEvent[i].txt_target, CollectionRaceEvent[i].Nation == 1 ? xorstr("<El Morad>") : xorstr("<Karus>"));
		}

		if (g_pMain->pCollectionRaceEvent.killCount[i] >= CollectionRaceEvent[i].targetCount)
		{
			g_pMain->SetStringColor(CollectionRaceEvent[i].txt_completion, D3DCOLOR_RGBA(255, 99, 71, 255));
			completionCount++;
		}
	}
}

void CUICollectionRacePlug::UpdateFinishUserCounter()
{
	m_iCompleteUserCount = g_pMain->pCollectionRaceEvent.m_iCompleteUserCount;
	g_pMain->SetString(text_winners, string_format(xorstr("%d of %d"), m_iCompleteUserCount, TotalParticipantUser));
}

void CUICollectionRacePlug::LoadItems(uint32 Items[3], uint32 Counts[3], uint8 nRate[3])
{
	for (int i = 0; i < 3; i++)
	{
		m_Items[i].ItemID = Items[i];

		TABLE_ITEM_BASIC* tbl = g_pMain->pClientTBLManager->GetItemData(Items[i]);
		if (tbl != nullptr)
		{
			m_Items[i].tbl = tbl;

			g_pMain->SetString(m_Items[i].txt_return_first, tbl->strName);
			g_pMain->SetString(m_Items[i].txt_return_second, to_string(Counts[i]));
			g_pMain->SetString(m_Items[i].txt_rate, "%" + to_string(nRate[i]));

			if (TABLE_ITEM_EXT* ext = g_pMain->pClientTBLManager->GetExtData(tbl->extNum, Items[i]))
			{
				DWORD dwIconID = tbl->dxtID;
				if (ext->dxtID > 0)
					dwIconID = ext->dxtID;

				std::string dxt = string_format(xorstr("UI\\ItemIcon_%.1d_%.4d_%.2d_%.1d.dxt"), dwIconID / 10000000u, dwIconID / 1000u % 10000, dwIconID / 10u % 100, dwIconID % 10);

				g_pMain->SetTexImage(m_Items[i].area_return, dxt);
			}
			else {
				std::vector<char> buffer(256, NULL);

				sprintf(&buffer[0], "ui\\itemicon_%.1d_%.4d_%.2d_%.1d.dxt",
					(tbl->dxtID / 10000000),
					(tbl->dxtID / 1000) % 10000,
					(tbl->dxtID / 10) % 100,
					tbl->dxtID % 10);

				std::string szIconFN = &buffer[0];

				g_pMain->SetTexImage(m_Items[i].area_return, szIconFN);
			}

			g_pMain->SetState(m_Items[i].btn_return, eUI_STATE::UI_STATE_BUTTON_ON);
		}
		else
		{
			m_Items[i].ItemID = m_Items[i].ItemCount = 0;
			m_Items[i].tbl = nullptr;
			g_pMain->SetString(m_Items[i].txt_return_first, xorstr(""));
			g_pMain->SetString(m_Items[i].txt_return_second, xorstr(""));
			g_pMain->SetString(m_Items[i].txt_rate, xorstr(""));
			g_pMain->SetVisible(requital[i], false);
		}
	}
}
void CUICollectionRacePlug::Close()
{
	g_pMain->SetVisible(m_dVTableAddr, false);
}

void CUICollectionRacePlug::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_cr = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)CollectionRaceReceiveMessage_Hook;
}