#include "UIDailyQuests.h"

extern std::string GetZoneName(uint16 zoneID);

HookQuestPage::HookQuestPage()
{
	vector<int>offsets;
#if (HOOK_SOURCE_VERSION == 1098 || HOOK_SOURCE_VERSION == 1534)
	offsets.push_back(0x5E0);
	offsets.push_back(0);
#else
	offsets.push_back(0x250);   // 2369		//ka_change Satýr : 33
	offsets.push_back(0);
#endif

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);

	m_btnOk = 0;
	m_btnReview = 0;
	m_btnCancel = 0;
	m_txtUprate = 0;
	text_page = 0;
	btn_up = 0;
	btn_down = 0;
	selectQuestID = 0;
	page = 1;
	m_Timer = NULL;
	pageCount = 1;
	pagecountpageCount = 1;
	for (int i = 0; i < 15; i++) {
		baseQuestButton[i] = 0;
		txt_quest_time[i] = 0;
		txt_quest_status[i] = 0;
		txt_quest_title[i] = 0;
		baseGroup[i] = 0;
		txt_quest_zone[i] = 0;
		txt_quest_killtype[i] = 0;
	}
	ParseUIElements();
	InitReceiveMessage();
}

HookQuestPage::~HookQuestPage()
{
}

void HookQuestPage::ParseUIElements()
{
	group_quest = g_pMain->GetChildByID(m_dVTableAddr, "group_quest");
	quest_rewards = g_pMain->GetChildByID(m_dVTableAddr, "quest_rewards");
	text_page = g_pMain->GetChildByID(group_quest, "text_page");
	btn_down = g_pMain->GetChildByID(group_quest, "btn_previouspage");
	btn_up = g_pMain->GetChildByID(group_quest, "btn_nextpage");
	for (int i = 0; i <= 14; i++) {
		if (i > 9)
			baseGroup[i] = g_pMain->GetChildByID(group_quest, string_format(xorstr("group_quest_%d"), i));
		else
			baseGroup[i] = g_pMain->GetChildByID(group_quest, string_format(xorstr("group_quest_0%d"), i));
	}
	for (int i = 0; i <= 14; i++) {
		if (i > 9)
			baseQuestButton[i] = g_pMain->GetChildByID(baseGroup[i], string_format(xorstr("btn_selectquest%d"), i));
		else
			baseQuestButton[i] = g_pMain->GetChildByID(baseGroup[i], string_format(xorstr("btn_selectquest0%d"), i));

		txt_quest_title[i] = g_pMain->GetChildByID(baseGroup[i], "txt_quest_title");
		txt_quest_status[i] = g_pMain->GetChildByID(baseGroup[i], "txt_quest_status");
		txt_quest_time[i] = g_pMain->GetChildByID(baseGroup[i], "txt_quest_time");
		txt_quest_zone[i] = g_pMain->GetChildByID(baseGroup[i], "txt_quest_zone");
		txt_quest_killtype[i] = g_pMain->GetChildByID(baseGroup[i], "txt_quest_type");
	}
	for (int i = 0; i <= 14; i++) {
		g_pMain->SetString(txt_quest_time[i], string_format("Timer-%d", i));
		g_pMain->SetString(txt_quest_status[i], string_format("Status-%d", i));
		g_pMain->SetString(txt_quest_title[i], string_format("Title-%d", i));
	}
	
	btn_close = g_pMain->GetChildByID(quest_rewards, "btn_close");
	str_quest = g_pMain->GetChildByID(quest_rewards, "text_quest_name");
	str_zone = g_pMain->GetChildByID(quest_rewards, "text_zone");
	hunt = g_pMain->GetChildByID(quest_rewards, "Hunt");
	str_hunt = g_pMain->GetChildByID(hunt, "text_hunt_name");
	str_hunt_count = g_pMain->GetChildByID(hunt, "text_hunt_count");
	hunt_area = g_pMain->GetChildByID(hunt, "icon");
	for (uint8 i = 0; i < 4; i++)
	{
		rewardBase[i].base = g_pMain->GetChildByID(quest_rewards, string_format(xorstr("Reward%d"), i + 1));
		rewardBase[i].str_name = g_pMain->GetChildByID(rewardBase[i].base, "str_name");
		rewardBase[i].str_count = g_pMain->GetChildByID(rewardBase[i].base, "str_count");
		rewardBase[i].str_btn = g_pMain->GetChildByID(rewardBase[i].base, "reward");
		rewardBase[i].icon = g_pMain->GetChildByID(rewardBase[i].base, "icon");
	}

	g_pMain->SetVisible(quest_rewards, false);
	m_Timer = new CTimer(true, 600);
}

