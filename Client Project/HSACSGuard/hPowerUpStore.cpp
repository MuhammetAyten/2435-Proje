#include "hPowerUpStore.h"
bool pus_compare(const PUSItem& a, const PUSItem& b);

CUIPowerUpStore::CUIPowerUpStore()
{
	m_dVTableAddr = NULL;

	vector<int>offsets;
	offsets.push_back(0x348);   // re_rental_message Satýr : 115
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);

	shopping_confirm = NULL;
	btn_close = NULL;
	btn_search = NULL;
	btn_reseller = NULL;
	btn_next = NULL;
	btn_previous = NULL;
	btn_useesn = NULL;
	txt_search = NULL;
	txt_page = NULL;

	txt_cash = NULL;
	txt_tlbalance = NULL;
	page = 1;
	pageCount = 1;
	cat = 1;
	itemscount = 0;
	esntime = GetTickCount64();
	m_LastTick = GetTickCount();
	ParseUIElements();
	InitReceiveMessage();
}

CUIPowerUpStore::~CUIPowerUpStore()
{

}

void CUIPowerUpStore::ParseUIElements()
{	
	// Base
	shopping_confirm = g_pMain->GetChildByID(m_dVTableAddr, "shopping_confirm");
	btn_confirm = g_pMain->GetChildByID(shopping_confirm, "btn_confirm");
	btn_cancel = g_pMain->GetChildByID(shopping_confirm, "btn_cancel");
	shopping_icon = g_pMain->GetChildByID(shopping_confirm, "shopping_icon");
	shopping_name = g_pMain->GetChildByID(shopping_confirm, "shopping_name");
	shopping_price = g_pMain->GetChildByID(shopping_confirm, "shopping_price");
	shopping_quantity = g_pMain->GetChildByID(shopping_confirm, "shopping_quantity");
	shopping_after = g_pMain->GetChildByID(shopping_confirm, "shopping_after");
	btn_up = g_pMain->GetChildByID(shopping_confirm, "btn_up");
	btn_down = g_pMain->GetChildByID(shopping_confirm, "btn_down");
	item_count = g_pMain->GetChildByID(shopping_confirm, "str_count");
	gift_edit_name = g_pMain->GetChildByID(shopping_confirm, "txt_gift_name");
	g_pMain->SetVisible(shopping_confirm, false);

	//basket menu	
	basket_confirm = g_pMain->GetChildByID(m_dVTableAddr, "Basket_confirm");
	basket_cash = g_pMain->GetChildByID(basket_confirm, "text_basket_total_price");
	basket_ok = g_pMain->GetChildByID(basket_confirm, "btn_all_confirm");
	basket_no = g_pMain->GetChildByID(basket_confirm, "btn_cancel");
	g_pMain->SetVisible(basket_confirm,false);

	// edit
	txt_code = g_pMain->GetChildByID(m_dVTableAddr, "esncode");

	// Buttons
	btn_close = g_pMain->GetChildByID(m_dVTableAddr, "btn_close");
	btn_search = g_pMain->GetChildByID(m_dVTableAddr, "btn_search");
	btn_reseller = g_pMain->GetChildByID(m_dVTableAddr, "btn_reseller");
	btn_next = g_pMain->GetChildByID(m_dVTableAddr, "btn_next");
	btn_previous = g_pMain->GetChildByID(m_dVTableAddr, "btn_previous");
	btn_useesn = g_pMain->GetChildByID(m_dVTableAddr, "btn_useesn");
	btn_pusrefund = g_pMain->GetChildByID(m_dVTableAddr, "btn_refund");

	for (int i = 1; i <= 5; i++)
		btn_tabs.push_back(g_pMain->GetChildByID(m_dVTableAddr, string_format(xorstr("btn_tab%d"), i)));
	
	g_pMain->SetState(btn_tabs[0], UI_STATE_BUTTON_DISABLE);

	// Texts
	edit_search = g_pMain->GetChildByID(m_dVTableAddr, "edit_search");
	txt_search = g_pMain->GetChildByID(m_dVTableAddr, "edit_search");
	txt_page = g_pMain->GetChildByID(m_dVTableAddr, "txt_page");
	txt_cash = g_pMain->GetChildByID(m_dVTableAddr, "txt_cash");
	txt_tlbalance = g_pMain->GetChildByID(m_dVTableAddr, "txt_tl_balance");
	total_cash = g_pMain->GetChildByID(m_dVTableAddr, "text_basket_total_price");
	btn_buyall = g_pMain->GetChildByID(m_dVTableAddr, "btn_buyall");
	btn_clearall = g_pMain->GetChildByID(m_dVTableAddr, "btn_clearall");
	basepusrefund = g_pMain->GetChildByID(m_dVTableAddr, "base_refund");
	
	for (int i = 0; i < 18; i++) {
		baserefunditem[i] = g_pMain->GetChildByID(basepusrefund, string_format(xorstr("refund_items_%d"), i));
		DWORD itemGroup = baserefunditem[i];
	
		UI_REFUND_LIST refund_list{};
		refund_list.sItemID = 0;
		refund_list.nBase = itemGroup;
		refund_list.name = g_pMain->GetChildByID(itemGroup, string_format(xorstr("refund_item%d"), i));
		refund_list.price = g_pMain->GetChildByID(itemGroup, string_format(xorstr("refund_price%d"), i));
		refund_list.icon = g_pMain->GetChildByID(itemGroup, xorstr("item_icon"));
		refund_list.btn_ireturnadd = g_pMain->GetChildByID(itemGroup, "btn_ireturn");
		g_pMain->SetString(refund_list.name, "-");
		g_pMain->SetString(refund_list.price, "-");
		g_pMain->SetVisible(refund_list.nBase, false);
		g_pMain->SetVisible(refund_list.icon, true);
		refund_items.push_back(refund_list);
	}

	btn_refundclose = g_pMain->GetChildByID(basepusrefund, "btn_close");
	g_pMain->SetVisible(basepusrefund, false);
	g_pMain->SetEditString(txt_code, "");

	// Pus Items
	for (int i = 1; i <= PUS_ITEMSCOUNT; i++)
	{
		itemGroupBase[i - 1] = g_pMain->GetChildByID(m_dVTableAddr, string_format(xorstr("items%d"), i));
		DWORD itemGroup = itemGroupBase[i - 1];
		for (int j = 1; j <= PUS_ITEMSCOUNT; j++)
		{
			DWORD item = 0;
			item = g_pMain->GetChildByID(itemGroup, string_format(xorstr("item%d"), j));

			UI_PUSITEM pus_item{};
			pus_item.nBase = item;
			pus_item.icon = g_pMain->GetChildByID(item, "item_icon");
			pus_item.name = g_pMain->GetChildByID(item, "item_name");
			pus_item.price = g_pMain->GetChildByID(item, "item_price");
			pus_item.quantitiy = g_pMain->GetChildByID(item, "item_quantitiy");
			pus_item.purchase = g_pMain->GetChildByID(item, "btn_purchase");
			pus_item.add_to = g_pMain->GetChildByID(item, "btn_add_basket");
			g_pMain->SetString(pus_item.name, "-");
			g_pMain->SetString(pus_item.price, "-");
			g_pMain->SetString(pus_item.quantitiy, "-");
			pus_items.push_back(pus_item);
		}
	}

	for (int i = 1; i <= 7; i++)
	{
		DWORD item = 0;
		item = g_pMain->GetChildByID(m_dVTableAddr, string_format(xorstr("purchase_list%d"), i));

		UI_PURCHASE_LIST pus_item{};
		pus_item.nBase = item;
		pus_item.icon = g_pMain->GetChildByID(item, "item_icon");
		pus_item.name = g_pMain->GetChildByID(item, "shopping_name");
		pus_item.price = g_pMain->GetChildByID(item, "shopping_price");
		pus_item.cancel = g_pMain->GetChildByID(item, "btn_cancel");
		g_pMain->SetString(pus_item.name, "-");
		g_pMain->SetString(pus_item.price, "-");
		g_pMain->SetVisible(item, false);
		purchase_items.push_back(pus_item);
	}
	g_pMain->SetString(txt_page, to_string(page));
	g_pMain->UIScreenCenter(m_dVTableAddr);
}

