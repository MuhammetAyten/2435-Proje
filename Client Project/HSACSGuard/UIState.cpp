#include "UIState.h"
#include "stdafx.h"

CUIClientStatePlug::CUIClientStatePlug()
{
	vector<int>offsets;
	offsets.push_back(0x50);
	offsets.push_back(0);
	offsets.push_back(0x1B8);
	offsets.push_back(0x134);
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);

	m_txtCash = 0;
	m_txtDagger = 0;
	m_txtSword = 0;
	m_txtClub = 0;
	m_txtAxe = 0;
	m_txtSpear = 0;
	m_txtArrow = 0;
	m_txtJamadar = 0;
	amktick = GetTickCount() + 150;
	m_iDagger = 0;
	m_iSword = 0;
	m_iClub = 0;
	m_iAxe = 0;
	m_iSpear = 0;
	m_iArrow = 0;
	m_iJamadar = 0;
	//m_btnResetReb = 0;
	//getStat = 0;
	//m_iCash = 0;
	////--------------------
	m_btnStr10 = 0;
	m_btnHp10 = 0;
	m_btnDex10 = 0;
	m_btnMp10 = 0;
	m_btnInt10 = 0;
	m_btnReset = 0;
	//--------------------

	cPage = pageCount = 1;
	_basePerks = basePerks();
	str_remPerkCount = 0;
	btn_perks = 0;
	back_character_page = 0;
	perkUseTick = 0;
	btn_reset_perks = 0;

	ParseUIElements();
	InitReceiveMessage();
 }

CUIClientStatePlug::~CUIClientStatePlug()
{
}

void CUIClientStatePlug::ParseUIElements()
{
	m_txtCash = g_pMain->GetChildByID(m_dVTableAddr, "Text_Cash");
	m_txtTLBalance = g_pMain->GetChildByID(m_dVTableAddr, "Text_tl_Cash");
	m_txtDagger = g_pMain->GetChildByID(m_dVTableAddr, "Text_DaggerAc");
	m_txtSword = g_pMain->GetChildByID(m_dVTableAddr, "Text_SwordAc");
	m_txtClub = g_pMain->GetChildByID(m_dVTableAddr, "Text_MaceAc");
	m_txtAxe = g_pMain->GetChildByID(m_dVTableAddr, "Text_AxeAc");
	m_txtSpear = g_pMain->GetChildByID(m_dVTableAddr, "Text_SpearAc");
	m_txtArrow = g_pMain->GetChildByID(m_dVTableAddr, "Text_BowAc");
	m_strFreeStatPoint = g_pMain->GetChildByID(m_dVTableAddr, "Text_BonusPoint");
	m_btnStr10 = g_pMain->GetChildByID(m_dVTableAddr, "btn_str");
	m_btnHp10 = g_pMain->GetChildByID(m_dVTableAddr, "btn_sta");
	m_btnDex10 = g_pMain->GetChildByID(m_dVTableAddr, "btn_Dex");
	m_btnMp10 = g_pMain->GetChildByID(m_dVTableAddr, "btn_Magic");
	m_btnInt10 = g_pMain->GetChildByID(m_dVTableAddr, "btn_Int");
	m_btnReset = g_pMain->GetChildByID(m_dVTableAddr, "btn_reset_stat");
	m_orgbtnReset = g_pMain->GetChildByID(m_dVTableAddr, "btn_preset");
	_basePerks.base = g_pMain->GetChildByID(m_dVTableAddr, "base_perks");
	_basePerks.str_page = g_pMain->GetChildByID(_basePerks.base, "str_page");
	_basePerks.btn_right = g_pMain->GetChildByID(_basePerks.base, "btn_right");
	_basePerks.btn_left = g_pMain->GetChildByID(_basePerks.base, "btn_left");
	str_remPerkCount = g_pMain->GetChildByID(_basePerks.base, "str_remPerkCount");
	back_character_page = g_pMain->GetChildByID(_basePerks.base, "back_character_page");
	btn_reset_perks = g_pMain->GetChildByID(_basePerks.base, "btn_reset_perks");
	_basePerks.base_bonus = g_pMain->GetChildByID(_basePerks.base, "base_bonus");

	for (int i = 0; i < UI_PERKCOUNT; i++) {
		_basePerks._basePerksBonus[i].base = g_pMain->GetChildByID(_basePerks.base_bonus, string_format("bonus%d", i + 1));
		_basePerks._basePerksBonus[i].btn_plus = g_pMain->GetChildByID(_basePerks._basePerksBonus[i].base, xorstr("btn_plus"));
		_basePerks._basePerksBonus[i].str_perk = g_pMain->GetChildByID(_basePerks._basePerksBonus[i].base, xorstr("str_perk"));
		_basePerks._basePerksBonus[i].str_maxPerk = g_pMain->GetChildByID(_basePerks._basePerksBonus[i].base, xorstr("str_maxPerk"));
		_basePerks._basePerksBonus[i].str_descp = g_pMain->GetChildByID(_basePerks._basePerksBonus[i].base, xorstr("str_descp"));
	}
	btn_perks = g_pMain->GetChildByID(m_dVTableAddr, "btn_perks");	
	resetPerks();
}

