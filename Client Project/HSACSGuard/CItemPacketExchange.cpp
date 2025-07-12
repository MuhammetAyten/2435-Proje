#include "CItemPacketExchange.h"

CItemPacketExchange::CItemPacketExchange()
{
	grp_selected = base_item = NULL;
	txt_title = NULL;
	area_slot = NULL;
	btn_close = NULL;
}

bool CItemPacketExchange::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false) return false;

	std::string find = xorstr("grp_selected");
	grp_selected = (CN3UIBase*)GetChildByID(find);
	find = xorstr("txt_title");
	txt_title = (CN3UIString*)GetChildByID(find);
	find = xorstr("btn_close");
	btn_close = (CN3UIButton*)GetChildByID(find);
	find = xorstr("base_item");
	base_item = (CN3UIBase*)GetChildByID(find);
	find = xorstr("img_slotbg");
	area_slot = (CN3UIArea*)base_item->GetChildByID(find);

	m_sHandlePackets.icon = new CN3UIIcon();
	m_sHandlePackets.icon->Init(this);
	m_sHandlePackets.icon->SetUIType(UI_TYPE_ICON);
	m_sHandlePackets.icon->SetStyle(UISTYLE_ICON_ITEM | UISTYLE_ICON_CERTIFICATION_NEED);
	m_sHandlePackets.icon->SetUVRect(0, 0, (float)45.0f / (float)64.0f, (float)45.0f / (float)64.0f);
	m_sHandlePackets.icon->SetRegion(base_item->GetRegion());

	/*std::string find = xorstr("btn_exit");
	btn_close = (CN3UIButton*)GetChildByID(find);
	find = xorstr("btn_ok");
	btn_ok = (CN3UIButton*)GetChildByID(find);
	find = xorstr("Text_0");
	Text_0 = (CN3UIString*)GetChildByID(find);
	find = xorstr("Base_Exchange");
	base[0] = (CN3UIBase*)GetChildByID(find); __ASSERT(base[0], "NULL Pointer");
	find = xorstr("Base_RewardItems");
	base[1] = (CN3UIBase*)GetChildByID(find);

	find = xorstr("Unit_0");
	Unit_0 = (CN3UIBase*)base[0]->GetChildByID(find);

	find = xorstr("ItemSlot");
	ItemSlot = (CN3UIArea*)Unit_0->GetChildByID(find);

	m_sHandlePackets.icon = new CN3UIIcon();
	m_sHandlePackets.icon->Init(this);
	m_sHandlePackets.icon->SetUIType(UI_TYPE_ICON);
	m_sHandlePackets.icon->SetStyle(UISTYLE_ICON_ITEM | UISTYLE_ICON_CERTIFICATION_NEED);
	m_sHandlePackets.icon->SetUVRect(0, 0, (float)45.0f / (float)64.0f, (float)45.0f / (float)64.0f);
	m_sHandlePackets.icon->SetRegion(Unit_0->GetRegion());

	float fUVAspect = (float)45.0f / (float)64.0f;
	for (int i = 0; i < 10; i++)
	{
		find = string_format(xorstr("Select_%d"), i);
		Select[i] = (CN3UIBase*)base[1]->GetChildByID(find);

		find = xorstr("ItemSlot");
		CN3UIArea* area = (CN3UIArea*)Select[i]->GetChildByID(find);

		CN3UIIcon* icon = new CN3UIIcon();
		icon->Init(this);
		icon->SetUIType(UI_TYPE_ICON);
		icon->SetStyle(UISTYLE_ICON_ITEM | UISTYLE_ICON_CERTIFICATION_NEED);
		icon->SetUVRect(0, 0, fUVAspect, fUVAspect);
		icon->SetRegion(Select[i]->GetRegion());

		ItemPacket* inf = new ItemPacket();
		inf->icon = icon;
		inf->tbl = NULL;
		inf->nItemID = 0;

		p_sItemPacket.push_back(inf);
	}*/
	SetPos(g_pMain->pClientHookManager->GetScreenCenter(this).x, g_pMain->pClientHookManager->GetScreenCenter(this).y);
	RECT rc;
	SetRect(&rc, m_rcRegion.left, m_rcRegion.top, m_rcRegion.left + GetWidth(), m_rcRegion.top + GetHeight());
	SetRegion(rc);
	SetMoveRect(GetRegion());
	Close();
	return true;
}