DWORD uiPus;
DWORD Func_puS;

bool CUIPowerUpStore::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	uiPus = m_dVTableAddr;
	if (!pSender 
		|| pSender == (DWORD*) 0 
		|| dwMsg != UIMSG_BUTTON_CLICK)
		return false;

	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == (DWORD*)btn_useesn) 
		{
			if (esntime > GetTickCount64())
			{
				g_pMain->WriteChatAddInfo(D3DCOLOR_ARGB(255, 197, 209, 189), false, string_format("please wait this time %d", (esntime - GetTickCount64()) / 1000).c_str());
				return false;
			}

			esntime = GetTickCount64() + 60 * 1000;
			std::string kesncode = g_pMain->GetEditString(txt_code).c_str(), checklog = "Please check the characters you entered";

			if (kesncode.empty() || kesncode.length() != 20 || !g_pMain->WordGuardSystem(kesncode, kesncode.length())) {
				g_pMain->WriteChatAddInfo(D3DCOLOR_ARGB(255, 197, 209, 189), false, checklog.c_str());
				return false;
			}

			string firstbox = kesncode.substr(0, 4), secondbox = kesncode.substr(4);
			Packet result(WIZ_EDIT_BOX);
			result.SByte();
			result << uint8(4) << std::stoi(firstbox) << secondbox;
			g_pMain->Send(&result);
		}
		
		if (pSender == (DWORD*)btn_refundclose) {
		
			setBaseItems(1);
			RefundSetItems();
			g_pMain->SetVisible(basepusrefund, false);
		}

		for (int i = 0; i < 18; i++) 
		{
			if (pSender == (DWORD*)refund_items[i].btn_ireturnadd) 
			{

				if (GetTickCount() - m_LastTick <= 5)
				{
					g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("time error!")).c_str(), 0xa30000);
					return false;
				} 
				m_LastTick = GetTickCount();

				Packet newpkt(WIZ_HSACS_HOOK, uint8(HSACSOpCodes::PusRefund));
				newpkt << uint8(pusrefunopcode::ireturn) << refund_items[i].serial;
				g_pMain->Send(&newpkt);
			}
		}

		if (pSender == (DWORD*)btn_close)
			Close();
		else if (pSender == (DWORD*)btn_next)
		{
			if (page < pageCount)
			{
				page++;
				SetItems(page, cat);
			}
		}
		else if (pSender == (DWORD*)btn_previous)
		{
			if (page > 1)
			{
				page--;
				SetItems(page, cat);
			}
		}
		else if (pSender == (DWORD*)btn_search)
		{
			SetItems(page, cat, g_pMain->GetEditString(txt_search).c_str());
		}
		else if (pSender == (DWORD*)btn_clearall)
			PurchaseDeleteAll();
		else if (pSender == (DWORD*)basket_ok)
			PurchaseBuyAll();
		else if (pSender == (DWORD*)basket_no)
			g_pMain->SetVisible(basket_confirm,false);
		else if (pSender == (DWORD*)btn_buyall)
			BasketMenuOpen();
		else if (pSender == (DWORD*)btn_confirm)
		{
			if (gift_edit_name == NULL) 
				return false;

			if (itemscount == 0) 
				itemscount = 1;

			Packet result(WIZ_HSACS_HOOK);
			result << uint8_t(HSACSOpCodes::PUS);
			result.DByte();

			std::string getgiftname = g_pMain->GetEditString(gift_edit_name).c_str();
			if (getgiftname.empty()) 
				result << uint8(0x01) << uint32(PusID) << uint8(itemscount);
			else 
				result << uint8(0x03) << sCodeType << uint32(PusID) << std::string(getgiftname);

			g_pMain->Send(&result);

			g_pMain->SetVisible(shopping_confirm,false);
			g_pMain->SetState(btn_confirm,UI_STATE_BUTTON_NORMAL);
			g_pMain->SetState(btn_cancel,UI_STATE_BUTTON_NORMAL);
			itemscount = 1;
			g_pMain->SetString(item_count,string_format("%d", itemscount));
			g_pMain->SetEditString(gift_edit_name, "");
		}
		else if (pSender == (DWORD*)btn_up)
		{
			itemscount++;

			if (itemscount > 28)
				itemscount = 28;

			g_pMain->SetString(item_count,string_format("%d", itemscount));
			g_pMain->SetString(shopping_quantity, string_format(xorstr("Quantity: %s"), g_pMain->StringHelper->NumberFormat(itemscount).c_str()));
			g_pMain->SetString(shopping_after,string_format(xorstr("%d Knight Cash"), itemscount * shopping_p));
		}
		else if (pSender == (DWORD*)btn_down)
		{
			if(itemscount > 0) 
				itemscount--;

			g_pMain->SetString(item_count,string_format("%d", itemscount));
			g_pMain->SetString(shopping_quantity, string_format(xorstr("Quantity: %s"), g_pMain->StringHelper->NumberFormat(itemscount).c_str()));
			g_pMain->SetString(shopping_after,string_format(xorstr("%d Knight Cash"), itemscount * shopping_p));
		}
		else if (pSender == (DWORD*)btn_cancel)
		{
			g_pMain->SetVisible(shopping_confirm,false);
			g_pMain->SetState(btn_confirm,UI_STATE_BUTTON_NORMAL);
			g_pMain->SetState(btn_cancel,UI_STATE_BUTTON_NORMAL);
			itemscount = 1;
			g_pMain->SetString(item_count,string_format("%d", itemscount));
		}
		else if (pSender == (DWORD*)btn_pusrefund) {
			RefundSetItems();
			setBaseItems(0);
		}
		else if (pSender == (DWORD*)btn_reseller)
		{
			if (g_pMain->uiHpMenuPlug != NULL && g_pMain->uiHpMenuPlug->ResellerURL.size())
				ShellExecute(NULL, xorstr("open"), g_pMain->uiHpMenuPlug->ResellerURL.c_str(), NULL, NULL, SW_SHOWNORMAL);
		}
		else
		{
			for (int i = 0; i < btn_tabs.size(); i++)
			{
				if (pSender == (DWORD*)btn_tabs[i])
				{
					for (int j = 0; j < btn_tabs.size(); j++)
						g_pMain->SetState(btn_tabs[j],UI_STATE_BUTTON_NORMAL);

					g_pMain->SetState(btn_tabs[i],UI_STATE_BUTTON_DISABLE);
					page = 1;
					cat = i+1;
					SetItems(page, cat);
				}
			}
			for (int i = 0; i < PUS_ITEMPERCOUNT; i++)
			{
				if (pus_items[i].purchase != NULL)
				{
					if (pSender == (DWORD*)pus_items[i].purchase)
					{
						shopping_id = pus_items[i].sItemID;
						for (int i = 0; i < item_list.size(); i++)
						{
							if (item_list[i].ItemID == shopping_id)
							{
								PusID = item_list[i].ID;
								shopping_p = item_list[i].Price;
								shopping_q = item_list[i].Quantitiy;
								sCodeType = item_list[i].isTlBalance;
								break;
							}
						}
						OpenShopping();
					}
				}
			}

			for (int i = 0; i < PUS_ITEMPERCOUNT; i++)
			{
				if (pus_items[i].add_to != NULL)
				{
					if (pSender == (DWORD*)pus_items[i].add_to)
					{
						shopping_id = pus_items[i].sItemID;
						if (shopping_id != NULL)
						{
							for (int i = 0; i < item_list.size(); i++)
							{
								if (item_list[i].ItemID == shopping_id)
								{
									PurchaseItemAdd(shopping_id, item_list[i].Price, item_list[i].isTlBalance);
									break;
								}
							}
						}
					}
				}
			}
		}
		PurchaseButtonClick((DWORD)pSender);
	}
	return true;
}

