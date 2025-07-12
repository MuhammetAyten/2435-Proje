#include "MerchantEye.h"

CUIMerchantEye::CUIMerchantEye()
{

}

CUIMerchantEye::~CUIMerchantEye()
{

}

bool CUIMerchantEye::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false) return false;

	string find = xorstr("btn_cancel");
	btn_cancel = (CN3UIButton*)GetChildByID(find);
	find = xorstr("search_merchant");
	search_merchant = (CN3UIEdit*)GetChildByID(find);
	SetPos(g_pMain->pClientHookManager->GetScreenCenter(this).x, g_pMain->pClientHookManager->GetScreenCenter(this).y);
	lastChar = "";
	return true;
}


bool CUIMerchantEye::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == btn_cancel)
			Close();
		else if (pSender == btn_sell)
			btn_buy->SetState(UI_STATE_BUTTON_NORMAL);
		else if (pSender == btn_buy)
			btn_sell->SetState(UI_STATE_BUTTON_NORMAL);
	}

	return true;
}


void CUIMerchantEye::Tick()
{
	if (!IsVisible() || !search_merchant->HaveFocus())
		return;

	if (search_merchant->GetString().size() > 21)
		return;

	if (g_pMain->pClientMerchantArray.size() > 0 && search_merchant->GetString().empty())
	{
		g_pMain->pClientMerchantEyeDataLock.lock();
		g_pMain->pClientMerchantArray.clear();
		g_pMain->pClientMerchantEyeDataLock.unlock();
		return;
	}

	if (lastChar == search_merchant->GetString())
		return;
	else
		lastChar = search_merchant->GetString();

	if (lastChar.empty())
		return;

	std::map<uint16, uint16> tmpFile;
	std::map<uint16, uint16> tmpDelete;

	g_pMain->pClientMerchantEyeDataLock.lock();
	foreach(itr, g_pMain->pClientMerchantEyeData)
	{
		MerchantEyeData pMerchantItem = itr->second;
		for (int i = 0; i < 12; i++)
		{
			if (pMerchantItem.strMerchantItem[i].empty())
				continue;

			if (!g_pMain->str_contains(pMerchantItem.strMerchantItem[i], lastChar))
				continue;

			g_pMain->pClientMerchantArray.insert(std::pair<uint16, uint16>(itr->first, itr->first));
			tmpFile.insert(std::pair<uint16, uint16>(itr->first, itr->first));
			break;
		}
	}

	foreach(itr, g_pMain->pClientMerchantArray)
	{
		auto it = tmpFile.find(itr->first);
		if (it == tmpFile.end())
			tmpDelete.insert(std::pair<uint16, uint16>(itr->first, itr->first));
	}

	foreach(itr, tmpDelete)
		g_pMain->pClientMerchantArray.erase(itr->first);
	g_pMain->pClientMerchantEyeDataLock.unlock();
}

void CUIMerchantEye::Open()
{
	g_pMain->pClientMerchantEyeLock.lock();
	g_pMain->pClientMerchantArray.clear();
	g_pMain->pClientMerchantEyeLock.unlock();
	SetVisible(true);
	lastChar = "";
	search_merchant->HaveFocus();
}

void CUIMerchantEye::Close()
{
	search_merchant->SetString("");
	search_merchant->KillFocus();
	SetVisible(false);
	g_pMain->pClientMerchantEyeLock.lock();
	g_pMain->pClientMerchantArray.clear();
	g_pMain->pClientMerchantEyeLock.unlock();
}

