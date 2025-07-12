#include "stdafx.h"
#include "PlayerRank.h"
ClientHookPlayerRank::ClientHookPlayerRank()
{


	vector<int>offsets;
	offsets.push_back(0x3F0);
	offsets.push_back(0);

	el_Base = g_pMain->rdword(KO_DLG, offsets);

	m_btnReview = 0;
	m_btnCancel = 0;
	nHumanBaseTop = 0;
	nKarusBaseTop = 0;
	for (int i = 0; i < 10; i++) {
		nHumanBase[i] = 0;
		nKarusBase[i] = 0;
		nHumanBaseEffeckt[i] = 0;
		nKarusBaseEffeckt[i] = 0;
	}




	ParseUIElements();

}
ClientHookPlayerRank::~ClientHookPlayerRank()
{
}

void ClientHookPlayerRank::ParseUIElements()
{
	std::string find = xorstr("el_symbol");
	g_pMain->GetChildByID(el_Base, find, nHumanBaseTop);

	for (int i = 0; i < 10; i++) {
		find = string_format(xorstr("rank%d"), i);
		g_pMain->GetChildByID(nHumanBaseTop, find, nHumanBase[i]);
		g_pMain->SetVisible(nHumanBase[i], false);
	}
	find = xorstr("ka_symbol");
	g_pMain->GetChildByID(el_Base, find, nKarusBaseTop);
	for (int i = 0; i < 10; i++) {
		find = string_format(xorstr("rank%d"), i);
		g_pMain->GetChildByID(nKarusBaseTop, find, nKarusBase[i]);
		g_pMain->SetVisible(nKarusBase[i], false);
	}
}

bool ClientHookPlayerRank::ResetBase()
{
	for (int i = 0; i < 10; i++) {
		g_pMain->SetVisible(nKarusBase[i], false);
		g_pMain->SetVisible(nHumanBase[i], false);
		g_pMain->SetVisible(nHumanBaseEffeckt[i], false);
		g_pMain->SetVisible(nKarusBaseEffeckt[i], false);

	}
	return false;
}

