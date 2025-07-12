#include "BanSystem.h"
#include "stdafx.h"
#include "HSACSGui.h"
#include "HDRReader.h"
#include "splash.h"
#include <TlHelp32.h>


BanSystem* pBannedSystem = NULL;
void OpenConsoleWindow();

HSACSEngine* g_pMain = NULL;
HDRReader* hdrReader = NULL;
std::string basePath = "";
int SkillBaseTime = GetTickCount() + 25000;

void LoadCrypto();



void Shutdown(string message = "");

bool _fexists(std::string& filename)
{
	std::ifstream ifile(filename.c_str());
	return (bool)ifile;
}

std::string getexepath()
{

	char result[MAX_PATH];
	string ret = std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
	return ret.substr(0, ret.find_last_of("\\"));
}

DWORD GetFileSize(const char* FileName)
{
	std::ifstream in(FileName, std::ifstream::ate | std::ifstream::binary);
	return in.tellg();
}

void Shutdown(string message)
{

	g_pMain->power = false;
	string s1 = xorstr("The file is corrupted. Please contact with administrator.\n");
	string s2 = xorstr("If you constantly see this message, ");
	string s3 = xorstr("disable the anti-virus program.");

	if (message == "")
		MessageBoxA(NULL, string(s1 + s2 + s3).c_str(), xorstr("KnightOnline.exe"), MB_OK | MB_ICONEXCLAMATION);
	else
		MessageBoxA(NULL, message.c_str(), xorstr("KnightOnLine.exe"), MB_OK | MB_ICONEXCLAMATION);

	exit(0);
	FreeLibrary(GetModuleHandle(NULL));
	TerminateProcess(GetCurrentProcess(), 0);
}

// dosya yol değiştirme

void eraseSubStr(std::string& mainStr, std::string& toErase)
{
	std::transform(mainStr.begin(), mainStr.end(), mainStr.begin(), ::tolower);
	std::transform(toErase.begin(), toErase.end(), toErase.begin(), ::tolower);

	size_t pos = mainStr.find(toErase);
	if (pos != std::string::npos)
	{
		// If found then erase it from string
		mainStr.erase(pos, toErase.length());
	}
}

typedef HANDLE(WINAPI* tCreateFileA)(
	LPCSTR                lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile
	);

tCreateFileA oCreateFileA;
tCreateFileA oCreateFileCameraA;

HANDLE WINAPI hkCreateFileCameraA(LPCSTR                lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile)
{
	return oCreateFileCameraA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

std::string __tmpfl = "";
DWORD _PID = 0;
HCRYPTKEY hKey = NULL;
std::string xCodeTMP = xorstr("Misc\\river\\hsacsx.dxt");

HANDLE WINAPI hkCreateFileA(LPCSTR                lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile)
{

	string fl = lpFileName;

	if ((int)dwFlagsAndAttributes != 137)
		return oCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	else
	{
		xCodeTMP = string_format(xorstr("Misc\\river\\hsacsx_%d.dxt"), _PID);
		PBYTE pbBuffer = NULL;
		HANDLE hSourceFile = oCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, hTemplateFile);
		HANDLE hDestinationFile = oCreateFileA(xCodeTMP.c_str(), FILE_WRITE_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE != hSourceFile && INVALID_HANDLE_VALUE != hDestinationFile)
		{
			bool fEOF = false;
			DWORD dwCount;
			LARGE_INTEGER size;
			GetFileSizeEx(hSourceFile, &size);
			LONG len = (size.u.HighPart << (LONG)32) | size.u.LowPart;
			DWORD dwBlockLen = len % 2 == 0 ? 32 : 31;
			DWORD dwBufferLen = dwBlockLen;

			if ((pbBuffer = (PBYTE)malloc(dwBufferLen)))
			{
				if (ReadFile(hSourceFile, pbBuffer, 4, &dwCount, NULL))
					WriteFile(hDestinationFile, pbBuffer, dwCount, &dwCount, NULL);

				ZeroMemory(pbBuffer, dwBufferLen);
				dwCount = 0;
				do
				{
					if (ReadFile(hSourceFile, pbBuffer, dwBlockLen, &dwCount, NULL))
					{
						if (dwCount < dwBlockLen)
							fEOF = TRUE;
						if (CryptDecrypt(hKey, 0, 1, 0, pbBuffer, &dwCount))
							WriteFile(hDestinationFile, pbBuffer, dwCount, &dwCount, NULL);
					}
				} while (!fEOF);
			}

			if (pbBuffer)
				free(pbBuffer);

			if (hSourceFile)
				CloseHandle(hSourceFile);

			if (hDestinationFile)
				CloseHandle(hDestinationFile);

			return oCreateFileA(xCodeTMP.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, hTemplateFile);
		}
	}
	return oCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}
