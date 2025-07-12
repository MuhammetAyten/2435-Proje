#include "stdafx.h"
#include "UIManager.h"
#include "HSACSGui.h"

CUIManager::CUIManager()
{
	s_bKeyPressed = false;
	s_bKeyPress = false;
	IgnoreMouseProc = false;
	pClientLocalInput = new CLocalInput();
	pClientUIBase = new CN3UIBase();
	pClientUIFocused = NULL;


	pClientUIMessageBox = NULL;
	pClientMerchantEye = NULL;
	pClientUICastleSiegeWarFare = NULL;
	pClientUIJuraidMountion = NULL;
	pClientUIPerkMenu = NULL;
	pClientUIGameMasterMode = NULL;
#if (HOOK_SOURCE_VERSION == 2369)
	pClientTargetSkillCheck = NULL;
#endif
	pusrefund_itemlist.clear();
	//pDailyQuestArray.clear();
}

CUIManager::~CUIManager()
{
	s_bKeyPressed = false;
	s_bKeyPress = false;
	IgnoreMouseProc = false;
	pClientLocalInput = NULL;
	pClientUIBase->Release();
	pClientUIFocused = NULL;
	pClientUICastleSiegeWarFare = NULL;
	pClientUIJuraidMountion = NULL;
	pClientMerchantEye = NULL;
	pClientUIMessageBox = NULL;
	pClientUIPerkMenu = NULL;
	pClientUIGameMasterMode = NULL;
#if (HOOK_SOURCE_VERSION == 2369)
	pClientTargetSkillCheck = NULL;
#endif
	Release();
}

void CUIManager::Init(LPDIRECT3DDEVICE9 lDevice)
{
	pClientUIBase->s_lpD3DDev = lDevice;
	char buff[50];
	sprintf_s(buff, "HSACSX Client[%d]", GetCurrentProcessId());
	pClientLocalInput->Init(GetModuleHandle(NULL), FindWindow(NULL, buff), FALSE);
	InitUifHookVtables();
	OpenItemInfo();

}

void CUIManager::AddChild(CN3UIBase* pChild, bool focused)
{
	//SetChildFonts(pChild, xorstr("Verdana"), 2, FALSE, FALSE);
	m_Children.push_front(pChild);

	if (focused)
		SetFocusedUI(pChild);
}

void CUIManager::RemoveChild(CN3UIBase* pChild)
{
	if (NULL == pChild) return;
	m_DestroyQueue.push_front(pChild);
}

void CUIManager::SetChildFonts(CN3UIBase* pChild, std::string fontName, int minusFontSize, bool bold, bool italic)
{
	if (pChild->UIType() == UI_TYPE_STRING)
	{
		CN3UIString* strChild = (CN3UIString*)pChild;
		strChild->SetFont(fontName, strChild->GetFontHeight() - minusFontSize, bold, italic);
	}
	else if (pChild->UIType() == UI_TYPE_LIST)
	{
		CN3UIList* listChild = (CN3UIList*)pChild;
		listChild->SetFont(fontName, listChild->FontHeight() - minusFontSize, bold, italic);
	}

	UIList myChildren = pChild->GetChildren();
	for (UIListItor itor = myChildren.begin(); myChildren.end() != itor; ++itor)
	{
		CN3UIBase* myChild = (*itor);
		SetChildFonts(myChild, fontName, minusFontSize, bold, italic);
	}
}

void CUIManager::SetChildFontsEx(CN3UIBase* pChild, std::string fontName, int fontSize, bool bold, bool italic)
{
	if (pChild->UIType() == UI_TYPE_STRING)
	{
		CN3UIString* strChild = (CN3UIString*)pChild;
		strChild->SetFont(fontName, fontSize, bold, italic);
	}
	else if (pChild->UIType() == UI_TYPE_LIST)
	{
		CN3UIList* listChild = (CN3UIList*)pChild;
		listChild->SetFont(fontName, fontSize, bold, italic);
	}

	UIList myChildren = pChild->GetChildren();
	for (UIListItor itor = myChildren.begin(); myChildren.end() != itor; ++itor)
	{
		CN3UIBase* myChild = (*itor);
		SetChildFontsEx(myChild, fontName, fontSize, bold, italic);
	}
}

