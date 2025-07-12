#include "CharacterToolTip.h"

CSealToolTip::CSealToolTip()
{
	m_dVTableAddr = NULL;
	base_skill_view = NULL;
	m_btnsituation = NULL;
	m_btndropresult = NULL;
	m_btnbug = NULL;
	btn_lottery = NULL;
	pageCount = cPage = 1;
	BasladimiBaslamadimi = false;
	vector<int>offsets;
	offsets.push_back(0x42C);
	offsets.push_back(0);
	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);

	memset(perkType, 0, sizeof(perkType));

	ParseUIElements();
	InitReceiveMessage();
}

CSealToolTip::~CSealToolTip()
{
}

void CSealToolTip::QuestCheck(Packet& pkt)
{
	uint8 nSize = 19;
	uint8 Status = 0;
	uint16 nQuestID = 0;

	for (int i = 0; i < nSize;i++)
	{
		pkt >> nQuestID >> Status;

		if (nQuestID == 334)
		{
			if(Status==2)
				g_pMain->SetTexImage(warrior_70_off, string_format("ui\\mark_flag02.dxt"));
			else
				g_pMain->SetTexImage(warrior_70_off, string_format("ui\\mark_flag01.dxt"));
		}else if (nQuestID == 359)
		{
			if (Status == 2)
				g_pMain->SetTexImage(warrior_75_off, string_format("ui\\mark_flag02.dxt"));
			else
				g_pMain->SetTexImage(warrior_75_off, string_format("ui\\mark_flag01.dxt"));
			
		}
		else if (nQuestID == 365)
		{
			if (Status == 2)
				g_pMain->SetTexImage(warrior_80_off, string_format("ui\\mark_flag02.dxt"));
			else
				g_pMain->SetTexImage(warrior_80_off, string_format("ui\\mark_flag01.dxt"));

				}
		else if (nQuestID == 335)
		{
			if (Status == 2)
				g_pMain->SetTexImage(Rogue_70_off, string_format("ui\\mark_flag02.dxt"));
			else
				g_pMain->SetTexImage(Rogue_70_off, string_format("ui\\mark_flag01.dxt"));
		}
		else if (nQuestID == 347)
		{
			if (Status == 2)
				g_pMain->SetTexImage(Rogue_72_off, string_format("ui\\mark_flag02.dxt"));
			else
				g_pMain->SetTexImage(Rogue_72_off, string_format("ui\\mark_flag01.dxt"));

		}
		else if (nQuestID == 360)
		{
			if (Status == 2)
				g_pMain->SetTexImage(Rogue_75_off, string_format("ui\\mark_flag02.dxt"));
			else
				g_pMain->SetTexImage(Rogue_75_off, string_format("ui\\mark_flag01.dxt"));

		}
		else if (nQuestID == 366)
		{
			if (Status == 2)
				g_pMain->SetTexImage(Rogue_80_off, string_format("ui\\mark_flag02.dxt"));
			else
				g_pMain->SetTexImage(Rogue_80_off, string_format("ui\\mark_flag01.dxt"));

		}
		else if (nQuestID == 336)
		{
			if (Status == 2)
				g_pMain->SetTexImage(Mage_70_off, string_format("ui\\mark_flag02.dxt"));
			else
				g_pMain->SetTexImage(Mage_70_off, string_format("ui\\mark_flag01.dxt"));

		}
		else if (nQuestID == 348)
		{
			if (Status == 2)
				g_pMain->SetTexImage(Mage_72_off, string_format("ui\\mark_flag02.dxt"));
			else
				g_pMain->SetTexImage(Mage_72_off, string_format("ui\\mark_flag01.dxt"));

		}
		else if (nQuestID == 361)
		{
			if (Status == 2)
				g_pMain->SetTexImage(Mage_75_off, string_format("ui\\mark_flag02.dxt"));
			else
				g_pMain->SetTexImage(Mage_75_off, string_format("ui\\mark_flag01.dxt"));

		}
		else if (nQuestID == 367)
		{
			if (Status == 2)
				g_pMain->SetTexImage(Mage_80_off, string_format("ui\\mark_flag02.dxt"));
			else
				g_pMain->SetTexImage(Mage_80_off, string_format("ui\\mark_flag01.dxt"));

		}
		else if (nQuestID == 516)
		{
			//g_pMain->SetTexImage(warrior_80_off,  string_format("ui\\mark_flag_0%d.dxt",Status));
		}
		else if (nQuestID == 337)
		{
			if (Status == 2)
				g_pMain->SetTexImage(Priest_70_off, string_format("ui\\mark_flag02.dxt"));
			else
				g_pMain->SetTexImage(Priest_70_off, string_format("ui\\mark_flag01.dxt"));

		}
		else if (nQuestID == 349)
		{
		if (Status == 2)
			g_pMain->SetTexImage(Priest_72_off, string_format("ui\\mark_flag02.dxt"));
		else
			g_pMain->SetTexImage(Priest_72_off, string_format("ui\\mark_flag01.dxt"));

		}
		else if (nQuestID == 357)
		{
		if (Status == 2)
			g_pMain->SetTexImage(Priest_74_off, string_format("ui\\mark_flag02.dxt"));
		else
			g_pMain->SetTexImage(Priest_74_off, string_format("ui\\mark_flag01.dxt"));

		}
		else if (nQuestID == 362)
		{
		if (Status == 2)
			g_pMain->SetTexImage(Priest_75_off, string_format("ui\\mark_flag02.dxt"));
		else
			g_pMain->SetTexImage(Priest_75_off, string_format("ui\\mark_flag01.dxt"));

		}
		else if (nQuestID == 363)
		{
		if (Status == 2)
			g_pMain->SetTexImage(Priest_76_off, string_format("ui\\mark_flag02.dxt"));
		else
			g_pMain->SetTexImage(Priest_76_off, string_format("ui\\mark_flag01.dxt"));

		}
		else if (nQuestID == 364)
		{
		if (Status == 2)
			g_pMain->SetTexImage(Priest_78_off, string_format("ui\\mark_flag02.dxt"));
		else
			g_pMain->SetTexImage(Priest_78_off, string_format("ui\\mark_flag01.dxt"));

		}
		else if (nQuestID == 368)
		{
				if (Status == 2)
			g_pMain->SetTexImage(Priest_80_off, string_format("ui\\mark_flag02.dxt"));
		else
			g_pMain->SetTexImage(Priest_80_off, string_format("ui\\mark_flag01.dxt"));
			//g_pMain->SetTexImage(Priest_80_off,  string_format("ui\\mark_flag_0%d.dxt",Status));
		}
	}

	g_pMain->SetVisible(perkInfo.base_perks_view, false);
	uint16 perk[PERK_COUNT]{};
	for (int i = 0; i < PERK_COUNT; i++) perk[i] = pkt.read<uint16>();
	memcpy(perkType, perk, sizeof(perkType));
	OrderPerk();
}