// dosya yol değiştirme ---------------------

#include <windows.h>
#include <tchar.h>

typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

LPFN_ISWOW64PROCESS fnIsWow64Process;

#define HSACS_Intercept void  __declspec( naked ) __stdcall
#define _sosm __asm
#define IMAGEBASE 0x0

unsigned long Call_KelimeOyunu	= IMAGEBASE + 0x00510E04;
unsigned long Call_Devam_mokoko	= IMAGEBASE + 0x005112B5;
unsigned long Call_Devam_keko	= IMAGEBASE + 0x005110D0;
unsigned long Call_Devam_ali	= IMAGEBASE + 0x005111C1;

HSACS_Intercept BotNameHpTarget()
{
	_sosm{
		CMP EDI,EBX
		JGE Call_Devam
		CMP ESI,-1
		JE Call_Devam_keke
		CMP ESI,0x270F
		JG Call_Devam_keke
		JMP Call_KelimeOyunu
		Call_Devam:
		JLE Call_Devam_Devam
		MOV EBP,0x0F
		JMP Call_Devam_ali
		Call_Devam_Devam:
		MOV ECX,DWORD PTR DS : [0x00F3E860]
		JMP Call_Devam_mokoko
		Call_Devam_keke:
		MOV EBP,0x0F
		JMP Call_Devam_keko
	}
}

float __cameraLimitS = 2.3125f;
unsigned long Call_Devam_aliS = IMAGEBASE + 0x00945CF6;
unsigned long Call_Devam_aliSI = IMAGEBASE + 0x00945CD9;
HSACS_Intercept haganYNG()
{
	_sosm{
		CMP EAX,3
		JE ABISI
		CMP EAX,1
		JE ABISI
		JMP DWORD PTR DS : [Call_Devam_aliS]
		ABISI:
		PUSHAD
		PUSHFD
		MOV EAX,DWORD PTR DS : [0xF368F8]
		MOVSS XMM0,__cameraLimitS
		MOVSS DWORD PTR DS : [EAX + 0x7A4] ,XMM0
		POPFD
		POPAD
		JMP DWORD PTR DS : [Call_Devam_aliSI]
	}
}