bool isCursorInOurUI = false, m_bDoneSomething = false;
void CUIManager::Tick()
{
	pClientLocalInput->Tick();
	POINT currentCursorPos = pClientLocalInput->MouseGetPos();
	int mouseFlag = pClientLocalInput->MouseGetFlag();
	IsCursorInOurUIs(currentCursorPos);
	ProcessUIKeyInput();

	DWORD dwRet = MouseProc(mouseFlag, currentCursorPos, pClientLocalInput->MouseGetPosOld());
	m_bDoneSomething = false;
	if (dwRet != UI_MOUSEPROC_NONE)
		m_bDoneSomething = true;

	TickDestroyQueue();
	TickFocusedUI();

	for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
	{
		CN3UIBase* pChild = (*itor);
		if (!pChild->IsCheckTick() && !pChild->IsVisible())
			continue;

		pChild->Tick();
	}

}

HWND gameWindow = NULL;

uint32_t CUIManager::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	if (gameWindow == NULL) {
		D3DDEVICE_CREATION_PARAMETERS cparams;
		RECT rect;
		pClientUIBase->s_lpD3DDev->GetCreationParameters(&cparams);
		gameWindow = cparams.hFocusWindow;
	}

	if (GetForegroundWindow() != gameWindow) return UI_MOUSEPROC_NONE;

	m_dwMouseFlagsCur = UI_MOUSEPROC_NONE;
	if (!m_bVisible || !m_bEnableOperation) return m_dwMouseFlagsCur;

	for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; )
	{
		CN3UIBase* pChild = (*itor);

		if (pChild->m_pChildUI && pChild->m_pChildUI->IsVisible())
		{
			uint32_t dwRet = pChild->m_pChildUI->MouseProc(dwFlags, ptCur, ptOld);
			if (UI_MOUSEPROC_DONESOMETHING & dwRet)
			{
				pChild->MouseProc(0, ptCur, ptOld);
				m_dwMouseFlagsCur |= (UI_MOUSEPROC_DONESOMETHING | UI_MOUSEPROC_CHILDDONESOMETHING);

				SetFocusedUI(pChild);

				return m_dwMouseFlagsCur;
			}
			else if ((UI_MOUSE_LBCLICK & dwFlags) && (UI_MOUSEPROC_INREGION & dwRet))
			{
				pChild->MouseProc(0, ptCur, ptOld);
				m_dwMouseFlagsCur |= (UI_MOUSEPROC_DIALOGFOCUS);

				SetFocusedUI(pChild);

				return m_dwMouseFlagsCur;
			}
		}

		uint32_t dwChildRet = pChild->MouseProc(dwFlags, ptCur, ptOld);
		if (UI_MOUSEPROC_DONESOMETHING & dwChildRet)
		{
			m_dwMouseFlagsCur |= (UI_MOUSEPROC_DONESOMETHING | UI_MOUSEPROC_CHILDDONESOMETHING);

			SetFocusedUI(pChild);

			return m_dwMouseFlagsCur;
		}
		else if ((UI_MOUSE_LBCLICK & dwFlags) && (UI_MOUSEPROC_INREGION & dwChildRet))
		{
			m_dwMouseFlagsCur |= (UI_MOUSEPROC_DIALOGFOCUS);

			SetFocusedUI(pChild);

			return m_dwMouseFlagsCur;
		}
		else ++itor;

		m_dwMouseFlagsCur |= dwChildRet;
	}

	return m_dwMouseFlagsCur;
}

