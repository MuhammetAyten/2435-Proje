#include "stdafx.h"
#include "hSearchMonsterDrop.h"

extern tGetObjectBase Func_GetObjectBase;
extern size_t findCaseInsensitive(std::string data, std::string toSearch);

CUISearchMonsterDropPlug::CUISearchMonsterDropPlug()
{
	vector<int>offsets;
	offsets.push_back(0x28C);   // co_KnightsOperation Sat�r : 37
	offsets.push_back(0);

#if (HOOK_SOURCE_VERSION == 1098)
	for (int i = 0; i < 20; i++)
		dropBase[i] = new ChestSlotBase();
#else
	for (int i = 0; i < 25; i++)
		dropBase[i] = new ChestSlotBase();
#endif

	for (int i = 0; i < 10; i++)
		baseBlock.blockItem[i] = new pBlockItem;

	updated = false;
	page = 1;
	m_MaxPage = 1;
	btn_close = NULL;
	blockcPage = blockpageCount = 1;
	mBlockItemList.clear();
	mtmpBlockItemList.clear();
	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);
	ParseUIElements();
	InitReceiveMessage();
}

extern std::string GetName(DWORD obj);

CUISearchMonsterDropPlug::~CUISearchMonsterDropPlug()
{

}

void CUISearchMonsterDropPlug::ParseUIElements()
{
	btn_close = g_pMain->GetChildByID(m_dVTableAddr, "btn_close");
	edit_search = g_pMain->GetChildByID(m_dVTableAddr, "edit_search");
	btn_search = g_pMain->GetChildByID(m_dVTableAddr, "btn_search");
	btn_next = g_pMain->GetChildByID(m_dVTableAddr, "btn_next");
	btn_previously = g_pMain->GetChildByID(m_dVTableAddr, "btn_previously");
	Drop_list = g_pMain->GetChildByID(m_dVTableAddr, "Drop_list");
	text_page = g_pMain->GetChildByID(m_dVTableAddr, "text_page");

	base_item = g_pMain->GetChildByID(m_dVTableAddr, xorstr("base_item"));
#if (HOOK_SOURCE_VERSION == 2369)
	base_percents = g_pMain->GetChildByID(m_dVTableAddr, xorstr("grp_percents"));
#endif
#if (HOOK_SOURCE_VERSION == 1098 || HOOK_SOURCE_VERSION == 1534)
	for (uint8 i = 0; i < 20; i++) {
#else
	for (uint8 i = 0; i < 25; i++) {
#endif
		dropBase[i]->area = g_pMain->GetChildByID(base_item, string_format(xorstr("bArea%d"), i+1));
#if (HOOK_SOURCE_VERSION == 2369)
		dropBase[i]->m_sPercent = g_pMain->GetChildByID(base_percents, string_format(xorstr("percent_%d"), i + 1));
#endif
		g_pMain->SetVisible(dropBase[i]->area, false);
#if (HOOK_SOURCE_VERSION == 2369)
		g_pMain->SetVisible(dropBase[i]->m_sPercent, false);
#endif
		dropBase[i]->itemID = 0;
	}

	baseBlock.base = g_pMain->GetChildByID(m_dVTableAddr,xorstr("base_block"));
	for (int i = 0; i < 10; i++) {
		baseBlock.blockItem[i]->itemID = 0;
		baseBlock.blockItem[i]->base = g_pMain->GetChildByID(baseBlock.base, string_format("base%d", i + 1));
		baseBlock.blockItem[i]->area = g_pMain->GetChildByID(baseBlock.blockItem[i]->base,xorstr("area"));
	}
	baseBlock.btn_left = g_pMain->GetChildByID(baseBlock.base,xorstr("btn_left"));
	baseBlock.btn_right = g_pMain->GetChildByID(baseBlock.base, xorstr("btn_right"));
	baseBlock.str_page = g_pMain->GetChildByID(baseBlock.base, xorstr("str_page"));
	baseBlock.btn_reset = g_pMain->GetChildByID(baseBlock.base, xorstr("btn_reset"));
	baseBlock.btn_save = g_pMain->GetChildByID(baseBlock.base, xorstr("btn_save"));
	g_pMain->SetString(baseBlock.str_page, xorstr("1/1"));

	txt_monster_name = g_pMain->GetChildByID(m_dVTableAddr, "txt_monster_name");
	str_title = g_pMain->GetChildByID(m_dVTableAddr, "str_title");
	mob_search = g_pMain->GetChildByID(m_dVTableAddr, "edit_search");
	g_pMain->UIScreenCenter(m_dVTableAddr);
}

DWORD uiDropList;

void CUISearchMonsterDropPlug::Save()
{
	if (!updated)
		return;

	updated = false;
	mBlockItemList = mtmpBlockItemList;

	Packet newpkt(WIZ_HSACS_HOOK, uint8(HSACSOpCodes::CHEST_BLOCKITEM));
	newpkt << uint8(0) << (uint16)mBlockItemList.size();
	foreach(itr, mBlockItemList) newpkt << *itr;
	g_pMain->Send(&newpkt);

	g_pMain->WriteInfoMessageExt(xorstr("Drop settings are saved."), D3DCOLOR_ARGB(255, 255, 111, 0));
}

void CUISearchMonsterDropPlug::Reset()
{
	updated = true;
	for (auto drop : dropBase) {
		if (drop->icon) {
			for (auto id : mtmpBlockItemList) {
				if (id == drop->itemID)
					g_pMain->SetItemFlag(drop->icon,uint8(ItemFlag::ITEM_FLAG_NONE));
			}
		}
	}

	mtmpBlockItemList.clear();
	blockcPage = blockpageCount = 1;
	reOrderBlock();
	g_pMain->SetString(baseBlock.str_page,string_format(xorstr("%d/%d"), blockcPage, blockpageCount));
	g_pMain->WriteInfoMessageExt(xorstr("The list has been cleared."), D3DCOLOR_ARGB(255, 255, 111, 0));
}

bool CUISearchMonsterDropPlug::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	uiDropList = m_dVTableAddr;
	if (!g_pMain->IsVisible(m_dVTableAddr) || !pSender || pSender == 0 || dwMsg != 0x00000001)
		return false;

	if (pSender ==(DWORD*) btn_close)
		Close();
	else if (pSender ==(DWORD*) btn_search)
	{
		g_pMain->ClearListString(Drop_list);
		tmpList.clear();

		std::string query = g_pMain->GetEditString(mob_search).c_str();
		
		auto mobs = g_pMain->pClientTBLManager->GetMobTable();
		for (auto &it : mobs)
		{
			if (it.second->strName.empty()) continue;
			if (findCaseInsensitive(it.second->strName, query) != string::npos)
			{
				std::string name = it.second->strName;
				if (it.second->strName.size() >= 20)
					name = name.substr(0, 20) + "...";
				g_pMain->AddListString(Drop_list, name, 0x00FF00);
				tmpList.push_back(ChestMonsterList(it.second->ID, name));
			}
		}
		return true;
	}
	else if (pSender ==(DWORD*) btn_previously && m_CurrentPage > 1)
	{
		Order(--m_CurrentPage);
		if (text_page)
			g_pMain->SetString(text_page,string_format(xorstr("%d/%d"), m_CurrentPage, m_MaxPage));
		return true;
	}
	else if (pSender ==(DWORD*) btn_next && m_CurrentPage < m_MaxPage)
	{
		Order(++m_CurrentPage);
		if (text_page)
			g_pMain->SetString(text_page,string_format(xorstr("%d/%d"), m_CurrentPage, m_MaxPage));
		return true;
	}

	DWORD bBlockReset = baseBlock.btn_reset;
	DWORD bBlockSave = baseBlock.btn_save;
	DWORD bBlockRight = baseBlock.btn_right, bBlockleft = baseBlock.btn_left;
	if (bBlockRight > 0 && pSender == (DWORD*)bBlockRight)
	{
		if (blockcPage >= blockpageCount)
			return true;

		blockcPage++;
		reOrderBlock(blockcPage);
		g_pMain->SetString(baseBlock.str_page,string_format(xorstr("%d/%d"), blockcPage, blockpageCount));
		return true;
	}
	else if (bBlockleft > 0 && pSender == (DWORD*)bBlockleft)
	{
		if (blockcPage > 1) blockcPage--;
		reOrderBlock(blockcPage);
		g_pMain->SetString(baseBlock.str_page, string_format(xorstr("%d/%d"), blockcPage, blockpageCount));
		return true;
	}
	else if (bBlockReset > 0 && pSender == (DWORD*)bBlockReset)
	{
		if (mtmpBlockItemList.empty()) {
			g_pMain->WriteInfoMessageExt(xorstr("The list is already empty."), D3DCOLOR_ARGB(255, 255, 111, 0));
			return true;
		}

		Packet newpkt(WIZ_HSACS_HOOK, uint8(HSACSOpCodes::CHEST_BLOCKITEM));
		newpkt << uint8(1);
		g_pMain->Send(&newpkt);
		return true;
	}
	else if (bBlockSave > 0 && pSender == (DWORD*)bBlockSave)
	{
		Save();
		return true;
	}
	return true;
}

