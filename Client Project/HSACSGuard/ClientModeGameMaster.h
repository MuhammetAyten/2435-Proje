#pragma once
class CUIClientModeGameMasterPlug;
#include "stdafx.h"
#include "HSACSEngine.h"
#include "Timer.h"
#include "N3BASE/N3UIBase.h"
#include "N3BASE/N3UIString.h"
#include "N3BASE/N3UIButton.h"
#include "N3BASE/N3UIEdit.h"

class CUIClientModeGameMasterPlug : public CN3UIBase
{
public:
	CN3UIButton* btn_close,* btn_pus, * btn_exchange, * btn_magics, * btn_special,* btn_Mlist, * btn_event, * btn_royal,
		* btn_MF,* btn_autoMFF,* btn_drop, * btn_itemop,* btn_itemsell,* btn_crsetting,* btn_crmonster,* btn_resource,
		* btn_esn,* btn_ranks,* btn_dungeon,* btn_upgrade,* btn_Cstone;
	CN3UIBase* reload_tables;
public:
	CUIClientModeGameMasterPlug();
	~CUIClientModeGameMasterPlug();
	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);
	void Close();
	void Open();
private:
};