void CUIManager::TickDestroyQueue()
{
	bool isThisFocused = false;
	for (UIListItor ito = m_DestroyQueue.begin(); m_DestroyQueue.end() != ito; ++ito)
	{
		for (UIListItor itor = m_Children.begin(); m_Children.end() != itor;)
		{
			if ((*itor) == (*ito))
			{
				if ((*itor) == pClientUIFocused)
					isThisFocused = true;

				CN3UIBase* pChild = (*itor);
				m_Children.remove(pChild);
				delete pChild;
				break;
			}
			else ++itor;
		}
	}

	m_DestroyQueue.clear();

	if (isThisFocused)
		SetFocusedUI(GetTopUI(true));
}

void CUIManager::TickFocusedUI()
{
	if (pClientUIFocused != NULL && !pClientUIFocused->IsVisible())
		SetFocusedUI(GetTopUI(true));
}

void CUIManager::Render()
{
	if (g_pMain->m_bLoading == true)
		return;

	PrepareRenderState();

	for (UIListReverseItor itor = m_Children.rbegin(); m_Children.rend() != itor; ++itor)
	{
		CN3UIBase* pChild = (*itor);
		if (!pChild->IsVisible())
			continue;

		pChild->Render();
	}

	RestoreRenderState();
}

void CUIManager::IsCursorInOurUIs(POINT currentCursorPos)
{
	isCursorInOurUI = false;

	for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
	{
		CN3UIBase* pChild = (*itor);
		if (!pChild->IsVisible())
			continue;

		if (pChild->IsIn(currentCursorPos.x, currentCursorPos.y))
		{
			isCursorInOurUI = true;
			break;
		}
	}
}

void CUIManager::SetFocusedUI(CN3UIBase* pUI)
{
	if (NULL == pUI)
	{
		pClientUIFocused = NULL;
		return;
	}

	UIListItor it = m_Children.begin(), itEnd = m_Children.end();
	it = m_Children.begin();
	bool isChild = false;
	for (; it != itEnd; it++)
	{
		if (pUI == *it)
		{
			isChild = true;
			break;
		}
	}

	if (it == itEnd || !isChild)
		return;

	it = m_Children.erase(it);
	m_Children.push_front(pUI);
	ReorderChildList();

	pClientUIFocused = GetTopUI(true);
}

void CUIManager::ReorderChildList()
{
	int iChildCount = m_Children.size();
	if (iChildCount <= 0) return;
	CN3UIBase** ppBuffer = new CN3UIBase * [iChildCount];
	int iAlwaysTopChildCount = 0;

	for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; )
	{
		CN3UIBase* pChild = (*itor);
		if (pChild->GetStyle() & UISTYLE_ALWAYSTOP)
		{
			itor = m_Children.erase(itor);
			ppBuffer[iAlwaysTopChildCount++] = pChild;
		}
		else ++itor;
	}
	int i;
	for (i = iAlwaysTopChildCount - 1; i >= 0; --i)
	{
		m_Children.push_front(ppBuffer[i]);
	}
	delete[] ppBuffer;
}

CN3UIBase* CUIManager::GetTopUI(bool bVisible)
{
	if (!bVisible)
	{
		if (m_Children.empty())
			return NULL;

		return *(m_Children.begin());
	}

	for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
	{
		CN3UIBase* pUI = (*itor);

		if (pUI->IsVisible())
			return pUI;
	}

	return NULL;
}

void CUIManager::HideAllUI()
{
	if (pClientUIFocused != NULL)
	{
		pClientUIFocused->m_bIsThisFocused = true;
		SetFocusedUI(NULL);
	}


	for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
	{
		CN3UIBase* pChild = (*itor);
		if (!pChild->IsVisible())
			continue;

		pChild->m_bNeedToRestore = true;
		pChild->SetVisible(false);
	}

	if (g_pMain->uiHpMenuPlug != NULL)
		g_pMain->SetVisible(g_pMain->uiHpMenuPlug->m_dVTableAddr, false);
}