const char* ADRES = "ui\\el_warfaremantlelist.uif";
const char* ADRES1 = "ui\\re_minicaptcha.uif";
unsigned long UIHOOK = IMAGEBASE + 0x00404CB0;
unsigned long UIHOOK1 = IMAGEBASE + 0x00B6C3AA;
unsigned long UIHOOK2 = IMAGEBASE + 0x0055F216;
HSACS_Intercept UIFHOOK()
{
	//string sa = char*(L"sa");
	_sosm{
		MOV DWORD PTR SS : [ESP + 0xE0] ,EBX
		MOV DWORD PTR SS : [ESP + 0xDC] ,0
		MOV BYTE PTR SS : [ESP + 0xCC] ,0
		MOV ECX,DWORD PTR DS : [ESI + 0x788]
		MOV EDX,DWORD PTR DS : [ECX]
		MOV EAX,DWORD PTR DS : [EDX + 0x60]
		PUSH 0
		PUSH 0
		PUSH 0
		CALL EAX
		MOV ECX,DWORD PTR DS : [ESI + 0x788]
		MOV EDX,DWORD PTR DS : [ECX]
		MOV EAX,DWORD PTR DS : [EDX + 0x4C]
		PUSH 0x11
		CALL EAX
		MOV ECX,DWORD PTR DS : [ESI + 0x78C]
		MOV EDX,DWORD PTR DS : [ECX]
		MOV EAX,DWORD PTR DS : [0x00F368E8]
		MOV EDX,DWORD PTR DS : [EDX + 0x80]
		PUSH EAX
		CALL EDX
		PUSH 0x15
		//mov ebx, ADRES1  // EAX kaydına str adresini yükle
		push 0x00C5BAE8      // EAX kaydındaki adresi yığın üzerine it
		//PUSH ADRES1
		LEA ECX, DWORD PTR SS : [ESP + 0x108]
		MOV DWORD PTR SS : [ESP + 0x120] , EBX
		MOV DWORD PTR SS : [ESP + 0x11C] , 0
		MOV BYTE PTR SS : [ESP + 0x10C] , 0
		CALL UIHOOK
		MOV DWORD PTR SS : [ESP + 0x28C] , 0x0A
		MOV ECX, DWORD PTR DS : [ESI + 0x78C]
		MOV EAX, DWORD PTR DS : [ECX]
		MOV EAX, DWORD PTR DS : [EAX + 0x08]
		LEA EDX, DWORD PTR SS : [ESP + 0x100]
		PUSH EDX
		CALL EAX
		MOV DWORD PTR SS : [ESP + 0x28C] , -1
		CMP DWORD PTR SS : [ESP + 0x118] , 0x10
		JB ANAL
		MOV ECX, DWORD PTR SS : [ESP + 0x104]
		PUSH ECX
		CALL UIHOOK1
		ADD ESP, 0x04
		ANAL:
		JMP UIHOOK2
		//MOV DWORD PTR SS : [ESP + 0xE0] ,EBX
		//MOV DWORD PTR SS : [ESP + 0xDC] ,0
		//MOV BYTE PTR SS : [ESP + 0xCC] ,0
		//MOV ECX,DWORD PTR DS : [ESI + 0x659]
		//MOV EDX,DWORD PTR DS : [ECX]
		//MOV EAX,DWORD PTR DS : [EDX + 0x60]
		//PUSH 0
		//PUSH 0
		//PUSH 0
		//CALL EAX
		//MOV ECX,DWORD PTR DS : [ESI + 0x659]
		//MOV EDX,DWORD PTR DS : [ECX]
		//MOV EAX,DWORD PTR DS : [EDX + 0x4C]
		//PUSH 0x11
		//CALL EAX
		//MOV ECX,DWORD PTR DS : [ESI + 0x65C]
		//MOV EDX,DWORD PTR DS : [ECX]
		//MOV EAX,DWORD PTR DS : [0x00F368E8]
		//MOV EDX,DWORD PTR DS : [EDX + 0x80]
		//PUSH EAX
		//CALL EDX
		//PUSH 0x15
		////mov ebx, ADRES  // EAX kaydına str adresini yükle
		//push 0x00C5BAE8
		////PUSH ADRES
		//LEA ECX, DWORD PTR SS : [ESP + 0x108]
		//MOV DWORD PTR SS : [ESP + 0x120] , EBX
		//MOV DWORD PTR SS : [ESP + 0x11C] , 0
		//MOV BYTE PTR SS : [ESP + 0x10C] , 0
		//CALL UIHOOK
		//MOV DWORD PTR SS : [ESP + 0x28C] , 0x0A
		//MOV ECX, DWORD PTR DS : [ESI + 0x65C]
		//MOV EAX, DWORD PTR DS : [ECX]
		//MOV EAX, DWORD PTR DS : [EAX + 0x08]
		//LEA EDX, DWORD PTR SS : [ESP + 0x100]
		//PUSH EDX
		//CALL EAX
		//MOV DWORD PTR SS : [ESP + 0x28C] , -1
		//CMP DWORD PTR SS : [ESP + 0x118] , 0x10
		//JB ANAL1
		//MOV ECX, DWORD PTR SS : [ESP + 0x104]
		//PUSH ECX
		//CALL UIHOOK1
		//ADD ESP, 0x04
		//ANAL1 :
		//JMP UIHOOK2
	}
}

