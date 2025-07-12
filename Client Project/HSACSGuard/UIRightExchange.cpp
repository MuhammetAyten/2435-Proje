#include "UIRightExchange.h"
#if(HOOK_SOURCE_VERSION == 2369 || HOOK_SOURCE_VERSION == 1098)
enum RightExchangeSubCode
{
	GetItemList,
	GetCurrentItemList,
	GetExchangeMessage,
	SendItemExchange,
	SendItemExchangeList,
	GetExchangeInformation,
	SendExchangeInformation,
	GetExchangeChestInformation,
	SendExchangeChestInformation
};

DWORD RightExchangevTable = 0;
DWORD Func_RightExchange;
#if(HOOK_SOURCE_VERSION == 1098)
extern __inline DWORD RDWORD(DWORD ulBase);
void __fastcall InvetoryMoveSystem(DWORD ItemSkillIcon)
{
	if (g_pMain->pClientRightExchange)
	{
		if (ItemSkillIcon && !IsBadReadPtr((VOID*)(ItemSkillIcon + 0x68), sizeof(DWORD)))
		{
			DWORD extBase = RDWORD(ItemSkillIcon + 0x6C);
			DWORD ID = RDWORD(RDWORD(ItemSkillIcon + 0x68));
			DWORD EXT = RDWORD(extBase);
			uint32 nItemID = ID + EXT;

			auto itr = g_pMain->pClientRightExchange->pRightClickExchangeReward;
			if (std::find(itr.begin(), itr.end(), nItemID) != itr.end())
			{
				Packet result(WIZ_HSACS_HOOK, uint8(WIZ_ITEM_EXCHANGE_INFO));
				result << uint8(3) << uint8(1) << nItemID;
				g_pMain->Send(&result);
			}
			else
			{
				itr = g_pMain->pClientRightExchange->pRightClickExchangeAll;
				if (std::find(itr.begin(), itr.end(), nItemID) != itr.end())
				{
					Packet result(WIZ_HSACS_HOOK, uint8(WIZ_ITEM_EXCHANGE_INFO));
					result << uint8(3) << uint8(2) << nItemID;
					g_pMain->Send(&result);
				}
				else
				{
					itr = g_pMain->pClientRightExchange->pRightClickExchangeKnightCash;
					if (std::find(itr.begin(), itr.end(), nItemID) != itr.end())
					{
						Packet result(WIZ_HSACS_HOOK, uint8(WIZ_ITEM_EXCHANGE_INFO));
						result << uint8(3) << uint8(4) << nItemID;
						g_pMain->Send(&result);
					}
					else
					{
						itr = g_pMain->pClientRightExchange->pRightClickExchangePremium;
						if (std::find(itr.begin(), itr.end(), nItemID) != itr.end())
						{
							Packet result(WIZ_HSACS_HOOK, uint8(WIZ_ITEM_EXCHANGE_INFO));
							result << uint8(3) << uint8(3) << nItemID;
							g_pMain->Send(&result);
						}
						else
						{
#if(HOOK_SOURCE_VERSION == 1098)
							itr = g_pMain->pClientRightExchange->pRightClickGeneratorExchange;
							if (std::find(itr.begin(), itr.end(), nItemID) != itr.end())
							{
								//printf("ItemMove\n");
								Packet result(WIZ_HSACS_HOOK, uint8(WIZ_ITEM_EXCHANGE_INFO));
								result << uint8(3) << uint8(5) << nItemID;
								g_pMain->Send(&result);
							}
							else
							{
								itr = g_pMain->pClientRightExchange->pRightClickExchangeGenie;
								if (std::find(itr.begin(), itr.end(), nItemID) != itr.end())
								{
									//printf("ItemMove\n");
									Packet result(WIZ_HSACS_HOOK, uint8(WIZ_ITEM_EXCHANGE_INFO));
									result << uint8(3) << uint8(6) << nItemID;
									g_pMain->Send(&result);
								}
								else
								{
									itr = g_pMain->pClientRightExchange->pRightClickExchangeKnightTL;
									if (std::find(itr.begin(), itr.end(), nItemID) != itr.end())
									{
										//printf("ItemMove\n");
										Packet result(WIZ_HSACS_HOOK, uint8(WIZ_ITEM_EXCHANGE_INFO));
										result << uint8(3) << uint8(7) << nItemID;
										g_pMain->Send(&result);
									}
									else
										return;
								}
							}
#endif
						}
					}
				}
			}
		}
	}
}
const DWORD KO_INVENTORY_ITEM_MOVE = 0x006012B5;
void __declspec(naked) hkInventMove()
{
	__asm 
	{
		pushad
		pushfd
		cmp eax, -256
		jne f_continue
		mov ecx, ebx
		call InvetoryMoveSystem
		f_continue :
		popfd
		popad
		pop ecx
		pop edi
		pop esi
		pop ebp
		pop ebx
		mov edx, KO_INVENTORY_ITEM_MOVE
		add edx, 5
		push edx
		ret
	}
}
#endif
CUIRightExchangePlug::CUIRightExchangePlug()
{
	vector<int>offsets;
	offsets.push_back(0x2A0);
	offsets.push_back(0x0);
	el_Base = g_pMain->rdword(KO_DLG, offsets);
	RightExchangevTable = el_Base;
	nBaseItemID = m_iCount = 0;
	ParseUIElements();
	InitReceiveMessage();
	posSet = false;
	m_Timer = new CTimer();
	bCurrentPage = 1;
	ExchangeItemStatus = false;
	m_iSlot = -1;
#if(HOOK_SOURCE_VERSION == 1098)
	DetourFunction((PBYTE)KO_INVENTORY_ITEM_MOVE, (PBYTE)hkInventMove);
#endif
}