void CUIClientStatePlug::SetAntiDefInfo(Packet& pkt)
{
	pkt >> m_iDagger >> m_iSword >> m_iAxe >> m_iClub >> m_iSpear >> m_iArrow >> m_iJamadar >> g_pMain->moneyReq;

	g_pMain->SetString(m_txtDagger, std::to_string(m_iDagger));
	g_pMain->SetString(m_txtSword, std::to_string(m_iSword));
	g_pMain->SetString(m_txtAxe, std::to_string(m_iAxe));
	g_pMain->SetString(m_txtClub, std::to_string(m_iClub));
	g_pMain->SetString(m_txtSpear, std::to_string(m_iSpear));
	g_pMain->SetString(m_txtArrow, std::to_string(m_iArrow));
	//g_pMain->SetString(m_txtJamadar, std::to_string(m_iJamadar));
}

void CUIClientStatePlug::UpdateKC(uint32 cash, uint32 balance)
{
	g_pMain->SetString(m_txtCash, g_pMain->StringHelper->NumberFormat(cash));
	g_pMain->SetString(m_txtTLBalance, g_pMain->StringHelper->NumberFormat(balance));
}

bool CUIClientStatePlug::UpdatePointButton(bool visible)
{
	g_pMain->SetVisible(m_btnStr10, visible);
	g_pMain->SetVisible(m_btnHp10, visible);
	g_pMain->SetVisible(m_btnDex10, visible);
	g_pMain->SetVisible(m_btnInt10, visible);
	g_pMain->SetVisible(m_btnMp10, visible);
	return true;
}
void CUIClientStatePlug::UpdateUI()
{
	g_pMain->SetString(m_txtCash, g_pMain->StringHelper->NumberFormat(m_iCash));
	g_pMain->SetString(m_txtTLBalance, g_pMain->StringHelper->NumberFormat(m_iBalance));
	g_pMain->SetString(m_txtDagger, std::to_string(m_iDagger));
	g_pMain->SetString(m_txtSword, std::to_string(m_iSword));
	g_pMain->SetString(m_txtAxe, std::to_string(m_iAxe));
	g_pMain->SetString(m_txtClub, std::to_string(m_iClub));
	g_pMain->SetString(m_txtSpear, std::to_string(m_iSpear));
	g_pMain->SetString(m_txtArrow, std::to_string(m_iArrow));
	//g_pMain->SetString(m_txtJamadar, std::to_string(m_iJamadar));
}

