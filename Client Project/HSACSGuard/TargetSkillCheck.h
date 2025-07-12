#pragma once

#include "../stdafx.h"
#include "../HSACSEngine.h"
#include "../HDRReader.h"

class CUITargetSkillCheck : public CN3UIBase
{
	CN3UIImage* icon_skill1_[20];
public:
	CUITargetSkillCheck();
	~CUITargetSkillCheck();
	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);
	bool OnKeyPress(int iKey);
	void Update(Packet pkt);
	void Close();
	void Open();
	void UpdatePosition();
	bool m_sTargetSkillStatus;
};