void CUISearchMonsterDropPlug::Order(uint8 page)
{
	for (auto& it : dropBase)
	{
		it->itemID = NULL;
		it->tbl = nullptr;
		it->spItemSkill = 0;
		if(it->icon) g_pMain->SetVisible(it->icon, false);
		it->icon = 0;
		g_pMain->SetVisible(it->area, false);
#if (HOOK_SOURCE_VERSION == 2369)
		g_pMain->SetVisible(it->m_sPercent, false);
#endif
		g_pMain->SetString(txt_monster_name, "");
	}

	m_CurrentPage = page;
#if (HOOK_SOURCE_VERSION == 1098 || HOOK_SOURCE_VERSION == 1534)
	m_MaxPage = abs(ceil((double)myItems.size() / (double)20));
#else
	m_MaxPage = abs(ceil((double)myItems.size() / (double)25));
#endif

	if (m_CurrentPage > m_MaxPage)
		m_CurrentPage = m_MaxPage;
	if (text_page) g_pMain->SetString(text_page, string_format(xorstr("%d/%d"), m_CurrentPage, m_MaxPage));

	int p = (page - 1);
	if (p < 0) p = 0;
	size_t i = 0;
	uint16 baseIndex = 0;
#if (HOOK_SOURCE_VERSION == 1098 || HOOK_SOURCE_VERSION == 1534)
	i = abs(p * 20);
#else
	i = abs(p * 25);
#endif

	for (auto base : dropBase)
	{
		if (i >= myItems.size())
			goto nope;


		if (!&myItems[i])
			goto nope;


		if (base)
		{
			TABLE_ITEM_BASIC* tbl = g_pMain->pClientTBLManager->GetItemData(myItems[i].nItemID);
			if (!tbl)
				goto nope;

			base->itemID = myItems[i].nItemID;
			base->tbl = tbl;

			if (!g_pMain->_InitIcon(base->icon, base->area, myItems[i].nItemID))
				goto nope;
	
			g_pMain->SetVisible(base->area, true);
#if (HOOK_SOURCE_VERSION == 2369)
#if 0
			g_pMain->SetVisible(base->m_sPercent, true);
			g_pMain->SetString(base->m_sPercent, string_format(xorstr("%.2f%%"), (float)(myItems[i].sPercent / 100)));
#else
			float per = (float)((float)myItems[i].sPercent / (float)100) > 100 ? 100 : (float)((float)myItems[i].sPercent / (float)100);
			g_pMain->SetVisible(base->m_sPercent, true);
			g_pMain->SetString(base->m_sPercent, string_format(xorstr("%.2lf%%"), per));
#endif
#endif
			base->spItemSkill = g_pMain->_CreateIconItemSkill(base->spItemSkill, base->icon, base->itemID, 1);
			g_pMain->SetRegion(base->icon, base->area);
			
			if (CheckBlockItem(base->itemID))
				g_pMain->SetItemFlag(base->icon, uint8(ItemFlag::ITEM_FLAG_SEALED));
			else
				g_pMain->SetItemFlag(base->icon, uint8(ItemFlag::ITEM_FLAG_NONE));
		}
		else {
		nope:
			if (base) {
				g_pMain->SetItemFlag(base->icon, uint8(ItemFlag::ITEM_FLAG_NONE));
				g_pMain->SetVisible(base->area, false);
#if (HOOK_SOURCE_VERSION == 2369)
				g_pMain->SetVisible(base->m_sPercent, false);
#endif
				if (base->icon) g_pMain->SetVisible(base->icon, false);
			}
		}
		i++;
	}
}