void HookQuestPage::LoadInfo(_DAILY_QUEST* quest)
{
	cMob = quest->Mob[0];
	std::string zone = GetZoneName(quest->ZoneID);
	g_pMain->SetString(str_zone,zone);

	if (auto mob = g_pMain->pClientTBLManager->GetMobData(quest->Mob[0]))
	{
		g_pMain->SetString(str_quest,string_format(xorstr("%s"), mob->strName.c_str()));
		g_pMain->SetString(str_hunt,mob->strName);
	}
	else 
	{
		if (quest->Mob[0] == 1) {
			g_pMain->SetString(str_quest,xorstr("<Kill Enemy>"));
			g_pMain->SetString(str_hunt,xorstr("<Kill Enemy>"));
		}
		else {
			g_pMain->SetString(str_quest,xorstr("<Unknown>"));
			g_pMain->SetString(str_hunt,xorstr("<Unknown>"));
		}
	}

	g_pMain->SetString(str_hunt_count,string_format(xorstr("%d / %d"), quest->MyCount, quest->KillTarget));
	uint32 OriginItemID = 900005000;

	TABLE_ITEM_BASIC* tbl = g_pMain->pClientTBLManager->GetItemData(OriginItemID);
	if (TABLE_ITEM_EXT* ext = g_pMain->pClientTBLManager->GetExtData(tbl->extNum, OriginItemID))
	{
		DWORD dwIconID = tbl->dxtID;
		if (ext->dxtID > 0)
			dwIconID = ext->dxtID;

		std::string dxt = string_format(xorstr("UI\\ItemIcon_%.1d_%.4d_%.2d_%.1d.dxt"), dwIconID / 10000000u, dwIconID / 1000u % 10000, dwIconID / 10u % 100, dwIconID % 10);	  
		g_pMain->SetTexImage(hunt_area,dxt);
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
		g_pMain->SetTexImage(hunt_area, szIconFN);
	}

	// Rewards
	int rewardCount = 0;
	for (uint8 i = 0; i < 4; i++)
	{
		if (quest->Reward[i])
		{
			rewardCount++;
		}
	}

	for (uint8 i = 0; i < 4; i++) 
	{
		if (quest->Reward[i]) 
		{
			rewardBase[i].nItemID = quest->Reward[i];
			g_pMain->SetVisible(rewardBase[i].base,true);
			g_pMain->SetVisible(rewardBase[i].icon, true);
			uint32 OriginItemID = quest->Reward[i];

			if (TABLE_ITEM_BASIC* tbl = g_pMain->pClientTBLManager->GetItemData(OriginItemID)) 
			{
				rewardBase[i].tbl = tbl;
				TABLE_ITEM_EXT* ext = g_pMain->pClientTBLManager->GetExtData(tbl->extNum, OriginItemID);
				if (ext) 
				{
					DWORD dwIconID = tbl->dxtID;
					if (ext->dxtID > 0) dwIconID = ext->dxtID;
					std::string dxt = string_format(xorstr("UI\\ItemIcon_%.1d_%.4d_%.2d_%.1d.dxt"), dwIconID / 10000000u, dwIconID / 1000u % 10000, dwIconID / 10u % 100, dwIconID % 10);

					g_pMain->SetTexImage(rewardBase[i].icon, dxt);
				}
				else 
				{
					std::vector<char> buffer(256, NULL);
					sprintf(&buffer[0], "ui\\itemicon_%.1d_%.4d_%.2d_%.1d.dxt", (tbl->dxtID / 10000000), (tbl->dxtID / 1000) % 10000, (tbl->dxtID / 10) % 100, tbl->dxtID % 10);
					std::string szIconFN = &buffer[0];
					g_pMain->SetTexImage(rewardBase[i].icon, szIconFN);
				}
				uint32 counta = quest->Count[i];
				g_pMain->SetString(rewardBase[i].str_name,tbl->strName);
				g_pMain->SetString(rewardBase[i].str_count,to_string(quest->Count[i]));
			}
		}
		else g_pMain->SetVisible(rewardBase[i].base,false);
	}
}

