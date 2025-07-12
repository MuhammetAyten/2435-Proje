#include "RightCorner.h"

CUIRightCorner::CUIRightCorner()
{
	btn_drop == NULL;

}

CUIRightCorner::~CUIRightCorner()
{

}

bool CUIRightCorner::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false) return false;

	std::string find = xorstr("btn_drop");
	btn_drop = (CN3UIButton*)GetChildByID(find);


	//find = xorstr("search_merchant");
	//search_merchant = (CN3UIEdit*)GetChildByID(find);
	
	POINT koScreen = *(POINT*)0x00DE297C;
	SetPos(koScreen.x-49, g_pMain->pClientHookManager->GetScreenCenter(this).y);
	return true;
}


bool CUIRightCorner::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
	
		if (pSender == (CN3UIButton*)btn_drop)
		{
			g_pMain->pClientHookManager->OpenMonsterSearch();
		}
	}

	return true;
}


void CUIRightCorner::Tick()
{
	/*if (!IsVisible() || !search_merchant->HaveFocus())
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
		pClientMerchantEyeData item = itr->second;
		for (int i = 0; i < 12; i++)
		{
			if (item.strMerchantItem[i].empty())
				continue;

			if (!g_pMain->str_contains(item.strMerchantItem[i], lastChar))
				continue;

			g_pMain->pClientMerchantArray.insert(std::pair<uint16, uint16>(itr->first, itr->first));
			tmpFile.insert(std::pair<uint16, uint16>(itr->first, itr->first));
			break;
		}
	}

	foreach(itr, g_pMain->MerchantID)
	{
		auto it = tmpFile.find(itr->first);
		if (it == tmpFile.end())
			tmpDelete.insert(std::pair<uint16, uint16>(itr->first, itr->first));
	}

	foreach(itr, tmpDelete)
		g_pMain->pClientMerchantArray.erase(itr->first);
	g_pMain->pClientMerchantEyeDataLock.unlock();*/
}

void CUIRightCorner::Open()
{
	SetVisible(true);
}

void CUIRightCorner::Close()
{
	/*search_merchant->SetString("");
	search_merchant->KillFocus();
	SetVisible(false);
	g_pMain->pClientMerchantEyeLock.lock();
	g_pMain->pClientMerchantArray.clear();
	g_pMain->pClientMerchantEyeLock.unlock();*/
}

