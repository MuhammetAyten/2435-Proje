#pragma once
#include "stdafx.h"
class CUIClientTopRightNewPlug;
#include "HSACSEngine.h"

class CUIClientTopRightNewPlug
{
public:
	bool BasladimiBaslamadimi;
	CUIClientTopRightNewPlug();
	~CUIClientTopRightNewPlug();
	std::string FacebookURL, DiscordURL, LiveURL;
	void ParseUIElements();
	void InitReceiveMessage();
	bool ReceiveMessage(DWORD* pSender, uint32_t dwMsg);
	void DropResultStatus(bool status);
	void UpdateTopLeftVisible(uint8_t Live = 0 , std::string LiveURLv = "");
	void OpenCollectionRaceButton();
	void OpenLotteryButton();
	void OpenCindirellaButton();
	void HideCollectionRaceButton();
	void HideLotteryButton();
	void HideCindirellaButton();

	DWORD m_dVTableAddr;
	DWORD m_btnsituation;
	DWORD m_lottery;
	DWORD m_collectionrace;
	DWORD m_cindirella;
	DWORD m_btndropresult;
	DWORD m_btnbug;
	DWORD m_btnlive;
	DWORD m_btnbaselive;
};