bool HookQuestPage::IsInSlots()
{
	//g_pMain->pClientHookManager->uiGetQuestInfo->Start(pDailyQuestListArray[selectQuestID].targetMob, pDailyQuestListArray[selectQuestID].targetCount, pDailyQuestListArray[selectQuestID].currentCount, g_pMain->pClientHookManager->uiDailyQuests->getZoneName(pDailyQuestListArray[selectQuestID].zone).c_str(), 0);
	return true;
}

#define MINUTE				60
#define HOUR				MINUTE * 60

void HookQuestPage::KillTrigger(uint8 questID, uint16 mob)
{
	foreach(itr, pDailyQuestArray)
	{
		if ((*itr)->index != questID
			|| (*itr)->Status == (uint8)DailyQuestStatus::comp
			|| (*itr)->Status == (uint8)DailyQuestStatus::timewait)
			continue;

		bool _v = false;
		for (int i = 0; i < 4; i++)
			if ((*itr)->Mob[i] == mob) _v = true;

		if (!_v) 
			continue;

		(*itr)->MyCount++;

		if ((*itr)->MyCount >= (*itr)->KillTarget)
		{
			if ((*itr)->questtype == (uint8)DailyQuesttimetype::single)
			{
				(*itr)->Status = (uint8)DailyQuestStatus::comp;
				continue;
			}

			if ((*itr)->questtype == (uint8)DailyQuesttimetype::time && (*itr)->remtime == 0) 
			{
				(*itr)->Status = (uint8)DailyQuestStatus::timewait;
				(*itr)->remtime = (*itr)->replaytime * 60 * 60;
			}
			else
			{
				(*itr)->MyCount = 0;
				(*itr)->remtime = 0;
				(*itr)->Status = (uint8)DailyQuestStatus::ongoing;
			}
		}

		if (g_pMain->IsVisible(quest_rewards))
			LoadInfo(pDailyQuestListArray[selectQuestID]);
		break;
	}

	InitQuests(page);
}

uint32_t HookQuestPage::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;
	if (g_pMain->IsVisible(quest_rewards))
	{
		bool showTooltip = false;

		for (int i = 0; i < 4; i++)
		{
			if (rewardBase[i].icon != NULL)
			{
				if (g_pMain->IsIn(rewardBase[i].icon,ptCur.x, ptCur.y) && rewardBase[i].tbl != nullptr && rewardBase[i].nItemID != 0)
				{
					g_pMain->ShowToolTipEx(rewardBase[i].nItemID, ptCur.x, ptCur.y);
					showTooltip = true;
					break;
				}
			}
		}
	}
	return dwRet;
}

