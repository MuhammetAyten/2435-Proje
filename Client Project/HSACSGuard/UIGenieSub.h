#pragma once
class CUIGenieSubPlug;
#include "stdafx.h"
#include "HSACSEngine.h"

class CUIGenieSubPlug
{
public:
	DWORD m_dVTableAddr;
	DWORD m_textGenieTime;
	DWORD TargetTwo, TargetSam;

public:
	CUIGenieSubPlug();
	~CUIGenieSubPlug();
	void ParseUIElements();

private:

};