void __stdcall UIPusReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->uiPowerUpStore->ReceiveMessage(pSender, dwMsg);
	__asm
	{
		MOV ECX, uiPus
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_puS
		CALL EAX
	}
}

void CUIPowerUpStore::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_puS = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)UIPusReceiveMessage_Hook;
}

void CUIPowerUpStore::OpenShopping()
{
	g_pMain->SetState(shopping_confirm,UI_STATE_BUTTON_NORMAL);

	if (g_pMain->Player.KnightCash < shopping_p)
		g_pMain->SetState(shopping_confirm,UI_STATE_BUTTON_DISABLE);

	g_pMain->SetEditString(gift_edit_name, "");

	TABLE_ITEM_BASIC* tbl = g_pMain->pClientTBLManager->GetItemData(shopping_id);
	if (tbl != nullptr)
	{
		if (TABLE_ITEM_EXT* ext = g_pMain->pClientTBLManager->GetExtData(tbl->extNum, shopping_id))
		{
			DWORD dwIconID = tbl->dxtID;

			if (ext->dxtID > 0)
				dwIconID = ext->dxtID;

			std::string dxt = string_format(xorstr("UI\\ItemIcon_%.1d_%.4d_%.2d_%.1d.dxt"), dwIconID / 10000000u, dwIconID / 1000u % 10000, dwIconID / 10u % 100, dwIconID % 10);
			g_pMain->SetTexImage(shopping_icon, dxt);
		}
		else 
		{
			std::vector<char> buffer(256, NULL);
			sprintf(&buffer[0], "ui\\itemicon_%.1d_%.4d_%.2d_%.1d.dxt",
				(tbl->dxtID / 10000000),
				(tbl->dxtID / 1000) % 10000,
				(tbl->dxtID / 10) % 100,
				tbl->dxtID % 10);

			std::string szIconFN = &buffer[0];
			g_pMain->SetTexImage(shopping_icon, szIconFN);
		}
		g_pMain->SetString(shopping_name,tbl->strName);
		g_pMain->SetString(shopping_price,string_format(xorstr("Price: %s"), g_pMain->StringHelper->NumberFormat(shopping_p).c_str()));
		g_pMain->SetString(shopping_quantity,string_format(xorstr("Quantity: %s"), g_pMain->StringHelper->NumberFormat(itemscount).c_str()));
		g_pMain->SetString(shopping_after,string_format(xorstr("%d Knight Cash"), shopping_p));
	}
	g_pMain->SetVisible(shopping_confirm,true);
}