void CSealToolTip::OrderPerk(uint16 page)
{
	for (int i = 0; i < 13; i++)
	{
		g_pMain->SetString(perkInfo.perkInfo[i].view_bonus_descp, "");
		g_pMain->SetString(perkInfo.perkInfo[i].view_bonus_perk, "");
		g_pMain->SetVisible(perkInfo.perkInfo[i].base, false);
	}

	std::vector<_PERK_INFO> pPerkInfo;
	foreach(itr, g_pMain->m_sPerkInfoArray)
		if (itr->second && itr->second->sStatus)
			pPerkInfo.push_back(*itr->second);

	if (pPerkInfo.empty())
		return;

	std::sort(pPerkInfo.begin(), pPerkInfo.end(),
		[](auto const& a, auto const& b) { return a.pIndex < b.pIndex; });

	pageCount = abs(ceil((double)pPerkInfo.size() / (double)UI_PERKCOUNT));
	if (pageCount < 1) pageCount = 1;
	g_pMain->SetString(perkInfo.str_page, string_format("%d/%d", cPage, pageCount));

	int p = (page - 1);
	if (p < 0) p = 0;
	size_t i = abs(p * UI_PERKCOUNT);

	uint8 c_slot = 0; uint16 count = 0;
	foreach(itr, pPerkInfo)
	{
		count++;

		if (count <= i)
			continue;
		
		g_pMain->SetVisible(perkInfo.perkInfo[c_slot].base, true);
		g_pMain->SetString(perkInfo.perkInfo[c_slot].view_bonus_descp, itr->strDescp);
		g_pMain->SetString(perkInfo.perkInfo[c_slot].view_bonus_perk, std::to_string(perkType[itr->pIndex]));

		c_slot++;

		if (c_slot >= UI_PERKCOUNT)
			break;
	}
}

