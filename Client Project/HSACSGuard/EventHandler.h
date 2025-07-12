#pragma once
#include "stdafx.h"
class CUIEventHandler;
#include "HSACSEngine.h"

class CUIEventHandler
{
public:
	CUIEventHandler();
	~CUIEventHandler();

	void ParseUIElements();
	void InitReceiveMessage();
	void EventHandler(Packet& pkt);
	bool ReceiveMessage(DWORD* pSender, uint32_t dwMsg);
	void Tick();
	void Close(uint8 sCode = 0);
	void Open(uint8 sCode = 0);
	DWORD m_dVTableAddr, grp_accept, quest_completed, skill_group, grp_juraid, grp_minimum, grp_maximum, by_details, by_bottom;
	DWORD btn_cancel, txt_time, btn_maximum, by_event_name, icon;
	CTimer* m_sTimes;
private:

};