uint32_t CUIPowerUpStore::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;
	{
		bool showTooltip = false;
		bool pass = false;

		if (g_pMain->IsIn(shopping_confirm,ptCur.x, ptCur.y) && g_pMain->IsVisible(shopping_confirm))
			pass = true;

		if (!pass)
		{
			for (auto it : pus_items)
			{
#if(PUS_ITEMSCOUNT == 4)
				if (it.icon != NULL && it.sItemID > 0 && g_pMain->IsVisible(itemGroupBase[0]) && g_pMain->IsVisible(itemGroupBase[0]) && g_pMain->IsVisible(itemGroupBase[2]) && g_pMain->IsVisible(itemGroupBase[3]))
#else
				if (it.icon != NULL && it.sItemID > 0 && g_pMain->IsVisible(itemGroupBase[0]) && g_pMain->IsVisible(itemGroupBase[1]) && g_pMain->IsVisible(itemGroupBase[2]))
#endif
				{
					if (g_pMain->IsIn(it.icon,ptCur.x, ptCur.y) && g_pMain->IsVisible(it.nBase))
					{
						g_pMain->ShowToolTipEx(it.sItemID, ptCur.x, ptCur.y);
						break;
					}
				}
			}

			for (auto& it : refund_items)
			{
				if (it.icon != NULL && it.sItemID > 0 && g_pMain->IsVisible(it.nBase) && g_pMain->IsIn(it.icon, ptCur.x, ptCur.y))
				{
					g_pMain->ShowToolTipEx(it.sItemID, ptCur.x, ptCur.y);
					break;
				}
			}
		}
	}
	return dwRet;
}