unsigned long Call_Devam_aliSS = IMAGEBASE + 0x00515B16;
unsigned long Call_Devam_aliSSI = IMAGEBASE + 0x00515B2F;
HSACS_Intercept HaganYNGSiker()
{
	_sosm{
		MOV AL,BYTE PTR DS : [EDX + 0x7B4]
		CMP AL,0x0A
		JE KO
		CMP AL,0x0C
		JE KO
		CMP AL,0x07
		JE KO
		JMP DWORD PTR DS : [Call_Devam_aliSS]
		RETN
		KO:
		JMP DWORD PTR DS : [Call_Devam_aliSSI]
		RETN
	}
}

unsigned long Call_Devam_aliSSS = IMAGEBASE + 0x0053427F;
HSACS_Intercept PartySystemTest()
{
	_sosm{
		CMP EBP,0x0C
		JE TEST1
		CMP EBP,0x24
		JE TEST2
		CMP EBP,0x25
		JE TEST3
		JMP DWORD PTR DS : [Call_Devam_aliSSS]
		TEST1:
		MOV BYTE PTR SS : [ESP + 0x1B] ,0x01
		MOV EDI,0xFFCD9BFF
		MOV EBP,0x01
		JMP DWORD PTR DS : [Call_Devam_aliSSS]
		TEST2:
		MOV BYTE PTR SS : [ESP + 0x1B] ,0x01
		MOV EDI,-3400
		MOV EBP,0x01
		JMP DWORD PTR DS : [Call_Devam_aliSSS]
		TEST3:
		MOV BYTE PTR SS : [ESP + 0x1B] ,0x01
		MOV EDI,0xFF1E90FF
		MOV EBP,0x01
		JMP DWORD PTR DS : [Call_Devam_aliSSS]
	}
}

void InterceptFunc(unsigned long Offset, void* func) {
	char* yareyare;
	unsigned long oldProtect;
	unsigned long Dummy;
	char Buffer[] = "\xE9\x00\x00\x00\x00";
	yareyare = reinterpret_cast<char*>(Offset);
	VirtualProtect(yareyare, 5L, PAGE_EXECUTE_READWRITE, &oldProtect);
	*reinterpret_cast<long*>(Buffer + 1) = reinterpret_cast<long>(func) - reinterpret_cast<long>(yareyare) - 5;
	*Buffer = static_cast<unsigned char>(0xE9);
	RtlMoveMemory(yareyare, Buffer, 5);
	VirtualProtect(yareyare, 5L, oldProtect, &Dummy);
}

bool IsWOW64()
{
	BOOL bIsWow64 = FALSE;

	//IsWow64Process is not available on all supported versions of Windows.
	//Use GetModuleHandle to get a handle to the DLL that contains the function
	//and GetProcAddress to get a pointer to the function if available.
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
		{
			//handle error
		}
	}
	return bIsWow64;
}

void MultiErrorMsg()
{
	g_pMain->power = false;
	std::string msg = "Sunumuzda Maksiumum 3 client açılmaktadır!";
	Shutdown(msg);

	ExitProcess(0);
	DWORD skillbase2 = 0;
	DWORD skillid2 = 0;
	DWORD value3 = 0;
	int counter = 0;
	unsigned int a2 = 0;
	DWORD skillbase4 = 0;
	DWORD skillid5 = 0;
	DWORD value6 = 0;
	int counter6 = 0;

	__asm
	{
		push   0
		push  0
		mov eax, 0
		mov ecx, 0
		call eax
		mov a2, eax
	}

	_endthread();
}

std::string toLower(const std::string& str) {
	std::string result = str;
	std::transform(result.begin(), result.end(), result.begin(), ::tolower);
	return result;
}