DWORD uiQuestPage;
DWORD Func_SendMessageQuest;
void HookQuestPage::InitQuests(uint8 p)
{
	if (p == 255) p = page;
	pDailyQuestListArray.clear();

	uint8 nation = 0;
	DWORD chr = *(DWORD*)KO_PTR_CHR;
	if (!chr)
		return;

	nation = *(uint8*)(chr + KO_OFF_NATION);

	for (int i = 0; i < pDailyQuestArray.size(); i++)
	{
		if (pDailyQuestArray[i]->ZoneID) {
			if (pDailyQuestArray[i]->ZoneID == ZONE_MORADON && !isInMoradon(g_pMain->Player.ZoneID)) continue;
			else if (pDailyQuestArray[i]->ZoneID == ZONE_ELMORAD && !isInElmoradCastle(g_pMain->Player.ZoneID)) continue;
			else if (pDailyQuestArray[i]->ZoneID == ZONE_KARUS && !isInLufersonCastle(g_pMain->Player.ZoneID)) continue;
			else if (pDailyQuestArray[i]->ZoneID == ZONE_ELMORAD_ESLANT && !isInElmoradEslant(g_pMain->Player.ZoneID)) continue;
			else if (pDailyQuestArray[i]->ZoneID == ZONE_KARUS_ESLANT && !isInKarusEslant(g_pMain->Player.ZoneID)) continue;
			else { if (pDailyQuestArray[i]->ZoneID != g_pMain->Player.ZoneID) continue; }
		}

		if ((pDailyQuestArray[i]->ZoneID == 1 || pDailyQuestArray[i]->ZoneID == 2) && pDailyQuestArray[i]->ZoneID != nation) continue;

		/*if (g_pMain->m_PlayerBase->GetLevel() <  pDailyQuestArray[i]->MinLevel || g_pMain->m_PlayerBase->GetLevel() > pDailyQuestArray[i]->MaxLevel)
			continue;*/

		pDailyQuestListArray.push_back(pDailyQuestArray[i]);
	}

	if (pDailyQuestListArray.empty()) {
		for (int i = 0; i < 15; i++)g_pMain->SetVisible(baseGroup[i], false);
		return;
	}

	pageCount = abs(ceil((double)pDailyQuestListArray.size() / (double)15));
	if (page > pageCount)
		page = 1;

	int begin = (p - 1) * 15;
	int j = -1;
	for (int i = begin; i < begin + 15; i++)
	{
		j++;

		if (j > 14)
			break;

		if (i > pDailyQuestListArray.size() - 1)
		{
			g_pMain->SetVisible(baseGroup[j], false);
			continue;
		}

		if (pDailyQuestListArray[i]->Mob[0]) 
		{
			if(auto data = g_pMain->pClientTBLManager->GetMobData(pDailyQuestListArray[i]->Mob[0]))
				g_pMain->SetString(txt_quest_title[j], string_format(xorstr("%s"), data->strName.c_str()));
			else 
				g_pMain->SetString(txt_quest_title[j], string_format(xorstr("%s"), "Kill Enemy"));
		}

		std::string zonename = GetZoneName(pDailyQuestListArray[i]->ZoneID);
		g_pMain->SetString(txt_quest_zone[j], string_format(xorstr("%s"), zonename.empty() ? "unknown" : zonename.c_str()));

		std::string killtypename = "-";

		if (pDailyQuestListArray[i]->killtype == 0)
			killtypename = "Solo";
		else if (pDailyQuestListArray[i]->killtype == 1)
			killtypename = "Party";
		else if (pDailyQuestListArray[i]->killtype == 2)
			killtypename = "Solo/Party";

		g_pMain->SetString(txt_quest_killtype[j], string_format(xorstr("%s"), killtypename.c_str()));
		g_pMain->SetVisible(baseGroup[j], true);
		g_pMain->SetString(text_page, to_string(page));

		switch (pDailyQuestListArray[i]->questtype)
		{
			case (uint8)DailyQuesttimetype::repeat:
				g_pMain->SetString(txt_quest_status[j], string_format("Repeatable"));
				g_pMain->SetStringColor(txt_quest_status[j], 0x129603);
				g_pMain->SetString(txt_quest_time[j], "-");
				break;
			case (uint8)DailyQuesttimetype::single:
			{
				if (pDailyQuestListArray[i]->Status == (uint8)DailyQuestStatus::ongoing)
				{
					g_pMain->SetString(txt_quest_status[j], string_format("Ongoing"));
					g_pMain->SetStringColor(txt_quest_status[j], 0x129603);
					g_pMain->SetString(txt_quest_time[j], "-");
				}
				else {
					g_pMain->SetString(txt_quest_status[j], string_format("Finished"));
					g_pMain->SetStringColor(txt_quest_status[j], 0xff7a70);
					g_pMain->SetString(txt_quest_time[j], "-");
				}
			}break;
			case (uint8)DailyQuesttimetype::time:
			{
				if (pDailyQuestListArray[i]->Status == (uint8)DailyQuestStatus::ongoing)
				{
					g_pMain->SetString(txt_quest_status[j], string_format("Ongoing"));
					g_pMain->SetStringColor(txt_quest_status[j], 0x129603);
					g_pMain->SetString(txt_quest_time[j], "-");
				}
				else 
				{
					g_pMain->SetString(txt_quest_status[j], string_format("Waiting"));
					g_pMain->SetStringColor(txt_quest_status[j], 0x129603);
				}
			}break;
		}
	}
}

bool HookQuestPage::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	uiQuestPage = m_dVTableAddr;
	if (!pSender 
		|| pSender == 0 
		|| dwMsg != UIMSG_BUTTON_CLICK)
		return false;

	std::string msg = "";

	if (pSender == (DWORD*)m_btnOk) 
	{
		if (!pDailyQuestListArray.empty()) 
		{
			LoadInfo(pDailyQuestListArray[selectQuestID]);
			g_pMain->SetVisible(quest_rewards, true);
		}
	}
	else if (pSender == (DWORD*)btn_up)
	{
		if (page < pageCount)
		{
			page++;
			InitQuests(page);
		}
	}
	else if (pSender == (DWORD*)btn_close)
		g_pMain->SetVisible(quest_rewards, false);
	else if (pSender == (DWORD*)btn_down)
	{
		if (page > 1)
		{
			page--;
			InitQuests(page);
		}
	}
	else 
	{
		for (int i = 0; i <= 14; i++)
		{
			if (pSender == (DWORD*)baseQuestButton[i]) {
				selectQuestID = ((page - 1) * 15) + i;
				g_pMain->SetState(baseQuestButton[i], UI_STATE_BUTTON_DOWN);
			}
			else
				g_pMain->SetState(baseQuestButton[i], UI_STATE_BUTTON_NORMAL);
		}

		InitQuests(page);

		for (int i = 0;i < 4;i++) 
		{
			if (pSender == (DWORD*)rewardBase[i].str_btn) 
			{
				if (rewardBase[i].nItemID == 900004000) 
				{
					Packet result(WIZ_HSACS_HOOK, uint8(HSACSOpCodes::DROP_REQUEST));
					result << uint8(4) << uint8(i);
					g_pMain->Send(&result);
				}
			}
		}

		if (!pDailyQuestListArray.empty())
		{
			LoadInfo(pDailyQuestListArray[selectQuestID]);
			g_pMain->SetVisible(quest_rewards, true);
		}
	}
	return true;
}

