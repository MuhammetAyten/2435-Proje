#pragma once
class CUIRightCorner;
#include "stdafx.h"
#include "HSACSEngine.h"
#include "Timer.h"
#include "N3BASE/N3UIBase.h"
#include "N3BASE/N3UIString.h"
#include "N3BASE/N3UIButton.h"
#include "N3BASE/N3UIEdit.h"

class CUIRightCorner : public CN3UIBase
{
public:

	CN3UIButton* btn_drop;
	// Max Group
	CN3UIEdit* search_merchant;
public:
	string eventName;
	CUIRightCorner();
	~CUIRightCorner();
	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);

	void Close();
	void Open();
	void Tick();
	std::string lastChar;


};