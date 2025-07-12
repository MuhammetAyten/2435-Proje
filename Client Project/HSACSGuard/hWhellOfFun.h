#pragma once
#pragma once
class CUIClientWheelOfFun;
#include "stdafx.h"
#include "HSACSEngine.h"

class CUIClientWheelOfFun
{
public:
	uint8 lastItem;
	DWORD m_dVTableAddr;
	DWORD m_btnViewInfo;
	DWORD base_item;
	DWORD nItem[15];

	DWORD btn_stop;
	DWORD btn_start;
	DWORD btn_close;

public:
	CUIClientWheelOfFun();
	~CUIClientWheelOfFun();
	void ParseUIElements();
	bool ReceiveMessage(DWORD* pSender, uint32_t dwMsg);
	void InitReceiveMessage();
	void UpdateCash(uint32 kc);

	uint32_t MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	void StartChallange();
	void ImageChange(int8 sID);
	void StopChallange();
	void GiveItemImage(uint32 nItemID);
	bool isActiveWhile;
	DWORD KnightCash;
	void Tick();
	long TickSay;
	void IsActive(bool enable);
	void OpenWheel();

private:

};