#pragma once
class CUIInventoryPlug;
#include "stdafx.h"
#include "HSACSEngine.h"

class CUIInventoryPlug
{
public:
	DWORD m_dVTableAddr;
#if (HOOK_SOURCE_VERSION == 2369)
	DWORD sInventorySlot[28];
#endif
	DWORD btn_trash;
	DWORD base_cos;
	DWORD btn_EmblemView, btn_PathosView;
#if (HOOK_SOURCE_VERSION == 2369)
	uint8 sItemSlot[28];
	uint8 sSubCode[28];
	uint32 sItemID[28];
#endif
public:
	CUIInventoryPlug();
	~CUIInventoryPlug();
	void ParseUIElements();
	bool ReceiveMessage(DWORD* pSender, uint32_t dwMsg);
	uint32_t MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	void InitReceiveMessage();
private:

};