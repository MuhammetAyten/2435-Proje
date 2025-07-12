#include "stdafx.h"
#include "hMerchantList.h"
uint8 CurPageCount = 1;
CUIMerchantListPlug::CUIMerchantListPlug()
{
	vector<int>offsets;
	offsets.push_back(0x468);	// co_movie_result satýr : 224
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);
	m_btngiftpus = 0;//14.11.2020 User Sag Tik Pus
	m_btnViewInfo = 0;

	ParseUIElements();
	InitReceiveMessage();
}

CUIMerchantListPlug::~CUIMerchantListPlug()
{
}

void CUIMerchantListPlug::ParseUIElements()
{

	btn_close = g_pMain->GetChildByID(m_dVTableAddr, xorstr("btn_close"));
	btn_search = g_pMain->GetChildByID(m_dVTableAddr, xorstr("btn_search"));
	btn_sort_price = g_pMain->GetChildByID(m_dVTableAddr, xorstr("btn_sort_price"));
	edit_itemname = g_pMain->GetChildByID(m_dVTableAddr, xorstr("edit_itemname"));
	txt_itemname = g_pMain->GetChildByID(m_dVTableAddr, xorstr("txt_itemname"));
	btn_previous = g_pMain->GetChildByID(m_dVTableAddr, xorstr("btn_previous"));
	btn_next = g_pMain->GetChildByID(m_dVTableAddr, xorstr("btn_next"));
	txt_page = g_pMain->GetChildByID(m_dVTableAddr, xorstr("txt_page"));

	for (int i = 0; i < 14; i++)
	{
		mData[i].row = g_pMain->GetChildByID(m_dVTableAddr, string_format(xorstr("row_%d"), i + 1));
		mData[i].txt_seller = g_pMain->GetChildByID(mData[i].row, xorstr("txt_seller"));
		mData[i].txt_item = g_pMain->GetChildByID(mData[i].row, xorstr("txt_item"));
		mData[i].txt_grade = g_pMain->GetChildByID(mData[i].row, xorstr("txt_grade"));
		mData[i].txt_type = g_pMain->GetChildByID(mData[i].row, xorstr("txt_type"));
		mData[i].txt_count = g_pMain->GetChildByID(mData[i].row, xorstr("txt_count"));
		mData[i].txt_price = g_pMain->GetChildByID(mData[i].row, xorstr("txt_price"));
		mData[i].btn_go = g_pMain->GetChildByID(mData[i].row, xorstr("btn_go"));
		mData[i].btn_pm = g_pMain->GetChildByID(mData[i].row, xorstr("btn_pm"));
	}

	page = 1;
	g_pMain->SetString(txt_page, to_string(page));
	ClearList();
	g_pMain->UIScreenCenter(m_dVTableAddr);
}

void CUIMerchantListPlug::ClearList()
{
	for (int i = 0; i < 14; i++)
	{
		g_pMain->SetString(mData[i].txt_seller,"");
		g_pMain->SetString(mData[i].txt_item,"");
		g_pMain->SetString(mData[i].txt_grade,"");
		g_pMain->SetString(mData[i].txt_type,"");
		g_pMain->SetString(mData[i].txt_count,"");
		g_pMain->SetString(mData[i].txt_price,"");
		g_pMain->SetVisible(mData[i].btn_go,false);
		g_pMain->SetVisible(mData[i].btn_pm,false);
	}
}
DWORD uiMerchant;

