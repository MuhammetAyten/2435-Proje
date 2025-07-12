#pragma once
#include "stdafx.h"
class CUIClientTargetBarPlug;
#include "HSACSEngine.h"

class CUIClientTargetBarPlug
{
public:
	CUIClientTargetBarPlug();
	~CUIClientTargetBarPlug();

	void ParseUIElements();

	DWORD str_name;

	DWORD m_dVTableAddr;

	DWORD m_btnDrop;
	DWORD m_user_info;
	DWORD m_dTextTargetHp;

	void SetTargetHp(Packet& pkt);
	void TargetNameSet(uint16 GetID);
	bool ReceiveMessage(DWORD* pSender, uint32_t dwMsg);
	void InitReceiveMessage();

	void SetInfo(DWORD color, uint8 lvl, std::string name);

private:

};