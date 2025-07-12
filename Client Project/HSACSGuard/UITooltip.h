#pragma once
#include "stdafx.h"
class CUITooltip;
#include "HSACSEngine.h"

class CUITooltip
{
public:
	CUITooltip();
	~CUITooltip();

	DWORD string0;

	void ParseUIElements();

	DWORD m_dVTableAddr;

private:

};