void CUISearchMonsterDropPlug::ListDrop(uint16 ssid, vector<DropItem> drops)
{
	myItems = drops;
	m_CurrentPage = 1;
	m_MaxPage = 1;
	Order();
}

bool CUISearchMonsterDropPlug::BlockItemAdd(uint32 itemid)
{
	if (mtmpBlockItemList.size() >= 100) {
		g_pMain->WriteInfoMessageExt(xorstr("You can add a maximum of 100 items."), D3DCOLOR_ARGB(255, 255, 111, 0));
		return false;
	}

	if (std::find(mtmpBlockItemList.begin(), mtmpBlockItemList.end(), itemid) != mtmpBlockItemList.end())
		return false;

	updated = true;
	mtmpBlockItemList.push_back(itemid);
	return true;
}

void CUISearchMonsterDropPlug::reOrderBlock(uint16 page)
{
	blockpageCount = abs(ceil((double)mtmpBlockItemList.size() / (double)10));
	if (blockpageCount < 1) blockpageCount = 1;
	g_pMain->SetString(baseBlock.str_page,string_format(xorstr("%d/%d"), blockcPage, blockpageCount));

	for (int i = 0; i < 10; i++)
	{
		g_pMain->SetVisible(baseBlock.blockItem[i]->area,false);
		if(baseBlock.blockItem[i]->icon) g_pMain->SetVisible(baseBlock.blockItem[i]->icon,false);
		baseBlock.blockItem[i]->itemID = 0;
		baseBlock.blockItem[i]->spItemSkill = 0;
		baseBlock.blockItem[i]->tbl = nullptr;
	}

	int p = (page - 1);
	if (p < 0) p = 0;
	size_t i = abs(p * 10);

	uint8 c_slot = 0; uint16 count = 0;
	foreach(itr, mtmpBlockItemList)
	{
		count++;
		if (count <= i)
			continue;

		TABLE_ITEM_BASIC* tbl = g_pMain->pClientTBLManager->GetItemData(*itr);
		if (!tbl) continue;

		auto* base = baseBlock.blockItem[c_slot];
		if (!base)
			continue;

		c_slot++;

		base->itemID = (*itr);
		base->tbl = tbl;
		
		if (g_pMain->_InitIcon(base->icon, base->area, (*itr)))
		{
			base->spItemSkill = g_pMain->_CreateIconItemSkill(base->spItemSkill, base->icon, (*itr), 1);
			g_pMain->SetRegion(base->icon, base->area);
			g_pMain->SetVisible(base->icon, true);
			g_pMain->SetVisible(base->area, true);
		}
		
		if (c_slot >= 10)
			break;
	}
}

