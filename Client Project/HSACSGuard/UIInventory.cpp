#include "stdafx.h"
#include "UIInventory.h"

#if (HOOK_SOURCE_VERSION == 2369)
const uint8 SLOT_MAX = 14;	// 14 equipped item slots
const uint8 HAVE_MAX = 28;	// 28 inventory slots
#endif

bool viewEmblem = true, viewPathos = true;
bool minervaHook = false;

const DWORD KO_PATHOS_CHECK = 0x438C24;
const DWORD KO_EMBLEM_LOOP_ITERATE = 0x00585E60;
const DWORD KO_UI_INVENTORY_MINERVA_RESTORE = 0x00607B64;
DWORD KO_UI_INVENTORY_MINERVA_RESTORE_ORG = 0;

void MinervaRestore()
{
	if (viewEmblem && g_pMain->pClientInventory) g_pMain->SetState(g_pMain->pClientInventory->btn_EmblemView, UI_STATE_BUTTON_DOWN);
	if (viewPathos && g_pMain->pClientInventory) g_pMain->SetState(g_pMain->pClientInventory->btn_PathosView, UI_STATE_BUTTON_DOWN);
}

void __declspec(naked) hkOpenMinerva()
{
	__asm {
		pushad
		pushfd
		call MinervaRestore
		popfd
		popad
		jmp KO_UI_INVENTORY_MINERVA_RESTORE_ORG
	}
}

CUIInventoryPlug::CUIInventoryPlug()
{
	vector<int>offsets;
	offsets.push_back(0x204); // re_Inventory Satýr : 11
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);
#if (HOOK_SOURCE_VERSION == 2369)
	for (int i = 0; i < HAVE_MAX; i++)
	{
		sInventorySlot[i] = 0;
		sSubCode[i] = 0;
		sItemSlot[i] = 0;
		sItemID[i] = 0;
	}
#endif
	ParseUIElements();
	InitReceiveMessage();
}

CUIInventoryPlug::~CUIInventoryPlug()
{
}

void CUIInventoryPlug::ParseUIElements()
{
	btn_trash = g_pMain->GetChildByID(m_dVTableAddr, "btn_trash");
	base_cos = g_pMain->GetChildByID(m_dVTableAddr, "base_cos");
#if (HOOK_SOURCE_VERSION == 2369)
	btn_EmblemView = g_pMain->GetChildByID(base_cos, "btn_emblePreView");
#else
	btn_EmblemView = g_pMain->GetChildByID(base_cos, "btn_EmblemView");
#endif
	btn_PathosView = g_pMain->GetChildByID(base_cos, "btn_PathosView");

#if (HOOK_SOURCE_VERSION == 2369)
	for (int i = 0; i < HAVE_MAX; i++)
		sInventorySlot[i] = g_pMain->GetChildByID(m_dVTableAddr, string_format(xorstr("exchange%d"), i));
#endif

	if (!minervaHook)
	{
		minervaHook = true;
		KO_UI_INVENTORY_MINERVA_RESTORE_ORG = (DWORD)DetourFunction((PBYTE)KO_UI_INVENTORY_MINERVA_RESTORE, (PBYTE)hkOpenMinerva);
	}

	viewEmblem = GetPrivateProfileIntA(xorstr("Effect"), xorstr("EmblemView"), 1, string(g_pMain->m_BasePath + xorstr("Option.ini")).c_str()) == 1 ? true : false;
	*(uint8*)KO_EMBLEM_LOOP_ITERATE = viewEmblem ? 2 : 1;

	viewPathos = GetPrivateProfileIntA(xorstr("Effect"), xorstr("PathosView"), 1, string(g_pMain->m_BasePath + xorstr("Option.ini")).c_str()) == 1 ? true : false;
	*(uint8*)KO_PATHOS_CHECK = viewPathos ? (uint8)0x74 : (uint8)0xEB;

#if (HOOK_SOURCE_VERSION == 2369)
	for (int i = 0; i < HAVE_MAX; i++)
		g_pMain->SetVisible(sInventorySlot[i], false);
#endif
}

