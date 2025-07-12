#include "UITargetBar.h"
#include <math.h>

CUIClientTargetBarPlug::CUIClientTargetBarPlug()
{
	vector<int>offsets;
	offsets.push_back(0x220);
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);

	m_dTextTargetHp = NULL;
	m_btnDrop = NULL;
	m_user_info = NULL; 
	ParseUIElements();
	InitReceiveMessage();
}

CUIClientTargetBarPlug::~CUIClientTargetBarPlug()
{
}

void CUIClientTargetBarPlug::ParseUIElements()
{
	str_name = g_pMain->GetChildByID(m_dVTableAddr, "name");
#if (HOOK_SOURCE_VERSION != 1098)
	m_btnDrop = g_pMain->GetChildByID(m_dVTableAddr, "Btn_Drops");
	g_pMain->SetVisible(m_btnDrop, false);
	g_pMain->SetState(m_btnDrop, UI_STATE_BUTTON_DISABLE);
	m_dTextTargetHp = g_pMain->GetChildByID(m_dVTableAddr, "Text_Health");
#endif
}

void CUIClientTargetBarPlug::SetInfo(DWORD color, uint8 lvl, std::string name)
{
	if(str_name) g_pMain->SetString(str_name, string_format("%s Lv. %d", name.c_str(), lvl));  // moblarýn isimlerinin yanýnda level görünüyor.

}

DWORD pClientTargetBarVTable;
DWORD Func_TargetBar = 0;

bool CUIClientTargetBarPlug::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	pClientTargetBarVTable = m_dVTableAddr;
	if (!pSender || pSender == 0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;

	g_pMain->m_zMob = *(DWORD*)((*(DWORD*)KO_PTR_CHR) + KO_OFF_MOB);
	uint16 sIID = 0;
	DWORD target = g_pMain->KOGetTarget();
	if (target > 0)
		sIID = *(uint16*)(target + KO_SSID);

	if (pSender == (DWORD*)m_btnDrop)
	{
		Packet pkt(WIZ_HSACS_HOOK);
		pkt << uint8(HSACSOpCodes::DROP_REQUEST) << uint8(3) << sIID;
		g_pMain->Send(&pkt);
	}
	else if (pSender == (DWORD*)m_user_info) 
	{
		Packet result(WIZ_USER_INFO, uint8(0x05));
		result << uint16(g_pMain->m_zMob);
		g_pMain->Send(&result);
	}
	return true;
}

void __stdcall pClientTargetBarReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->pClientTargetBar->ReceiveMessage(pSender, dwMsg);
}

void CUIClientTargetBarPlug::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_TargetBar = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)pClientTargetBarReceiveMessage_Hook;
}
__inline DWORD PRDWORD(DWORD ulBase)
{
	if (!IsBadReadPtr((VOID*)ulBase, sizeof(DWORD)))
	{
		return(*(DWORD*)(ulBase));
	}
	return 0;
}

void CUIClientTargetBarPlug::TargetNameSet(uint16 GetID)
{
	DWORD base = PRDWORD(PRDWORD(KO_DLG) + KO_OFF_PTBASE);
	DWORD buffer = PRDWORD(base + KO_OFF_PT);
	DWORD partyCount = PRDWORD(base + KO_OFF_PTCOUNT);
	bool isparty = false;
	for (int i = 0; i < partyCount; i++)
	{
		if (partyCount > 0)
		{
			buffer = PRDWORD(buffer);
			if (buffer)
			{
				USHORT Id = *(USHORT*)(buffer + 0x8);

				if (GetID == Id)
				{
					g_pMain->SetStringColor(str_name, 0xFFFFFF00);
					isparty = true;
					break;
				}
			}
		}
	}

	if (!isparty)
	{
		DWORD target = g_pMain->KOGetTarget();
		if (target > 0)
		{
			if (*(uint16*)(target + KO_OFF_ID) < NPC_BAND)
			{
				uint8 myLvl = *(uint8*)(*(DWORD*)KO_PTR_CHR + KO_OFF_LEVEL);
				if (myLvl < 30)
					g_pMain->SetStringColor(str_name, 0xFFFFFFFF);
			}
		}
	}
}

void CUIClientTargetBarPlug::SetTargetHp(Packet & pkt)
{
	uint16 tid, damage, ssid;
	uint8 echo, prototype;
	int32 maxhp, hp;
	pkt >> tid >> echo >> maxhp >> hp >> damage >> ssid;
	pkt >> prototype;
	/*
		1.monster
		2.npc
		3.bot
		4.real user
	*/
	if (ssid == tid)
		return;

	if (tid < NPC_BAND || prototype == 3)
	{
		g_pMain->SetVisible(m_user_info, true); 
		g_pMain->SetVisible(m_btnDrop, false);
		g_pMain->SetState(m_user_info, UI_STATE_BUTTON_NORMAL);
		g_pMain->SetState(m_btnDrop, UI_STATE_BUTTON_DISABLE);
		TargetNameSet(tid);
	}
	else if (tid > NPC_BAND)
	{
		if (prototype == 2)
			g_pMain->SetVisible(m_btnDrop, false);
		else
			g_pMain->SetVisible(m_btnDrop, true);

		g_pMain->KOGetSSID = ssid;
		g_pMain->SetVisible(m_user_info, false);
		
		g_pMain->SetState(m_user_info, UI_STATE_BUTTON_DISABLE);
		g_pMain->SetState(m_btnDrop, UI_STATE_BUTTON_NORMAL);
	}
#if (HOOK_SOURCE_VERSION != 1098)
	if (g_pMain->m_PlayerBase && !g_pMain->m_PlayerBase->isInPKZone()) 
	{

		if (m_dTextTargetHp == NULL)
			return;

		g_pMain->SetVisible(m_dTextTargetHp, true);
		int percent = (int)ceil((hp * 100) / maxhp);
		std::string max = g_pMain->StringHelper->NumberFormat(maxhp);
		std::string cur = g_pMain->StringHelper->NumberFormat(hp);
		std::string str = xorstr("%s/%s (%d%%)");
		g_pMain->SetString(m_dTextTargetHp, string_format(str, cur.c_str(), max.c_str(), percent));
	}
	else
	{
		if (m_dTextTargetHp == NULL)
			return;

		g_pMain->SetVisible(m_dTextTargetHp, false);
	}
#endif
}