CUIRightExchangePlug::~CUIRightExchangePlug() {}

void CUIRightExchangePlug::CUIRightExchangePlugPacket(Packet pkt)
{
	uint8 bOpCode = pkt.read<uint8>();

	switch (bOpCode)
	{
	case RightExchangeSubCode::GetItemList:
	{
		/*pRightClickExchangeList.clear();
		uint8 bCount = pkt.read<uint8>();

		for (int i = 0; i < bCount; i++)
		{
			uint32 itemID = pkt.read<uint32>();
			pRightClickExchangeList.push_back(itemID);
		}
		break;*/
	}

	case RightExchangeSubCode::GetCurrentItemList:
	{
		for (int i = 0; i < 25; i++)
		{
			uint32 nItemID;
			uint32 nRentalTime;
			pkt >> nItemID >> nRentalTime;
			if (nItemID == 0)
			{
				g_pMain->SetVisible(m_Icon[i]->icon, false);
				g_pMain->SetUIStyle(m_Icon[i]->icon, 0x20);
				continue;
			}
			g_pMain->SetTexImage(m_Icon[i]->icon, g_pMain->GetItemDXT(nItemID));
			m_Icon[i]->nItemID = nItemID;
			m_Icon[i]->nRentalTime = nRentalTime;
			g_pMain->SetVisible(m_Icon[i]->icon, true);
			g_pMain->SetUIStyle(m_Icon[i]->icon, 0x20);
		}

		nBaseItemID = pkt.read<uint32>();
		CItem* pData = (CItem*)g_pMain->pClientTBLManager->GetItemData(nBaseItemID);
		if (!pData)
			break;

		g_pMain->SetString(txt_title, "Right Click Exchange");
		g_pMain->SetTexImage(base_itemicon, g_pMain->GetItemDXT(nBaseItemID));
		g_pMain->SetVisible(base_itemicon, true);
		Open();
	}
	break;
	case RightExchangeSubCode::GetExchangeMessage:
	{
		uint8 bSucces = pkt.read<uint8>();
		char buff[50];
		if (bSucces)
		{
			sprintf_s(buff, "RightClick Exchange successfully.");
			g_pMain->WriteInfoMessage(buff, 0x009700);
			this->Close();
		}
		else if (!bSucces) 
		{
			sprintf_s(buff, "RightClick Exchange failed.");
			g_pMain->WriteInfoMessage(buff, 0xBA0F30);
		}
		sSelectItem = 0;
	}
	case RightExchangeSubCode::SendItemExchangeList:
	{
		uint16 nCount = pkt.read<uint16>();
		//for (int i = 0; i < nCount;i++)
			//g_pMain->pClientRightExchange->pRightClickExchangeList.push_back(pkt.read<uint32>());
	}
	break;
	case RightExchangeSubCode::SendExchangeInformation:
	{
		uint32 nExchangeGemID;
		uint16 nListSize;
		ExchangeItemStatus = true;
		GetExchangeList.clear();
		pkt >> nExchangeGemID >> nListSize;
		for (int i = 0; i < nListSize;i++)
		{
			ExchangeInformation* apInfo = new ExchangeInformation();
			pkt >> apInfo->nItemID >> apInfo->sPercent;
			if (apInfo->nItemID == 0)
			{
				delete apInfo;
				continue;
			}
			GetExchangeList.push_back(apInfo);
		}

		bPageCount = abs(ceil((double)GetExchangeList.size() / (double)PAGE_COUNT));
		Open();
		g_pMain->SetTexImage(base_itemicon, g_pMain->GetItemDXT(nExchangeGemID));
		GetExchangePage(1);
		bCurrentPage = 1;
		g_pMain->SetVisible(btn_exchange, false);
	}
	break;
	case RightExchangeSubCode::SendExchangeChestInformation:
	{
		uint32 nExchangeGemID;
		uint16 nListSize;
		ExchangeItemStatus = true;
		GetExchangeList.clear();
		pkt >> nExchangeGemID >> nListSize;
		for (int i = 0; i < nListSize;i++)
		{
			ExchangeInformation* apInfo = new ExchangeInformation();
			pkt >> apInfo->nItemID >> apInfo->sPercent;
			if (apInfo->nItemID == 0)
			{
				delete apInfo;
				continue;
			}
			GetExchangeList.push_back(apInfo);
		}

		bPageCount = abs(ceil((double)GetExchangeList.size() / (double)PAGE_COUNT));
		Open();
		g_pMain->SetTexImage(base_itemicon, g_pMain->GetItemDXT(nExchangeGemID));
		GetExchangePage(1);
		bCurrentPage = 1;
		g_pMain->SetVisible(btn_exchange, false);
	}
	break;
	default:
		break;
	}


}
void CUIRightExchangePlug::GetExchangePage(uint8 bPage)
{
	if (bPage > bPageCount) 
		return;

	g_pMain->SetString(page, std::to_string(bPage));
	g_pMain->SetVisible(btn_down, true);
	g_pMain->SetVisible(btn_next, true);
	g_pMain->SetVisible(page, true);
	int begin = (bPage - 1) * PAGE_COUNT;

	for (int i = 0; i < 25; i++)
	{
		g_pMain->SetVisible(m_Icon[i]->icon, false);
		g_pMain->SetUIStyle(m_Icon[i]->icon, 0x20);
		g_pMain->SetVisible(percent[i], false);
	}

	for (int i = begin; i < begin + PAGE_COUNT; i++)
	{
		if (i >= GetExchangeList.size()) // maxsize control
			continue;

		ExchangeInformation* pInfo = GetExchangeList[i];
		g_pMain->SetTexImage(m_Icon[i - begin]->icon, g_pMain->GetItemDXT(pInfo->nItemID));
		float per = (float)pInfo->sPercent / 100;
		g_pMain->SetString(percent[i - begin], string_format(xorstr("%.2f%%"), per));
		g_pMain->SetVisible(percent[i - begin], true);
		g_pMain->SetVisible(m_Icon[i - begin]->icon, true);
		m_Icon[i - begin]->nItemID = GetExchangeList[i]->nItemID;
		m_Icon[i - begin]->nRentalTime = 0;
		g_pMain->SetUIStyle(m_Icon[i - begin]->icon, 0x20);
	}
}
void CUIRightExchangePlug::ParseUIElements()
{
	std::string find = xorstr("Base_PacketExchange");
	Base_PacketExchange = g_pMain->GetChildByID(el_Base, find);
	find = xorstr("txt_title");
	txt_title = g_pMain->GetChildByID(Base_PacketExchange, find);
	find = xorstr("btn_exchange");
	btn_exchange = g_pMain->GetChildByID(Base_PacketExchange, find);
	find = xorstr("grp_percents");
	percent_base = g_pMain->GetChildByID(Base_PacketExchange, find);
	find = xorstr("click_area");
	click_area = g_pMain->GetChildByID(Base_PacketExchange, find);
	find = xorstr("btn_close");
	btn_close = g_pMain->GetChildByID(Base_PacketExchange, find);
	find = xorstr("grp_selected");
	grp_selected = g_pMain->GetChildByID(Base_PacketExchange, find);
	find = xorstr("base_item");
	base_item = g_pMain->GetChildByID(Base_PacketExchange, find);
	find = xorstr("page");
	page = g_pMain->GetChildByID(Base_PacketExchange, find);
	find = xorstr("btn_next");
	btn_next = g_pMain->GetChildByID(Base_PacketExchange, find);
	find = xorstr("btn_down");
	btn_down = g_pMain->GetChildByID(Base_PacketExchange, find);
	g_pMain->SetString(page, "1");
	g_pMain->SetVisible(btn_down, false);
	g_pMain->SetVisible(page, false);
	find = xorstr("area_slot");
	base_item_area = g_pMain->GetChildByID(base_item, find);

	for (int i = 1; i <= 25;i++)
	{
		percent[i - 1] = g_pMain->GetChildByID(percent_base, string_format(xorstr("percent_%d"), i));
		g_pMain->SetVisible(percent[i - 1], false);
		find = string_format(xorstr("item_%d"), i);
		ItemBase[i - 1] = g_pMain->GetChildByID(Base_PacketExchange, find);
		find = xorstr("area_slot");
		area_slot[i - 1] = g_pMain->GetChildByID(ItemBase[i - 1], find);;
		DWORD bCreateIcon = 0x00;

		if (!g_pMain->_InitIcon(bCreateIcon, area_slot[i - 1], 910014000))
			continue;

		ItemInfo* inf = new ItemInfo();
		inf->icon = bCreateIcon;
		inf->nItemID = 0;
		g_pMain->SetRegion(inf->icon, area_slot[i - 1]);
		g_pMain->SetVisible(inf->icon, true);
		g_pMain->SetVisible(bCreateIcon, false);
		m_Icon.push_back(inf);
	}

	DWORD baseCreateIcon = 0x00;

	if (!g_pMain->_InitIcon(baseCreateIcon, base_item_area, 910014000))
		printf("Error Code : 1 \n");

	base_itemicon = baseCreateIcon;
	g_pMain->SetRegion(baseCreateIcon, base_item_area);
	g_pMain->SetVisible(baseCreateIcon, true);

	Base_Exchange = g_pMain->GetChildByID(el_Base, "Base_Exchange");
	Unit_0 = g_pMain->GetChildByID(Base_Exchange, "Unit_0");
	Base_btn_exchange = g_pMain->GetChildByID(Unit_0, "Exchange");
	Edit_Count = g_pMain->GetChildByID(Unit_0, "Edit_Count");
	String_Count = g_pMain->GetChildByID(Edit_Count, "String_Count");
	Text_0 = g_pMain->GetChildByID(Unit_0, "Text_0");
	ItemSlot = g_pMain->GetChildByID(Unit_0, "ItemSlot");

	DWORD bCreateIcon = 0x00;

	if (!g_pMain->_InitIcon(bCreateIcon, ItemSlot, 910014000))
		return;

	ItemInfo* inf = new ItemInfo();
	inf->icon = bCreateIcon;
	inf->nItemID = 0;
	g_pMain->SetRegion(inf->icon, ItemSlot);
	g_pMain->SetVisible(inf->icon, true);
	g_pMain->SetVisible(bCreateIcon, false);
	Right.push_back(inf);

	btn_exit = g_pMain->GetChildByID(el_Base, "btn_exit");

	g_pMain->SetVisible(Base_Exchange, false);
	g_pMain->SetVisible(Base_PacketExchange, false);
}