void CUIMerchantListPlug::SetItem(uint8 _p, string q)
{
	page = _p;
	pageCount = abs(ceil((double)merchantList.size() / (double)14));

	ClearList();

	for (uint32 i = 0; i < 14; i++)
	{
		if (14 * (page - 1) + i > merchantList.size() - 1 || merchantList.size() < i + 1)
			break;

		int j = i + 14 * (page - 1);
		TABLE_ITEM_BASIC* tbl = g_pMain->pClientTBLManager->GetItemData(merchantList[j].nItemID);

		if (tbl != nullptr)
		{
			if (q != "" && g_pMain->StringHelper->IsContains(tbl->strName, q))
			{
				merchantList[j].tbl = tbl;
				g_pMain->SetString(mData[i].txt_seller,merchantList[j].seller);
				g_pMain->SetString(mData[i].txt_item,tbl->strName);

				// Grade algorithm
				TABLE_ITEM_EXT* ext = g_pMain->pClientTBLManager->GetExtData(tbl->extNum, merchantList[j].nItemID);
				if (ext != nullptr)
				{
					if (ext->baseItemID > 0)
						g_pMain->SetString(mData[i].txt_item,ext->szHeader);

					if (ext->extensionID % 10 != 0)
					{
						char szExtID[20];
						sprintf(szExtID, "+%d", ext->extensionID % 10);
						g_pMain->SetString(mData[i].txt_grade,szExtID);
					}
					else
						g_pMain->SetString(mData[i].txt_grade,"-");
				}

				g_pMain->SetString(mData[i].txt_type,merchantList[j].type == 0 ? "Sell" : "Buy");
				g_pMain->SetString(mData[i].txt_count,to_string(merchantList[j].count));

				if (merchantList[j].isKC)
					g_pMain->SetString(mData[i].txt_price,string_format(xorstr("%s knight cash"), g_pMain->StringHelper->NumberFormat(merchantList[j].price).c_str()));
				else
					g_pMain->SetString(mData[i].txt_price,string_format(xorstr("%s coins"), g_pMain->StringHelper->NumberFormat(merchantList[j].price).c_str()));

				g_pMain->SetVisible(mData[i].btn_go,true);
				g_pMain->SetVisible(mData[i].btn_pm,true);
			}
			else if (q == "")
			{
				g_pMain->SetString(mData[i].txt_seller,merchantList[j].seller);
				g_pMain->SetString(mData[i].txt_item,tbl->strName);

				// Grade algorithm
				TABLE_ITEM_EXT* ext = g_pMain->pClientTBLManager->GetExtData(tbl->extNum, merchantList[j].nItemID);
				if (ext != nullptr)
				{
					if (ext->baseItemID > 0)
						g_pMain->SetString(mData[i].txt_item,ext->szHeader);

					if (ext->extensionID % 10 != 0)
					{
						char szExtID[20];
						sprintf(szExtID, "+%d", ext->extensionID % 10);
						g_pMain->SetString(mData[i].txt_grade,szExtID);
					}
					else
						g_pMain->SetString(mData[i].txt_grade,"-");
				}

				g_pMain->SetString(mData[i].txt_type,merchantList[j].type == 0 ? "Sell" : "Buy");
				g_pMain->SetString(mData[i].txt_count,to_string(merchantList[j].count));

				if (merchantList[j].isKC)
					g_pMain->SetString(mData[i].txt_price,string_format(xorstr("%s knight cash"), g_pMain->StringHelper->NumberFormat(merchantList[j].price).c_str()));
				else
					g_pMain->SetString(mData[i].txt_price,string_format(xorstr("%s coins"), g_pMain->StringHelper->NumberFormat(merchantList[j].price).c_str()));

				g_pMain->SetVisible(mData[i].btn_go,true);
				g_pMain->SetVisible(mData[i].btn_pm,true);
			}
		}
	}

	g_pMain->SetString(txt_page,to_string(page));
}
void CUIMerchantListPlug::Open()
{
	g_pMain->SetVisible(m_dVTableAddr, true);
}

void CUIMerchantListPlug::Close()
{
	g_pMain->EditKillFocus(edit_itemname);
	g_pMain->SetVisible(m_dVTableAddr,false);
}

void CUIMerchantListPlug::OpenMerchantList()
{

}
bool MerchantListCompare(const MerchantData& a, const MerchantData& b)
{
	return a.price < b.price;
}

void CUIMerchantListPlug::GO(string name)
{
	Packet result(WIZ_HSACS_HOOK);
	result << uint8_t(HSACSOpCodes::MERCHANTLIST) << uint8(1) << name;
	g_pMain->Send(&result);
}

void CUIMerchantListPlug::PM(string name)
{
	Packet result(WIZ_HSACS_HOOK);
	result << uint8_t(HSACSOpCodes::MERCHANTLIST) << uint8(2) << name;
	g_pMain->Send(&result);
}
DWORD func_merch = 0;
bool CUIMerchantListPlug::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	uiMerchant = m_dVTableAddr;

	if (!pSender || pSender ==(DWORD*) 0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;

	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender ==(DWORD*) btn_close)
			Close();
		else if (pSender ==(DWORD*) btn_search)
			SetItem(1, g_pMain->GetEditString(edit_itemname).c_str());
		else if (pSender ==(DWORD*) btn_sort_price)
		{
			std::sort(merchantList.begin(), merchantList.end(), MerchantListCompare);
			SetItem(1);
		}
		else if (pSender == (DWORD*)btn_next)
		{
			pageCount = abs(ceil((double)merchantList.size() / (double)14));

			if (pageCount > 1)
			{
				if (pageCount >= (CurPageCount + 1))
				{
					CurPageCount++;
					SetItem(CurPageCount, g_pMain->GetEditString(edit_itemname).c_str());
				
				}
			}
		}
		else if (pSender == (DWORD*)btn_previous)
		{
			pageCount = abs(ceil((double)merchantList.size() / (double)14));

			if (CurPageCount > 1)
			{
				if ((CurPageCount - 1) > 0)
				{
					CurPageCount--;
					SetItem(CurPageCount, g_pMain->GetEditString(edit_itemname).c_str());
			
				}
			}
		}

		for (int i = 0; i < 14; i++)
		{
			if (pSender ==(DWORD*) mData[i].btn_go)
				GO(merchantList[i * page].seller);
			else if (pSender ==(DWORD*) mData[i].btn_pm)
				PM(merchantList[i * page].seller);
		}
	}


	return true;
}

void __stdcall UiMerchantReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->uiMerchantList->ReceiveMessage(pSender, dwMsg);

	__asm
	{
		MOV ECX, uiMerchant
		PUSH dwMsg
		PUSH pSender
		MOV EAX, func_merch
		CALL EAX
	}
}

void CUIMerchantListPlug::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	func_merch = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)UiMerchantReceiveMessage_Hook;
}