void CUIManager::ShowAllUI()
{
	for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
	{
		CN3UIBase* pChild = (*itor);

		if (pChild->m_bNeedToRestore)
		{
			pChild->SetVisible(true);
			pChild->m_bNeedToRestore = false;
		}
	}

	if (g_pMain->pClientCollection != NULL && g_pMain->m_cCollettionStatus)
		g_pMain->SetVisible(g_pMain->pClientCollection->m_dVTableAddr,true);

	if (g_pMain->pClientLottyEvent != NULL && g_pMain->pLotteryEvent.sRemainingTime > 0)
		g_pMain->SetVisible(g_pMain->pClientLottyEvent->m_dVTableAddr, true);	

	if (g_pMain->uiCindirella != NULL && g_pMain->uiCindirella->remainingTime)
		g_pMain->SetVisible(g_pMain->uiCindirella->m_dVTableAddr, true);

	if (g_pMain->pClientTopRightNewPlug != NULL)
		g_pMain->SetVisible(g_pMain->pClientTopRightNewPlug->m_dVTableAddr, true);

	for (UIListItor itor = m_Children.begin(); m_Children.end() != itor; ++itor)
	{
		CN3UIBase* pChild = (*itor);

		if (pChild->m_bIsThisFocused)
		{
			SetFocusedUI(pChild);
			break;
		}
	}
}

void CUIManager::PrepareRenderState()
{
	pClientUIBase->s_lpD3DDev->GetRenderState(D3DRS_ZENABLE, &dwZEnable);
	pClientUIBase->s_lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlphaBlend);
	pClientUIBase->s_lpD3DDev->GetRenderState(D3DRS_SRCBLEND, &dwSrcBlend);
	pClientUIBase->s_lpD3DDev->GetRenderState(D3DRS_DESTBLEND, &dwDestBlend);
	pClientUIBase->s_lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);

	pClientUIBase->s_lpD3DDev->GetSamplerState(0, D3DSAMP_MAGFILTER, &dwMagFilter);
	pClientUIBase->s_lpD3DDev->GetSamplerState(0, D3DSAMP_MINFILTER, &dwMinFilter);
	pClientUIBase->s_lpD3DDev->GetSamplerState(0, D3DSAMP_MIPFILTER, &dwMipFilter);

	if (D3DZB_FALSE != dwZEnable) pClientUIBase->s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	if (TRUE != dwAlphaBlend) pClientUIBase->s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	if (D3DBLEND_SRCALPHA != dwSrcBlend) pClientUIBase->s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	if (D3DBLEND_INVSRCALPHA != dwDestBlend) pClientUIBase->s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	if (FALSE != dwFog) pClientUIBase->s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	if (D3DTEXF_POINT != dwMagFilter) pClientUIBase->s_lpD3DDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	if (D3DTEXF_POINT != dwMinFilter) pClientUIBase->s_lpD3DDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	if (D3DTEXF_NONE != dwMipFilter) pClientUIBase->s_lpD3DDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CUIManager::RestoreRenderState()
{
	if (D3DZB_FALSE != dwZEnable) pClientUIBase->s_lpD3DDev->SetRenderState(D3DRS_ZENABLE, dwZEnable);
	if (TRUE != dwAlphaBlend) pClientUIBase->s_lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlphaBlend);
	if (D3DBLEND_SRCALPHA != dwSrcBlend) pClientUIBase->s_lpD3DDev->SetRenderState(D3DRS_SRCBLEND, dwSrcBlend);
	if (D3DBLEND_INVSRCALPHA != dwDestBlend) pClientUIBase->s_lpD3DDev->SetRenderState(D3DRS_DESTBLEND, dwDestBlend);
	if (FALSE != dwFog) pClientUIBase->s_lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	if (D3DTEXF_POINT != dwMagFilter) pClientUIBase->s_lpD3DDev->SetSamplerState(0, D3DSAMP_MAGFILTER, dwMagFilter);
	if (D3DTEXF_POINT != dwMinFilter) pClientUIBase->s_lpD3DDev->SetSamplerState(0, D3DSAMP_MINFILTER, dwMinFilter);
	if (D3DTEXF_NONE != dwMipFilter) pClientUIBase->s_lpD3DDev->SetSamplerState(0, D3DSAMP_MIPFILTER, dwMipFilter);
}

