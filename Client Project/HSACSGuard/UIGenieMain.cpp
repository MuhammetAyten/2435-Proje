#include "stdafx.h"
#include "UIGenieMain.h"
extern bool isLeaderAttack;
CUIGenieMain::CUIGenieMain()
{
	vector<int>offsets;
	offsets.push_back(0x50);
	offsets.push_back(0);
	offsets.push_back(0x518);
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);

	btn_auto_r_attack_on = NULL;
	btn_auto_r_attack_off = NULL;
	btn_party_leader_on = NULL;
	btn_party_leader_off = NULL;
	btn_basic_attack_off = NULL;

	ParseUIElements();
	InitReceiveMessage();
}

CUIGenieMain::~CUIGenieMain()
{
}

void CUIGenieMain::ParseUIElements()
{
	genie = g_pMain->GetChildByID(m_dVTableAddr, "genie");
	etc = g_pMain->GetChildByID(genie, "etc");
	attack = g_pMain->GetChildByID(genie, "attack");
	btn_basic_attack_off = g_pMain->GetChildByID(attack, "btn_basic_attack_off");
	btn_auto_r_attack_on = g_pMain->GetChildByID(attack, "btn_auto_r_attack_on");
	btn_auto_r_attack_off = g_pMain->GetChildByID(attack, "btn_auto_r_attack_off");
	scr_att_range = g_pMain->GetChildByID(attack, "scr_att_range");
	scr_act_range = g_pMain->GetChildByID(attack, "scr_act_range");
	btn_party_leader_on = g_pMain->GetChildByID(attack, "btn_party_leader_on");
	btn_party_leader_off = g_pMain->GetChildByID(attack, "btn_party_leader_off");
	btn_auto_attack_off = g_pMain->GetChildByID(attack, "btn_auto_attack_off");
	btn_auto_attack_on = g_pMain->GetChildByID(attack, "btn_auto_attack_on");
}

extern bool m_bGenieAttackStatus;
extern bool __genieAttackMove;

void CUIGenieMain::Tick()
{
	static bool bSet = false;
	if (!bSet && g_pMain->IsVisible(attack))
	{
		if (m_bGenieAttackStatus) {
			g_pMain->SetState(btn_auto_r_attack_on, UI_STATE_BUTTON_NORMAL);
			g_pMain->SetState(btn_auto_r_attack_off, UI_STATE_BUTTON_DOWN);
		} else {
			g_pMain->SetState(btn_auto_r_attack_on, UI_STATE_BUTTON_DOWN);
			g_pMain->SetState(btn_auto_r_attack_off, UI_STATE_BUTTON_NORMAL);
		}
		
		if (__genieAttackMove) {
			g_pMain->SetState(btn_basic_attack_off, UI_STATE_BUTTON_NORMAL);
		}
		else {
			g_pMain->SetState(btn_basic_attack_off, UI_STATE_BUTTON_DOWN);
			
		}
		if (isLeaderAttack) {
			g_pMain->SetState(btn_party_leader_on, UI_STATE_BUTTON_DOWN);
			g_pMain->SetState(btn_party_leader_off, UI_STATE_BUTTON_NORMAL);
		}
		else {
			g_pMain->SetState(btn_party_leader_on, UI_STATE_BUTTON_NORMAL);
			g_pMain->SetState(btn_party_leader_off, UI_STATE_BUTTON_DOWN);
		}
		bSet = true;
	}
	else if (!g_pMain->IsVisible(attack)) bSet = false;
}

void CUIGenieMain::SendBasicAttack(bool status)
{
	Packet newpkt(WIZ_GENIE, uint8(25));
	newpkt << uint8(status);
	g_pMain->Send(&newpkt);
}

DWORD uiGenieMainVTable;
bool CUIGenieMain::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	uiGenieMainVTable = m_dVTableAddr;
	if (!pSender || pSender == 0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;

	std::string msg = "";
	if (pSender == (DWORD*)btn_party_leader_off)
	{
		//vector<int>offsets;
		//offsets.push_back(0x208);
		//offsets.push_back(0x138);
		//offsets.push_back(0x0);
		//DWORD el_Base = g_pMain->rdword(KO_DLG, offsets);
		//g_pMain->SetVisible(el_Base, true);

		isLeaderAttack = false;
		g_pMain->SetState(btn_party_leader_on, UI_STATE_BUTTON_NORMAL);
		g_pMain->SetState(btn_party_leader_off, UI_STATE_BUTTON_DOWN);
	}
	if (pSender == (DWORD*)btn_basic_attack_off)
	{
		__genieAttackMove = !__genieAttackMove;
		g_pMain->SetState(btn_basic_attack_off, (__genieAttackMove ? UI_STATE_BUTTON_NORMAL : UI_STATE_BUTTON_DOWN));
		SendBasicAttack(__genieAttackMove);
	}
	if (pSender == (DWORD*)btn_party_leader_on)
	{
		isLeaderAttack = true;
		g_pMain->SetState(btn_party_leader_on, UI_STATE_BUTTON_DOWN);
		g_pMain->SetState(btn_party_leader_off, UI_STATE_BUTTON_NORMAL);
	}
	if (pSender == (DWORD*)btn_auto_r_attack_on)
	{
		m_bGenieAttackStatus = false;
		g_pMain->SetState(btn_auto_r_attack_on, UI_STATE_BUTTON_DOWN);
		g_pMain->SetState(btn_auto_r_attack_off, UI_STATE_BUTTON_NORMAL);
	}
	else if (pSender == (DWORD*)btn_auto_r_attack_off)
	{
		m_bGenieAttackStatus = true;
		g_pMain->SetState(btn_auto_r_attack_on, UI_STATE_BUTTON_NORMAL);
		g_pMain->SetState(btn_auto_r_attack_off, UI_STATE_BUTTON_DOWN);
	}
	else if (pSender == (DWORD*)btn_auto_attack_on)
		g_pMain->m_bGenieAttackerStatus = true;
	else if (pSender == (DWORD*)btn_auto_attack_off)
		g_pMain->m_bGenieAttackerStatus = false;
	return true;
}

DWORD Func_GenieMain = 0;
void __stdcall UiGenieMainReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->uiGenieMain->ReceiveMessage(pSender, dwMsg);
	__asm
	{
		MOV ECX, uiGenieMainVTable
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_GenieMain
		CALL EAX
	}
}

void CUIGenieMain::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_GenieMain = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)UiGenieMainReceiveMessage_Hook;
}