DWORD pClientUIStateVTable;
bool CUIClientStatePlug::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	pClientUIStateVTable = m_dVTableAddr;
	if (!pSender || pSender == 0 || dwMsg != UIMSG_BUTTON_CLICK)
	{
		if (pSender == (DWORD*)m_btnReset) {
			std::string reqcoins = "Would you like to reset your stat points for :    " + std::to_string(g_pMain->Player.moneyreq) + " coins.";
			g_pMain->pClientHookManager->ShowMessageBox("Stat Reset", reqcoins, MsgBoxTypes::YesNo, ParentTypes::PARENT_PAGE_STATE);
		}
		else if (pSender == (DWORD*)m_btnStr10)
		{
			Packet pkt(WIZ_POINT_CHANGE, uint8(1));
			pkt << uint8(1);
			g_pMain->Send(&pkt);
		}
		else if (pSender == (DWORD*)m_btnHp10)
		{
			Packet pkt(WIZ_POINT_CHANGE, uint8(2));
			pkt << uint8(2);
			g_pMain->Send(&pkt);
		}
		else if(pSender == (DWORD*)m_btnDex10)
		{
			Packet pkt(WIZ_POINT_CHANGE, uint8(3));
			pkt << uint8(3);
			g_pMain->Send(&pkt);
		}
		else if(pSender == (DWORD*)m_btnInt10)
		{
			Packet pkt(WIZ_POINT_CHANGE, uint8(4));
			pkt << uint8(4);
			g_pMain->Send(&pkt);
		}
		else if (pSender == (DWORD*)m_btnMp10)
		{
			Packet pkt(WIZ_POINT_CHANGE, uint8(5));
			pkt << uint8(5);
			g_pMain->Send(&pkt);
		}
		return false;
	}

	if (pSender && dwMsg == 0x00000001)
	{
		if (pSender == (DWORD*)btn_perks) {
			perksOpen();
			return true;
		}
		else if (pSender == (DWORD*)back_character_page) {
			resetPerks();
			return true;
		}
		else if (pSender == (DWORD*)btn_reset_perks) {

			uint32 perk = 0;
			for (int i = 0; i < PERK_COUNT; i++)
				perk += g_pMain->sPerksDataInfo.sPerkType[i];

			if (perk == 0) {
				g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("There are no Perk points to reset.")).c_str(), 0xf2224c);
				return true;
			}

			g_pMain->pClientHookManager->ShowMessageBox(xorstr("Perk Reset"), string_format("Do you want to reset your PERKS POINT for %s ?",
				g_pMain->StringHelper->NumberFormat(g_pMain->sPerksDataInfo.sPerkCoins).c_str(), "Coins"), YesNo, PARENT_PERK_RESET);
			return true;
		}
		else if (pSender == (DWORD*)_basePerks.btn_right) {
			if (cPage >= pageCount)
				return true;

			cPage++;
			g_pMain->SetString(_basePerks.str_page, string_format(xorstr("%d/%d"), cPage, pageCount));
			reOrderPerk(cPage);
			return true;
		}
		else if (pSender == (DWORD*)_basePerks.btn_left) {
			if (cPage > 1) {
				cPage--;
				g_pMain->SetString(_basePerks.str_page, string_format(xorstr("%d/%d"), cPage, pageCount));
				reOrderPerk(cPage);
			}
			return true;
		}

		for (int i = 0; i < UI_PERKCOUNT; i++)
		{
			if (pSender == (DWORD*)_basePerks._basePerksBonus[i].btn_plus)
			{
				if (!g_pMain->sPerksDataInfo.sRemPerk) {
					g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("You do not have perk rights to use.")).c_str(), 0xf2224c);
					return true;
				}

				int32 index = _basePerks._basePerksBonus[i].index;
				if (index == -1)
					return true;

				auto itr = g_pMain->m_sPerkInfoArray.find(index);
				if (itr == g_pMain->m_sPerkInfoArray.end()) {
					g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("No such perk was found. please try later.")).c_str(), 0xf2224c);
					return true;
				}

				if (!itr->second->sStatus) {
					g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("This perk is deprecated.")).c_str(), 0xf2224c);
					return true;
				}

				if (g_pMain->sPerksDataInfo.sPerkType[index] >= itr->second->sMaxPerkCount) {
					g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("You have reached the maximum number for this perk. Please see other perks.")).c_str(), 0xf2224c);
					return true;
				}

				if (perkUseTick > getMSTime())
				{
					uint32 time = perkUseTick - getMSTime();
					time /= 1000;
					if (time < 1)
						time = 1;
					g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Please wait %d second to used perk."), time).c_str(), D3DCOLOR_ARGB(255, 255, 111, 0));
					return true;
				}

				perkUseTick = getMSTime() + (500);
				Packet newpkt(WIZ_HSACS_HOOK, uint8(HSACSOpCodes::PERKS));
				newpkt << uint8(perksSub::perkPlus) << _basePerks._basePerksBonus[i].index;
				g_pMain->Send(&newpkt);
				return true;
			}
		}
	}

	return false;
}


DWORD Func_State = 0;
void __stdcall pClientUIStateReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->pClientUIState->ReceiveMessage(pSender, dwMsg);

	__asm
	{
		MOV ECX, pClientUIStateVTable
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_State
		CALL EAX
	}
}