DWORD uiMgrVTable;
void CUIManager::InitUifHookVtables()
{
	CalcVtable();
	InitProcesspClientLocalInput();
	InitMouseProc();
	InitUIHideAll();
	InitUIShowAll();
	StaticMemberInit();
}

void CUIManager::CalcVtable()
{
	m_dVTableAddr = *(DWORD*)0xF368E8; // 2228

	uiMgrVTable = m_dVTableAddr;
}

void CUIManager::StaticMemberInit()
{
	D3DDEVICE_CREATION_PARAMETERS cparams;
	CN3UIBase::s_lpD3DDev->GetCreationParameters(&cparams);
	CN3Base::s_hWndBase = cparams.hFocusWindow;

	RECT rc;
	::GetClientRect(CN3Base::s_hWndBase, &rc);
	RECT rcTmp = rc; rcTmp.left = (rc.right - rc.left) / 2; rcTmp.bottom = rcTmp.top + 30;
	CN3UIEdit::CreateEditWindow(s_hWndBase, rcTmp);
	pClientLocalInput->CreateMouseWheelWindow(s_hWndBase, rcTmp);
}

void CUIManager::ProcessUIKeyInput()
{
	s_bKeyPressed = false;

	if (pClientUIFocused != NULL && pClientUIFocused && pClientUIFocused->IsVisible())
	{
		for (int i = 0; i < NUMDIKEYS; i++)
		{
			if (pClientLocalInput->IsKeyPress(i))
			{
				if (pClientUIFocused->m_pChildUI && pClientUIFocused->m_pChildUI->IsVisible())
					s_bKeyPress |= pClientUIFocused->m_pChildUI->OnKeyPress(i);
				else s_bKeyPress |= pClientUIFocused->OnKeyPress(i);
			}

			if (pClientLocalInput->IsKeyPressed(i))
			{
				if (pClientUIFocused->m_pChildUI && pClientUIFocused->m_pChildUI->IsVisible())
					s_bKeyPressed |= pClientUIFocused->m_pChildUI->OnKeyPressed(i);
				else s_bKeyPressed |= pClientUIFocused->OnKeyPressed(i);
			}
		}
	}

	if (s_bKeyPress)
	{
		for (int i = 0; i < NUMDIKEYS; i++)
		{
			if (pClientLocalInput->IsKeyPressed(i))
			{
				if (!s_bKeyPressed) s_bKeyPress = false;
				break;
			}
		}
	}
}

RECT CUIManager::GetScreenRect()
{
	D3DDEVICE_CREATION_PARAMETERS cparams;
	RECT rect;
	CN3UIBase::s_lpD3DDev->GetCreationParameters(&cparams);
	GetWindowRect(cparams.hFocusWindow, &rect);

	IDirect3DSwapChain9* swapChain;

	CN3UIBase::s_lpD3DDev->GetSwapChain(0, &swapChain);
	D3DPRESENT_PARAMETERS params;
	swapChain->GetPresentParameters(&params);

	if (params.Windowed) rect.bottom -= 40;
	else rect.bottom += 120;

	return rect;
}

POINT CUIManager::GetScreenCenter(CN3UIBase* ui)
{
	D3DDEVICE_CREATION_PARAMETERS cparams;
	RECT rect;
	CN3UIBase::s_lpD3DDev->GetCreationParameters(&cparams);
	GetWindowRect(cparams.hFocusWindow, &rect);

	POINT ret;
	ret.x = (rect.right / 2) - (ui->GetWidth() / 2);
	ret.y = (rect.bottom / 2) - (ui->GetHeight() / 2);

	return ret;
}

const	DWORD	KO_PROCESS_LOCAL_INPUT_CALL_ADDR = 0x005541E5; // 2228
const	DWORD	KO_PROCESS_LOCAL_INPUT_FUNC = 0x005503B0; // 2228