void CUIPowerUpStore::Open()
{
	g_pMain->SetEditString(txt_code, "");
	g_pMain->SetEditString(txt_search, "");
	g_pMain->SetVisible(txt_search, true);

	g_pMain->UIScreenCenter(m_dVTableAddr);
	g_pMain->SetString(txt_cash,g_pMain->StringHelper->NumberFormat(g_pMain->Player.KnightCash).c_str());
	g_pMain->SetString(txt_tlbalance,g_pMain->StringHelper->NumberFormat(g_pMain->Player.TlBalance).c_str());
	SetItems();

	UpdateRefundItemList(g_pMain->pClientHookManager->pusrefund_itemlist);

	for (int i = 0; i < cat_list.size(); i++)
	{
		DWORD tmp = g_pMain->GetChildByID(btn_tabs[i], string_format(xorstr("txt")));
		if (cat_list[i].Status)
		{
			g_pMain->SetString(tmp, cat_list[i].categoryName.c_str());
			g_pMain->SetVisible(btn_tabs[i],true);
		}
		else
			g_pMain->SetVisible(btn_tabs[i],false);
	}
	g_pMain->SetVisible(m_dVTableAddr,true);
}

void CUIPowerUpStore::setBaseItems(int value) {
	return;
	for (int i = 1; i <= PUS_ITEMSCOUNT; i++)
		g_pMain->SetVisible(itemGroupBase[i - 1], (value==1 ? true : false));
}
void CUIPowerUpStore::Close()
{
	g_pMain->SetVisible(m_dVTableAddr,false);
	g_pMain->SetEditString(edit_search, "");
	g_pMain->SetVisible(txt_search, false);
	g_pMain->EditKillFocus(edit_search);
	g_pMain->EditKillFocus(txt_code);
	g_pMain->EditKillFocus(gift_edit_name);
}

void CUIPowerUpStore::UpdateItemList(vector<PUSItem> items, vector<PusCategory> cats)
{
	item_list = items;
	cat_list = cats;
	page = 1;
	pageCount = abs(ceil((double)item_list.size() / (double)PUS_ITEMPERCOUNT));
}

void CUIPowerUpStore::UpdateRefundItemList(vector<PUSREFUNDITEM> refund_items) { refund_list = refund_items; }

