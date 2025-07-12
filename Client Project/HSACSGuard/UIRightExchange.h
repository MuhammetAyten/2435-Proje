#pragma once

class CUIRightExchangePlug;

#include "stdafx.h"
#include "HSACSEngine.h"
#include "Timer.h"
#if(HOOK_SOURCE_VERSION == 2369 || HOOK_SOURCE_VERSION == 1098)
class UIObject;

class CUIRightExchangePlug
{
	bool posSet;
public:
	struct ItemInfo
	{
		uint32 nItemID;
		uint16 nRentalTime;
		DWORD icon;

		ItemInfo()
		{
			nItemID = 0;
			nRentalTime = 0;
			icon = NULL;
		}
	};

	struct ExchangeInformation
	{
		uint32 nItemID;
		uint16 sPercent;
	};

	vector<ExchangeInformation*>GetExchangeList;
	DWORD Base_PacketExchange, el_Base, click_area, base_item, base_item_area, txt_title, btn_exchange, btn_close, grp_selected, base_itemicon;
	DWORD ItemBase[25];
	DWORD area_slot[25];

	DWORD btn_exit, Base_Exchange, Base_btn_exchange, Unit_0, Edit_Count, String_Count, Text_0, ItemSlot;
	uint8 sType;
	int8 m_iSlot;
	bool ExchangeItemStatus;
	uint32 m_iCount, nBaseItemID;
	uint32 sSelectItem;
	uint8 bCurrentPage;
	uint8 bPageCount;
	DWORD percent[25], percent_base, btn_down, page, btn_next;
	std::vector<ItemInfo*> m_Icon;
	std::vector<ItemInfo*> Right;
	CUIRightExchangePlug();
	~CUIRightExchangePlug();
	CTimer* m_Timer;
	INLINE bool isStard() { return ExchangeItemStatus; };
	void GetExchangePage(uint8 bPage);
	bool ReceiveMessage(DWORD* pSender, uint32_t dwMsg);
	uint32_t MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	void Close();
	void Open(uint8 sCode = 0);
	void InitReceiveMessage();
	void ParseUIElements();
	void Tick();
	void CUIRightExchangePlugPacket(Packet pkt);
	std::vector <uint32> pRightClickExchangeReward;
	std::vector <uint32> pRightClickExchangeAll;
	std::vector <uint32> pRightClickExchangeKnightCash;
	std::vector <uint32> pRightClickExchangePremium;
	std::vector <uint32> pRightClickExchangeGenie;
	std::vector <uint32> pRightClickExchangeKnightTL;
#if(HOOK_SOURCE_VERSION == 1098)
	std::vector <uint32> pRightClickGeneratorExchange;
#endif
private:
#define PAGE_COUNT 25
};
#endif