void __stdcall ProcesspClientLocalInput_Hook(uint32_t dwMouseFlags)
{
	DWORD thisPtr;
	__asm
	{
		MOV thisPtr, ECX
	}

	CN3UIEdit* focusedEdit = CN3UIEdit::GetFocusedEdit();
	if (focusedEdit != NULL)
	{
		if (!m_bDoneSomething)
			focusedEdit->KillFocus();
		return;
	}

	if (dwMouseFlags != 0x0 && m_bDoneSomething)
		dwMouseFlags = 0x0;

	__asm
	{
		MOV ECX, thisPtr
		PUSH dwMouseFlags
		MOV EAX, KO_PROCESS_LOCAL_INPUT_FUNC
		CALL EAX
	}
}

void CUIManager::InitProcesspClientLocalInput()
{
	g_pMain->InitCallHook(KO_PROCESS_LOCAL_INPUT_CALL_ADDR, (DWORD)ProcesspClientLocalInput_Hook);
}

const POINT CUIManager::MouseGetPos(const POINT& ptCur)
{
	POINT cur;
	if (isCursorInOurUI)
		cur.x = cur.y = 0;
	else
		cur = ptCur;

	return cur;
}

void __stdcall MouseProc_Hook(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	if ((dwFlags != 0x0 && isCursorInOurUI) || g_pMain->pClientHookManager != NULL && g_pMain->pClientHookManager->pClientUIMessageBox != NULL)
		dwFlags = 0x0;

	g_pMain->lastMousePos = ptCur;

	if (g_pMain->uiPowerUpStore != NULL)
	{
		bool showTooltip = false;
		if (g_pMain->IsVisible(g_pMain->uiPowerUpStore->m_dVTableAddr))
			g_pMain->uiPowerUpStore->MouseProc(dwFlags, ptCur, ptOld);
	}

	if (g_pMain->m_cCollettionStatus && g_pMain->pClientCollection != NULL)
	{
		if (g_pMain->IsVisible(g_pMain->pClientCollection->m_dVTableAddr))
			g_pMain->pClientCollection->MouseProc(dwFlags, ptCur, ptOld);
	}

	if (g_pMain->pClientLottyEvent != NULL)
	{
		if (g_pMain->IsVisible(g_pMain->pClientLottyEvent->m_dVTableAddr))
			g_pMain->pClientLottyEvent->MouseProc(dwFlags, ptCur, ptOld);
	}

	if (g_pMain->uiQuestPage != NULL)
	{
		if (g_pMain->IsVisible(g_pMain->uiQuestPage->m_dVTableAddr))
			g_pMain->uiQuestPage->MouseProc(dwFlags, ptCur, ptOld);
	}

	if (g_pMain->pClientDropResult != NULL)
	{
		if (g_pMain->IsVisible(g_pMain->pClientDropResult->m_dVTableAddr))
			g_pMain->pClientDropResult->MouseProc(dwFlags, ptCur, ptOld);
	}

	if (g_pMain->uiSearchMonster != NULL)
	{
		if (g_pMain->IsVisible(g_pMain->uiSearchMonster->m_dVTableAddr))
			g_pMain->uiSearchMonster->MouseProc(dwFlags, ptCur, ptOld);
	}

	if (g_pMain->uiTagChange != NULL)
	{
		if (g_pMain->IsVisible(g_pMain->uiTagChange->m_dVTableAddr))
			g_pMain->uiTagChange->MouseProc(dwFlags, ptCur, ptOld);
	}

	if (g_pMain->pClientInventory != NULL)
	{
		if (g_pMain->IsVisible(g_pMain->pClientInventory->m_dVTableAddr))
			g_pMain->pClientInventory->MouseProc(dwFlags, ptCur, ptOld);
	}
#if(HOOK_SOURCE_VERSION == 2369)
	if (g_pMain->pClientRightExchange)
	{
		if (g_pMain->IsVisible(g_pMain->pClientRightExchange->el_Base))
			g_pMain->pClientRightExchange->MouseProc(dwFlags, ptCur, ptOld);
	}

	if (g_pMain->pClientDailyReward)
	{
		if (g_pMain->IsVisible(g_pMain->pClientDailyReward->m_dVTableAddr))
			g_pMain->pClientDailyReward->MouseProc(dwFlags, ptCur, ptOld);
	}
#else
	if (g_pMain->pClientRightExchange)
	{
		if (g_pMain->IsVisible(g_pMain->pClientRightExchange->el_Base))
			g_pMain->pClientRightExchange->MouseProc(dwFlags, ptCur, ptOld);
	}

	if (g_pMain->pClientDailyReward)
	{
		if (g_pMain->IsVisible(g_pMain->pClientDailyReward->m_dVTableAddr))
			g_pMain->pClientDailyReward->MouseProc(dwFlags, ptCur, ptOld);
	}
#endif
	if (g_pMain->pClientHookManager->pClientMerchantEye != NULL && g_pMain->pClientHookManager->pClientMerchantEye->IsVisible() && dwFlags == 4 && !g_pMain->pClientHookManager->pClientMerchantEye->IsIn(ptCur.x, ptCur.y))
		g_pMain->pClientHookManager->pClientMerchantEye->search_merchant->KillFocus();

	if (uiMgrVTable == 0)
	{
		DWORD thisvTable;
		__asm
		{
			MOV thisvTable, ECX

			MOV ECX, thisvTable
			PUSH ptOld
			PUSH ptCur
			PUSH dwFlags
			CALL KO_IU_MGR_MOUSE_PROC_FUNC
		}

		return;
	}

	__asm
	{
		MOV ECX, uiMgrVTable
		PUSH ptOld
		PUSH ptCur
		PUSH dwFlags
		CALL KO_IU_MGR_MOUSE_PROC_FUNC
	}
}

