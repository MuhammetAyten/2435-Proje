#include "hAccountRegister.h"

CUIAccountRegister::CUIAccountRegister()
{
	m_dVTableAddr = NULL;

	vector<int>offsets;
	offsets.push_back(0x2C0);	// 2369	co_DuelList.uif	Satýr : 60
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);

	 edit_otp_password = NULL;
	 edit_seal_password = NULL;
	 edit_phone_number = NULL;
	 edit_email = NULL;
	 btn_confirm = NULL;
	 btn_close = NULL;

	ParseUIElements();
	InitReceiveMessage();
}

CUIAccountRegister::~CUIAccountRegister()
{
}

void CUIAccountRegister::ParseUIElements()
{
	g_pMain->GetChildByID(m_dVTableAddr, "edit_otp_password", edit_otp_password);
	g_pMain->GetChildByID(m_dVTableAddr, "edit_seal_password", edit_seal_password);
	g_pMain->GetChildByID(m_dVTableAddr, "edit_phone_number", edit_phone_number);
	g_pMain->GetChildByID(m_dVTableAddr, "edit_email", edit_email);
	g_pMain->GetChildByID(m_dVTableAddr, "btn_confirm", btn_confirm);
	g_pMain->GetChildByID(m_dVTableAddr, "btn_close", btn_close);
}

DWORD Func_Register;
DWORD uiRegister;
bool CUIAccountRegister::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	uiRegister = m_dVTableAddr;
	if (!pSender || pSender == 0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;

	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (btn_confirm == NULL || btn_close == NULL)
			return false;

		if (pSender == (DWORD*)btn_confirm) {

			if (edit_email == NULL)  return false;
			if (edit_otp_password == NULL) return false;
			if (edit_phone_number == NULL) return false;
			if (edit_seal_password == NULL) return false;
		
			string email = g_pMain->GetEditString(edit_email).c_str();
			string phone = g_pMain->GetEditString(edit_phone_number).c_str();
			string seal = g_pMain->GetEditString(edit_seal_password).c_str();
			string otp = g_pMain->GetEditString(edit_otp_password).c_str();

			if (email.empty()) {
				g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Account Register : Email address cannot be left blank.")).c_str(), 0xa30000);
				RefreshText();
				return false;
			}

			if (phone.empty()) {
				g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Account Register : Phone number cannot be left blank.")).c_str(), 0xa30000);
				RefreshText();
				return false;
			}

			if (phone.size() != 11) {
				g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Account Register : You must enter your phone number correctly. Ex: 05320001122")).c_str(), 0xa30000);
				RefreshText();
				return false;
			}

			if (!g_pMain->NumberValid(phone)) {
				g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Account Register : You must enter your phone number correctly. Ex: 05320001122")).c_str(), 0xa30000);
				RefreshText();
				return false;
			}

			std::string numberformat = phone.substr(0, 2);
			if (numberformat != "05") {
				g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Power Up Store : Your phone number must start with 05. Ex: 05320001122")).c_str(), 0xa30000);
				RefreshText();
				return false;
			}

			if (seal.empty()) {
				g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Account Register : Item seal password cannot be left blank. Ex: 12345678")).c_str(), 0xa30000);
				RefreshText();
				return false;
			}

			if (seal.size() != 8) {
				g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Account Register : The item seal password must consist of a maximum of 8 digits. Ex: 12345678")).c_str(), 0xa30000);
				RefreshText();
				return false;
			}

			if (!g_pMain->NumberValid(seal)) {
				g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Account Register : You must enter your item seal password correctly. Ex: 12345678")).c_str(), 0xa30000);
				RefreshText();
				return false;
			}

			if (otp.empty()) {
				g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Account Register : OTP number cannot be left blank. Ex: 123456")).c_str(), 0xa30000);
				RefreshText();
				return false;
			}

			if (otp.size() != 6) {
				g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Account Register : The OTP number must consist of a maximum of 6 digits. Ex: 123456")).c_str(), 0xa30000);
				RefreshText();
				return false;
			}

			if (!g_pMain->NumberValid(otp)) {
				g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Account Register : You must enter your OTP number correctly. Ex: 123456")).c_str(), 0xa30000);
				RefreshText();
				return false;
			}

			Packet test(WIZ_HSACS_HOOK, uint8(HSACSOpCodes::ACCOUNT_INFO_SAVE));
			test.DByte();
			test << uint8(1) << email << phone << seal << otp;
			g_pMain->Send(&test);
		}
		else if (pSender == (DWORD*)btn_close) {
			Close();
			Packet test(WIZ_HSACS_HOOK, uint8(HSACSOpCodes::ACCOUNT_INFO_SAVE));
			test << uint8(2);
			g_pMain->Send(&test);
		}
	}


	return true;
}
void CUIAccountRegister::Open()
{
	g_pMain->SetVisible(m_dVTableAddr,true);
}

void CUIAccountRegister::Close()
{
	g_pMain->SetVisible(m_dVTableAddr,false);
}
bool HSACSEngine::NumberValid(const std::string& str2)
{
	std::string str = str2;
	STRTOLOWER(str);
	char a[10] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };

	int size = (int)str.length();
	for (int i = 0; i < size; i++)
	{
		if (str.at(i) != a[0]
			&& str.at(i) != a[1]
			&& str.at(i) != a[2]
			&& str.at(i) != a[3]
			&& str.at(i) != a[4]
			&& str.at(i) != a[5]
			&& str.at(i) != a[6]
			&& str.at(i) != a[7]
			&& str.at(i) != a[8]
			&& str.at(i) != a[9])
			return false;
	}
	return true;
}

void CUIAccountRegister::RefreshText()
{
	if (NULL == edit_email)  return;
	if (NULL == edit_otp_password) return;
	if (NULL == edit_phone_number) return;
	if (NULL == edit_seal_password) return;


	g_pMain->SetEditString(edit_email, "");
	g_pMain->SetEditString(edit_otp_password, "");
	g_pMain->SetEditString(edit_phone_number, "");
	g_pMain->SetEditString(edit_seal_password, "");
}

void __stdcall UIAccountRegisterReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->uiAccountRegisterPlug->ReceiveMessage(pSender, dwMsg);
	__asm
	{
		MOV ECX, uiRegister
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_Register
		CALL EAX
	}
}

void CUIAccountRegister::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_Register = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)UIAccountRegisterReceiveMessage_Hook;
}