bool CUIRightExchangePlug::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	RightExchangevTable = el_Base;
	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == (DWORD*)btn_close
			|| pSender == (DWORD*)btn_exit)
			Close();
		else if (pSender == (DWORD*)btn_exchange)
		{
			Packet result(WIZ_HSACS_HOOK, uint8(WIZ_ITEM_EXCHANGE_INFO));
			result << uint8(4);
			if (sType == 1)
				result << sType << nBaseItemID << sSelectItem;
			else
				result << sType << nBaseItemID;

			g_pMain->Send(&result);
			return true;
		}
		else if (pSender == (DWORD*)btn_down)
		{
			if (bCurrentPage > 1)
			{
				bCurrentPage--;
				GetExchangePage(bCurrentPage);
			}
		}
		else if (pSender == (DWORD*)btn_next)
		{
			if (bPageCount > 1 && bCurrentPage < bPageCount)
			{
				bCurrentPage++;
				GetExchangePage(bCurrentPage);
			}
		}
		else if (pSender == (DWORD*)Base_btn_exchange)
		{
			uint16 curCount = atoi(g_pMain->GetString(String_Count).c_str());
			if (!curCount)
				curCount = 1;
			else if (curCount > 100)
				curCount = 100;

			Packet result(WIZ_HSACS_HOOK, uint8(WIZ_ITEM_EXCHANGE_INFO));
#if(HOOK_SOURCE_VERSION == 2369)
			result << uint8(5) << nBaseItemID << m_iSlot << uint8(0) << uint8(0) << uint8(curCount);
#else
			result << uint8(5) << nBaseItemID << uint8(0) << uint8(0) << uint8(0) << uint8(curCount);
#endif
			g_pMain->Send(&result);
		}
	}
	return true;
}