void CUIClientStatePlug::Tick()
{
	//if (!g_pMain->IsVisible(m_dVTableAddr))
	//	return;

	//if (amktick > GetTickCount())
	//	return;

	//if (g_pMain->GetState(m_btnStr10) == UI_STATE_BUTTON_DOWN)
	//{
	//	amktick = GetTickCount() + 100;
	//	pClientUIStateReceiveMessage_Hook((DWORD*)m_btnStr10, UIMSG_BUTTON_CLICK);
	//}else if (g_pMain->GetState(m_btnHp10) == UI_STATE_BUTTON_DOWN)
	//{
	//	amktick = GetTickCount() + 100;
	//	pClientUIStateReceiveMessage_Hook((DWORD*)m_btnHp10, UIMSG_BUTTON_CLICK);
	//}else if (g_pMain->GetState(m_btnDex10) == UI_STATE_BUTTON_DOWN)
	//{
	//	amktick = GetTickCount() + 100;
	//	pClientUIStateReceiveMessage_Hook((DWORD*)m_btnDex10, UIMSG_BUTTON_CLICK);
	//}else if (g_pMain->GetState(m_btnMp10) == UI_STATE_BUTTON_DOWN)
	//{
	//	amktick = GetTickCount() + 100;
	//
	//	pClientUIStateReceiveMessage_Hook((DWORD*)m_btnMp10, UIMSG_BUTTON_CLICK);
	//}else if (g_pMain->GetState(m_btnInt10) == UI_STATE_BUTTON_DOWN)
	//{
	//	amktick = GetTickCount() + 100;
	//	pClientUIStateReceiveMessage_Hook((DWORD*)m_btnInt10, UIMSG_BUTTON_CLICK);
	//}
	
}
void CUIClientStatePlug::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_State = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)pClientUIStateReceiveMessage_Hook;
}

void CUIClientStatePlug::resetPerks()
{
	g_pMain->SetVisible(m_btnReset, true);
	g_pMain->SetVisible(_basePerks.base, false);
}

void CUIClientStatePlug::reOrderPerk(uint16 page)
{
	g_pMain->SetVisible(_basePerks.base_bonus, false);
	for (int i = 0; i < UI_PERKCOUNT; i++)
	{
		_basePerks._basePerksBonus[i].index = -1;
		g_pMain->SetVisible(_basePerks._basePerksBonus[i].base, false);
		g_pMain->SetVisible(_basePerks._basePerksBonus[i].btn_plus, false);
		g_pMain->SetString(_basePerks._basePerksBonus[i].str_descp, "");
		g_pMain->SetString(_basePerks._basePerksBonus[i].str_perk, "");
		g_pMain->SetString(_basePerks._basePerksBonus[i].str_maxPerk, "");
	}

	std::vector<_PERK_INFO> pPerkInfo;
	foreach(itr, g_pMain->m_sPerkInfoArray)
		if (itr->second && itr->second->sStatus)
			pPerkInfo.push_back(*itr->second);

	if (pPerkInfo.empty())
		return;

	std::sort(pPerkInfo.begin(), pPerkInfo.end(),
		[](auto const& a, auto const& b) { return a.pIndex < b.pIndex; });

	g_pMain->SetVisible(_basePerks.base_bonus, true);

	pageCount = abs(ceil((double)pPerkInfo.size() / (double)UI_PERKCOUNT));
	if (pageCount < 1) pageCount = 1;
	g_pMain->SetString(_basePerks.str_page, string_format("%d/%d", cPage, pageCount));

	int p = (page - 1);
	if (p < 0) p = 0;
	size_t i = abs(p * UI_PERKCOUNT);

	uint8 c_slot = 0; uint16 count = 0;
	foreach(itr, pPerkInfo)
	{
		count++;

		if (count <= i)
			continue;

		_basePerks._basePerksBonus[c_slot].index = itr->pIndex;
		g_pMain->SetVisible(_basePerks._basePerksBonus[c_slot].base, true);
		std::string descp = string_format("%s%d %s", itr->sPercentage ? "%" : "", itr->sPerkCount, itr->strDescp.c_str());
		g_pMain->SetString(_basePerks._basePerksBonus[c_slot].str_descp, descp);
		g_pMain->SetString(_basePerks._basePerksBonus[c_slot].str_perk, std::to_string(g_pMain->sPerksDataInfo.sPerkType[itr->pIndex]));
		g_pMain->SetString(_basePerks._basePerksBonus[c_slot].str_maxPerk, string_format("max:%d", itr->sMaxPerkCount));
		g_pMain->SetVisible(_basePerks._basePerksBonus[c_slot].btn_plus, true);

		if (g_pMain->sPerksDataInfo.sPerkType[itr->pIndex] >= itr->sMaxPerkCount)
			g_pMain->SetVisible(_basePerks._basePerksBonus[c_slot].btn_plus, false);
		else
			g_pMain->SetVisible(_basePerks._basePerksBonus[c_slot].btn_plus, g_pMain->sPerksDataInfo.sRemPerk ? true : false);

		c_slot++;

		if (c_slot >= UI_PERKCOUNT)
			break;
	}
}