std::string calcDiff(int h1, int m1, int s1, int h2, int m2, int s2) {
	std::chrono::seconds d = std::chrono::hours(h2 - h1)
		+ std::chrono::minutes(m2 - m1)
		+ std::chrono::seconds(s2 - s1);

	return string_format("%d:%d:%d", std::chrono::duration_cast<std::chrono::hours>(d).count(),
		std::chrono::duration_cast<std::chrono::minutes>(d % std::chrono::hours(1)).count(),
		std::chrono::duration_cast<std::chrono::seconds>(d % std::chrono::minutes(1)).count());
}

long long timePassed = 0;

void HookQuestPage::Tick()
{
	if (timePassed == 0) timePassed = clock();

	if (timePassed > clock() - 1000)
		return;

	timePassed = clock();

	for (size_t i = 0; i < pDailyQuestArray.size(); i++)
	{
		if (pDailyQuestArray[i]->remtime)
			pDailyQuestArray[i]->remtime--;
	}

	if (!g_pMain->IsVisible(m_dVTableAddr))
		return;
	
	if (pDailyQuestListArray.empty())
		return;

	if (page > pageCount)
		page = 1;

	int begin = (page - 1) * 15;
	int j = -1;
	for (int i = begin; i < begin + 15; i++)
	{
		j++;
		if (i > pDailyQuestListArray.size() - 1) continue;

		if (pDailyQuestListArray[i]->questtype == (uint8)DailyQuesttimetype::time && pDailyQuestListArray[i]->Status == (uint8)DailyQuestStatus::timewait) {
			
			if (pDailyQuestListArray[i]->remtime) {

				std::string remainingTime = "";
				uint32_t days = (uint32_t)floor(pDailyQuestListArray[i]->remtime / 86400);
				uint32_t hours = (uint32_t)floor(((pDailyQuestListArray[i]->remtime - days * 86400) / 3600) % 24);
				uint32_t minutes = (uint32_t)floor((((pDailyQuestListArray[i]->remtime - days * 86400) - hours * 3600) / 60) % 60);
				uint32_t seconds = (uint32_t)floor(((((pDailyQuestListArray[i]->remtime - days * 86400) - hours * 3600) - minutes * 60)) % 60);

				if (days)
					remainingTime += string_format(xorstr("%s%u:"), days < 10 ? "0" : "", days);

				if (hours)
					remainingTime += string_format(xorstr("%s%u:"), hours < 10 ? "0" : "", hours);
				else
					remainingTime += "00:";

				if (minutes)
					remainingTime += string_format(xorstr("%s%u:"), minutes < 10 ? "0" : "", minutes);
				else
					remainingTime += "00:";

				if (seconds)
					remainingTime += string_format(xorstr("%s%u"), seconds < 10 ? "0" : "", seconds);
				else
					remainingTime += "00";
				
				g_pMain->SetString(txt_quest_status[j], string_format("Waiting"));
				g_pMain->SetStringColor(txt_quest_status[j], 0x129603);
				g_pMain->SetString(txt_quest_time[j], remainingTime.c_str());
			}
			else {
				pDailyQuestListArray[i]->Status = (uint8)DailyQuestStatus::ongoing;
			}
		}
	}
}

void __stdcall UiQuestReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->uiQuestPage->ReceiveMessage(pSender, dwMsg);

	__asm
	{
		MOV ECX, uiQuestPage
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_SendMessageQuest
		CALL EAX
	}
}

void HookQuestPage::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_SendMessageQuest = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)UiQuestReceiveMessage_Hook;
}