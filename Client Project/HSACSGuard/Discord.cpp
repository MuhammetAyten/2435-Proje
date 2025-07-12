#include "Discord.h"

extern std::string GetZoneName(uint16 zoneID);
std::string a = "";
std::string b = "";
std::string c = "";

std::string GetName(DWORD obj)
{
	DWORD nameLen = *(DWORD*)((DWORD)obj + KO_OFF_NAMELEN);
	char* name = new char[nameLen + 1]{ 0 };
	if (nameLen < 16)
		memcpy(name, (char*)((DWORD)obj + KO_OFF_NAME), nameLen);
	else
		memcpy(name, (char*)*(DWORD*)((DWORD)obj + KO_OFF_NAME), nameLen);
	std::string n = name;
	std::free(name);
	return n;
}

std::string GetZoneName(uint16 zoneID)
{
	switch (zoneID)
	{
	case 0: return xorstr("All Zones"); break;
	case 1: return xorstr("Luferson Castle"); break;
	case 2: return xorstr("El Morad Castle"); break;
	case 5: return xorstr("Luferson Castle"); break;
	case 6: return xorstr("Luferson Castle"); break;
	case 7: return xorstr("El Morad Castle"); break;
	case 8: return xorstr("El Morad Castle"); break;
	case 11: return xorstr("Eslant"); break;
	case 12: return xorstr("Eslant"); break;
	case 13: return xorstr("Eslant"); break;
	case 14: return xorstr("Eslant"); break;
	case 15: return xorstr("Eslant"); break;
	case 16: return xorstr("Eslant"); break;
	case 18: return xorstr("Old Karus"); break;
	case 21: return xorstr("Moradon"); break;
	case 22: return xorstr("Moradon"); break;
	case 23: return xorstr("Moradon"); break;
	case 24: return xorstr("Moradon"); break;
	case 25: return xorstr("Moradon"); break;
	case 28: return xorstr("Old Human"); break;
	case 29: return xorstr("Old Moradon"); break;
	case 30: return xorstr("Delos"); break;
	case 31: return xorstr("Bifrost"); break;
	case 32: return xorstr("Desperation Abyss"); break;
	case 33: return xorstr("Hell Abyss"); break;
	case 34: return xorstr("Felankor Lair"); break;
	case 35: return xorstr("Delos Basement"); break;
	case 36: return xorstr("Test"); break;
	case 48: return xorstr("Battle Arena"); break;
	case 51: return xorstr("Orc Prisoner Quest"); break;
	case 52: return xorstr("Blood Don Quest"); break;
	case 53: return xorstr("Goblin Quest"); break;
	case 54: return xorstr("Cape Quest"); break;
	case 55: return xorstr("Forgotten Temple"); break;
	case 61: return xorstr("Napies Gorge"); break;
	case 62: return xorstr("Alseids Prairie"); break;
	case 63: return xorstr("Nieds Triangle"); break;
	case 64: return xorstr("Nereids Island"); break;
	case 65: return xorstr("Moradon War"); break;
	case 66: return xorstr("Oreads"); break;
	case 67: return xorstr("Test Zone"); break;
	case 69: return xorstr("Snow War"); break;
	case 71: return xorstr("Colony Zone"); break;
	case 72: return xorstr("Ardream"); break;
	case 73: return xorstr("Ronark Land Base"); break;
	case 74: return xorstr("Test Zone"); break;
	case 75: return xorstr("Krowaz Domion"); break;
	case 76: return xorstr("Knight Royale"); break;
	case 77: return xorstr("Clan War Ardream"); break;
	case 78: return xorstr("Clan War Ronark Land"); break;
	case 81: return xorstr("Monster Stone"); break;
	case 82: return xorstr("Monster Stone"); break;
	case 83: return xorstr("Monster Stone"); break;
	case 84: return xorstr("Border Defance War"); break;
	case 85: return xorstr("Chaos Dengueon"); break;
	case 86: return xorstr("Under The Castle"); break;
	case 87: return xorstr("Juraid Mountain"); break;
	case 88: return xorstr("Lk War"); break;
	case 89: return xorstr("Dungeon Defence"); break;
	case 91: return xorstr("Zindan War"); break;
	case 92: return xorstr("Prison"); break;
	case 93: return xorstr("Isillion Lair"); break;
	case 94: return xorstr("Felankor Lair"); break;
	case 95: return xorstr("Drakis Tower"); break;
	case 96: return xorstr("Party War"); break;
	case 97: return xorstr("Party War"); break;
	case 98: return xorstr("Party War"); break;
	case 99: return xorstr("Party War"); break;
	case 105: return xorstr("Zindan War"); break;
	case 106: return xorstr("Piana War"); break;
	case 107: return xorstr("Breath War"); break;
	case 108: return xorstr("Colony Castle War"); break;
	case 109: return xorstr("Moradon War"); break;
	case 110: return xorstr("Snow War"); break;
	case 111: return xorstr("Koba War"); break;
	default: return xorstr("Unknown"); break;
	}
	return xorstr("Unknown");
}