bool CUISearchMonsterDropPlug::BlockItemRemove(uint32 itemid)
{
	if (std::find(mtmpBlockItemList.begin(), mtmpBlockItemList.end(), itemid) == mtmpBlockItemList.end())
		return false;

	updated = true;
	mtmpBlockItemList.erase(std::remove(mtmpBlockItemList.begin(), mtmpBlockItemList.end(), itemid), mtmpBlockItemList.end());
	return true;
}

bool CUISearchMonsterDropPlug::CheckBlockItem(uint32 itemid)
{
	return std::find(mtmpBlockItemList.begin(), mtmpBlockItemList.end(), itemid) != mtmpBlockItemList.end();
}

uint32_t CUISearchMonsterDropPlug::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	if (!g_pMain->IsVisible(m_dVTableAddr) || !g_pMain->IsIn(m_dVTableAddr,ptCur.x,ptCur.y))
		return 0;

	if ((dwFlags & 0x00000040))
	{
		if (g_pMain->IsIn(base_item, ptCur.x, ptCur.y))
		{
			for (auto base : dropBase)
			{
				if (base->icon && g_pMain->IsIn(base->icon, ptCur.x, ptCur.y) && base->tbl != nullptr)
				{
					if (base->itemID == 900000000)
						return 0;

					if (CheckBlockItem(base->itemID) || !BlockItemAdd(base->itemID))
						return 0;

					blockcPage = blockpageCount = abs(ceil((double)mtmpBlockItemList.size() / (double)10));
					if (blockcPage < 1) blockcPage = blockpageCount = 1;
					reOrderBlock(blockcPage);
					g_pMain->SetItemFlag(base->icon,uint8(ItemFlag::ITEM_FLAG_SEALED));
				}
			}
		}
		else if (g_pMain->IsIn(baseBlock.base, ptCur.x, ptCur.y))
		{
			for (auto base : baseBlock.blockItem)
			{
				if (base->icon && g_pMain->IsIn(base->icon, ptCur.x, ptCur.y) && base->tbl != nullptr)
				{
					if (base->itemID == 900000000)
						return 0;

					if (!CheckBlockItem(base->itemID) || !BlockItemRemove(base->itemID))
						return 0;

					for (auto drop : dropBase) {
						if (drop->icon && drop->itemID == base->itemID) {
							g_pMain->SetItemFlag(drop->icon,uint8(ItemFlag::ITEM_FLAG_NONE));
							break;
						}
					}

					int maxCount = abs(ceil((double)mtmpBlockItemList.size() / (double)10));
					if (blockcPage > 1 && blockcPage > maxCount) blockcPage = maxCount;
					reOrderBlock(blockcPage);
				}
			}
		}
	}

	if ((dwFlags & 0x4) && g_pMain->IsIn(Drop_list, ptCur.x, ptCur.y))
	{
		auto index = g_pMain->GetListSelect(Drop_list);
		if (index < 0)
			return true;

		if (index >= tmpList.size())
			return true;

		auto& mob = tmpList[index];

		Packet pkt(WIZ_HSACS_HOOK, uint8(HSACSOpCodes::DROP_REQUEST));
		pkt << uint8(3) << uint16(mob.strID);
		g_pMain->Send(&pkt);
		return 0;
	}

	for (auto it : dropBase)
	{
		if (it->icon)
		{
			POINT pos = g_pMain->GetUiPos(it->icon);
			if (ptCur.x >= pos.x && ptCur.y >= pos.y && ptCur.x <= pos.x + 45 && ptCur.y <= pos.y + 45) {
				if (it->spItemSkill != NULL)
				{
					g_pMain->_ShowToolTip(it->spItemSkill, ptCur.x, ptCur.y, TOOLTIP_TYPE::DROP_BOX);
					return 0;
				}
			}
		}
	}

	for (auto it : baseBlock.blockItem)
	{
		if (it->icon)
		{
			POINT pos = g_pMain->GetUiPos(it->icon);
			if (ptCur.x >= pos.x && ptCur.y >= pos.y && ptCur.x <= pos.x + 45 && ptCur.y <= pos.y + 45) {
				if (it->spItemSkill != NULL)
				{
					g_pMain->_ShowToolTip(it->spItemSkill, ptCur.x, ptCur.y, TOOLTIP_TYPE::DROP_BOX);
					return 0;
				}
			}
		}
	}
	return 0;
}

