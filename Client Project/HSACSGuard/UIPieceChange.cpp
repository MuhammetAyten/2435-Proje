#include "UIPieceChange.h"

CUIPieceChangePlug::CUIPieceChangePlug()
{
	vector<int>offsets;
	offsets.push_back(0x378);
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);

	btn_bank = btnSell = btn_down = btn_up = 0;
	m_sell = m_bank = false;

	m_btnStart = 0;
	m_btnClose = 0;
	m_btnStop = 0;
	gemcount = 0;
	m_textExCount = 0;

	memset(m_slots, 0, sizeof(m_slots));

	n_control = false;
	m_nObjectID = 0;
	m_nExchangeItemID = 0;
	m_nExchangeRobItemSlot = 0;

	ParseUIElements();
	InitReceiveMessage();
}

CUIPieceChangePlug::~CUIPieceChangePlug()
{
}

void CUIPieceChangePlug::ParseUIElements()
{
	btn_bank = g_pMain->GetChildByID(m_dVTableAddr, "btn_bank");
	btnSell = g_pMain->GetChildByID(m_dVTableAddr, "btn_sell");
	btn_up = g_pMain->GetChildByID(m_dVTableAddr, "btn_plus");
	btn_down = g_pMain->GetChildByID(m_dVTableAddr, "btn_down");
	m_btnStart = g_pMain->GetChildByID(m_dVTableAddr, "btn_start");
	m_btnStop = g_pMain->GetChildByID(m_dVTableAddr, "btn_stop");
	m_btnClose = g_pMain->GetChildByID(m_dVTableAddr, "btn_close");
	m_textExCount = g_pMain->GetChildByID(m_dVTableAddr, "text_count");
}

DWORD pieceChangeVTable;
bool pieceChangeStopImmediate;
DWORD Func_PieceChange = 0;
void __stdcall PieceChangeReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	g_pMain->uiPieceChangePlug->ReceiveMessage(pSender, dwMsg);

	__asm
	{
		MOV ECX, pieceChangeVTable
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_PieceChange
		CALL EAX
	}

	if (pieceChangeStopImmediate)
	{
		pieceChangeStopImmediate = false;
		PieceChangeReceiveMessage_Hook((DWORD*)g_pMain->uiPieceChangePlug->m_btnStop, UIMSG_BUTTON_CLICK);
	}
}

void CUIPieceChangePlug::ClickStop()
{
	PieceChangeReceiveMessage_Hook((DWORD*)g_pMain->uiPieceChangePlug->m_btnStop, UIMSG_BUTTON_CLICK);
}

bool CUIPieceChangePlug::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	pieceChangeVTable = m_dVTableAddr;
	if (!pSender || pSender == 0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;

	if (pSender == (DWORD*)btn_bank)
		m_bank = !m_bank;
	else if (pSender == (DWORD*)btnSell)
		m_sell = !m_sell;
	else if (pSender == (DWORD*)m_btnStart)
	{
		return true;
		uint16 curCount = atoi(g_pMain->GetString(m_textExCount).c_str());
		if (!curCount) curCount = 1;
		else if (curCount > 100) curCount = 100;
	
		bool bank = g_pMain->uiPieceChangePlug->m_bank, sell = g_pMain->uiPieceChangePlug->m_sell;

		Packet pkt(WIZ_HSACS_HOOK, uint8(HSACSOpCodes::CHAOTIC_EXCHANGE));
		pkt << curCount << bank << sell;
		g_pMain->Send(&pkt);
	}
	else if (pSender == (DWORD*)m_btnStop) {

	}
	else if (pSender == (DWORD*)m_btnClose) {
		m_sell = m_bank = false;
	}
	else if (pSender == (DWORD*)btn_up) {
		if(g_pMain->uiPieceChangePlug->gemcount < 100) 
			g_pMain->uiPieceChangePlug->gemcount++;
		g_pMain->SetString(g_pMain->uiPieceChangePlug->m_textExCount, string_format("%d", g_pMain->uiPieceChangePlug->gemcount));
	}
	else if (pSender == (DWORD*)btn_down) {
		if (g_pMain->uiPieceChangePlug->gemcount > 0)
			g_pMain->uiPieceChangePlug->gemcount--;
		g_pMain->SetString(g_pMain->uiPieceChangePlug->m_textExCount, string_format("%d", g_pMain->uiPieceChangePlug->gemcount));
	}

	return true;
}

void CUIPieceChangePlug::InitReceiveMessage()
{
	DWORD ptrMsg = g_pMain->GetRecvMessagePtr(m_dVTableAddr);
	Func_PieceChange = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)PieceChangeReceiveMessage_Hook;
}