DWORD uiInventoryVTable;
bool CUIInventoryPlug::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	uiInventoryVTable = m_dVTableAddr;
	if (!pSender || pSender == 0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;

	if (pSender == (DWORD*)btn_trash)
	{
		Packet result(WIZ_HSACS_HOOK);
		result << uint8_t(HSACSOpCodes::SendRepurchaseMsg);
		g_pMain->Send(&result);
	}
	else if (pSender == (DWORD*)btn_EmblemView)
	{
		viewEmblem = !viewEmblem;
		*(uint8*)KO_EMBLEM_LOOP_ITERATE = viewEmblem ? 2 : 1;

		WritePrivateProfileStringA(xorstr("Effect"), xorstr("EmblemView"), viewEmblem ? xorstr("1") : xorstr("0"), string(g_pMain->m_BasePath + xorstr("Option.ini")).c_str());
	}
	else if (pSender == (DWORD*)btn_PathosView)
	{
		viewPathos = !viewPathos;
		*(uint8*)KO_PATHOS_CHECK = viewPathos ? (uint8)0x74 : (uint8)0xEB;

		WritePrivateProfileStringA(xorstr("Effect"), xorstr("PathosView"), viewPathos ? xorstr("1") : xorstr("0"), string(g_pMain->m_BasePath + xorstr("Option.ini")).c_str());
	}
	return true;
}

DWORD Func_Inventory;
void __stdcall UIInventoryReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->pClientInventory->ReceiveMessage(pSender, dwMsg);
	__asm
	{
		MOV ECX, uiInventoryVTable
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_Inventory
		CALL EAX
	}
}

uint32_t CUIInventoryPlug::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;
#if (HOOK_SOURCE_VERSION == 2369)
	if (g_pMain->pClientInventory != NULL)
	{
		if (dwFlags & UI_MOUSE_RBCLICKED)
		{
			for (int i = 0; i < HAVE_MAX; i++)
			{
				if (sItemSlot[i] == 1)
				{
					POINT pt1;
					POINT pt2;
					g_pMain->GetCoordination(g_pMain->pClientInventory->sInventorySlot[i], pt1, pt2);
					if (ptCur.x < pt1.x && ptCur.x >pt2.x && ptCur.y < pt1.y && ptCur.y >pt2.y)
					{
						Packet result(WIZ_HSACS_HOOK, uint8(WIZ_ITEM_EXCHANGE_INFO));
						result << sSubCode[i] << uint8(SLOT_MAX + i) << g_pMain->pClientInventory->sItemID[i];
						g_pMain->Send(&result);
					}
				}
			}
		}
		else if (dwFlags & UI_MOUSE_LBDOWN)
		{
			for (int i = 0; i < HAVE_MAX; i++)
			{
				if (sItemSlot[i] == 1)
				{
					POINT pt1;
					POINT pt2;
					g_pMain->GetCoordination(g_pMain->pClientInventory->sInventorySlot[i], pt1, pt2);

					if (ptCur.x < pt1.x && ptCur.x >pt2.x && ptCur.y < pt1.y && ptCur.y >pt2.y)
					{
						if (g_pMain->IsVisible(sInventorySlot[i]))
							g_pMain->SetVisible(sInventorySlot[i], false);
					}
				}
			}
		}
		else if (dwFlags == 0x00000000)
		{
			for (int i = 0; i < HAVE_MAX; i++)
			{
				if (sItemSlot[i] == 1)
					g_pMain->SetVisible(sInventorySlot[i], true);
				else
					g_pMain->SetVisible(sInventorySlot[i], false);
			}
		}
	}
#endif
	return dwRet;
}

void CUIInventoryPlug::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_Inventory = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)UIInventoryReceiveMessage_Hook;
}