void CUIManager::SendMouseProc(uint32 dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	MouseProc_Hook(dwFlags, ptCur, ptOld);
}

void CUIManager::InitMouseProc()
{
	*(DWORD*)KO_IU_MGR_MOUSE_PROC_PTR = (DWORD)MouseProc_Hook;
}

void __stdcall UIHideAll_Hook()
{
	DWORD thisPtr;
	__asm
	{
		MOV thisPtr, ECX
	}

	g_pMain->pClientHookManager->HideAllUI();

	__asm
	{
		MOV ECX, thisPtr
		MOV EAX, KO_UI_HIDE_ALL_FUNC
		CALL EAX
	}
}

void CUIManager::InitUIHideAll()
{
	g_pMain->InitCallHook(KO_UI_HIDE_ALL_CALL_ADDR, (DWORD)UIHideAll_Hook);
}

void __stdcall UIShowAll_Hook()
{
	DWORD thisPtr;
	__asm
	{
		MOV thisPtr, ECX
	}

	g_pMain->pClientHookManager->ShowAllUI();

	__asm
	{
		MOV ECX, thisPtr
		MOV EAX, KO_UI_SHOW_ALL_FUNC
		CALL EAX
	}
}

void CUIManager::InitUIShowAll()
{
	g_pMain->InitCallHook(KO_UI_SHOW_ALL_CALL_ADDR, (DWORD)UIShowAll_Hook);
}

void CUIManager::ShowMessageBox(string title, string text, MsgBoxTypes type, ParentTypes parent)
{
	if (g_pMain->pClientHookManager == NULL)
		return;

	if (pClientUIMessageBox == NULL) 
		g_pMain->pClientHookManager->pClientUIMessageBox->OpenMessageBox(type, parent);
	else 
		pClientUIMessageBox->Update(type, parent);

	if (pClientUIMessageBox != NULL)
	{
		pClientUIMessageBox->SetTitle(title);
		pClientUIMessageBox->SetMessage(text);
		pClientUIMessageBox->SetVisible(true);
		SetFocusedUI(pClientUIMessageBox);
	}
}

