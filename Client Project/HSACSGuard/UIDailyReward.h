#pragma once
class CUIDailyRewardPlug;
#include "stdafx.h"
#include "HSACSEngine.h"

class CUIDailyRewardPlug
{
//#if(HOOK_SOURCE_VERSION == 2369)
public:
	DWORD m_dVTableAddr;
	DWORD group_daily_reward[25],  group_select,  group_cumulative_reward[25];
	DWORD btn_close;
	DWORD str_item_reward_count[25],  str_days[25],  str_item_reward[25],  str_remaining_time,  str_item_reward_cumulative[3];
	DWORD img_complete[25],  img_accept[25],  img_item_reward[25],  img_select,  img_item_cumulative_reward[25];

	struct ItemInfo
	{
		uint32 nItemID;
		DWORD icon;
		TABLE_ITEM_BASIC* tbl;
		ItemInfo()
		{
			nItemID = 0;
			icon = NULL;
			tbl = nullptr;
		}
	};

	uint8 TotalLogging;

	vector<ItemInfo*> items;
	vector<ItemInfo*> Cumulative;
	CTimer* m_Timer;

public:
	CUIDailyRewardPlug();
	~CUIDailyRewardPlug();
	uint8 kills;
	void ParseUIElements();
	bool ReceiveMessage(DWORD* pSender, uint32_t dwMsg);
	void Update(uint32 Itemid[], uint8 isUsed[], uint8 sDays[], uint32 sCumulative[]);
	uint32_t MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	void Open();
	void Close();
	void OnKeyPress(int iKey);
	void InitReceiveMessage();
	void Tick();
private:
//#endif
};