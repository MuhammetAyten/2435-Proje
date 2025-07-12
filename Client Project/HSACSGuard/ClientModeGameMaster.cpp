#include "ClientModeGameMaster.h"

CUIClientModeGameMasterPlug::CUIClientModeGameMasterPlug() 
{ 
	btn_close = NULL; 
	reload_tables = NULL; 
	btn_pus = btn_exchange = btn_itemsell = btn_Cstone = btn_upgrade = btn_ranks = btn_dungeon = btn_esn = btn_crsetting = btn_crmonster = btn_resource = btn_itemop = btn_magics = btn_special = btn_Mlist = btn_royal = btn_event = btn_MF = btn_autoMFF = btn_drop = NULL;
}

CUIClientModeGameMasterPlug::~CUIClientModeGameMasterPlug(){}

bool CUIClientModeGameMasterPlug::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false) return false;
	btn_close = (CN3UIButton*)GetChildByID(xorstr("btn_close"));

	reload_tables = (CN3UIBase*)GetChildByID(xorstr("reload_tables"));
	btn_pus = (CN3UIButton*)reload_tables->GetChildByID(xorstr("btn_pus"));
	btn_exchange = (CN3UIButton*)reload_tables->GetChildByID(xorstr("btn_exchange"));
	btn_magics = (CN3UIButton*)reload_tables->GetChildByID(xorstr("btn_magics"));
	btn_special = (CN3UIButton*)reload_tables->GetChildByID(xorstr("btn_special"));
	btn_Mlist = (CN3UIButton*)reload_tables->GetChildByID(xorstr("btn_Mlist"));
	btn_event = (CN3UIButton*)reload_tables->GetChildByID(xorstr("btn_event"));
	btn_royal = (CN3UIButton*)reload_tables->GetChildByID(xorstr("btn_royal"));
	btn_MF = (CN3UIButton*)reload_tables->GetChildByID(xorstr("btn_MF"));
	btn_autoMFF = (CN3UIButton*)reload_tables->GetChildByID(xorstr("btn_autoMFF"));
	btn_drop = (CN3UIButton*)reload_tables->GetChildByID(xorstr("btn_drop"));
	btn_itemop = (CN3UIButton*)reload_tables->GetChildByID(xorstr("btn_itemop"));
	btn_itemsell = (CN3UIButton*)reload_tables->GetChildByID(xorstr("btn_itemsell"));
	btn_crsetting = (CN3UIButton*)reload_tables->GetChildByID(xorstr("btn_crsetting"));
	btn_crmonster = (CN3UIButton*)reload_tables->GetChildByID(xorstr("btn_crmonster"));
	btn_resource = (CN3UIButton*)reload_tables->GetChildByID(xorstr("btn_resource"));
	btn_esn = (CN3UIButton*)reload_tables->GetChildByID(xorstr("btn_esn"));
	btn_ranks = (CN3UIButton*)reload_tables->GetChildByID(xorstr("btn_ranks"));
	btn_dungeon = (CN3UIButton*)reload_tables->GetChildByID(xorstr("btn_dungeon"));
	btn_upgrade = (CN3UIButton*)reload_tables->GetChildByID(xorstr("btn_upgrade"));
	btn_Cstone = (CN3UIButton*)reload_tables->GetChildByID(xorstr("btn_Cstone"));

	SetPos(g_pMain->pClientHookManager->GetScreenCenter(this).x, g_pMain->pClientHookManager->GetScreenCenter(this).y);
	Close();
	return true;
}

bool CUIClientModeGameMasterPlug::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if (!IsVisible() 
		|| !pSender 
		|| dwMsg != 0x00000001)
		return false;

	if (pSender == btn_close)
		Close();
	else if (pSender == btn_pus)
		Close();
	else if (pSender == btn_Cstone)
		Close();
	return true;
}

void CUIClientModeGameMasterPlug::Open() { SetVisible(true); }

void CUIClientModeGameMasterPlug::Close() { SetVisible(false); }