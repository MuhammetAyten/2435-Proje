#pragma once

class CItemPacketExchange;

#include "../stdafx.h"
#include "../HSACSEngine.h"
#include "../HDRReader.h"
#include "../Timer.h"
#include "../N3BASE/N3UIBase.h"
#include "../N3BASE/N3UIString.h"
#include "../N3BASE/N3UIButton.h"

class CItemPacketExchange : public CN3UIBase
{
	CN3UIBase* grp_selected,* base_item;
	CN3UIString* txt_title;
	CN3UIArea* area_slot;
	CN3UIButton* btn_close;
	struct ItemPacket
	{
		uint32 nItemID;
		CN3UIIcon* icon;
		TABLE_ITEM_BASIC* tbl;
		ItemPacket()
		{
			nItemID = 0;
			icon = NULL;
			tbl = nullptr;
		}
	};
	vector<ItemPacket*> p_sItemPacket;

	struct HandlePacketItems
	{
		uint8 sSlot;
		uint32 itemID;
		TABLE_ITEM_BASIC* tbl;
		CN3UIIcon* icon;
	};

	HandlePacketItems m_sHandlePackets;
public:
	CItemPacketExchange();
	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);
	uint32_t MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	bool OnKeyPress(int iKey);
	void Close();
	void Open();
	//void Update(uint8 sSlot, uint32 ItemID, vector<uint32> p_sItems);
	void Update(uint32 ItemID);
};