#include "sha1.hpp"
HANDLE myMutex = NULL;
#define CONSOLE_MODE 1 // 0 OLUNCA CONSOL KAPALI - 1 OLUNCA KONSOL AÇIK OLUYOR.
extern  "C"  __declspec(dllexport) void __cdecl Init()
{
	oCreateFileA = (tCreateFileA)DetourFunction((PBYTE)CreateFileA, (PBYTE)hkCreateFileA); // dosya yol değiştirme ---------------------
#if HOOK_SOURCE_VERSION == 1098
	//oCreateFileCameraA = (tCreateFileA)DetourFunction((PBYTE)0x004E200C, (PBYTE)hkCreateFileCameraA); // kamera dosya yol değiştirme ---------------------
	//*(DWORD*)(0x004E200C) = 0xE8; // for camera oCreateFileCameraA
	*(DWORD*)(0x004E1FFE) = 0x89;	// n3camera için şifreleme
	*(DWORD*)(0x004DBD51) = 0x89;	// n3camera için şifreleme
	*(DWORD*)(0x004DDCB6) = 0x89;	// n3camera için şifreleme
	*(DWORD*)(0x004DE89E) = 0x89;	// n3camera için şifreleme
	*(DWORD*)(0x004DDBD2) = 0x89;	// n3camera için şifreleme
	*(DWORD*)(0x004DDE6D) = 0x89;	// n3camera için şifreleme
	*(DWORD*)(0x004DEB9E) = 0x89;	// n3camera için şifreleme
	*(DWORD*)(0x004DDF46) = 0x89;	// n3camera için şifreleme //açılmicak
#endif

#if CONSOLE_MODE 1
	AllocConsole();
	freopen(xorstr("CONOUT$"), xorstr("w"), stdout);
#endif	

	PatchMulti();

	basePath = getexepath();
	string myname(basePath + xorstr("\\KnightOnLine.exe"));
	if (!_fexists(myname))
	{
		MessageBoxA(NULL, xorstr("KnightOnLine.exe name invalid"), xorstr("Error"), MB_OK | MB_ICONEXCLAMATION);
		Shutdown();
		return;
	}

	uint8_t	multicount = 0;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE) {
		do {
			std::string processName = toLower(entry.szExeFile);
			std::size_t pos = processName.find("knightonline.exe");

			if (pos != std::string::npos) {
				multicount++;
			}
		} while (Process32Next(snapshot, &entry) == TRUE);
	}
	CloseHandle(snapshot);

	if (multicount > 3)
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MultiErrorMsg, NULL, NULL, NULL);

	myname = basePath + xorstr("\\HSACSX.dll");
	if (!_fexists(myname))
	{
		MessageBoxA(NULL, xorstr("\\HSACSX.dll"), xorstr("Need This File In Directory"), MB_OK | MB_ICONEXCLAMATION);
		Shutdown();
		return;
	}

	char nm[100];
	GetModuleBaseNameA(GetCurrentProcess(), GetModuleHandle(NULL), nm, sizeof(nm));
	string _nm(nm);

	g_pMain = new HSACSEngine(basePath + xorstr("\\"));
	hdrReader = new HDRReader(basePath + xorstr("\\"));

	g_pMain->SetOSVersion();
	g_pMain->beginlog();
	char text[1024]; memset(text, 0, 1024);
	sprintf_s(text, "[KK][%x]", (((reinterpret_cast<unsigned long>(g_pMain->Settings->szReg) ^ 0xBB) * 0x94) & 0xCC));
	g_pMain->InsertLog(text);
	sprintf(text, "[SZ1][%x]", (((static_cast<unsigned long>(g_pMain->Settings->ModuleInfo.SizeOfImage) ^ 0xAC) * 0x75) & 93));
	g_pMain->InsertLog(text, false);
	sprintf_s(text, "[SZ2][%x]", (((reinterpret_cast<unsigned long>(g_pMain->Settings->ModuleInfo.lpBaseOfDll) ^ 0xB2) * 0x36) & 20));
	g_pMain->InsertLog(text, false);
	sprintf_s(text, "[SZ3][%x]", (((reinterpret_cast<unsigned long>(g_pMain->Settings->ModuleInfo.EntryPoint) ^ 0xDE) * 0x71) & 78));
	g_pMain->InsertLog(text, false);
	sprintf_s(text, "CSRSS process id: %i", g_pMain->Settings->csrsspid);
	g_pMain->InsertLog(text, false);
	sprintf_s(text, "WLG process id: %i", g_pMain->Settings->winlogonpid);
	g_pMain->InsertLog(text, false);
	sprintf_s(text, "[SM1][%x]", (((reinterpret_cast<unsigned long>(g_pMain->Settings->MyModule) ^ 0x72) * 0x41) & 33));
	g_pMain->InsertLog(text, false);
	sprintf_s(text, "[SM2][%x]", (((static_cast<unsigned long>(g_pMain->Settings->gamethread) ^ 0x67) * 0x88) & 0xE3));
	g_pMain->InsertLog(text, false);

	if (IsWOW64()) {
		g_pMain->InsertLog("(x64) sistem belirlendi");
		g_pMain->InsertLog("Sistem servisi başlatılıyor", false);
		g_pMain->InsertLog("Sistem servisi başarıyla başlatıldı", false);
	}

	g_pMain->InsertLog("Islemci calisma kumesi olusturuldu.");
	g_pMain->InsertLog("Bellek arayuzu entegrasyonu basladi");
	//CheckMemoryRegions();
	g_pMain->InsertLog("Bellek arayuzu entegrasyonu tamamlandi");
	g_pMain->InsertLog("Essiz kimlik olusturuldu");

	sprintf_s(text, "Yansı oluşturuldu [%x]", (((static_cast<unsigned long>(g_pMain->Settings->debugpid) ^ 0x72) * 0x41) & 33));
	g_pMain->InsertLog(text, false);

	g_pMain->InsertLog("AA sistemi aktif edildi", false);
	g_pMain->InsertLog("Haklar alınıyor", false);
	g_pMain->EnableDebugPrivilege();
	g_pMain->InsertLog("Haklar elde edildi", false);

	g_pMain->InsertLog("Sistem ayarları yapılıyor");
	g_pMain->ProtectLocalHooks((char*)"kernel32.dll", (char*)"WriteProcessMemory");
	g_pMain->ProtectLocalHooks((char*)"kernel32.dll", (char*)"ReadProcessMemory");
	g_pMain->ProtectLocalHooks((char*)"kernel32.dll", (char*)"LoadLibraryA");
	g_pMain->ProtectLocalHooks((char*)"kernel32.dll", (char*)"OpenProcess");
	g_pMain->ProtectLocalHooks((char*)"kernel32.dll", (char*)"GetTickCount");
	g_pMain->ProtectLocalHooks((char*)"kernel32.dll", (char*)"QueryPerformanceCounter");
	g_pMain->ProtectLocalHooks((char*)"Winmm.dll", (char*)"timeGetTime");

	g_pMain->SystemSend();
	g_pMain->InsertLog("Sistem segmentleri kontrol ediliyor", false);
	//g_pMain->SystemLog();
	g_pMain->InsertLog("Log sistemi aktif ediliyor", false);
	//SystemPointers();
	g_pMain->InsertLog("Adres yönergeleri aktif ediliyor", false);
	//InterceptFunc(IMAGEBASE + 0x005C96E7, static_cast<void*>(getmygm));
	g_pMain->InsertLog("Kimlik yönetimi aktif ediliyor", false);
	//InterceptFunc(IMAGEBASE + 0x0064BC04, static_cast<void*>(WideScreen));
	g_pMain->InsertLog("Geniş ekran desteği aktif edildi", false);

	LoadCrypto();
	g_pMain->InsertLog("Şifreli uif yükleniyor.");
	pBannedSystem = new BanSystem;
	bool a = pBannedSystem->RegCheck();
	bool b = pBannedSystem->FileCheck();
	g_pMain->InsertLog("PC ban sistemi kontrol ediliyor.");
