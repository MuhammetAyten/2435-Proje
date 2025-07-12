#include "stdafx.h"
#include "Cindirella.h"

CUICindirellaPlug::CUICindirellaPlug()
{
	m_dVTableAddr = NULL;
	m_Timer = NULL;
	status = prepare = ison = false;
	remainingTime = selectedtime = 0;
	index = -1;
	t_test = false;
	txt_select_time = txt_remaining_time = txt_your_death_count = txt_your_kill_count = txt_elmo_kill_count = NULL;
	txt_karus_kill_count = NULL;
	btn_elmo = NULL;
	btn_karus = NULL;
	btn_close = NULL, btn_priest = NULL, btn_mage = NULL, btn_rogue = NULL, btn_warrior = NULL;

	vector<int>offsets;
	offsets.push_back(0x3D0);   //re_ugo3d_mini.uif Satýr : 333
	offsets.push_back(0);
	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);
	
	ParseUIElements();
	InitReceiveMessage();
}

CUICindirellaPlug::~CUICindirellaPlug()
{
	
}

void CUICindirellaPlug::ParseUIElements()
{
	txt_select_time = g_pMain->GetChildByID(m_dVTableAddr, "txt_select_time");
	txt_remaining_time = g_pMain->GetChildByID(m_dVTableAddr, "txt_remaining_time");
	txt_your_death_count = g_pMain->GetChildByID(m_dVTableAddr, "txt_your_death_count");
	txt_your_kill_count = g_pMain->GetChildByID(m_dVTableAddr, "txt_your_kill_count");
	txt_karus_kill_count = g_pMain->GetChildByID(m_dVTableAddr, "txt_karus_kill_count");
	txt_elmo_kill_count = g_pMain->GetChildByID(m_dVTableAddr, "txt_elmo_kill_count");
	btn_priest = g_pMain->GetChildByID(m_dVTableAddr, "btn_priest");
	btn_mage = g_pMain->GetChildByID(m_dVTableAddr, "btn_mage");
	btn_rogue = g_pMain->GetChildByID(m_dVTableAddr, "btn_rogue");
	btn_warrior = g_pMain->GetChildByID(m_dVTableAddr, "btn_warrior");
	btn_elmo = g_pMain->GetChildByID(m_dVTableAddr, "btn_elmo");
	btn_karus = g_pMain->GetChildByID(m_dVTableAddr, "btn_karus");
	btn_close = g_pMain->GetChildByID(m_dVTableAddr, "btn_close");
	g_pMain->UIScreenCenter(m_dVTableAddr);
}

DWORD uiCindirella;
DWORD Func_Cindirella;
bool CUICindirellaPlug::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	uiCindirella = m_dVTableAddr;

	if (!pSender || pSender == 0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;
	
	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		int8 ch_select = -1;
		if (pSender == (DWORD*)btn_close)
		{
			Close();

			if (g_pMain->pClientTopRightNewPlug != NULL)
				g_pMain->pClientTopRightNewPlug->OpenCindirellaButton();
		}
		else if (pSender == (DWORD*)btn_karus)
		{
			Packet pkt(WIZ_HSACS_HOOK, uint8(HSACSOpCodes::CINDIRELLA));
			pkt << uint8(cindopcode::nationchange) << uint8(1);
			g_pMain->Send(&pkt);
		}
		else if (pSender == (DWORD*)btn_elmo)
		{
			Packet pkt(WIZ_HSACS_HOOK, uint8(HSACSOpCodes::CINDIRELLA));
			pkt << uint8(cindopcode::nationchange) << uint8(2);
			g_pMain->Send(&pkt);
		}
		else if (pSender == (DWORD*)btn_warrior)
			ch_select = 0;
		else if (pSender == (DWORD*)btn_rogue)
			ch_select = 1;
		else if (pSender == (DWORD*)btn_mage)
			ch_select = 2;
		else if (pSender == (DWORD*)btn_priest)
			ch_select = 3;
		
		if (ch_select >= 0) {
			Packet pkt(WIZ_HSACS_HOOK, uint8(HSACSOpCodes::CINDIRELLA));
			pkt << uint8(cindopcode::selectclass) << ch_select;
			g_pMain->Send(&pkt);
		}
	}

	return true;
}

void CUICindirellaPlug::Start() {
	auto& p = g_pMain->m_cindirellaInfo;
	remainingTime = p.remainingtime;
	if (remainingTime) status = true;

	prepare = false; 
	if (status) g_pMain->pClientHookManager->ShowMessageBox("Cindirella War", "The event has started.We wish you luck.", MsgBoxTypes::Ok);
}

void CUICindirellaPlug::CharacterSelect(uint8 index) {

	this->index = index;
	selectedtime = 90;
	g_pMain->WriteInfoMessage((char*)xorstr("Class selections applied successfully."), D3DCOLOR_ARGB(255, 255, 64, 89));
}