uint32_t CUIRightExchangePlug::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;
	if (g_pMain->IsVisible(Base_PacketExchange))
	{
		if (g_pMain->IsIn(click_area, ptCur.x, ptCur.y))
		{
			foreach(itr, m_Icon)
			{
				ItemInfo* pData = (*itr);
				if (!g_pMain->IsVisible(pData->icon) || !g_pMain->IsIn(pData->icon, ptCur.x, ptCur.y))
					continue;

				g_pMain->ShowToolTipEx(pData->nItemID, ptCur.x, ptCur.y);
			}

			if ((dwFlags == UI_MOUSE_RBCLICKED || dwFlags == UI_MOUSE_LBCLICKED) && sType == 1)
			{
				for (int i = 0; i < 25;i++)
					g_pMain->SetUIStyle(m_Icon[i]->icon, UISTYLE_USER_MOVE_HIDE);

				foreach(itr, m_Icon)
				{
					ItemInfo* pData = (*itr);
					if (!g_pMain->IsVisible(pData->icon) || !g_pMain->IsIn(pData->icon, ptCur.x, ptCur.y))
						continue;

					sSelectItem = pData->nItemID;
					g_pMain->SetUIStyle(pData->icon, UISTYLE_STRING_ALIGNRIGHT);
					break;
				}
			}
		}
	}
	return dwRet;
}