#if(ANTICHEAT_MODE)
	if (a || b)
	{
		g_pMain->InsertLog("Sorry, you blocked me from performing an unwanted action.", true);
		Shutdown("Sorry, you blocked me from performing an unwanted action.");
		return;
	}
#endif

#if CONSOLE_MODE 1
	SHA1 sha;
	string itemorg = md5(sha.from_file(g_pMain->m_BasePath + xorstr("Data\\item_org_us.tbl")));
	string skillmagic = md5(sha.from_file(g_pMain->m_BasePath + xorstr("Data\\Skill_Magic_Main_us.tbl")));
	string zones = md5(sha.from_file(g_pMain->m_BasePath + xorstr("Data\\Zones.tbl")));
	string itemsell_table = md5(sha.from_file(g_pMain->m_BasePath + xorstr("Data\\itemsell_table.tbl")));

	printf("itemorg %s\n", itemorg.c_str());
	printf("skillmagic %s\n", skillmagic.c_str());
	printf("zones %s\n", zones.c_str());
	printf("itemsell_table %s\n", itemsell_table.c_str());
#endif

	InterceptFunc(0x00510DE7, (void*)BotNameHpTarget);
	InterceptFunc(0x00945CD4, (void*)haganYNG);
	InterceptFunc(0x00515B08, (void*)HaganYNGSiker);
	InterceptFunc(0x0053425E, (void*)PartySystemTest);
	//InterceptFunc(0x0055F152, (void*)UIFHOOK);