void CUIManager::ShowDropList(Packet& pkt)
{
	uint16 mob;

	uint8 subcode;
	pkt >> subcode;

	if (subcode == 1)
	{
		vector<DropItem> drops;
		pkt >> mob;

		uint32 itemSize = pkt.read<uint32>();
		for (uint32 i = 0; i < itemSize; i++) {
#if (HOOK_SOURCE_VERSION == 2369)
			uint32 nItemID = 0;
			uint16 sPercent = 0;
			pkt >> nItemID >> sPercent;
			drops.push_back(DropItem(nItemID, sPercent));
#else
			uint32 nItemID = 0;
			pkt >> nItemID;
			drops.push_back(DropItem(nItemID, 1));
#endif
		}
		uint8 iasMonster;
		pkt >> iasMonster;

		if (iasMonster != 0 && iasMonster != 1 && iasMonster != 2) iasMonster = 0;

		g_pMain->uiSearchMonster->ListDrop(mob, drops);
		g_pMain->uiSearchMonster->Open();
	}
}

void CUIManager::ShowDropResult()
{
	if (g_pMain->pClientDropResult)
		g_pMain->pClientDropResult->Open();
}

void CUIManager::ShowMerchantList(Packet& pkt)
{
	if (g_pMain->m_PlayerBase == NULL)
		return;



	vector<MerchantData> merchantList;

	uint32 merchantCount;
	pkt >> merchantCount;
	for (int i = 0; i < merchantCount; i++)
	{
		MerchantData data;
		uint32 merchantID;
		uint16 socket;
		string seller;
		uint32 itemID, price;
		uint16 count;
		uint8 isKC;
		uint8 type;
		float x, y, z;
		pkt >> socket >> merchantID >> seller >> type >> itemID >> count >> price >> isKC >> x >> y >> z;
		data.type = type;
		data.socketID = socket;
		data.merchantID = merchantID;
		data.seller = seller;
		data.nItemID = itemID;
		data.price = price;
		data.count = count;
		data.isKC = isKC == 1 ? true : false;
		data.x = x;
		data.y = y;
		data.z = z;
		data.tbl = nullptr;
		merchantList.push_back(data);
	}
	g_pMain->uiMerchantList->merchantList = merchantList;
	g_pMain->uiMerchantList->SetItem(1);
	g_pMain->uiMerchantList->Open();
}

void CUIManager::OpenMonsterSearch()
{
	if (g_pMain->uiSearchMonster != NULL)
		g_pMain->uiSearchMonster->Open();
}

void CUIManager::OpenScheduler()
{
	//if (g_pMain->pClientHookManager->pClientEvetShowList == NULL)
	//	g_pMain->pClientHookManager->pClientEvetShowList->OpenScheduler();
}

void CUIManager::OpenDailyQuest() {

	if (g_pMain->uiQuestPage)g_pMain->SetVisible(g_pMain->uiQuestPage->m_dVTableAddr, true);
}

void CUIManager::OpenItemInfo()
{

}

void CUIManager::OpenMerchantNotice()
{

}

void HSACSEngine::OpenjuraidScreen(bool update, uint16 karus, uint16 elmo, uint16 remtime)
{
	if (!pClientHookManager)
		return;

	if (!pClientHookManager->pClientUIJuraidMountion) {
		std::string name = g_pMain->dcpUIF(xorstr("HSACSX\\UI\\re_war_juraid_enter.hsacsx"));
		g_pMain->pClientHookManager->pClientUIJuraidMountion = new CUIJuraidMountain();
		g_pMain->pClientHookManager->pClientUIJuraidMountion->Init(g_pMain->pClientHookManager);
		g_pMain->pClientHookManager->pClientUIJuraidMountion->LoadFromFile(g_pMain->m_BasePath + name.c_str(), N3FORMAT_VER_1068);
		g_pMain->pClientHookManager->AddChild(g_pMain->pClientHookManager->pClientUIJuraidMountion);
		remove(name.c_str());
	}

	if (pClientHookManager->pClientUIJuraidMountion)
		pClientHookManager->pClientUIJuraidMountion->setOptions(update, remtime, karus, elmo);
}