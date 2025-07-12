#include "CTagChange.h"


CTagChange::CTagChange()
{
	vector<int>offsets;
	offsets.push_back(0x2F8);  // co_CastleUnion Satýr : 82
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);
	btn_cancel = NULL;
	btn_ok = NULL;
	edit_id = NULL;
	plate = NULL;
	r = 255;
	g = 255;
	b = 255;
	ParseUIElements();
	InitReceiveMessage();
}

DWORD Func_tag;
DWORD uiTag;
CTagChange::~CTagChange()
{

}

void CTagChange::ParseUIElements()
{
	txt_id = g_pMain->GetChildByID(m_dVTableAddr, "edit_id");
	btn_cancel = g_pMain->GetChildByID(m_dVTableAddr, "btn_cancel");
	btn_ok = g_pMain->GetChildByID(m_dVTableAddr, "btn_ok");
	text_title = g_pMain->GetChildByID(m_dVTableAddr, "Text_title");
	plate = g_pMain->GetChildByID(m_dVTableAddr, "plate");
	text_explanation = g_pMain->GetChildByID(m_dVTableAddr, "text_explanation");

	if (text_explanation) g_pMain->SetString(text_explanation,"Please enter the new your Tag ID.");
	
	g_pMain->UIScreenCenter(m_dVTableAddr);
}


void __stdcall UITagChangeReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->uiTagChange->ReceiveMessage(pSender, dwMsg);
	__asm
	{
		MOV ECX, uiTag
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_tag
		CALL EAX
	}
}
void CTagChange::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_tag = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)UITagChangeReceiveMessage_Hook;
}
bool CTagChange::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{

	uiTag = m_dVTableAddr;
	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == (DWORD*)btn_ok) 
		{
			if (!txt_id) 
				return true;

			std::string id = g_pMain->GetEditString(txt_id);
			if (id.empty()) 
			{
				g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Tag change : Tag is empty or incorret word!")).c_str(), 0xa30000);
				return true;
			}

			if (id.size() > 40) 
			{
				g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Tag change : tag size soo long!")).c_str(), 0xa30000);
				return true;
			}

			Packet result(WIZ_HSACS_HOOK, (uint8)HSACSOpCodes::TagInfo);
			result << (uint8)tagerror::newtag << g_pMain->GetEditString(txt_id) << r << g << b;
			g_pMain->Send(&result);

			g_pMain->SetVisible(m_dVTableAddr,false);
			g_pMain->EditKillFocus(txt_id);

			if (txt_id) 
				g_pMain->SetEditString(txt_id, "");
		}
		else if (pSender == (DWORD*)btn_cancel) 
		{
			if (txt_id) 
				g_pMain->SetEditString(txt_id, "");

			g_pMain->EditKillFocus(txt_id);
			g_pMain->SetVisible(m_dVTableAddr, false);
		}
	}
	return true;
}

extern HWND window;

uint32_t CTagChange::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;

	if ((dwFlags & 0x4) && window && g_pMain->IsIn(plate,ptCur.x, ptCur.y))
	{
		HDC dc = GetDC(window);
		COLORREF color = GetPixel(dc, ptCur.x, ptCur.y);
		ReleaseDC(window, dc);
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);
		g_pMain->SetStringColor(text_title,D3DCOLOR_ARGB(255, r, g, b));
	}


	return dwRet;
}


void CTagChange::Open()
{
	r = 255; g = 255; b = 255;
	g_pMain->SetVisible(m_dVTableAddr,true);
}

void CTagChange::Close()
{
	g_pMain->EditKillFocus(txt_id);
	g_pMain->SetVisible(m_dVTableAddr, false);
}

void CTagChange::OpenTagChange()
{
	g_pMain->UIScreenCenter(m_dVTableAddr);
	g_pMain->SetVisible(m_dVTableAddr,true);

}