bool CItemPacketExchange::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	printf("dwMsg %d\n", dwMsg);
	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == btn_close)
			Close();
		else
		{
			if (pSender == (CN3UIBase*)area_slot)
				printf("img_slotbg\n");
		}
	}
	return true;
}

bool CItemPacketExchange::OnKeyPress(int iKey)
{
	if (!IsVisible()
		|| g_pMain->pClientHookManager->pClientUIFocused != this)
		return CN3UIBase::OnKeyPress(iKey);

	switch (iKey)
	{
	case DIK_ESCAPE:
		ReceiveMessage(btn_close, UIMSG_BUTTON_CLICK);
		return true;
	}

	return CN3UIBase::OnKeyPress(iKey);
}

uint32_t CItemPacketExchange::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;

	if (g_pMain->pClientHookManager->pClientUIFocused == this)
	{
		bool showTooltip = false;
		if (m_sHandlePackets.icon != NULL) 
		{
			if (m_sHandlePackets.icon->IsIn(ptCur.x, ptCur.y) && m_sHandlePackets.tbl != nullptr && m_sHandlePackets.itemID != 0)
			{
				g_pMain->ShowToolTipEx(m_sHandlePackets.itemID, ptCur.x, ptCur.y);
				showTooltip = true;
			}
		}
	}

	dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
	return dwRet;
}

void CItemPacketExchange::Close() { SetVisible(false); }

void CItemPacketExchange::Open() { SetVisible(true);	g_pMain->pClientHookManager->SetFocusedUI(this); }

uint32 getItemOriginIDExGs(uint32 sID) { return std::atoi((std::to_string(sID).substr(0, 6) + xorstr("000")).c_str()); }

//void CItemPacketExchange::Update(uint8 sSlot, uint32 ItemID, vector<uint32> p_sItems)
void CItemPacketExchange::Update(uint32 ItemID)
{
	m_sHandlePackets.itemID = ItemID;
	//m_sHandlePackets.sSlot = sSlot;
	printf("ARRAY %d\n", ItemID);
	TABLE_ITEM_BASIC* tbl = g_pMain->pClientTBLManager->GetItemData(ItemID);
	if (tbl != nullptr)
	{
		m_sHandlePackets.tbl = tbl;
		std::vector<char> buffer(256, NULL);

		sprintf(&buffer[0], "ui\\itemicon_%.1d_%.4d_%.2d_%.1d.dxt",
			(tbl->dxtID / 10000000),
			(tbl->dxtID / 1000) % 10000,
			(tbl->dxtID / 10) % 100,
			tbl->dxtID % 10);

		std::string szIconFN = &buffer[0];

		SRC dxt = hdrReader->GetSRC(szIconFN);

		if (dxt.sizeInBytes == 0) {
			string iconID = to_string(tbl->Num);
			szIconFN = "itemicon_" + iconID.substr(0, 1) + "_" + iconID.substr(1, 4) + "_" + iconID.substr(5, 2) + "_" + iconID.substr(7, 1) + ".dxt";
		}
		m_sHandlePackets.icon->SetTex(szIconFN);
		txt_title->SetString(string_format(xorstr("%s"), tbl->strName.c_str()));
	}

	/*int i = 0;
	for (auto it : p_sItemPacket)
	{
		it->nItemID = p_sItems[i];
		TABLE_ITEM_BASIC* tbl = g_pMain->pClientTBLManager->GetItemData(getItemOriginIDExGs(p_sItems[i]));
		if (tbl == nullptr)
		{
			it->icon->SetTex(xorstr("UI\\itemicon_noimage.dxt"));
			it->tbl = nullptr;
		}
		else {
			std::vector<char> buffer(256, NULL);

			sprintf(&buffer[0], "ui\\itemicon_%.1d_%.4d_%.2d_%.1d.dxt",
				(tbl->dxtID / 10000000),
				(tbl->dxtID / 1000) % 10000,
				(tbl->dxtID / 10) % 100,
				tbl->dxtID % 10);

			std::string szIconFN = &buffer[0];

			SRC dxt = hdrReader->GetSRC(szIconFN);

			if (dxt.sizeInBytes == 0) {
				string iconID = to_string(tbl->Num);
				szIconFN = "itemicon_" + iconID.substr(0, 1) + "_" + iconID.substr(1, 4) + "_" + iconID.substr(5, 2) + "_" + iconID.substr(7, 1) + ".dxt";
			}
			it->icon->SetTex(szIconFN);
			it->tbl = tbl;
		}
		i++;
	}*/
}