void CUIRightExchangePlug::Open(uint8 sCode) 
{ 
	g_pMain->SetVisible(el_Base, true);
	sSelectItem = 0;
	if (sCode == 1)
	{
		g_pMain->SetVisible(Base_Exchange, false);
		g_pMain->SetVisible(btn_exit, false);
		g_pMain->SetVisible(Base_PacketExchange, true);
	}
	else if (sCode == 2)
	{
		g_pMain->SetVisible(Base_PacketExchange, false);
		g_pMain->SetVisible(Base_Exchange, true);
		g_pMain->SetVisible(btn_exit, true);
	}
	else 
	{
		g_pMain->SetVisible(Base_PacketExchange, false);
		g_pMain->SetVisible(Base_Exchange, false);
		g_pMain->SetVisible(btn_exit, false);
		g_pMain->SetVisible(el_Base, false);
		return;
	}
	g_pMain->UIScreenCenter(el_Base);
}

void CUIRightExchangePlug::Close() { sSelectItem = NULL; g_pMain->SetVisible(el_Base, false); g_pMain->SetVisible(Base_PacketExchange, false); g_pMain->SetVisible(Base_Exchange, false); g_pMain->SetVisible(btn_exit, false); ExchangeItemStatus = false; m_Timer = NULL; m_iSlot = -1; nBaseItemID = NULL; m_iCount = NULL; }

DWORD oRecvMsgCUIRightExchangePlug;
void __stdcall UiRightExchangeReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->pClientRightExchange->ReceiveMessage(pSender, dwMsg);

	__asm
	{
		MOV ECX, RightExchangevTable
		PUSH dwMsg
		PUSH pSender
		call oRecvMsgCUIRightExchangePlug
	}
}

void CUIRightExchangePlug::Tick()
{
	if (g_pMain->IsVisible(Base_Exchange))
	{
		if (isStard())
		{
			if (m_Timer == NULL)
				m_Timer = new CTimer(true, 500);

			if (m_Timer->IsElapsedSecond())
			{
				uint16 m_sCount = atoi(g_pMain->GetString(String_Count).c_str());
				if (m_sCount == NULL)
				{
					g_pMain->pClientHookManager->ShowMessageBox("Item Generator Exchange", "Lütfen tane kýrmak istiyorsanýz sayýsý giriniz.!!");
					Close();
				}
				else
				{
					if (m_iCount >= m_sCount - 1)
					{
						Close();
						return;
					}
					Packet result(WIZ_HSACS_HOOK, uint8(WIZ_ITEM_EXCHANGE_INFO));
					result << uint8(2) << uint8(1) << m_iSlot << nBaseItemID;
					g_pMain->Send(&result);
					m_iCount++;
				}
			}
		}
	}
}

void CUIRightExchangePlug::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(el_Base);
	oRecvMsgCUIRightExchangePlug = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)UiRightExchangeReceiveMessage_Hook;
}
#endif