#include "EventHandler.h"

#if (HOOK_SOURCE_VERSION == 2369)
CUIEventHandler::CUIEventHandler()
{
	vector<int>offsets;
	offsets.push_back(0x264);
	offsets.push_back(0x00);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);
	ParseUIElements();
	InitReceiveMessage();
}

DWORD EventHandlerTable = 0;
DWORD Func_EventHandler;
CUIEventHandler::~CUIEventHandler()
{
}

void CUIEventHandler::ParseUIElements()
{
	m_sTimes = NULL;
	/*grp_accept = g_pMain->GetChildByID(m_dVTableAddr, "grp_accept");
	quest_completed = g_pMain->GetChildByID(m_dVTableAddr, "quest_completed");
	skill_group = g_pMain->GetChildByID(m_dVTableAddr, "skill_group");
	grp_juraid = g_pMain->GetChildByID(m_dVTableAddr, "grp_juraid");
	grp_minimum = g_pMain->GetChildByID(grp_juraid, "grp_minimum");
	grp_maximum = g_pMain->GetChildByID(grp_juraid, "grp_maximum");
	by_details = g_pMain->GetChildByID(grp_juraid, "by_details");
	by_bottom = g_pMain->GetChildByID(grp_juraid, "by_bottom");

	txt_time = g_pMain->GetChildByID(grp_minimum, "txt_time");
	btn_maximum = g_pMain->GetChildByID(grp_minimum, "btn_maximum");
	by_event_name = g_pMain->GetChildByID(btn_maximum, "by_event_name");
	icon = g_pMain->GetChildByID(icon, "icon");

	btn_cancel = g_pMain->GetChildByID(grp_accept, "btn_cancel");

	g_pMain->SetVisible(grp_accept, false);
	g_pMain->SetVisible(quest_completed, false);
	g_pMain->SetVisible(skill_group, false);
	g_pMain->SetVisible(grp_juraid, false);
	g_pMain->SetVisible(m_dVTableAddr, true);*/
}

void EventHandlerEventStart(Packet& pkt, uint8 OpCode)
{
	enum EventOpCode
	{
		TEMPLE_EVENT_BORDER_DEFENCE_WAR = 4,
		TEMPLE_EVENT_MONSTER_STONE = 14,
		TEMPLE_EVENT_CHAOS = 24,
		TEMPLE_EVENT_JURAD_MOUNTAIN = 100,
	};

	if (g_pMain->pClientEventHandler == NULL)
		return;

	if (g_pMain->pClientEventSystem.m_bStatus)
		return;

	if (OpCode == TEMPLE_EVENT_BORDER_DEFENCE_WAR)
		g_pMain->SetString(g_pMain->pClientEventHandler->by_event_name, string_format(xorstr("Border Defance War")));
	else if (OpCode == TEMPLE_EVENT_CHAOS)
		g_pMain->SetString(g_pMain->pClientEventHandler->by_event_name, string_format(xorstr("CHAOS")));
	else if (OpCode == TEMPLE_EVENT_JURAD_MOUNTAIN)
		g_pMain->SetString(g_pMain->pClientEventHandler->by_event_name, string_format(xorstr("juraid Mountain")));
	else
		return;

	pkt >> g_pMain->pClientEventSystem.m_sEventTimer;
	g_pMain->pClientEventSystem.m_bStatus = true;
	g_pMain->pClientEventHandler->m_sTimes = new CTimer(false);


	RECT minReg = g_pMain->GetUiRegion(g_pMain->pClientEventHandler->grp_minimum);
	RECT group_times = g_pMain->GetUiRegion(g_pMain->pClientEventHandler->grp_maximum);
	minReg.bottom += group_times.bottom - group_times.top;
	RECT baseSize = g_pMain->GetUiRegion(g_pMain->pClientEventHandler->by_details);
	RECT itemSize = g_pMain->GetUiRegion(g_pMain->pClientEventHandler->by_bottom);
	minReg.bottom += itemSize.bottom - itemSize.top;
	minReg.bottom += 3 * (baseSize.bottom - baseSize.top);
	g_pMain->SetUiRegion(g_pMain->pClientEventHandler->grp_juraid, minReg);

	g_pMain->SetVisible(g_pMain->pClientEventHandler->grp_juraid, true);
}

