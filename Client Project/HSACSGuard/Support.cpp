#include "Support.h"

CUISupportReportPlug::CUISupportReportPlug()
{
	m_dVTableAddr = NULL;


	vector<int>offsets;
	offsets.push_back(0x440);   //co_clan_advertisement.uif Satýr : 237
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);

	bug = true;
	txt_subject = NULL;
	txt_message = NULL;
	btn_bug = NULL;
	btn_koxp = NULL;
	btn_report = NULL;
	btn_close = NULL;
	
	ParseUIElements();
	InitReceiveMessage();
}
DWORD Func_Support;
CUISupportReportPlug::~CUISupportReportPlug()
{
}

void CUISupportReportPlug::ParseUIElements()
{
	txt_subject = g_pMain->GetChildByID(m_dVTableAddr, "edit_subject");
	txt_message = g_pMain->GetChildByID(m_dVTableAddr, "edit_message");
	btn_bug = g_pMain->GetChildByID(m_dVTableAddr, "btn_bug");
	btn_koxp = g_pMain->GetChildByID(m_dVTableAddr, "btn_koxp");
	btn_report = g_pMain->GetChildByID(m_dVTableAddr, "btn_report");
	btn_close = g_pMain->GetChildByID(m_dVTableAddr, "btn_close");
}

DWORD SupportReport;
bool CUISupportReportPlug::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	SupportReport = m_dVTableAddr;
	if (!pSender || pSender == 0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;

	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
	if (pSender == (DWORD*)btn_bug || pSender == (DWORD*)btn_koxp)
		{
			bug = !bug;
			if (bug) {
				g_pMain->SetState(btn_bug,UI_STATE_BUTTON_DOWN);
				g_pMain->SetState(btn_koxp,UI_STATE_BUTTON_NORMAL);
			}
			else {
				g_pMain->SetState(btn_bug,UI_STATE_BUTTON_NORMAL);
				g_pMain->SetState(btn_koxp,UI_STATE_BUTTON_DOWN);
			}
		}
		else if (pSender == (DWORD*)btn_close)
			Close();
		else if (pSender == (DWORD*)btn_report)
		{
			string subject = g_pMain->GetEditString(txt_subject);	
			string message = g_pMain->GetEditString(txt_message);	
			if (subject.size() > 25)
			{
				g_pMain->pClientHookManager->ShowMessageBox(xorstr("Support Failed"), xorstr("The subject cannot be longer than 25 characters."), Ok);
				Close();
				return true;
			}
			if (message.size() > 40)
			{
				g_pMain->pClientHookManager->ShowMessageBox(xorstr("Support Failed"), xorstr("The message cannot be longer than 40 characters."), Ok);
				Close();
				return true;
			}
			if (subject.size() < 3)
			{
				g_pMain->pClientHookManager->ShowMessageBox(xorstr("Support Failed"), xorstr("The subject must be at least 3 characters."), Ok);
				Close();
				return true;
			}
			if (message.size() < 10)
			{
				g_pMain->pClientHookManager->ShowMessageBox(xorstr("Support Failed"), xorstr("The message must be at least 10 characters."), Ok);
				Close();
				return true;
			}

			Packet result(WIZ_HSACS_HOOK);
			result << uint8_t(HSACSOpCodes::SUPPORT) << uint8(bug ? 0 : 1) << subject << message;
			g_pMain->Send(&result);

			g_pMain->pClientHookManager->ShowMessageBox(xorstr("Support"), xorstr("Your message has been sent."), Ok);
			Close();
		}
	}



	return true;
}

void __stdcall SupportReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->uiSupport->ReceiveMessage(pSender, dwMsg);
	__asm
	{
		MOV ECX, SupportReport
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_Support
		CALL EAX
	}
}

void CUISupportReportPlug::OpenSupport()
{
	g_pMain->SetVisible(m_dVTableAddr, true);
	g_pMain->UIScreenCenter(m_dVTableAddr);
}
void CUISupportReportPlug::Close()
{
	g_pMain->EditKillFocus(txt_message);
	g_pMain->EditKillFocus(txt_subject);

	g_pMain->SetVisible(m_dVTableAddr, false);

}
void CUISupportReportPlug::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_Support = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)SupportReceiveMessage_Hook;
}