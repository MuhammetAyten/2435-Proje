#include "UIHPBar.h"
time_t nModul = 0;
CUIClientHPBarPlug::CUIClientHPBarPlug()
{
	m_dVTableAddr = NULL;

	vector<int>offsets;
	offsets.push_back(0x3E0);
	offsets.push_back(0);

	m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);
	MaxExp = 0;
	nflexptime = 0;
	text_nick = 0;
	text_level = 0;
	text_cordinate = 0;
	progress_flame = 0;
	Text_burning = 0;
	base_burning_frame = 0;
	ParseUIElements();
	time_t UNIXTIME;
	tm g_localTime;
	UNIXTIME = time(nullptr);
	g_localTime = *localtime(&UNIXTIME);
	nModul = UNIXTIME + 60;
}

CUIClientHPBarPlug::~CUIClientHPBarPlug()
{
}

void CUIClientHPBarPlug::ParseUIElements()
{
	text_hp = g_pMain->GetChildByID(m_dVTableAddr, "Text_HP");
	text_mp = g_pMain->GetChildByID(m_dVTableAddr, "Text_MSP");
#if (HOOK_SOURCE_VERSION == 1098 || HOOK_SOURCE_VERSION == 1534)
	if (HOOK_SOURCE_VERSION == 1098) {
		str_exp = g_pMain->GetChildByID(m_dVTableAddr, "str_exp");
		text_exp = g_pMain->GetChildByID(m_dVTableAddr, "Text_ExpP");
		progress_exp = g_pMain->GetChildByID(m_dVTableAddr, "progress_exp");
		progress_Sexp = g_pMain->GetChildByID(m_dVTableAddr, "progress_Sexp");
		g_pMain->SetProgressRange(progress_exp, 0, 100.0f); // 0 min deðer 100 max deðer progressbar için
		g_pMain->SetProgressRange(progress_Sexp, 0, 100.0f);
	}
	text_nick = g_pMain->GetChildByID(m_dVTableAddr, "text_nick");
	text_level = g_pMain->GetChildByID(m_dVTableAddr, "text_level");
#else
	Text_burning = g_pMain->GetChildByID(m_dVTableAddr, "Text_burning");
	base_burning_frame = g_pMain->GetChildByID(m_dVTableAddr, "base_burning_frame");
	progress_flame = g_pMain->GetChildByID(base_burning_frame, "Progress_burning");
	g_pMain->SetProgressRange(progress_flame, 0, float(burntime*3)); // 0 min deðer 100 max deðer progressbar için
	g_pMain->SetString(Text_burning, string_format("Lv 0"));
#endif
}

void CUIClientHPBarPlug::HPChange(uint16 hp, uint16 maxhp)
{
	return;
	if (maxhp == 0)
		return;

	if (m_dVTableAddr != NULL) {
		std::string str = xorstr("%s/%s (%d%%)");
		int percent = (int)ceil((hp * 100) / maxhp);
		std::string max = g_pMain->StringHelper->NumberFormat(maxhp);
		std::string cur = g_pMain->StringHelper->NumberFormat(hp);
		g_pMain->SetString(text_hp, string_format(str, cur.c_str(), max.c_str(), percent));
	}
}

void CUIClientHPBarPlug::MPChange(uint16 mp, uint16 maxmp)
{
	return;

	if (maxmp == 0)
		return;

	if (m_dVTableAddr != NULL) {
		std::string str = xorstr("%s/%s (%d%%)");
		int percent = (int)ceil((mp * 100) / maxmp);
		std::string max = g_pMain->StringHelper->NumberFormat(maxmp);
		std::string cur = g_pMain->StringHelper->NumberFormat(mp);
		g_pMain->SetString(text_mp, string_format(str, cur.c_str(), max.c_str(), percent));
	}
}

void CUIClientHPBarPlug::ExpChange(int64 exp, int64 maxexp) {
#if (HOOK_SOURCE_VERSION == 1098)
	if (m_dVTableAddr != NULL) {
		std::string yaz = "";
		std::vector<std::string> out;
		yaz = g_pMain->GetString(g_pMain->uiTaskbarMain->str_exp).c_str();

		g_pMain->str_replace(yaz, "EXP:", "");
		g_pMain->str_replace(yaz, "%", "");
		g_pMain->str_split(yaz.c_str(), '.', out);

		int percent = (int)ceil((exp * 100) / maxexp);

		if (out.size() == 2)
		{
			g_pMain->SetCurValue(progress_Sexp, (float)atoi(out[1].c_str()), 15.0f);
			g_pMain->SetString(str_exp, string_format("%d.%s", percent, out[1].c_str()));
		}
		g_pMain->SetCurValue(progress_exp, (float)percent, 15.0f);
	}
#endif
}

void CUIClientHPBarPlug::BurningChange(uint16 ilevel) {
#if (HOOK_SOURCE_VERSION == 2369)
	if (m_dVTableAddr == NULL)
		return;

	if (ilevel > 3)
		ilevel = 3;

	nflexptime = float(burntime * ilevel);

	std::string text = ilevel < 3 ? std::to_string(ilevel) : "Max";
	g_pMain->SetCurValue(progress_flame, nflexptime, 15.0f);
	g_pMain->SetString(Text_burning, string_format("Lv %s", text.c_str()));
#endif
}

long timeLaps = 0;

extern std::string GetName(DWORD obj);
extern std::string GetZoneName(uint16 zoneID);
int nTime = 0;

void CUIClientHPBarPlug::Tick()
{
	if (timeLaps > clock() - 200)
		return;

	timeLaps = clock();

	DWORD KO_CHR = *(DWORD*)KO_PTR_CHR;

	

#if (HOOK_SOURCE_VERSION == 1098 || HOOK_SOURCE_VERSION == 1534)
	
	if (text_nick)
		g_pMain->SetString(text_nick, GetName(KO_CHR).c_str());

	if (text_level) {
		uint8 lvl = *(uint8*)(KO_CHR + KO_OFF_LEVEL);
		if (HOOK_SOURCE_VERSION == 1098)
			g_pMain->SetString(text_level, string_format("Level : %d", lvl));
		else
			g_pMain->SetString(text_level, string_format(" %d", lvl));
	}

	/*if (text_cordinate)
	{
		g_pMain->SetVisible(text_cordinate, false);
		float x, z;
		x = *(float*)(KO_CHR + KO_OFF_X);
		z = *(float*)(KO_CHR + KO_OFF_Z);
		g_pMain->SetString(text_cordinate, string_format("%s %d, %d", GetZoneName(*(uint16*)(KO_CHR + KO_OFF_ZONE)).c_str(), static_cast<int>(x), static_cast<int>(z)));
	}*/
#else
	time_t UNIXTIME;
	tm g_localTime;
	UNIXTIME = time(nullptr);
	g_localTime = *localtime(&UNIXTIME);

	if (g_pMain->Player.burninglevel < 3 && UNIXTIME > nModul)
	{
		nModul = UNIXTIME + burntime;
		nTime++;
		nflexptime++;
		//g_pMain->SetCurValue(g_pMain->pClientUIBarPlug->progress_flame, (float)nflexptime, 15.0f);
		g_pMain->SetCurValue(g_pMain->pClientUIBarPlug->progress_flame, (float)nflexptime, 15.0f);

		if (nTime > (burntime - 1)) {
			g_pMain->Player.burninglevel++;
			std::string text = g_pMain->Player.burninglevel < 3 ? std::to_string(g_pMain->Player.burninglevel) : "Max";
			g_pMain->SetString(Text_burning, string_format("Lv %s", text.c_str()));
			nTime = 0;
		}
	}
#endif
}