#if HOOK_SOURCE_VERSION == 1098
	*(uint8*)0x0057F312 = 0xEB;
	//YENİ EKLENEN
	* (uint8*)0x0054B259 = 0x00;
	* (uint8*)0x0054B2CD = 0x00;
	* (uint8*)0x0054B36B = 0x00;
	* (uint8*)0x0054B4C4 = 0x00;
	* (uint8*)0x0054B4EA = 0x00;
	* (uint8*)0x0053AC74 = 0x00;
	* (uint8*)0x00504FA0 = 0x00;
	* (uint8*)0x0052EA45 = 0x00;
	* (uint8*)0x0053F69F = 0x00;
	* (uint8*)0x00540588 = 0x00;
	* (uint8*)0x00562E06 = 0x00;
	* (uint8*)0x00671C02 = 0x00;
	* (uint8*)0x0054B439 = 0x00;
	* (uint8*)0x00782956 = 0x00;
	* (uint8*)0x0054B259 = 0x00;

	* (uint8*)0x0066D3EA = 0xEB;
	* (uint8*)0x005CAD57 = 0xEB;
	* (uint8*)0x0060DE3E = 0xEB;
	* (uint8*)0x00608769 = 0xEB;
	* (uint8*)0x00608E49 = 0xEB;
	* (uint8*)0x0060DE3E = 0xEB;
	* (uint8*)0x00634369 = 0xEB;
	* (uint8*)0x009A7F6A = 0xEB;
	* (uint8*)0x009A8012 = 0xEB;

	*(uint32*)0x0061C373 = 0xB7E9;
	*(uint32*)0x004B9F25 = 0x7A120;
	*(uint32*)0x00617AB2 = 0x7A120;
	*(uint32*)0x007CBD89 = 0x7A120;
	*(uint32*)0x006173B3 = 0x7A120;
	*(uint32*)0x006178D4 = 0x7A120;
	*(uint32*)0x00585F68 = 0x00EB;
#endif
	UIMain();
	g_pMain->InsertLog("ClientPlugin aktif edildi.");
}

void OpenConsoleWindow()
{
	AllocConsole();

	freopen("CONOUT$", "a", stdout);
	freopen("CONIN$", "r", stdin);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH) {

		//HINSTANCE hGetProcIDDLL = LoadLibrary("dinput8.dll");

		//if (hGetProcIDDLL == NULL) {
		//	exit(0);// std::cout << "cannot locate the .dll file" << std::endl;
		//}
		///*else {
		//	std::cout << "it has been called" << std::endl;
		//	return -1;
		//}*/
		DisableThreadLibraryCalls(hModule);
		
		/*SPLASH m_splash[6];
		for (int i = 0; i < 6; i++)
			m_splash[i].Init(NULL, hModule, IDB_BITMAP1 + i);
		for (int i = 0; i < 6; i++)
		{
			m_splash[i].Show();
			Sleep(600);
			m_splash[i].Hide();
		}*/
		Init();
	}
	return TRUE;
}

HCRYPTPROV hCryptProv = NULL;
HCRYPTHASH hHash = NULL;
DWORD Func_Load = 0;
const std::string& fileStr = "";


bool has_suffix(const std::string& str, const std::string& suffix) { return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0; }