void CUISearchMonsterDropPlug::OpenWithAnimation(uint32 frameTime)
{
	Open();
}

DWORD Func_Search=0;
void CUISearchMonsterDropPlug::Close()
{
	g_pMain->SetEditString(edit_search,"");
	g_pMain->EditKillFocus(edit_search);
	for (auto& it : dropBase)
	{
		it->itemID = NULL;
		it->tbl = nullptr;
		it->spItemSkill = 0;
#if (HOOK_SOURCE_VERSION == 2369)
		g_pMain->SetVisible(it->m_sPercent, false);
#endif
		g_pMain->SetVisible(it->area, false);
		g_pMain->SetString(txt_monster_name,"");
	}
	g_pMain->SetVisible(m_dVTableAddr,false);
}

void CUISearchMonsterDropPlug::Open()
{
	if (g_pMain->IsVisible(m_dVTableAddr))
		return;

	g_pMain->ClearListString(Drop_list);
	tmpList.clear();

	auto mobs = g_pMain->pClientTBLManager->GetMobTable();
	Func_GetObjectBase = (tGetObjectBase)KO_FMBS;
	for (uint16 i = NPC_BAND; i < 32567; i++) {
		if (DWORD mob = Func_GetObjectBase(*(DWORD*)KO_FLDB, i, 1))
		{
			std::string mobName = GetName(mob);
			if (mobName.empty())
				continue;

			uint16 protoID = *(uint16*)(mob + KO_SSID);
			if (mobs.find(protoID) == mobs.end())
				continue;

			if (mobName.empty()) continue;
			if (std::find_if(tmpList.begin(), tmpList.end(), [&](const ChestMonsterList& a)
				{ return a.strID == protoID; }) == tmpList.end())
			{
				std::string name = mobName;

				if (mobName.size() >= 20)
					name = name.substr(0, 20) + "...";

				g_pMain->AddListString(Drop_list, name, 0x00FF00);
				tmpList.push_back(ChestMonsterList(protoID, name));
			}
		}
	}


	g_pMain->UIScreenCenter(m_dVTableAddr);
	g_pMain->SetVisible(m_dVTableAddr, true);
}

void __stdcall UiSearchReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->uiSearchMonster->ReceiveMessage(pSender, dwMsg);
	__asm
	{
		MOV ECX, uiDropList
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_Search
		CALL EAX
	}
}
void CUISearchMonsterDropPlug::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_Search = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)UiSearchReceiveMessage_Hook;
}