Discord::Discord()
{
	memset(&handlers, 0, sizeof(handlers));
	memset(&discordPresence, 0, sizeof(discordPresence));
	StartTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void Discord::Initialize(std::string DiscordTokenID)
{
	Discord_Initialize(DiscordTokenID.c_str(), &handlers, 1, "0");
}

void Discord::Initialize()
{
	//Discord_Initialize("928764505452863569", &handlers, 1, "0");		// First Myko discord api
	//Discord_Initialize("930506539649884160", &handlers, 1, "0");		// Myko Network discord api
	//Discord_Initialize("999807079663538228", &handlers, 1, "0");		// MykoZone discord api
	//Discord_Initialize("1005939085787004978", &handlers, 1, "0");		// xydonisempire discord api
	//Discord_Initialize("1014921939179028480", &handlers, 1, "0");		// HomekoGame discord api
	//Discord_Initialize("1033129022571413567", &handlers, 1, "0");		// BorderKO discord api
	//Discord_Initialize("1041093076938133585", &handlers, 1, "0");		// PkOnline discord api
	//Discord_Initialize("1046742873934139462", &handlers, 1, "0");		// Beramus Online discord api
	//Discord_Initialize("1059382920936177754", &handlers, 1, "0");		// MykoNet discord api
	//Discord_Initialize("1059382920936177754", &handlers, 1, "0");		// MykoNet discord api
	//Discord_Initialize("1080231994732064788", &handlers, 1, "0");		// Playko4u discord api
	//Discord_Initialize("1080841104825536672", &handlers, 1, "0");		// LilaKo discord api
	//Discord_Initialize("1088472770410729513", &handlers, 1, "0");		// CyprusKO discord api
	//Discord_Initialize("1168205369231806524", &handlers, 1, "0");		// SimurgGame discord api
	//Discord_Initialize("858419925525987348", &handlers, 1, "0");		// JapKO Discord api
	//Discord_Initialize("857719360280854538", &handlers, 1, "0");		// K2HOMEKO discord api
	//Discord_Initialize("1178434190379843695", &handlers, 1, "0");		// K2HOMEKO discord api
	//Discord_Initialize("1195472742997303316", &handlers, 1, "0");		// WorldOfWar discord api
	//Discord_Initialize("1184234423936356433", &handlers, 1, "0");		// KnightGenie discord api
	//Discord_Initialize("1178697304295882833", &handlers, 1, "0");		// KnightWar discord api
	//Discord_Initialize("1197633328237531206", &handlers, 1, "0");		// BiteKO DiscordID
	//Discord_Initialize("1184219166396387378", &handlers, 1, "0");		// ParsOnline
	//Discord_Initialize("1200874435821965383", &handlers, 1, "0");		// PrmGame
	//Discord_Initialize("1193848989980840006", &handlers, 1, "0");		// MyKOFun DiscordID
	//Discord_Initialize("857719360280854538", &handlers, 1, "0");		// RosetKO DiscordID
}

void Discord::Update(bool inGame)
{
	memset(&discordPresence, 0, sizeof(discordPresence));
	//discordPresence.largeImageKey = "First Myko";					// First Myko discord logo
	//discordPresence.largeImageText = "First Myko";				// First Myko discord logo
	//discordPresence.largeImageKey = "komykoimg";					// Myko Network discord logo
	//discordPresence.largeImageText = "KO-MYKO.COM";				// Myko Network discord logo
	//discordPresence.largeImageKey = "mykozoneimg";				// Mykozone discord logo
	//discordPresence.largeImageText = "mykozoneimg";				// Mykozone discord logo
	//discordPresence.largeImageKey = "xydonisempire";				// xydonisempire discord logo
	//discordPresence.largeImageText = "xydonisempire";				// xydonisempire discord logo
	//discordPresence.largeImageKey = "homekogame";					// HomekoGame discord logo
	//discordPresence.largeImageText = "homekogame";				// HomekoGame discord logo
	//discordPresence.largeImageKey = "borderko";					// BorderKO discord logo
	//discordPresence.largeImageText = "borderko";					// BorderKO discord logo
	//discordPresence.largeImageKey = "pkonline";					// PK Online discord logo
	//discordPresence.largeImageText = "pkonline";					// PK Online discord logo
	//discordPresence.largeImageKey = "beramusonline";				// Beramus Online discord logo
	//discordPresence.largeImageText = "beramusonline";				// Beramus Online discord logo
	//discordPresence.largeImageKey = "mykonet";					// MykoNET discord logo
	//discordPresence.largeImageText = "mykonet";					// MykoNET discord logo
	//discordPresence.largeImageKey = "PlayKO4U";					// playko4u discord logo
	//discordPresence.largeImageText = "PlayKO4U";					// playko4u discord logo
	//discordPresence.largeImageKey = "LilaKo";						// LilaKo discord logo
	//discordPresence.largeImageText = "LilaKo";					// LilaKo discord logo
	//discordPresence.largeImageKey = "CyprusKO";					// CyprusKO discord logo
	//discordPresence.largeImageText = "CyprusKO";					// CyprusKO discord logo
	//discordPresence.largeImageKey = "hs";							// GloryKO discord logo
	//discordPresence.largeImageText = "hs";						// GloryKO discord logo
	//discordPresence.largeImageKey = "simurggame";					// simurggame discord logo
	//discordPresence.largeImageText = "simurggame";				// simurggame discord logo
	discordPresence.largeImageKey = "hs";							// simurggame discord logo
	discordPresence.largeImageText = "hs";							// simurggame discord logo

	if (inGame) {
		DWORD KO_CHR = *(DWORD*)KO_PTR_CHR;
		if (!KO_CHR) 
			return;

		uint8 lvl = *(uint8*)(KO_CHR + KO_OFF_LEVEL);
		uint16 zoneid = *(uint16*)(KO_CHR + KO_OFF_ZONE);

#if (HOOK_SOURCE_VERSION == 1098)
		a = string_format("HSACSX v1.098");
#elif(HOOK_SOURCE_VERSION == 1534)
		a = string_format("HSACSX v1.534");
#else
		a = string_format("HSACSX v2.369");
#endif

		b = string_format("Nick : %s - Lvl :%d", GetName(KO_CHR).c_str(), lvl).c_str();
		c = GetZoneName(zoneid).c_str();
		
		discordPresence.state = b.c_str();
		discordPresence.details = a.c_str();
	}
	else {
		discordPresence.state = "Entering Game";
		discordPresence.details = "";
		discordPresence.details = "";
	}
	discordPresence.startTimestamp = StartTime;
	discordPresence.endTimestamp = NULL;

	Discord_UpdatePresence(&discordPresence);
}