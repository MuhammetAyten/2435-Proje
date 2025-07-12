#pragma once
class CUISearchMonsterDropPlug;
#include "stdafx.h"
#include "HSACSEngine.h"

struct ChestSlotBase
{
	DWORD icon, area, spItemSkill;
#if (HOOK_SOURCE_VERSION == 2369)
	DWORD m_sPercent;
#endif
	uint32 itemID;
	TABLE_ITEM_BASIC* tbl;
#if (HOOK_SOURCE_VERSION == 2369)
	ChestSlotBase() :itemID(0), tbl(NULL), icon(NULL), area(NULL), spItemSkill(NULL), m_sPercent(NULL){}
#else
	ChestSlotBase() : itemID(0), tbl(NULL), icon(NULL), area(NULL), spItemSkill(NULL){}
#endif

};

struct ChestMonsterList
{
	uint16 strID;
	std::string strName;
	ChestMonsterList(uint16 strID, std::string strName)
	{
		this->strID = strID;
		this->strName = strName;
	}
};

struct pBlockItem
{
	uint32 itemID;
	DWORD base, area, spItemSkill;
	DWORD icon;
	TABLE_ITEM_BASIC* tbl;
	pBlockItem() :itemID(0), area(NULL), icon(NULL), tbl(NULL), spItemSkill(NULL) {}
};

struct baseBlock
{
	DWORD base;
	pBlockItem* blockItem[10];
	DWORD str_page, btn_right, btn_left;
	DWORD btn_reset, btn_save;
};

class CUISearchMonsterDropPlug
{
	DWORD btn_close,  btn_search, btn_drops, btn_previously,  btn_next;
	DWORD edit_search;

	DWORD Drop_list;

#if (HOOK_SOURCE_VERSION == 1098)
	ChestSlotBase *dropBase[20];
#else
	ChestSlotBase *dropBase[25];
#endif

	DWORD base_item, base_percents;
	DWORD txt_monster_name,  str_title,  mob_search,  text_page;
	bool updated;
	uint32 page;
public:
	DWORD m_dVTableAddr;
	uint8 m_CurrentPage;
	uint8 m_MaxPage;
	vector<DropItem> myItems;
	vector<ChestMonsterList> tmpList;
	std::vector<uint32> mBlockItemList, mtmpBlockItemList;
	uint16 blockcPage, blockpageCount;
	baseBlock baseBlock;

	CUISearchMonsterDropPlug();
	~CUISearchMonsterDropPlug();
	void ParseUIElements();
	bool ReceiveMessage(DWORD* pSender, uint32_t dwMsg);
	void InitReceiveMessage();
	bool Load(HANDLE hFile);
	void ListDrop(uint16 ssid, vector<DropItem> drops);
	uint32_t MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	bool OnKeyPress(int iKey);
	void Close();
	void Open();
	virtual void	OpenWithAnimation(uint32 frameTime = 200);
	void OpenSearchMonster();
	void Save();
	void Reset();
	bool BlockItemAdd(uint32 itemid);
	bool BlockItemRemove(uint32 itemid);
	bool CheckBlockItem(uint32 itemid);
	void reOrderBlock(uint16 cPage = 1);
	std::string* GetItemText(CN3UIButton* item);
	void Order(uint8 page = 1);
};