void CUICindirellaPlug::Reset() {

	if (g_pMain)
		g_pMain->m_cindirellaInfo.Initialize();

	status = false;
	prepare = false;
	remainingTime = 0;
}

void CUICindirellaPlug::UpdateKDA() {
	auto& p = g_pMain->m_cindirellaInfo;

	g_pMain->SetString(txt_your_death_count, string_format(xorstr("%d"), p.mydead_count));
	g_pMain->SetString(txt_your_kill_count, string_format(xorstr("%d"), p.mykill_count));

	g_pMain->SetString(txt_elmo_kill_count, string_format(xorstr("%d"), p.e_kill_count));
	g_pMain->SetString(txt_karus_kill_count, string_format(xorstr("%d"), p.k_kill_count));
}

void CUICindirellaPlug::ShowPanel() {

	remainingTime = 0;
	auto& p = g_pMain->m_cindirellaInfo;
	prepare = p.prepare;
	index = p.index;

	g_pMain->SetString(txt_your_death_count, string_format(xorstr("%d"), p.mydead_count));
	g_pMain->SetString(txt_your_kill_count, string_format(xorstr("%d"), p.mykill_count));
	
	g_pMain->SetString(txt_elmo_kill_count, string_format(xorstr("%d"), p.e_kill_count));
	g_pMain->SetString(txt_karus_kill_count, string_format(xorstr("%d"), p.k_kill_count));

	remainingTime = p.remainingtime;
	UpdateTime();
	Open();
}

void CUICindirellaPlug::Open()
{
	g_pMain->UIScreenCenter(m_dVTableAddr);
	g_pMain->SetVisible(m_dVTableAddr, true);
	t_test = false;
}

void CUICindirellaPlug::Close()
{
	g_pMain->SetVisible(m_dVTableAddr, false);
	t_test = true;
}

bool t_1 = false;
void CUICindirellaPlug::UpdateTime() {

	if (!ison)
		return;

	if (m_Timer == NULL)
		m_Timer = new CTimer();

	bool elapsed = m_Timer->IsElapsedSecond();

	if (remainingTime > 0) {
		if (elapsed)
			remainingTime--;

		uint16_t remainingMinutes = (uint16_t)ceil(remainingTime / 60);
		uint16_t remainingSeconds = remainingTime - (remainingMinutes * 60);
		std::string remainingTimes;
		if (remainingMinutes < 10 && remainingSeconds < 10)
			remainingTimes = string_format(xorstr("0%d : 0%d"), remainingMinutes, remainingSeconds);
		else if (remainingMinutes < 10)
			remainingTimes = string_format(xorstr("0%d : %d"), remainingMinutes, remainingSeconds);
		else if (remainingSeconds < 10)
			remainingTimes = string_format(xorstr("%d : 0%d"), remainingMinutes, remainingSeconds);
		else
			remainingTimes = string_format(xorstr("%d : %d"), remainingMinutes, remainingSeconds);
		g_pMain->SetString(txt_remaining_time, string_format(xorstr("%s"), remainingTimes.c_str()));

		if (remainingTime && !t_test && !g_pMain->IsVisible(m_dVTableAddr))
			g_pMain->SetVisible(m_dVTableAddr, true);
	}

	if (selectedtime > 0) {
		if (elapsed)
			selectedtime--;

		uint16_t remainingMinutes = (uint16_t)ceil(selectedtime / 60);
		uint16_t remainingSeconds = selectedtime - (remainingMinutes * 60);
		std::string remainingTimes;
		if (remainingMinutes < 10 && remainingSeconds < 10)
			remainingTimes = string_format(xorstr("0%d : 0%d"), remainingMinutes, remainingSeconds);
		else if (remainingMinutes < 10)
			remainingTimes = string_format(xorstr("0%d : %d"), remainingMinutes, remainingSeconds);
		else if (remainingSeconds < 10)
			remainingTimes = string_format(xorstr("%d : 0%d"), remainingMinutes, remainingSeconds);
		else
			remainingTimes = string_format(xorstr("%d : %d"), remainingMinutes, remainingSeconds);
		g_pMain->SetString(txt_select_time, string_format(xorstr("%s"), remainingTimes.c_str()));
	}
}

void CUICindirellaPlug::Tick()
{
	UpdateTime();
}

void __stdcall uiCindirellaReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->uiCindirella->ReceiveMessage(pSender, dwMsg);

	__asm
	{
		MOV ECX, uiCindirella
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_Cindirella
		CALL EAX
	}
}

void CUICindirellaPlug::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_Cindirella = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)uiCindirellaReceiveMessage_Hook;
}