void CUIClientStatePlug::perksOpen()
{
	g_pMain->SetVisible(m_btnReset, false);
	cPage = pageCount = 1;
	g_pMain->SetString(str_remPerkCount, std::to_string(g_pMain->sPerksDataInfo.sRemPerk));
	reOrderPerk();
	g_pMain->SetVisible(_basePerks.base, true);
}

void CUIClientStatePlug::resetPerkPoint(Packet& pkt)
{
	pkt >> g_pMain->sPerksDataInfo.sRemPerk;
	memset(g_pMain->sPerksDataInfo.sPerkType, 0, sizeof(g_pMain->sPerksDataInfo.sPerkType));
	if (g_pMain->IsVisible(_basePerks.base)
		&& g_pMain->IsVisible(_basePerks.base_bonus)) {
		cPage = pageCount = 1;
		reOrderPerk();
	}
	g_pMain->SetString(str_remPerkCount, std::to_string(g_pMain->sPerksDataInfo.sRemPerk));
	g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("The perk points has been resed successfully. Congratulations.")).c_str(), 0x42d1eb);
}

void CUIClientStatePlug::perkUseItem(Packet& pkt)
{
	pkt >> g_pMain->sPerksDataInfo.sRemPerk;
	setOptions();
	g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("You have successfully activated the perk item. Congratulations.")).c_str(), 0x42d1eb);
}

void CUIClientStatePlug::setOptions()
{
	if (g_pMain->IsVisible(_basePerks.base) && g_pMain->IsVisible(_basePerks.base_bonus)) {
		for (int i = 0; i < UI_PERKCOUNT; i++)
		{
			int32 index = _basePerks._basePerksBonus[i].index;
			if (index == -1) continue;
			auto itr = g_pMain->m_sPerkInfoArray.find(index);
			if (itr == g_pMain->m_sPerkInfoArray.end())
				continue;

			if (g_pMain->sPerksDataInfo.sPerkType[index] >= itr->second->sMaxPerkCount)
				g_pMain->SetVisible(_basePerks._basePerksBonus[i].btn_plus, false);
			else
				g_pMain->SetVisible(_basePerks._basePerksBonus[i].btn_plus, g_pMain->sPerksDataInfo.sRemPerk ? true : false);

			g_pMain->SetString(_basePerks._basePerksBonus[i].str_perk, std::to_string(g_pMain->sPerksDataInfo.sPerkType[index]));
		}
	}
	g_pMain->SetString(str_remPerkCount, std::to_string(g_pMain->sPerksDataInfo.sRemPerk));
}

void CUIClientStatePlug::perkPlus(Packet& pkt)
{
	uint8 bResult = pkt.read<uint8>();
	if ((PerkErrorCode)bResult == PerkErrorCode::success)
	{
		uint32 index; uint16 perkCount, remPerkCount;
		pkt >> index >> perkCount >> remPerkCount;

		if (index >= PERK_COUNT)
			return;

		auto itr = g_pMain->m_sPerkInfoArray.find(index);
		if (itr == g_pMain->m_sPerkInfoArray.end())
			return;

		g_pMain->sPerksDataInfo.sPerkType[index]++;
		g_pMain->sPerksDataInfo.sRemPerk = remPerkCount;
		setOptions();
		g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("The perk has been used successfully. Congratulations.")).c_str(), 0x42d1eb);
	}
	else
	{
		switch ((PerkErrorCode)bResult)
		{
		case PerkErrorCode::remPerks:
			g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("You do not have perk rights to use.")).c_str(), 0x42d1eb);
			break;
		case PerkErrorCode::notFound:
			g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("No such perk was found. please try later..")).c_str(), 0x42d1eb);
			break;
		case PerkErrorCode::maxPerkCount:
			g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("You have reached the maximum number for this perk. Please see other perks.")).c_str(), 0x42d1eb);
			break;
		}
	}
}