bool pus_refund(const PUSREFUNDITEM& a, const PUSREFUNDITEM& b) { return a.expiredtime < b.expiredtime; }

void CUIPowerUpStore::RefundSetItems(bool dontopen) 
{
	vector<PUSREFUNDITEM> tmpList;
	for (int i = 0; i < (int)refund_list.size(); i++)  
		tmpList.push_back(refund_list[i]);

	if (tmpList.empty()) 
	{
		for (int i = 0; i < 18; i++) 
			g_pMain->SetVisible(refund_items[i].nBase,false);
		return;
	}

	std::sort(tmpList.begin(), tmpList.end(), pus_refund);
	int count = 0, size = (int)tmpList.size();

	for (int i = 0; i < 18; i++) 
	{
		if (++count > 18 || size == 0) 
			break;

		if (i >= size) 
		{
			g_pMain->SetVisible(refund_items[i].nBase,false);
			continue;
		}

		TABLE_ITEM_BASIC* tbl = g_pMain->pClientTBLManager->GetItemData(tmpList[i].itemid);
		if (tbl == nullptr) 
		{
			g_pMain->SetVisible(refund_items[i].nBase,false);
			continue;
		}

		g_pMain->SetVisible(basepusrefund, !dontopen);

		if (TABLE_ITEM_EXT* ext = g_pMain->pClientTBLManager->GetExtData(tbl->extNum, tmpList[i].itemid)) 
		{
			DWORD dwIconID = tbl->dxtID;
			if (ext->dxtID > 0) dwIconID = ext->dxtID;
			std::string dxt = string_format(xorstr("UI\\ItemIcon_%.1d_%.4d_%.2d_%.1d.dxt"), dwIconID / 10000000u, dwIconID / 1000u % 10000, dwIconID / 10u % 100, dwIconID % 10);
			g_pMain->SetTexImage(refund_items[i].icon, dxt);
		}
		else
		{
			std::vector<char> buffer(256, NULL);
			sprintf(&buffer[0], "ui\\itemicon_%.1d_%.4d_%.2d_%.1d.dxt", (tbl->dxtID / 10000000), (tbl->dxtID / 1000) % 10000,
				(tbl->dxtID / 10) % 100, tbl->dxtID % 10);

			std::string szIconFN = &buffer[0];
			g_pMain->SetTexImage(refund_items[i].icon, szIconFN);
		}
		
		refund_items[i].sItemID = tmpList[i].itemid;
		refund_items[i].serial = tmpList[i].serial;
		g_pMain->SetString(refund_items[i].name,tbl->strName);
		g_pMain->SetString(refund_items[i].price,string_format(xorstr("Price: %s"), g_pMain->StringHelper->NumberFormat(tmpList[i].kcprice).c_str()));
		g_pMain->SetVisible(refund_items[i].nBase,true);
	}
}