void __stdcall uifEncrpytOperation()
{
	xCodeTMP = fileStr;

	if (!fileStr.empty() && has_suffix(fileStr, xorstr(".hsacsx")))
	{
		//printf("UIF HOOK %s\n", fileStr.c_str());
		//g_pMain->InsertLog(fileStr.c_str(), false);
		xCodeTMP = string_format(xorstr("Misc\\river\\hsacsx_%d.dxt"), _PID);
		PBYTE pbBuffer = NULL;
		HANDLE hSourceFile = CreateFileA(fileStr.c_str(), FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		HANDLE hDestinationFile = CreateFileA(xCodeTMP.c_str(), FILE_WRITE_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE != hSourceFile && INVALID_HANDLE_VALUE != hDestinationFile)
		{
			bool fEOF = false;
			DWORD dwCount;
			LARGE_INTEGER size;
			GetFileSizeEx(hSourceFile, &size);
			LONG len = (size.u.HighPart << (LONG)32) | size.u.LowPart;
			DWORD dwBlockLen = len % 2 == 0 ? 32 : 31;
			DWORD dwBufferLen = dwBlockLen;
			if ((pbBuffer = (PBYTE)malloc(dwBufferLen)))
			{
				if (ReadFile(hSourceFile, pbBuffer, 4, &dwCount, NULL))
					WriteFile(hDestinationFile, pbBuffer, dwCount, &dwCount, NULL);

				ZeroMemory(pbBuffer, dwBufferLen);
				dwCount = 0;
				do
				{
					if (ReadFile(hSourceFile, pbBuffer, dwBlockLen, &dwCount, NULL))
					{
						if (dwCount < dwBlockLen)
							fEOF = TRUE;
						if (CryptDecrypt(hKey, 0, 1, 0, pbBuffer, &dwCount))
							WriteFile(hDestinationFile, pbBuffer, dwCount, &dwCount, NULL);
					}
				} while (!fEOF);
			}
		}

		if (pbBuffer)
			free(pbBuffer);

		if (hSourceFile)
			CloseHandle(hSourceFile);

		if (hDestinationFile)
			CloseHandle(hDestinationFile);
	}
}

void __declspec(naked) hkLoad()
{
	__asm {
		mov edx, [esp + 4]
		mov fileStr, edx
		pushad
		pushfd
		call uifEncrpytOperation
		popfd
		popad
		mov edx, offset xCodeTMP
		mov[esp + 4], edx
		jmp Func_Load
	}
}

void __fastcall RemoveTMP()
{
	std::string file = string_format(xorstr("Misc\\river\\hsacsx_%d.dxt"), _PID);
	std::remove(file.c_str());
}

void __declspec(naked) hkEndOfLoad()
{
	__asm {
		pushad
		pushfd
	}

	__asm {
		popfd
		popad
		call KO_UIF_FILE_LOAD_ORG_CALL
		pushad
		pushfd
		call RemoveTMP
		popfd
		popad
		jmp KO_UIF_FILE_LOAD_RET
	}
}

void LoadCrypto()
{
	VIRTUALIZER_START
	_PID = GetCurrentProcessId();
	if (CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0))
	{
		if (CryptCreateHash(hCryptProv, 0x8004, 0, 0, &hHash))
		{
			if (CryptHashData(hHash, (BYTE*)xorstr("(A;dq1DPVFgVs1Aez$VS3R0hge@NvM_TJvblD4.af0h@r4bUzp"), 29, CRYPT_USERDATA))  // Uif cryptosu buradan değişiyor. Sonu böyle olmak zorunda "%!@%51"
			{
				if (!CryptDeriveKey(hCryptProv, 0x6801, hHash, 0x00800000, &hKey))
					TerminateProcess(GetCurrentProcess(), 0);
			}
		}
	}
	Func_Load = (DWORD)DetourFunction((PBYTE)KO_UIF_FILE_LOAD, (PBYTE)hkLoad);
	DetourFunction((PBYTE)(KO_UIF_FILE_LOAD + 0x56), (PBYTE)hkEndOfLoad);
	VIRTUALIZER_END
}