#include "stdafx.h"
#include "hWhellOfFun.h"

CUIClientWheelOfFun::CUIClientWheelOfFun()
{
	vector<int>offsets;
	offsets.push_back(0x338);    // co_rookietip Satýr : 107	
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);
	lastItem = 0;
	KnightCash = NULL;
	base_item = NULL;
	btn_start = NULL;
	btn_close = NULL;
	btn_stop = NULL;
	base_item = NULL;
	TickSay = clock();
	isActiveWhile = false;
	ParseUIElements();
	InitReceiveMessage();
}

CUIClientWheelOfFun::~CUIClientWheelOfFun()
{
}

void CUIClientWheelOfFun::ParseUIElements()
{
	std::string find = xorstr("base_item");


	char szBuf[64];
	for (int i = 0; i < 15; i++)
	{
		nItem[i] = g_pMain->GetChildByID(m_dVTableAddr, string_format(xorstr("item%.2d"), i));
		/*sprintf(szBuf, "item%.2d", i);
		g_pMain->GetChildByID(m_dVTableAddr, szBuf, nItem[i]);*/
	}

	btn_stop = g_pMain->GetChildByID(m_dVTableAddr, "btn_stop");
	btn_start = g_pMain->GetChildByID(m_dVTableAddr, "btn_start");
	btn_close = g_pMain->GetChildByID(m_dVTableAddr, "btn_close");
	KnightCash = g_pMain->GetChildByID(m_dVTableAddr, "txt_cash");
}
void CUIClientWheelOfFun::ImageChange(int8 sID)
{
	auto it = g_pMain->WheelItemData.find(sID);
	if (it == g_pMain->WheelItemData.end())
		return;

	g_pMain->SetTexImage(nItem[sID],g_pMain->GetItemDXT(it->second));
	g_pMain->SetTexImage(nItem[lastItem],"ui\\itemicon_noimage.dxt");
	lastItem = sID;
}
void CUIClientWheelOfFun::StartChallange()
{
	for (int i = 0; i < 15; i++)
		g_pMain->SetTexImage(nItem[i],"ui\\itemicon_noimage.dxt");

	g_pMain->SetState(btn_start,UI_STATE_BUTTON_DISABLE);
	g_pMain->SetState(btn_stop,UI_STATE_BUTTON_NORMAL);
	isActiveWhile = true;

}
void CUIClientWheelOfFun::OpenWheel()
{
	g_pMain->SetString(KnightCash, string_format("Knight Cash : %d", g_pMain->pClientUIState->m_iCash).c_str());
	//g_pMain->SetString(KnightCash, g_pMain->StringHelper->NumberFormat(g_pMain->pClientUIState->m_iCash).c_str());

	g_pMain->SetVisible(m_dVTableAddr, true);
	g_pMain->UIScreenCenter(m_dVTableAddr);
	for (int i = 0; i < 15; i++)
	{
		auto it = g_pMain->WheelItemData.find(i);
		if (it == g_pMain->WheelItemData.end())
			return;

		g_pMain->SetTexImage(nItem[i], g_pMain->GetItemDXT(it->second));

	}
}
uint32_t CUIClientWheelOfFun::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{

	
	if (!g_pMain->IsVisible(m_dVTableAddr)) return false;
	uint32_t dwRet = UI_MOUSEPROC_NONE;

		bool showTooltip = false;

		int i = 0;
		for (auto it : g_pMain->WheelItemData)
		{
			if (i >= g_pMain->WheelItemData.size())
				break;


			if (g_pMain->IsVisible(nItem[i])  && g_pMain->IsIn(nItem[i],ptCur.x, ptCur.y))
			{
		/*		if (g_pMain->str_contains(nItem[i]->GetTexFN().c_str(), xorstr("itemicon_noimage.dxt")))
					continue;*/

				//printf("%s \n", nItem[i]->GetTexFN().c_str());
				TABLE_ITEM_BASIC* item = g_pMain->pClientTBLManager->GetItemData(it.second);
				if (!item) {
					i++;
					continue;
				}
				g_pMain->ShowToolTipEx(it.second, ptCur.x, ptCur.y);
				showTooltip = true;
				break;
			}
			i++;
		}

	return dwRet;

}

void CUIClientWheelOfFun::GiveItemImage(uint32 nItemID)
{
	auto it = g_pMain->WheelItemData.begin();
	while (it != g_pMain->WheelItemData.end())
	{
		if (it->second == nItemID)
		{
			for (int i = 0; i < 15; i++)
				g_pMain->SetTexImage(nItem[i],"ui\\itemicon_noimage.dxt"); // hepsini gizle

			g_pMain->SetTexImage(nItem[it->first],g_pMain->GetItemDXT(it->second)); // çýkan itemi göster

			break;
		}

		it++;
	}

}
void CUIClientWheelOfFun::StopChallange()
{
	g_pMain->SetState(btn_start,UI_STATE_BUTTON_NORMAL);
	g_pMain->SetState(btn_stop,UI_STATE_BUTTON_DISABLE);
	isActiveWhile = false;
}

void CUIClientWheelOfFun::IsActive(bool enable)
{
	g_pMain->SetVisible(m_dVTableAddr, enable);

	if (!isActiveWhile)
	{
		for (int i = 0; i < 15; i++)
		{
			auto it = g_pMain->WheelItemData.find(i);
			if (it == g_pMain->WheelItemData.end())
				return;

			g_pMain->SetTexImage(nItem[i],g_pMain->GetItemDXT(it->second));

		}


	}
	if (enable)
	{
		
		g_pMain->SetString(KnightCash,string_format("Knight Cash : %d", g_pMain->pClientUIState->m_iCash).c_str());
		g_pMain->UIScreenCenter(m_dVTableAddr);

	}
}
int myrands(int a, int b)
{
	int div = b - a;
	return a + (rand() % (b - a));
}
void CUIClientWheelOfFun::Tick()
{
	if (!g_pMain->IsVisible(m_dVTableAddr) || !isActiveWhile)
		return;

	if (TickSay < clock() - 120)
	{
		TickSay = clock();

		int8 Rand = myrands(0, 24);

		ImageChange(Rand);
	}

}
DWORD Wheelui;
DWORD Func_Wheel;


bool CUIClientWheelOfFun::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	Wheelui = m_dVTableAddr;

	if (!pSender || pSender == 0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;

	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == (DWORD*)btn_start)
			StartChallange();
		else if (pSender ==(DWORD*)btn_stop)
		{
			Packet pkt(WIZ_HSACS_HOOK, uint8_t(HSACSOpCodes::WheelData));
			pkt << uint8(1);
			g_pMain->Send(&pkt);
		}
		else if (pSender == (DWORD*)btn_close)
			IsActive(false);
	}

	return true;
}

void __stdcall pClientWheelOfFunPlugReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->pClientWheelOfFunPlug->ReceiveMessage(pSender, dwMsg);

	__asm
	{
		MOV ECX, Wheelui
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_Wheel
		CALL EAX
	}
}

void CUIClientWheelOfFun::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_Wheel = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)pClientWheelOfFunPlugReceiveMessage_Hook;
}