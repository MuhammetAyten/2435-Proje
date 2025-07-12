#include "LicenseSystem.h"

#define GUN 0
#define AY 1
#define YIL 2

static const string SERVER_INI = xorstr("Server.ini");
struct tm* timeInfoEx;

DWORD WINAPI LicenseEngine(HSACSEngine* e)
{
	time_t rawtime;
	time(&rawtime);
	timeInfoEx = localtime(&rawtime);

	string Path = e->m_BasePath + SERVER_INI;
	char INICurrentIP[16];
	while (true)
	{
		GetPrivateProfileStringA(xorstr("Server"), xorstr("IP0"), xorstr("212.68.34.22"), INICurrentIP, 16, Path.c_str());

		bool kappa = false;

		uint32 yil = timeInfoEx->tm_year + 1900;

		if (yil > lisansTarih[YIL])
			kappa = true;
		else if (timeInfoEx->tm_mon > lisansTarih[AY] - 1 && yil == lisansTarih[YIL])
			kappa = true;
		else if (timeInfoEx->tm_mday > lisansTarih[GUN] && timeInfoEx->tm_mon == lisansTarih[AY] - 1 && yil == lisansTarih[YIL])
			kappa = true;

		/*if (!IsLicensed(string(INICurrentIP)))
			kappa = true;*/     // Server.ini Lisansý kapatýldý.

		if (!e->m_connectedIP.empty() && !IsLicensed(e->m_connectedIP))
			kappa = true;

		if (kappa)
		{
			e->power = false;
			exit(0);
			FreeLibrary(GetModuleHandle(NULL));
			TerminateProcess(GetCurrentProcess(), 0);
		}

		Sleep(5000);
	}
}

bool IsLicensed(std::string ip)
{
	bool ret = false;
	for (string pattern : subnetLisanlar)
	{
		const char* tmp = pattern.c_str(); // 185.160.30.xx
		bool f = true;
		for (size_t i = 0; i < pattern.length(); i++)
			if (tmp[i] != 'x' && tmp[i] != ip.c_str()[i])
				f = false;
		ret = f;
	}

	for (string license : ipLisanslari)
		if (ip == license)
			ret = true;

	return ret;
}