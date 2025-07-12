#include "TargetSkillCheck.h"


extern CSpell* GetSkillBase(int iSkillID);

CUITargetSkillCheck::CUITargetSkillCheck()
{
	for (int i = 0; i < 20; i++)
		icon_skill1_[i] = 0;

	m_sTargetSkillStatus = false;
}

CUITargetSkillCheck::~CUITargetSkillCheck() {}

bool CUITargetSkillCheck::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false)
		return false;

	CN3UIBase* PieceCount = (CN3UIBase*)GetChildByID("skill_group");
	for (int i = 0; i < 20; i++)
		icon_skill1_[i] = (CN3UIImage*)PieceCount->GetChildByID(string_format("target_skillicon%d", i));

	POINT eventNoticePos;
	g_pMain->GetUiPos(g_pMain->pClientTargetBar->m_dVTableAddr, eventNoticePos);
	SetPos(eventNoticePos.x + g_pMain->GetUiWidth(g_pMain->pClientTargetBar->m_dVTableAddr) - 310, eventNoticePos.y + 85);
	return true;
}

bool CUITargetSkillCheck::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if (dwMsg == UIMSG_BUTTON_CLICK) {}
	return true;
}

bool CUITargetSkillCheck::OnKeyPress(int iKey)
{
	if (!IsVisible())
		return CN3UIBase::OnKeyPress(iKey);

	return true;
}

void CUITargetSkillCheck::Open() { SetVisible(true); }
void CUITargetSkillCheck::Close() { SetVisible(false); }

void CUITargetSkillCheck::Update(Packet pkt)
{
	uint32_t skills[20] = { 0 };
	for (int i = 0; i < 20; i++)
	{
		pkt >> skills[i];
		CSpell* spell = GetSkillBase(skills[i]);
		if (spell == nullptr)
			icon_skill1_[i]->SetVisible(false);
		else
		{
			std::vector<char> buffer(256, NULL);
			sprintf(&buffer[0], "UI\\skillicon_%.2d_%d.dxt", (spell->dwIconID % 100), (spell->dwIconID / 100));
			std::string szIconFN = &buffer[0];
			SRC dxt = hdrReader->GetSRC(szIconFN);

			if (dxt.sizeInBytes == 0)
				icon_skill1_[i]->SetVisible(false);
			else
			{
				icon_skill1_[i]->SetTex(szIconFN);
				icon_skill1_[i]->SetUVRect(0, 0, (float)45.0f / (float)45.0f, (float)45.0f / (float)45.0f);
				icon_skill1_[i]->SetVisible(true);
			}
		}
	}
}

void CUITargetSkillCheck::UpdatePosition()
{
	m_sTargetSkillStatus = false;
	if (g_pMain->pClientTargetBar != NULL && !m_sTargetSkillStatus)
	{
		POINT eventNoticePos;
		g_pMain->GetUiPos(g_pMain->pClientTargetBar->m_dVTableAddr, eventNoticePos);
		m_sTargetSkillStatus = true;
		SetPos(eventNoticePos.x + g_pMain->GetUiWidth(g_pMain->pClientTargetBar->m_dVTableAddr) - 310, eventNoticePos.y + 85);
	}
}