void EventHandlerNewSystem(Packet& pkt)
{
	enum EventOpCode
	{
		TEMPLE_EVENT_BORDER_DEFENCE_WAR = 4,
		TEMPLE_EVENT_MONSTER_STONE = 14,
		TEMPLE_EVENT_CHAOS = 24,
		TEMPLE_EVENT_JURAD_MOUNTAIN = 100,
	};

	uint8 OpCode;
	pkt >> OpCode;
	/*
	* 1 JOIN
	* 2 DISABLE
	* 3 UPDATE
	*/
	switch (OpCode)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case TEMPLE_EVENT_BORDER_DEFENCE_WAR:
	case TEMPLE_EVENT_CHAOS:
	case TEMPLE_EVENT_JURAD_MOUNTAIN:
		EventHandlerEventStart(pkt, OpCode);
		break;
	default:
		break;
	}
}

void CUIEventHandler::EventHandler(Packet& pkt)
{
	uint8 OpCode;
	pkt >> OpCode;
	/*
	* 1 BORDER - JURAID - CHAOS
	* 2 PARTY MONSTER STONE
	* 3 TARGET SKILL
	*/
	switch (OpCode)
	{
	case 1:
		EventHandlerNewSystem(pkt);
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}
}

void CUIEventHandler::Open(uint8 sCode)
{
	g_pMain->SetVisible(m_dVTableAddr, true);
	g_pMain->UIScreenCenter(m_dVTableAddr);
}

void CUIEventHandler::Close(uint8 sCode)
{
	switch (sCode)
	{
	case 1:
		g_pMain->SetVisible(grp_juraid, true);
		g_pMain->pClientEventSystem.Clean();
		break;
	default:
		break;
	}
}

bool CUIEventHandler::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	EventHandlerTable = m_dVTableAddr;
	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == (DWORD*)btn_cancel)
			Close();
		printf("saas\n");
	}
	return false;
}

DWORD oRecvMsgCUIEventHandlerPlug;
void __stdcall CUIEventHandlerMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->pClientEventHandler->ReceiveMessage(pSender, dwMsg);

	__asm
	{
		MOV ECX, EventHandlerTable
		PUSH dwMsg
		PUSH pSender
		call oRecvMsgCUIEventHandlerPlug
	}
}

void CUIEventHandler::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	oRecvMsgCUIEventHandlerPlug = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)CUIEventHandlerMessage_Hook;
}

void CUIEventHandler::Tick()
{
	if (g_pMain->pClientEventSystem.m_bStatus)
	{
		if (m_sTimes->IsElapsedSecond())
			g_pMain->pClientEventSystem.m_sEventTimer--;

		if (g_pMain->pClientEventSystem.m_sEventTimer == NULL)
		{

		}
		uint16_t remainingMinutes = (uint16_t)ceil(g_pMain->pClientEventSystem.m_sEventTimer / 60);
		uint16_t remainingSeconds = g_pMain->pClientEventSystem.m_sEventTimer - (remainingMinutes * 60);

		std::string remainingTime;
		if (remainingMinutes < 10 && remainingSeconds < 10)
			remainingTime = string_format(xorstr("0%d : 0%d"), remainingMinutes, remainingSeconds);
		else if (remainingMinutes < 10)
			remainingTime = string_format(xorstr("0%d : %d"), remainingMinutes, remainingSeconds);
		else if (remainingSeconds < 10)
			remainingTime = string_format(xorstr("%d : 0%d"), remainingMinutes, remainingSeconds);
		else
			remainingTime = string_format(xorstr("%d : %d"), remainingMinutes, remainingSeconds);

		g_pMain->SetString(txt_time, string_format(xorstr("%s"), remainingTime.c_str()));
	}
}
#endif