void CUIPowerUpStore::SetItems(int p, int cat, string q)
{
	vector<PUSItem> tmpList;
	for (int i = 0; i < item_list.size(); i++)
	{
		if (q == "")
		{
			if (item_list[i].Category == cat)
				tmpList.push_back(item_list[i]);
		}
		
		else if (q != "")
		{
			TABLE_ITEM_BASIC* tbl = g_pMain->pClientTBLManager->GetItemData(item_list[i].ItemID);
			if (tbl != nullptr)
			{
				if (g_pMain->StringHelper->IsContains(tbl->strName, q))
					tmpList.push_back(item_list[i]);
			}	
		}
		else
			tmpList.push_back(item_list[i]);
	}

	if (tmpList.size() == 0)
	{
		for (int i = 0; i < PUS_ITEMPERCOUNT; i++)
			g_pMain->SetVisible(pus_items[i].nBase,false);

		g_pMain->SetString(txt_page,to_string(page));
		pageCount = 1;
		return;
	}

	std::sort(tmpList.begin(), tmpList.end(), pus_compare);
	pageCount = abs(ceil((double)tmpList.size() / (double)PUS_ITEMPERCOUNT));

	int begin = (p - 1) * PUS_ITEMPERCOUNT;
	int j = -1;
	for (int i = begin; i < begin + PUS_ITEMPERCOUNT; i++)
	{
		j++;
#if (PUS_ITEMPERCOUNT == 16)
		if (j > 15)
			break;
#else
		if (j > 11)
			break;
#endif

		if (i > tmpList.size() - 1)
		{
			g_pMain->SetVisible(pus_items[j].nBase,false);
			continue;
		}

		TABLE_ITEM_BASIC* tbl = g_pMain->pClientTBLManager->GetItemData(tmpList[i].ItemID);
		if (tbl == nullptr)
		{
			g_pMain->SetVisible(pus_items[j].nBase,false);
			continue;
		}

		if (TABLE_ITEM_EXT* ext = g_pMain->pClientTBLManager->GetExtData(tbl->extNum, tmpList[i].ItemID))
		{
			DWORD dwIconID = tbl->dxtID;
			if (ext->dxtID > 0)
				dwIconID = ext->dxtID;

			std::string dxt = string_format(xorstr("UI\\ItemIcon_%.1d_%.4d_%.2d_%.1d.dxt"), dwIconID / 10000000u, dwIconID / 1000u % 10000, dwIconID / 10u % 100, dwIconID % 10);
			g_pMain->SetTexImage(pus_items[j].icon, dxt);
		
		}
		else {
			std::vector<char> buffer(256, NULL);

			sprintf(&buffer[0], "ui\\itemicon_%.1d_%.4d_%.2d_%.1d.dxt",
				(tbl->dxtID / 10000000),
				(tbl->dxtID / 1000) % 10000,
				(tbl->dxtID / 10) % 100,
				tbl->dxtID % 10);

			std::string szIconFN = &buffer[0];
			g_pMain->SetTexImage(pus_items[j].icon, szIconFN);
		}

		pus_items[j].sItemID = tmpList[i].ItemID;
		g_pMain->SetString(pus_items[j].name,tbl->strName);
		g_pMain->SetString(pus_items[j].price,string_format(xorstr("%s %s"), g_pMain->StringHelper->NumberFormat(tmpList[i].Price).c_str(), (tmpList[i].isTlBalance == 1 ? "TL" : "KC")));
		g_pMain->SetString(pus_items[j].quantitiy,string_format(xorstr("%s"), g_pMain->StringHelper->NumberFormat(tmpList[i].Quantitiy).c_str()));

		bool enoughtMoney = true;
		bool enoughtTLBalance = true;
		if (g_pMain->Player.KnightCash < tmpList[i].Price)
			enoughtMoney = false;
		if (g_pMain->Player.TlBalance < tmpList[i].Price)
			enoughtTLBalance = false;
		if (!tmpList[i].isTlBalance)
		{
			if (enoughtMoney)
			{
				g_pMain->SetStringColor(pus_items[j].price,D3DCOLOR_RGBA(255, 0, 0, 255));
				g_pMain->SetState(pus_items[j].purchase,UI_STATE_BUTTON_NORMAL);
			}
			else
			{
				g_pMain->SetStringColor(pus_items[j].price, D3DCOLOR_RGBA(255, 0, 0, 255));	
				g_pMain->SetState(pus_items[j].purchase, UI_STATE_BUTTON_DISABLE);
			}
		}
		else {
			if (enoughtTLBalance)
			{
				g_pMain->SetStringColor(pus_items[j].price, D3DCOLOR_RGBA(255, 0, 0, 255));
				g_pMain->SetState(pus_items[j].purchase, UI_STATE_BUTTON_NORMAL);
			
			}
			else
			{
				g_pMain->SetStringColor(pus_items[j].price, D3DCOLOR_RGBA(255, 0, 0, 255));
				g_pMain->SetState(pus_items[j].purchase, UI_STATE_BUTTON_DISABLE);
			}
		}
		g_pMain->SetVisible(pus_items[j].nBase,true);
	}
	g_pMain->SetString(txt_page,to_string(page));
}

void CUIPowerUpStore::UpdateCash(uint32 kc, uint32 tl)
{
	g_pMain->SetString(txt_cash, g_pMain->StringHelper->NumberFormat(kc).c_str());
	g_pMain->SetString(txt_tlbalance, g_pMain->StringHelper->NumberFormat(tl).c_str());

}

bool pus_compare(const PUSItem& a, const PUSItem& b) { return a.ID < b.ID; }

void CUIPowerUpStore::OpenPowerUpStore() {}