void CSealToolTip::ParseUIElements()
{

	btn_view_perks = g_pMain->GetChildByID(m_dVTableAddr, "btn_view_perks");
	perkInfo.base_perks_view = g_pMain->GetChildByID(m_dVTableAddr, "base_perks_view");
	perkInfo.base_bonus = g_pMain->GetChildByID(perkInfo.base_perks_view, "base_bonus");
	perkInfo.btn_back_view = g_pMain->GetChildByID(perkInfo.base_perks_view, "btn_back_view");
	perkInfo.btn_left = g_pMain->GetChildByID(perkInfo.base_perks_view, "btn_left");
	perkInfo.btn_right = g_pMain->GetChildByID(perkInfo.base_perks_view, "btn_right");
	perkInfo.str_page = g_pMain->GetChildByID(perkInfo.base_perks_view, "str_page");
	for (int i = 0; i < 13; i++)
	{
		perkInfo.perkInfo[i].base = g_pMain->GetChildByID(perkInfo.base_bonus, string_format(xorstr("base_perk_bonus%d"), i + 1));
		perkInfo.perkInfo[i].view_bonus_descp = g_pMain->GetChildByID(perkInfo.perkInfo[i].base, "view_bonus_descp");
		perkInfo.perkInfo[i].view_bonus_perk = g_pMain->GetChildByID(perkInfo.perkInfo[i].base, "view_bonus_perk");
	}
	g_pMain->SetVisible(perkInfo.base_perks_view, false);
	base_skill_view = g_pMain->GetChildByID(m_dVTableAddr, "base_skill_view");
	m_70_quest_off = g_pMain->GetChildByID(base_skill_view, "70_quest_off");
	m_70_quest_reset_off = g_pMain->GetChildByID(base_skill_view, "m_70_quest_reset_off");
	warrior_70_off = g_pMain->GetChildByID(base_skill_view, "warrior_70_off");
	warrior_75_off = g_pMain->GetChildByID(base_skill_view, "warrior_75_off");
	warrior_80_off = g_pMain->GetChildByID(base_skill_view, "warrior_80_off");
	warrior_berserk_off = g_pMain->GetChildByID(base_skill_view, "warrior_berserk_off");
	Rogue_70_off = g_pMain->GetChildByID(base_skill_view, "Rogue_70_off");
	Rogue_72_off = g_pMain->GetChildByID(base_skill_view, "Rogue_72_off");
	Rogue_75_off = g_pMain->GetChildByID(base_skill_view, "Rogue_75_off");
	Rogue_80_off = g_pMain->GetChildByID(base_skill_view, "Rogue_80_off");
	Mage_70_off = g_pMain->GetChildByID(base_skill_view, "Mage_70_off");
	Mage_72_off = g_pMain->GetChildByID(base_skill_view, "Mage_72_off");
	Mage_75_off = g_pMain->GetChildByID(base_skill_view, "Mage_75_off");
	Mage_80_off = g_pMain->GetChildByID(base_skill_view, "Mage_80_off");
	Priest_70_off = g_pMain->GetChildByID(base_skill_view, "Priest_70_off");
	Priest_72_off = g_pMain->GetChildByID(base_skill_view, "Priest_72_off");
	Priest_74_off = g_pMain->GetChildByID(base_skill_view, "Priest_74_off");
	Priest_75_off = g_pMain->GetChildByID(base_skill_view, "Priest_75_off");
	Priest_76_off = g_pMain->GetChildByID(base_skill_view, "Priest_76_off");
	Priest_78_off = g_pMain->GetChildByID(base_skill_view, "Priest_78_off");
	Priest_80_off = g_pMain->GetChildByID(base_skill_view, "Priest_80_off");
}

void CSealToolTip::DropResultStatus(bool status)
{
	if (this)
	{
		g_pMain->SetVisible(m_btndropresult, status);
		g_pMain->SetState(m_btndropresult, status ? UI_STATE_BUTTON_NORMAL : UI_STATE_BUTTON_DISABLE);
	}
}

DWORD uiTool;
DWORD Func_tool;
bool CSealToolTip::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	uiTool = m_dVTableAddr;
	if (!pSender || pSender == (DWORD*)0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;

	if (pSender == (DWORD*)btn_view_perks)
	{
		cPage = 1;
		OrderPerk();
		g_pMain->SetVisible(perkInfo.base_perks_view, true);
		return true;
	}
	else if (pSender == (DWORD*)perkInfo.btn_back_view)
	{
		g_pMain->SetVisible(perkInfo.base_perks_view, false);
		return true;
	}

	return true;
}

void __stdcall UISealTooltipReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->pClientSealToolTip->ReceiveMessage(pSender, dwMsg);
	__asm
	{
		MOV ECX, uiTool
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_tool
		CALL EAX
	}
}


void CSealToolTip::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_tool = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)UISealTooltipReceiveMessage_Hook;
}
