#include "stdafx.h"
#include "hEventShowList.h"

CEventShowListPlug::CEventShowListPlug()
{
	vector<int>offsets;
	offsets.push_back(0x444);  //co_force_join Satýr : 202
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);
	el_base = g_pMain->rdword(KO_DLG, offsets);
	btn_details = 0;
	base_news = 0;
	btn_news_close = 0;

	ParseUIElements();
	InitReceiveMessage();
}

CEventShowListPlug::~CEventShowListPlug()
{
}

void CEventShowListPlug::ParseUIElements()  
{
	btn_down = g_pMain->GetChildByID(m_dVTableAddr, "btn_previously");
	btn_up = g_pMain->GetChildByID(m_dVTableAddr, "btn_next");
	text_page = g_pMain->GetChildByID(m_dVTableAddr, "text_page");
	btn_close = g_pMain->GetChildByID(m_dVTableAddr, "btn_close");
	text_server_time = g_pMain->GetChildByID(m_dVTableAddr, "txt_server_time");
#if (HOOK_SOURCE_VERSION != 1098)
	base_news = g_pMain->GetChildByID(m_dVTableAddr, "base_news");
	btn_news_close = g_pMain->GetChildByID(base_news, "btn_news_close");
	btn_details = g_pMain->GetChildByID(m_dVTableAddr, "btn_details");
#endif

	for (int i = 0; i < 10; i++)
	{
		baseGroup[i] = g_pMain->GetChildByID(m_dVTableAddr, string_format(xorstr("base_event%d"), i + 1));
	}
	
	for (int i = 0; i < 10; i++) {
		DWORD detailgroup = baseGroup[i];

		txt_event_time[i] = g_pMain->GetChildByID(detailgroup, "txt_event_time");
		txt_event_name[i] = g_pMain->GetChildByID(detailgroup, "txt_event_name");
		g_pMain->SetString(txt_event_time[i],string_format("Timer-%d", i));
		g_pMain->SetString(txt_event_name[i],string_format("name-%d", i));
	}
	g_pMain->UIScreenCenter(el_base);
	m_Timer = new CTimer(true, 1000);
}

DWORD pClientEvetShowList;


bool CEventShowListPlug::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	pClientEvetShowList = el_base;

	if (!pSender || pSender == 0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;


	if (pSender ==(DWORD*) btn_close) Close();
	else if (pSender == (DWORD*)btn_up) {
		if (page < pageCount) {
			page++;
			InitList(page);
		}
	}
	else if (pSender == (DWORD*)btn_down) {
		if (page > 1) {
			page--;
			InitList(page);
		}
	}

	if (pSender == (DWORD*)btn_details) {
		g_pMain->SetVisible(base_news, true);
	}
	if (pSender == (DWORD*)btn_news_close) {
		g_pMain->SetVisible(base_news, false);
	}

	return true;
}


DWORD Func_EventShowList;
void CEventShowListPlug::InitList(uint8 p)
{
	if (g_pMain->pClientEvetShowList) m_bEventShowList = g_pMain->pClientHookManager->m_sEventShowList;
	if (m_bEventShowList.empty()) return;

	if (p == 255) p = page;

	std::sort(m_bEventShowList.begin(),
		m_bEventShowList.end(), [](auto const& a, auto const& b) { return a.hour < b.hour;});

	pageCount = abs(ceil((double)m_bEventShowList.size() / (double)10));
	if (page > pageCount) page = 1;

	g_pMain->SetString(text_server_time,string_format("%s:%s:%s", to_string(g_pMain->serverhour).c_str(),
		to_string(g_pMain->serverminute).c_str(),
		to_string(g_pMain->serversecond).c_str()));

	int begin = (p - 1) * 10;
	int j = -1;
	for (int i = begin; i < begin + 10; i++) {
		j++;
		if (j > 9) break;
		if (i > m_bEventShowList.size() - 1) { g_pMain->SetVisible(baseGroup[j],false);continue; }

		g_pMain->SetString(txt_event_name[j], m_bEventShowList[i].name);
		g_pMain->SetString(txt_event_time[j], m_bEventShowList[i].time);

		g_pMain->SetVisible(baseGroup[j], true);
		g_pMain->SetString(text_page, to_string(page));
	}
}

void CEventShowListPlug::Tick()
{
	if (!g_pMain || !m_Timer->IsElapsedSecond()) return;

	g_pMain->serversecond++;
	if (g_pMain->serversecond > 59) {
		g_pMain->serverminute++;
		g_pMain->serversecond = 0;
	}
	if (g_pMain->serverminute > 59) {
		g_pMain->serverhour++;
		g_pMain->serverminute = 0;
		g_pMain->serversecond = 0;
	}
	if (g_pMain->serverhour > 23) {
		g_pMain->serverhour = 0;
		g_pMain->serverminute = 0;
		g_pMain->serversecond = 0;
	}

	g_pMain->SetString(text_server_time,string_format("%s:%s:%s", to_string(g_pMain->serverhour).c_str(),
		to_string(g_pMain->serverminute).c_str(),
		to_string(g_pMain->serversecond).c_str()));
}


void CEventShowListPlug::Open()
{
	if (g_pMain->pClientEvetShowList && g_pMain->IsVisible(el_base))
		g_pMain->SetVisible(el_base,false);


	g_pMain->SetVisible(el_base,true);
	g_pMain->SetVisible(base_news, false);
	g_pMain->UIScreenCenter(el_base);
}

void CEventShowListPlug::Close()
{
	if (g_pMain->pClientEvetShowList && !g_pMain->IsVisible(el_base))
		g_pMain->SetVisible(el_base, true);

	g_pMain->SetVisible(el_base,false);
	g_pMain->UIScreenCenter(el_base);
}
void CEventShowListPlug::OpenScheduler()
{

}
void __stdcall pClientEvetShowListReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->pClientEvetShowList->ReceiveMessage(pSender, dwMsg);

	__asm
	{
		MOV ECX, pClientEvetShowList
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_EventShowList
		CALL EAX
	}
}

void CEventShowListPlug::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(el_base);
	Func_EventShowList = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)pClientEvetShowListReceiveMessage_Hook;
}