void CUIPowerUpStore::PurchaseItemAdd(uint32 ItemID, uint32 Price, uint8 PriceType)
{
	for (int i = 0; i < 7; i++)
	{
		if (g_pMain->IsVisible(purchase_items[i].nBase))
			continue;

		TABLE_ITEM_BASIC* tbl = g_pMain->pClientTBLManager->GetItemData(ItemID);
		if (tbl != nullptr)
		{
			if (TABLE_ITEM_EXT* ext = g_pMain->pClientTBLManager->GetExtData(tbl->extNum, shopping_id))
			{
				DWORD dwIconID = tbl->dxtID;
				if (ext->dxtID > 0)
					dwIconID = ext->dxtID;

				std::string dxt = string_format(xorstr("UI\\ItemIcon_%.1d_%.4d_%.2d_%.1d.dxt"), dwIconID / 10000000u, dwIconID / 1000u % 10000, dwIconID / 10u % 100, dwIconID % 10);

				g_pMain->SetTexImage(purchase_items[i].icon, dxt);
				g_pMain->SetString(purchase_items[i].name,tbl->strName);
				purchase_items[i].sItemID = ItemID;
				purchase_items[i].sCash = Price;

				std::string ncash = string_format("%s %s", g_pMain->StringHelper->NumberFormat(Price).c_str(), (PriceType == 1 ? "TL" : " KnightCash"));
				g_pMain->SetString(purchase_items[i].price,ncash);
				g_pMain->SetVisible(purchase_items[i].nBase,true);
			}
			else
			{
				std::vector<char> buffer(256, NULL);

				sprintf(&buffer[0], "ui\\itemicon_%.1d_%.4d_%.2d_%.1d.dxt",
					(tbl->dxtID / 10000000),
					(tbl->dxtID / 1000) % 10000,
					(tbl->dxtID / 10) % 100,
					tbl->dxtID % 10);

				std::string szIconFN = &buffer[0];

				g_pMain->SetTexImage(purchase_items[i].icon, szIconFN);
				g_pMain->SetString(purchase_items[i].name, tbl->strName);
				purchase_items[i].sItemID = ItemID;
				purchase_items[i].sCash = Price;

				std::string ncash = string_format("%s %s", g_pMain->StringHelper->NumberFormat(Price).c_str(), (PriceType == 1 ? "TL" : " KnightCash"));
				g_pMain->SetString(purchase_items[i].price,ncash);
				g_pMain->SetVisible(purchase_items[i].nBase,true);
			}
			break;
		}
	}
	PurchaseTotal();
}

void CUIPowerUpStore::PurchaseButtonClick(DWORD pSender)
{
	for (int i = 0; i < 7; i++)
	{
		if (purchase_items[i].cancel != NULL)
		{
			if (pSender ==purchase_items[i].cancel)
			{
				purchase_items[i].sItemID = 0;
				purchase_items[i].sCash = 0;
				g_pMain->SetVisible(purchase_items[i].nBase,false);
				PurchaseTotal();
				break;
			}
		}
	}
}

void CUIPowerUpStore::PurchaseTotal()
{
	uint32 totalprice = 0;

	for (int i = 0; i < 7; i++)
	{
		if (g_pMain->IsVisible(purchase_items[i].nBase))
			totalprice += purchase_items[i].sCash;
	}

	g_pMain->SetString(total_cash,string_format(xorstr("%s"), g_pMain->StringHelper->NumberFormat(totalprice).c_str()));
}

void CUIPowerUpStore::PurchaseDeleteAll()
{
	for (int i = 0; i < 7; i++)
	{
		purchase_items[i].sItemID = 0;
		purchase_items[i].sCash = 0;
		purchase_items[i].type = 0;
		g_pMain->SetVisible(purchase_items[i].nBase,false);
	}

	PurchaseTotal();
}

void CUIPowerUpStore::PurchaseBuyAll()
{
	g_pMain->SetVisible(basket_confirm,false);

	for (int i = 0; i < 7; i++)
	{
		if (g_pMain->IsVisible(purchase_items[i].nBase) && purchase_items[i].sItemID > 0)
		{
			for (int j = 0; j < item_list.size(); j++)
			{
				if (item_list[j].ItemID == purchase_items[i].sItemID)
				{
					Packet pkt(WIZ_HSACS_HOOK);
					pkt << uint8_t(HSACSOpCodes::PUS) << uint8(0x01) << uint32(item_list[j].ID) << uint8(1);
					g_pMain->Send(&pkt);
					break;
				}
			}
		}
	}

	PurchaseDeleteAll();
	PurchaseTotal();
}

void CUIPowerUpStore::BasketMenuOpen()
{
	uint32 totalprice = 0;

	for (int i = 0; i < 7; i++)
	{
		if (g_pMain->IsVisible(purchase_items[i].nBase))
			totalprice += purchase_items[i].sCash;
	}

	g_pMain->SetString(basket_cash,string_format(xorstr("%s Knight Cash"), g_pMain->StringHelper->NumberFormat(totalprice).c_str()));
	g_pMain->SetVisible(basket_confirm,true);
}