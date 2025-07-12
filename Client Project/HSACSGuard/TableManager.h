#pragma once
class CTableManager;
#include "N3BASE/GameDef.h"
#include "stdafx.h"
#include "N3BASE/N3TableBase.h"

#include "HSACSEngine.h"


class CTableManager
{
public:
	
	CTableManager();
	~CTableManager();
	void Init();
	__TABLE_ITEM_BASIC* GetItemData(uint32 ID);
	__TABLE_ITEM_EXT* GetExtData(uint32 extNum, uint32 ID);
	std::map<uint32_t, __TABLE_ITEM_BASIC>* GetItemTable();
	TABLE_MOB* GetMobData(uint32 ID);
	TABLE_NPC* GetNpcData(uint32 ID);

	std::map<uint32_t, TABLE_MOB*>  GetMobTable();
	std::map<uint32_t, TABLE_NPC>* GetNpcTable();
private:

};