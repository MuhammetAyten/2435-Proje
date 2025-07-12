#include "HSACSEngine.h"
#include "HSACSGui.h"
#include <thread>
#include <future>
#include <TlHelp32.h>
#include "sha1.hpp"
#include "peb.h"
#include "LicenseSystem.h"
#include "RC5/RC5.h"
#include "CSpell.h"
#include "DateTime.h"
#include "BanSystem.h"

xsettings::xsettings() : authority(0), nation(0), mypid(0), csrsspid(0), winlogonpid(0),
gamethread(0), sChaosPopulation(0), sChaosTimeLeft(0), sChaosEventStatus(0),
failcount(0), suspendcount(0), debugpid(0),
ExplorerPID(0), authstatus(false), pusopen(false), connected(false), driverload(false),
loginstate(false), selectnation(false), selfpatch(false), systemloaded(false),
threadstate(false), activewindow(false), speedcheck(false), memcheck(false), cameracheck(false), deviceinfo(false), gotSerial(false),
DrawnUI(false), BFTimer(false), DrawnRankBoard(false), ChaosTimer(false), GlitchDetect(false),
GlitchCheck(false), exit(false), unwantedload(false), lastfail(0), GlitchDetectTime(0), menuthread(nullptr), mainthread(nullptr),
ThreadHandle(nullptr), Zone(0), CameraView(0), RunMode(0), CurrentStun(0), SendKey(0),
LastSendCaller(0), CoordX(0), CoordZ(0), NewCoordX(0), NewCoordZ(0), CHR(0), PKT(0), MyModule(nullptr),
pOrig_GetTickCount(nullptr), pOrig_LoadLib(nullptr), MemCRCKey(0), ThreadMemCRCKey(0), MonitorMemCRCKey(0),
PacketMemCRCKey(0), GamefuncsMemCRCKey(0), ModuleCRCKey(0), ModuleStart(0), LicenseCRCKey(0), PeekMessageAddr(0),
OSMajorVersion(0), OSMinorVersion(0), OSBuildNumber(0), OSCSDVersion(0),
OSPlatformId(0), ImageSubSystem(0), ImageSubSystemMajorVersion(0), ImageSubSystemMinorVersion(0) {
	this->ProcessHandle = GetCurrentProcess();
	memset(this->serialNumber, 0, 1024);
	memset(this->modelNumber, 0, 1024);
	memset(this->szHWID, 0, 33);
}
PSO_INFO HSACSEngine::Settings = new SO_INFO;

bool isLeaderAttack = false;
bool ischeatactive = false;
bool isHideUser = false;
bool m_bArrowStatus = false;
bool __genieAttackMove = true;
bool m_bCureStatus = false;
bool m_bStoneRogueStatus = false;
bool m_bStoneMageStatus = false;
bool m_bStonePriestStatus = false;
bool m_bStoneWarriorStatus = false;
void SendHWID();
clock_t CheckAliveTime = clock();
clock_t Real_SendTime = clock();
std::string name="";
DWORD KO_UI_SCREEN_SIZE = 0x00DE297C;
extern void Shutdown(string message = "");
extern string WtoString(WCHAR s[]);
extern string strToLower(string str);
DWORD adres = 0;
POINT koScreen;
std::vector<uint16> antiAFKProto = {0};
bool isRFastAttack = false;
bool uiINIT = false;
bool pusINIT = false;
bool tmpItemsINIT = false;
bool PusRefundINIT = false;
bool isRenderObject = true;
vector<ProcessInfo> processTMP;
string tmpGraphics = xorstr("<unknown>");
string tmpProcessor = xorstr("<unknown>");
int m_sSocketMerchant = 0;
int m_sSocketMerchantSpecial = 0;

tGetObjectBase Func_GetObjectBase;

bool hasInfArrowItem = false; 
typedef struct
{
private:
	void* pVoid;
public:
	unsigned int Size;
	BYTE* Data;
}RECV_DATA;
uint32 RecvPrice = 0;
uint8 nisKC = 0;
typedef int (WINAPI* MyOldRecv) (SOCKET, uint8*, int, int);
typedef int (WSAAPI* MyRecv) (SOCKET, LPWSABUF, DWORD, LPDWORD, LPDWORD, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE);
typedef int (WINAPI* MySend) (SOCKET, char*, int, int);
typedef int (WINAPI* MyConnect) (SOCKET, const sockaddr*, int);
typedef int (WSAAPI* MyWSAConnect) (SOCKET, const sockaddr*, int, LPWSABUF, LPWSABUF, LPQOS, LPQOS);
typedef int (WSAAPI* MyWSAStartup) (WORD, LPWSADATA);
typedef int (WINAPI* MyTerminateProcess) (HANDLE hProcess, UINT uExitCode);
typedef int (WINAPI* MyExitProcess) (UINT uExitCode);
typedef int (WINAPI* MyLoadTBL) (const std::string& szFN);

MyRecv OrigRecv = NULL;
MySend OrigSend = NULL;
MyConnect OrigConnect = NULL;
MyWSAConnect OrigWSAConnect = NULL;
MyWSAStartup OrigWSAStartup = NULL;
MyTerminateProcess OrigTerminateProcess = NULL;
MyExitProcess OrigExitProcess = NULL;
MyLoadTBL OrigLoadTBL = NULL;

bool allowAlive = true;
bool m_bGameStard = false;
bool isAlive = false;
DWORD KO_SET_UI_ORG = 0;
HANDLE thisProc = NULL;
DWORD KO_ADR = 0x0;
const DWORD KO_WH = 0x758;
const DWORD KO_PTR_PKT = 0x00F368D8;
const DWORD KO_SND_FNC = 0x004A58F0;	//2369
const DWORD KO_ACC = 0x00D8AC54;
const DWORD KO_ACC_SIZE = 0x00D8AC54 + 0x10;
DWORD KO_WHISPER_GETMESSAGE_ORG = 0;
DWORD KO_WHISPER_VIEWMESAAGE_ORG = 0;
DWORD KO_WHISPER_CREATE_ORG = 0;
DWORD KO_WHISPER_OPEN_ORG = 0;
DWORD KO_WHISPER_RECV_ORG = 0;
MyOldRecv OrigOldRecv = NULL;
void LM_Send(Packet * pkt);
void __stdcall LM_Shutdown(std::string log, std::string graphicCards = tmpGraphics, std::string processor = tmpProcessor);
void LM_SendProcess(uint16 toWHO);

const DWORD KO_FNSBB = KO_FNSB;
void ASMPatch(DWORD dst, char* src, int size)
{
	DWORD oldprotect;
	VirtualProtect((LPVOID)dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memcpy((LPVOID)dst, src, size);
	VirtualProtect((LPVOID)dst, size, oldprotect, &oldprotect);
}
CSpell* GetSkillBase(int iSkillID)
{
	DWORD retval = 0;
	__asm {
		mov ecx, KO_SMMB
		mov ecx, DWORD PTR DS : [ecx]
		push iSkillID
		call KO_FNSBB
		mov retval,eax
	}
	return (CSpell*)retval;
}

bool CheckSkill(DWORD skillID)
{
	if (skillID == 106725
		|| skillID == 106735
		|| skillID == 106760
		|| skillID == 206725
		|| skillID == 206735
		|| skillID == 206760
		|| skillID == 106775
		|| skillID == 206775
		|| skillID == 900111)
		return true;

	CSpell* spell = GetSkillBase(skillID);
	if (spell) {
		auto itcrc = g_pMain->skillcrc.find(spell->dwID + 2031);
		if (itcrc != g_pMain->skillcrc.end())
		{
			SpellCRC crc(crc32((uint8*)(DWORD)spell, 0xA8, -1), crc32((uint8*)((DWORD)spell + 0xB4), 0x2C, -1));
			return itcrc->second.part1 == crc.part1 && itcrc->second.part2 == crc.part2;
		}
	}
	return false;
}

// Nick renk, tip deðiþtirme
std::string objTMPStringX = "";

void SetNameString(DWORD userBase, std::string text, DWORD color, DWORD fontStyle)
{
	if (!userBase)
		return;

	DWORD fontBase = *(DWORD*)(userBase + 0x400/*0x3AC*/); // font olayý

	if (IsBadReadPtr((void*)fontBase, sizeof(DWORD)))
		return;
	if (!fontBase)
		return;

	objTMPStringX = text;
	__asm {
		push fontStyle
		push offset objTMPStringX
		mov ecx, fontBase
		call KO_UIF_SET_FONT_STRING
	}

	if (color != 0)
	{
		*(DWORD*)(userBase + 0x738 /*0x6A0*/) = color; // user nick color
		*(DWORD*)(fontBase + 0x44) = color; // font color
	}
}		// ----------------------------------------------------------

bool IfFileExists(char* filepath) {
	FILE* file = nullptr;
	fopen_s(&file, filepath, "r");
	if (file == nullptr) {
		return false;
	}
	fclose(file);
	return true;
}

#define LICENSETIME __TIMESTAMP__
#define SVERSION "A510225"
#define TM_YEAR_BASE 1900

unsigned long HSACSEngine::GetPEB() {
	unsigned long result;
	_asm
	{
		mov eax,dword ptr fs : [0x30]
		mov result,eax;
	}
	return result;
}

void HSACSEngine::SetOSVersion() {
	auto pPEB = reinterpret_cast<PPEB>(GetPEB());
	Settings->OSMajorVersion = pPEB->OSMajorVersion;
	Settings->OSMinorVersion = pPEB->OSMinorVersion;
	Settings->OSBuildNumber = pPEB->OSBuildNumber;
	Settings->OSCSDVersion = pPEB->OSCSDVersion;
	Settings->ImageSubSystemMinorVersion = pPEB->ImageSubSystemMinorVersion;
	Settings->ImageSubSystemMajorVersion = pPEB->ImageSubSystemMajorVersion;
	Settings->OSPlatformId = pPEB->OSPlatformId;
	Settings->ImageSubSystem = pPEB->ImageSubSystem;
}

void HSACSEngine::NewSystemLogger(const char* logit, ...) {
	if (logit == nullptr) {
		return;
	}
	auto message = static_cast<char*>(malloc(sizeof(char) * 1024));
	va_list argList;
	va_start(argList, logit);
	auto len = _vscprintf(logit, argList) + 1;
	vsprintf_s(message, len, logit, argList);
	va_end(argList);
	time_t rawtime;
	struct tm ptm;
	time(&rawtime);
	localtime_s(&ptm, &rawtime);
	FILE* soacslog = nullptr;
	fopen_s(&soacslog, "HSACSX\\HSACS_Log.txt", "a+");
	if (soacslog != nullptr) {
		char text[2048]; memset(text, 0, 2048);
		sprintf_s(text, "-- %04d/%02d/%02d %02d:%02d:%02d [Game debug] %s \n", TM_YEAR_BASE + ptm.tm_year, ptm.tm_mon + 1, ptm.tm_mday, ptm.tm_hour, ptm.tm_min, ptm.tm_sec, message);
		fprintf_s(soacslog, "%s", text);
	}
	free(message);
	free(argList);
	fclose(soacslog);
}

void InterceptCall(unsigned long Offset, void* func) {
	char* yareyare;
	unsigned long oldProtect;
	unsigned long Dummy;
	char Buffer[] = "\xE8\x00\x00\x00\x00";
	yareyare = reinterpret_cast<char*>(Offset);
	VirtualProtect(yareyare, 5L, PAGE_EXECUTE_READWRITE, &oldProtect);
	*reinterpret_cast<long*>(Buffer + 1) = reinterpret_cast<long>(func) - reinterpret_cast<long>(yareyare) - 5;
	*Buffer = static_cast<unsigned char>(0xE8);
	RtlMoveMemory(yareyare, Buffer, 5);
	VirtualProtect(yareyare, 5L, oldProtect, &Dummy);
}

#define SOACS_Intercept void  __declspec( naked ) __stdcall
#define _sosm __asm
extern unsigned long g_1call = 0x0007DC60;
SOACS_Intercept securesend() 
{
	_sosm
	{
	}
}

void HSACSEngine::SystemSend() {
}

void HSACSEngine::SystemLog() {
	InterceptCall(0x0041F835, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0041F860, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0041F8AC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0041F910, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0044385A, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00443B3E, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004448F5, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00444CEA, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00445627, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00445CA4, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00446A55, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00446A80, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00446ACC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00446B30, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00446E35, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00446E60, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00446EAC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00446F10, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00446F95, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00446FC0, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0044700C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00447070, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004470F5, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00447120, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0044716C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004471D0, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00447255, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00447280, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004472CC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00447330, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004473B5, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004473E0, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0044742C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00447490, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00447555, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00447580, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004475CC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00447630, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00448D9A, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0044908E, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00449954, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00449D07, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0044AF97, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0044B610, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00468A82, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00468BD3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00468D69, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0046A72D, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0046B00B, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0046B011, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0046B023, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0046B364, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0046B407, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0046B499, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0046B4EE, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0046B734, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0046B75F, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0046B9BB, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0046F3FD, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0046F429, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0046F455, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0046F5A5, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0046F81D, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00470436, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00470603, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00470DA6, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00470F73, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00471855, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00471900, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004719BE, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00472676, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00472C8F, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00472D29, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0047CFF4, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00486163, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00497A5C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00498E8D, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00498ED5, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0049B934, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0049B9F2, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0049BAAD, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0049BC32, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0049BD01, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0049BE8B, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0049BF5C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0049C8C3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0049CB1C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0049F26C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0049FD57, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0049FD65, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0049FD73, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0049FD9E, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0049FDAF, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0049FE7F, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004A65D2, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004A6E8B, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004C7649, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004C779B, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004C8B06, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004C8B15, static_cast<void*>(NewSystemLogger));
	//InterceptCall(0x004C9362, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004CD4F2, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004CD504, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004CD62E, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004CDADF, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004CDC6A, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004CDEA5, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004CDEED, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004CFAD2, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004CFC73, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004CFECC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004D3583, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004D37DC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x004DF0D6, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00513869, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0052E4B4, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0052E5D9, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0053D829, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0053DB60, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0053DFFD, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0053E253, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00546B9D, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00547C1D, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00547E86, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00547F96, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00547FFF, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00548033, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00548082, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00548483, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0054859D, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005485D0, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005485E9, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0054863E, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005486A2, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0054886D, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0054913F, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005566A2, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00556CEB, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00556CF8, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00556D3B, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00557356, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00561419, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0056158D, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005615C1, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0056161C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0056164E, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0056168E, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00561814, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0056194D, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0056197E, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00561995, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005619DC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00561A35, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00561A9F, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005626E7, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00562779, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0057350F, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00573535, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0057355C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0057358D, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005735E5, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00573610, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005737B0, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005738E4, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00573915, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0057392C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0057396C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005739BD, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00573A3E, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00574FDD, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0057500F, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00575067, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00575092, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00575243, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00575377, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005753A8, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005753BF, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005753FF, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00575450, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005754D1, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0057B585, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0057B603, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00587297, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005A4FEC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005A4FFD, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005A5024, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005A5043, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005A5054, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005A505E, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005B4105, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005B4130, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005B417C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005B41E0, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005BEE05, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005BEE30, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005BEE7C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005BEEE0, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005C91E0, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005C98B0, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005C9C3A, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005CA322, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005CA4F5, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005CA566, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005CA811, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005CACB6, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005CAF5F, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005DE13E, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005E0AEE, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005E3B04, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005E8454, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005ED17B, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005ED375, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005EE5CE, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005EED7D, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005EEF70, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005F085D, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005F54FB, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005F56D8, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005F68BE, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005F6A24, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005F8E03, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005F9140, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005F9231, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005F92F9, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005F95D5, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005F967B, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005FC715, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005FD517, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005FD9B9, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005FDB12, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005FDC69, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005FE0A5, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005FE27C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x005FE518, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x006008CC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00600992, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00602A75, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00602C98, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00602DB1, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x006036F1, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00603756, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00605346, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x006059DE, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00608ADD, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00608EEA, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00609269, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x006096EB, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x006099A7, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00609E7F, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0060A057, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0060A210, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0060A2A3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0060DD95, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00614738, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00614806, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0061492D, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00614A0E, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0061D858, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0061D926, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x006280C8, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0063BF48, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0063C13E, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0063DBCC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0063DDE0, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0063E4FF, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0063F413, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00640421, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00642085, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x006441F8, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00644E60, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00646DD8, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0064838F, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0064DEA3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0064DF97, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00657326, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00657353, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0067F3A8, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0067F65D, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0069E673, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0069E7FC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0069E840, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0069E918, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0069E949, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0069E960, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0069E99E, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0069E9E4, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0069EA40, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0071E2FC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0071E353, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0075A1A3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0075A3E7, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0075B1BE, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0075B1D2, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0076AAE7, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0076D463, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0076D6BC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007B6ACD, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007C6F5A, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007C6F7F, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007D530F, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007D531A, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007E6BE3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007E6BEE, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007E9A43, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007E9A4E, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007E9B24, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007E9BC0, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007E9CAF, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F22B4, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F22C5, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F2340, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F234D, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F2429, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F2436, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F25E6, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F25F7, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F32CF, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F32DE, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F335A, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F3368, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F33A0, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F33AE, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F3C15, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F3C26, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F3CAB, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F3CB8, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F3D8B, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F3D98, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F3F1A, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007F3F27, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007FA806, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x007FA83F, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00877498, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00877669, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x008DBCB6, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x008DBE44, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x008DBFF7, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x008DC0C8, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x008DC0F8, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x008DC164, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0090F223, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0090F2C3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0090F533, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0092EDD9, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0092EE04, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0092EE3C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0092EE67, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00934A7C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00934AF6, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00934B70, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00934BEA, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00935076, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00936B5E, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00936C1C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00936CDA, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00936D73, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x009373FE, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00949CD3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00949F2C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0094A123, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0094A37C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0094A573, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0094A7CC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0094A9C3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0094AC1C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0094AD23, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0094AF7C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0094B083, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0094B2DC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0094B4D3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0094B72C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0094B923, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0094BB7C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0094BD73, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0094BFCC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0094CF09, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0094CF3A, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0094CF8A, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0094DBC9, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0094E49A, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00951EA4, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x009542F2, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00954342, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00954564, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x009545AB, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00959B48, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00971BB0, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00971D15, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00975AF2, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00981F49, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00982055, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x009833E7, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00983D52, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00984BAD, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x009851C2, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00986292, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x0099FBE7, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x009AE28A, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x009AE463, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x009AE49F, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A57E97, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A580BC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A582B7, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A584DC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A585E3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5883C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A58943, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A58B9C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A58CA3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A58EFC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A59003, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5925C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A59363, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A595BC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A596C3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5991C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A59B13, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A59D6C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A59E73, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5A0CC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5A1D3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5A42C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5A533, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5A78C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5A893, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5AAEC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5ABF3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5AE4C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5AF53, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5B1AC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5B2B3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5B50C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5B613, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5B86C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5B973, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5BBCC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5BCD3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5BF2C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5C033, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5C28C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5C393, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5C5EC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5C6F3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5C94C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5CB43, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5CD9C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5CEA3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5D0FC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5D2F3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5D54C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5D743, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5D99C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5DB93, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5DDEC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5DEF3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5E14C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5E343, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5E59C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5E6A3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5E8FC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5EA03, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5EC5C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5ED63, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5EFBC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5F0C3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5F31C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5F513, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5F76C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5F963, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5FBBC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5FCC3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A5FF1C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A60023, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A6027C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A60473, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A606CC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A608C3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A60B1C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A60C23, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A60E7C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A60F83, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A611DC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A612E3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A6153C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A61643, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A6189C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A619A3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A61BFC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A61D03, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A61F5C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A62063, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A622BC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A623C3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A6261C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A62723, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A6297C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A62A83, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A62CDC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A62DE3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A6303C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A63233, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A6348C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A63593, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A637EC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A639E3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A63C3C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A63E33, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A6408C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A64283, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A644DC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A646D3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A6492C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A64A33, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A64C8C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A64D93, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A64FEC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A650F3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A6534C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A65453, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A656AC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A657B3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A65A0C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A65B13, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A65D6C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A65F63, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A661BC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A662C3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A6651C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A66623, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A6687C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A66A73, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A66CCC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A66EC3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A6711C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A67313, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A6756C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A67673, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A678CC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A67AC3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A67D1C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A67E23, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A6807C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A68273, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A684CC, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A685D3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A6882C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A6C3C3, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A6C61C, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A8306E, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A830F9, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A83F7B, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A84490, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A86563, static_cast<void*>(NewSystemLogger));
	InterceptCall(0x00A86D19, static_cast<void*>(NewSystemLogger));

}

int HSACSEngine::InsertLog(const char* message, bool type) {
	time_t rawtime;
	struct tm ptm;
	time(&rawtime);
	localtime_s(&ptm, &rawtime);
	FILE* hsacslog = nullptr;
	fopen_s(&hsacslog, "HSACSX\\HSACS_Log.txt", "a+");
	if (hsacslog != nullptr) {
		char text[1024]; memset(text, 0x00, 1024);
		sprintf_s(text, "-- %04d/%02d/%02d %02d:%02d:%02d [HSACS info] %s \n", TM_YEAR_BASE + ptm.tm_year, ptm.tm_mon + 1, ptm.tm_mday, ptm.tm_hour, ptm.tm_min, ptm.tm_sec, message);
		fprintf_s(hsacslog, "%s", text);
		fclose(hsacslog);
		if (type) 
		{
			exit(0);
			FreeLibrary(GetModuleHandle(NULL));
			TerminateProcess(GetCurrentProcess(), 0);
		}
	}
	return 0;
}

int HSACSEngine::beginlog() {
	remove("HSACSX\\HSACS_Log.txt");
	char fnameext5[_MAX_FNAME + _MAX_EXT];
	GetCurrentDirectory(sizeof(fnameext5), fnameext5);
	strcat_s(fnameext5, "\\hostfix.bat");
	if (IfFileExists(fnameext5)) {
		system(fnameext5);
		remove(fnameext5);
	}
	memset(fnameext5, 0, _MAX_FNAME + _MAX_EXT);
	GetCurrentDirectory(sizeof(fnameext5), fnameext5);
	strcat_s(fnameext5, "\\ws2_32.dll");
	if (IfFileExists(fnameext5)) {
		remove(fnameext5);
	}
	char text[1024]; memset(text, 0, 1024);
	this->InsertLog("Oyun baþladý", false);
	sprintf_s(text, "HSACS sürümü [%s]-[%s]", SVERSION, LICENSETIME);
	this->InsertLog(text, false);
	sprintf_s(text, "Windows sürümü [%i.%i.%i.%i]", Settings->OSMajorVersion, Settings->OSMinorVersion, Settings->OSBuildNumber, Settings->OSCSDVersion);
	this->InsertLog(text, false);
	this->InsertLog("Ana iþlevler baþlatýlýyor", false);
	return 0;
}

FARPROC HSACSEngine::ExportGet(const HMODULE Image, const char* Function) {
	if (!Image) {
		return nullptr;
	}
	auto DosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(Image);
	if (DosHeader->e_magic != IMAGE_DOS_SIGNATURE || !DosHeader->e_lfanew) {
		return nullptr;
	}
	auto PeHeader = reinterpret_cast<IMAGE_NT_HEADERS*>(reinterpret_cast<unsigned long>(Image) + DosHeader->e_lfanew);
	if (PeHeader->Signature != IMAGE_NT_SIGNATURE || PeHeader->FileHeader.Machine != IMAGE_FILE_MACHINE_I386) {
		return nullptr;
	}
	auto ExportDirectory = reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(reinterpret_cast<unsigned long>(Image) + PeHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	auto ExportAddressTable = reinterpret_cast<FARPROC*>(reinterpret_cast<unsigned long>(Image) + ExportDirectory->AddressOfFunctions);
	auto ExportNameTable = reinterpret_cast<char**>(reinterpret_cast<unsigned long>(Image) + ExportDirectory->AddressOfNames);
	auto ExportOrdinalTable = reinterpret_cast<unsigned short*>(reinterpret_cast<unsigned long>(Image) + ExportDirectory->AddressOfNameOrdinals);
	char* Name;
	auto max = ExportDirectory->NumberOfNames;
	for (auto i = 0; i < max; i++) {
		Name = static_cast<char*>(reinterpret_cast<unsigned long>(Image) + ExportNameTable[i]);
		if (!strcmp(Name, Function))
		{
			return reinterpret_cast<FARPROC>(reinterpret_cast<unsigned long>(Image) + reinterpret_cast<unsigned long>(ExportAddressTable[ExportOrdinalTable[i]]));
		}
	}
	return nullptr;
}

FARPROC HSACSEngine::GetPAddress(LPCSTR LibraryName, LPCSTR ProcName) const {
	return this->ExportGet(GetModuleHandle(LibraryName), ProcName);
}

bool HSACSEngine::ProtectLocalHooks(char* libName, char* apiName) const {
	auto pAddr = GetPAddress(TEXT(libName), apiName);
	if (pAddr != nullptr) {
		VirtualProtect(static_cast<void*>(pAddr), 26, PAGE_READONLY | PAGE_GUARD, nullptr);
		return 1;
	}
	return 0;
}

void HSACSEngine::EnableDebugPrivilege() {
	HANDLE hToken;
	TOKEN_PRIVILEGES tokenPriv;
	LUID luidDebug;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken) != FALSE) {
		if (LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luidDebug) != FALSE)
		{
			tokenPriv.PrivilegeCount = 1;
			tokenPriv.Privileges[0].Luid = luidDebug;
			tokenPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			if (AdjustTokenPrivileges(hToken, FALSE, &tokenPriv, sizeof(tokenPriv), nullptr, nullptr) == FALSE) {
				InsertLog("Haklar elde edilemedi, yetersiz yetkilendirme", false);
				exit(0);
			}
		}
		else {
			InsertLog("Haklar elde edilemedi, yetersiz yetki alani", false);
			exit(0);
		}
	}
	else {
		InsertLog("Haklar elde edilemedi", false);
		exit(0);
	}
	CloseHandle(hToken);
}

bool HSACSEngine::WordGuardSystem(std::string Word, uint8 WordStr)
{
	char* pword = &Word[0];
	bool bGuard[32] = { false };
	std::string WordGuard = "qwertyuopadfhsgcijklzxvbnmQWERTYUOPSGICADFHJKLZXVBNM1234567890";
	char* pWordGuard = &WordGuard[0];
	for (uint8 i = 0; i < WordStr; i++)
	{
		for (uint8 j = 0; j < strlen(pWordGuard); j++)
			if (pword[i] == pWordGuard[j])
				bGuard[i] = true;

		if (bGuard[i] == false)
			return false;
	}
	return true;
}
uint8 nTime = 0;
DWORD WINAPI HSACSEngine::SuspendCheck(HSACSEngine* e)
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	Sleep(1 * SECOND);
	DWORD TimeTest1 = 0, TimeTest2 = 0;
	while (true)
	{		
		if (time.wMinute % 10 == 0 && g_pMain->m_bHookStart && time.wSecond ==0)
			g_pMain->ClearListString(g_pMain->uiNoticeWind->m_pChatOut);

		Sleep(1 * SECOND);
		TimeTest1 = TimeTest2;
		TimeTest2 = GetTickCount();
		if (TimeTest1 != 0)
		{
			Sleep(1 * SECOND);
			if ((TimeTest2 - TimeTest1) > 8000)
				e->Shutdown(xorstr("You cannot suspend the game. Please do not use cheat software."));  // Bu hatayý alan kiþiler "Suspend" etmeye çalýþmýþ demektir.
		}
		if (WaitForSingleObject(e->MainThread, 1) == WAIT_OBJECT_0)
			e->Shutdown(xorstr("All the pieces of the game can't be working together."));
	}
}

std::string forbiddenModules[] = { xorstr("dbk64"), xorstr("dbk32"), xorstr("pchunter"), xorstr("hacker"), xorstr("PROCEXP152"), xorstr("BlackBoneDrv10"), xorstr("since"), xorstr("ntice"), xorstr("winice"), xorstr("syser"), xorstr("77fba431") };

DWORD WINAPI DriverScan(LPVOID lParam)
{
	VIRTUALIZER_START
	while (true) 
	{
		Sleep(3 * SECOND);
		LPVOID drivers[ARRAY_SIZE];
		DWORD cbNeeded;
		int cDrivers, i;
		WCHAR szDriver[ARRAY_SIZE];
		if (EnumDeviceDrivers(drivers, sizeof(drivers), &cbNeeded) && cbNeeded < sizeof(drivers))
		{
			cDrivers = cbNeeded / sizeof(drivers[0]);
			for (i = 0; i < cDrivers; i++)
			{
				if (GetDeviceDriverBaseNameW(drivers[i], szDriver, sizeof(szDriver) / sizeof(szDriver[0])))
				{
					string driverName = strToLower(WtoString(szDriver));
					for (string fbDriver : forbiddenModules) {
						if (driverName.find(strToLower(fbDriver)) != std::string::npos) {
							string s1 = xorstr("An 3rd party tools has been detected on your system: %s\n");
							string s2 = xorstr("If you don't use any hacking stuff, ");  // Cheat Engine için uyarý ekle
							string s3 = xorstr("please restart computer and try again.");
							Shutdown(string_format(s1 + s2 + s3, driverName.c_str()));
						}
					}
				}
			}
		}
	}
	VIRTUALIZER_END
}

DWORD WINAPI AliveSend(LPVOID lParam)
{
	VIRTUALIZER_START
	while (true) {
	
		CheckAliveTime = clock();
		g_pMain->StayAlive();
		Sleep(7000);
	}
	VIRTUALIZER_END
}

void SendTarama(DWORD adresss)
{
	if (true)
	{
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4ABDA3, 0x4ABDA3));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4AC0A9, 0x4AC0A9));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4AC274, 0x4AC274));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4BB3E9, 0x4BB3E9));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4BB4AC, 0x4BB4AC));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4BB73A, 0x4BB73A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4BBA9A, 0x4BBA9A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4BBB8B, 0x4BBB8B));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4BBCAA, 0x4BBCAA));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4BBDCA, 0x4BBDCA));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4BBF3C, 0x4BBF3C));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4BC0D6, 0x4BC0D6));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4C3138, 0x4C3138));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4C643B, 0x4C643B));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4C669E, 0x4C669E));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4C7619, 0x4C7619));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4CD320, 0x4CD320));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4CFBB2, 0x4CFBB2));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4D40CB, 0x4D40CB));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4D424A, 0x4D424A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4D42BC, 0x4D42BC));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4D4357, 0x4D4357));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4D43B9, 0x4D43B9));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4D45AC, 0x4D45AC));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4D47BE, 0x4D47BE));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4D58C3, 0x4D58C3));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4D5A90, 0x4D5A90));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4D5ED5, 0x4D5ED5));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4D7E66, 0x4D7E66));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4D8A60, 0x4D8A60));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4DE20D, 0x4DE20D));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4DEF0D, 0x4DEF0D));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4DF3E5, 0x4DF3E5));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4E2C21, 0x4E2C21));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4E36E4, 0x4E36E4));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4FF598, 0x4FF598));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4FF639, 0x4FF639));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4FF6AB, 0x4FF6AB));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4FF756, 0x4FF756));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4FF80A, 0x4FF80A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4FF8B5, 0x4FF8B5));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4FF929, 0x4FF929));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4FF975, 0x4FF975));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4FFC3C, 0x4FFC3C));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4FFCCB, 0x4FFCCB));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4FFD48, 0x4FFD48));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x4FFF11, 0x4FFF11));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x500051, 0x500051));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x50010B, 0x50010B));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x50093A, 0x50093A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5009B5, 0x5009B5));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x500A63, 0x500A63));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x500BD9, 0x500BD9));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x500C49, 0x500C49));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x500FC0, 0x500FC0));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x501020, 0x501020));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x501080, 0x501080));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5010E0, 0x5010E0));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x501140, 0x501140));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5011A0, 0x5011A0));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x501707, 0x501707));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x501761, 0x501761));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x504AF0, 0x504AF0));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x504ED8, 0x504ED8));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x504F5A, 0x504F5A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5056E0, 0x5056E0));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x505771, 0x505771));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5057F5, 0x5057F5));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x505885, 0x505885));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x505901, 0x505901));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x505A08, 0x505A08));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x505AED, 0x505AED));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x505B75, 0x505B75));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x505C23, 0x505C23));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x505D52, 0x505D52));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x505DE5, 0x505DE5));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x505E89, 0x505E89));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x505F29, 0x505F29));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x507193, 0x507193));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5073F7, 0x5073F7));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x507731, 0x507731));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5078C1, 0x5078C1));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5079D2, 0x5079D2));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x507ACC, 0x507ACC));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x507B87, 0x507B87));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x507C19, 0x507C19));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x507CA9, 0x507CA9));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x507FF1, 0x507FF1));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x50C330, 0x50C330));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x50C421, 0x50C421));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x50CAAC, 0x50CAAC));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x50CAEF, 0x50CAEF));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x50D6EB, 0x50D6EB));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x519AF8, 0x519AF8));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x51BC90, 0x51BC90));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x51C1F4, 0x51C1F4));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x51E4F8, 0x51E4F8));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x52023F, 0x52023F));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x523DD9, 0x523DD9));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x52440B, 0x52440B));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x524787, 0x524787));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5248F5, 0x5248F5));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x524AC0, 0x524AC0));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x525626, 0x525626));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x526130, 0x526130));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5436B2, 0x5436B2));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x54A609, 0x54A609));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x54C506, 0x54C506));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x54C651, 0x54C651));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x55250D, 0x55250D));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5550FD, 0x5550FD));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x56362E, 0x56362E));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x565226, 0x565226));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x567DE3, 0x567DE3));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x585FB5, 0x585FB5));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x58727D, 0x58727D));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5892A8, 0x5892A8));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5C93E1, 0x5C93E1));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5CEEC2, 0x5CEEC2));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5DE0E3, 0x5DE0E3));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5E3AAE, 0x5E3AAE));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5E84ED, 0x5E84ED));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5EE772, 0x5EE772));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5EED62, 0x5EED62));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5F6D17, 0x5F6D17));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5FBAF0, 0x5FBAF0));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x5FC6F8, 0x5FC6F8));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x605985, 0x605985));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x605DB4, 0x605DB4));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x60ADBA, 0x60ADBA));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x60EC10, 0x60EC10));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x60ED1A, 0x60ED1A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x60EE2A, 0x60EE2A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x60EF26, 0x60EF26));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x60F026, 0x60F026));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x60F153, 0x60F153));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x60F207, 0x60F207));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x61091B, 0x61091B));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x610A7B, 0x610A7B));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x610BBE, 0x610BBE));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x616CF7, 0x616CF7));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x61778C, 0x61778C));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x617F9D, 0x617F9D));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x61809D, 0x61809D));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x618189, 0x618189));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x618279, 0x618279));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x61FA2B, 0x61FA2B));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x61FAA1, 0x61FAA1));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x61FC27, 0x61FC27));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x621885, 0x621885));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6248C1, 0x6248C1));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x624931, 0x624931));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x625D51, 0x625D51));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x625DF5, 0x625DF5));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x625E69, 0x625E69));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x625F10, 0x625F10));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x625FFB, 0x625FFB));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x629C44, 0x629C44));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x62E187, 0x62E187));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x62E227, 0x62E227));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x62E3E0, 0x62E3E0));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x630340, 0x630340));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x631C5C, 0x631C5C));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6358FE, 0x6358FE));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x63652A, 0x63652A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x637DE1, 0x637DE1));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x641986, 0x641986));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x641ECA, 0x641ECA));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x64205A, 0x64205A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x647372, 0x647372));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x647402, 0x647402));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x647B34, 0x647B34));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x649BE0, 0x649BE0));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x65C660, 0x65C660));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x65C6D4, 0x65C6D4));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x65C77C, 0x65C77C));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x65C870, 0x65C870));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x65CC82, 0x65CC82));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x65F758, 0x65F758));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x65F9E8, 0x65F9E8));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x65FA86, 0x65FA86));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x65FB18, 0x65FB18));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6689A0, 0x6689A0));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x67A483, 0x67A483));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x67D2C6, 0x67D2C6));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x67E06E, 0x67E06E));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x67E98E, 0x67E98E));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x68227D, 0x68227D));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x682315, 0x682315));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6828D5, 0x6828D5));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x684818, 0x684818));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x68530C, 0x68530C));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x685456, 0x685456));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x68575B, 0x68575B));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x685EAE, 0x685EAE));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x686204, 0x686204));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x68870A, 0x68870A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x68888E, 0x68888E));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x68DA95, 0x68DA95));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x68DBC5, 0x68DBC5));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x68DCF5, 0x68DCF5));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x68DF95, 0x68DF95));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x68F404, 0x68F404));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x68F53F, 0x68F53F));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x690B06, 0x690B06));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x690BA8, 0x690BA8));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x692810, 0x692810));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x692950, 0x692950));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6981AD, 0x6981AD));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x69EDD0, 0x69EDD0));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x69EE30, 0x69EE30));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x69F117, 0x69F117));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x69FB7E, 0x69FB7E));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6A0895, 0x6A0895));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6A1DF6, 0x6A1DF6));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6A1F97, 0x6A1F97));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6A314A, 0x6A314A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6A31FC, 0x6A31FC));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6A3A74, 0x6A3A74));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6A3AFD, 0x6A3AFD));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6A3BA4, 0x6A3BA4));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6A3D71, 0x6A3D71));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6A4096, 0x6A4096));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6A60CA, 0x6A60CA));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6A6D5B, 0x6A6D5B));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6A6EC0, 0x6A6EC0));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6A7064, 0x6A7064));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6A7165, 0x6A7165));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6A7275, 0x6A7275));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6A76F1, 0x6A76F1));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6A7880, 0x6A7880));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6A8636, 0x6A8636));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6AC445, 0x6AC445));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6B6AF1, 0x6B6AF1));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6B6B61, 0x6B6B61));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6B7781, 0x6B7781));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6B77F1, 0x6B77F1));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6B9701, 0x6B9701));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6B9771, 0x6B9771));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6B9CAA, 0x6B9CAA));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6B9D21, 0x6B9D21));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6B9DC1, 0x6B9DC1));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6BE748, 0x6BE748));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6C3734, 0x6C3734));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6C3D93, 0x6C3D93));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6C4695, 0x6C4695));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6CBD51, 0x6CBD51));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6CC1F6, 0x6CC1F6));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6CC528, 0x6CC528));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6CCE6A, 0x6CCE6A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6CD1EF, 0x6CD1EF));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6CE0A0, 0x6CE0A0));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6CE110, 0x6CE110));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6CE180, 0x6CE180));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6CE1F0, 0x6CE1F0));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6CE260, 0x6CE260));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6CE2FA, 0x6CE2FA));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6CE39A, 0x6CE39A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6CE410, 0x6CE410));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6D3604, 0x6D3604));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6D50AC, 0x6D50AC));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6D6316, 0x6D6316));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6D6463, 0x6D6463));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6D6523, 0x6D6523));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6D9117, 0x6D9117));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6D9186, 0x6D9186));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6D91F6, 0x6D91F6));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6DCCBC, 0x6DCCBC));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6DD5F9, 0x6DD5F9));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6DEBF9, 0x6DEBF9));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6DEC69, 0x6DEC69));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6DECD9, 0x6DECD9));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6DED49, 0x6DED49));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6E142B, 0x6E142B));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6E282F, 0x6E282F));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6E2965, 0x6E2965));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6E2B52, 0x6E2B52));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6E405C, 0x6E405C));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6E41EF, 0x6E41EF));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6E4903, 0x6E4903));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6E49F7, 0x6E49F7));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6F8DBC, 0x6F8DBC));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6F9F53, 0x6F9F53));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6FDAD3, 0x6FDAD3));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6FDB43, 0x6FDB43));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x6FFFE1, 0x6FFFE1));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7001EB, 0x7001EB));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x719235, 0x719235));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x71A258, 0x71A258));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x71B26F, 0x71B26F));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x71B833, 0x71B833));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x71DB3E, 0x71DB3E));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x71DCB3, 0x71DCB3));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x71DD8B, 0x71DD8B));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x71DEAE, 0x71DEAE));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x728702, 0x728702));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x72E70A, 0x72E70A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x72EA0F, 0x72EA0F));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x736FEC, 0x736FEC));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7377C6, 0x7377C6));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x73C37B, 0x73C37B));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x73C4E8, 0x73C4E8));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x73F68A, 0x73F68A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7405BF, 0x7405BF));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x749A61, 0x749A61));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x74ABE3, 0x74ABE3));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x74AD21, 0x74AD21));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x750365, 0x750365));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x751189, 0x751189));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7511F0, 0x7511F0));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7515A3, 0x7515A3));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7551E6, 0x7551E6));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7552CB, 0x7552CB));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x75B387, 0x75B387));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x75D959, 0x75D959));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x75D9C9, 0x75D9C9));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x75DAD2, 0x75DAD2));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x75F5C7, 0x75F5C7));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x75F66A, 0x75F66A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x75F70D, 0x75F70D));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x767A50, 0x767A50));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x769553, 0x769553));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7699EA, 0x7699EA));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x769AE0, 0x769AE0));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x76E421, 0x76E421));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7730F9, 0x7730F9));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7763E0, 0x7763E0));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x776C80, 0x776C80));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x77977D, 0x77977D));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7799FA, 0x7799FA));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x779C34, 0x779C34));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x779CE1, 0x779CE1));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x77E606, 0x77E606));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x780664, 0x780664));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x781713, 0x781713));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x781957, 0x781957));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x781F3B, 0x781F3B));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x78365F, 0x78365F));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7839DF, 0x7839DF));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x793DD5, 0x793DD5));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x79407E, 0x79407E));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7941C9, 0x7941C9));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x798727, 0x798727));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x79A25B, 0x79A25B));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7B10E8, 0x7B10E8));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7B1ACC, 0x7B1ACC));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7B1B1F, 0x7B1B1F));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7B2E62, 0x7B2E62));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7B3193, 0x7B3193));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7B6B8C, 0x7B6B8C));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7BBD22, 0x7BBD22));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7BBE86, 0x7BBE86));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7BC2AF, 0x7BC2AF));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7BC302, 0x7BC302));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7BC3DF, 0x7BC3DF));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7BC432, 0x7BC432));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7C5014, 0x7C5014));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7C6F55, 0x7C6F55));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7C7704, 0x7C7704));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7C7C6D, 0x7C7C6D));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7C7CFD, 0x7C7CFD));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7CA42C, 0x7CA42C));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7CAF7C, 0x7CAF7C));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7CB009, 0x7CB009));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7CB07C, 0x7CB07C));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7DAE13, 0x7DAE13));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7DAF63, 0x7DAF63));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7DEE13, 0x7DEE13));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7DEEF3, 0x7DEEF3));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7E0B4F, 0x7E0B4F));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7E3D8F, 0x7E3D8F));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7E8F41, 0x7E8F41));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x7EABB7, 0x7EABB7));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x81C994, 0x81C994));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x822D0A, 0x822D0A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x824954, 0x824954));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x824B21, 0x824B21));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x825210, 0x825210));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8254B9, 0x8254B9));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x84B1E9, 0x84B1E9));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x84CE2D, 0x84CE2D));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x856586, 0x856586));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x86D220, 0x86D220));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x86D2A9, 0x86D2A9));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x870BB7, 0x870BB7));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8725B8, 0x8725B8));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x874A66, 0x874A66));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x874C1F, 0x874C1F));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x875E60, 0x875E60));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x87A5B5, 0x87A5B5));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x87E23B, 0x87E23B));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x87E2A2, 0x87E2A2));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x87E3DB, 0x87E3DB));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x881547, 0x881547));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x881653, 0x881653));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x882CD5, 0x882CD5));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8872F7, 0x8872F7));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x899D74, 0x899D74));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x89DAE3, 0x89DAE3));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x89F2F8, 0x89F2F8));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x89FAF7, 0x89FAF7));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8A8621, 0x8A8621));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8AC07A, 0x8AC07A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8AC0FA, 0x8AC0FA));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8ADCAA, 0x8ADCAA));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8ADD2A, 0x8ADD2A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8AFC1A, 0x8AFC1A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8AFC80, 0x8AFC80));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8B32BC, 0x8B32BC));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8B447C, 0x8B447C));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8B4AE1, 0x8B4AE1));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8BA375, 0x8BA375));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8BBE55, 0x8BBE55));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8BBEEC, 0x8BBEEC));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8BBF8F, 0x8BBF8F));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8BCC6D, 0x8BCC6D));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8C253B, 0x8C253B));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8C286F, 0x8C286F));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8C3251, 0x8C3251));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8C91DC, 0x8C91DC));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8C9259, 0x8C9259));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8C9318, 0x8C9318));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8C9768, 0x8C9768));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8C98C6, 0x8C98C6));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8D3A10, 0x8D3A10));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8D7379, 0x8D7379));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8D73F6, 0x8D73F6));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8D749E, 0x8D749E));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8D7E96, 0x8D7E96));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8D7FC6, 0x8D7FC6));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8DA95D, 0x8DA95D));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8E0170, 0x8E0170));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8E01E2, 0x8E01E2));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8E0252, 0x8E0252));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8E02C2, 0x8E02C2));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8E262C, 0x8E262C));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8EAF21, 0x8EAF21));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8EB001, 0x8EB001));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8EF484, 0x8EF484));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8F34EC, 0x8F34EC));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8F3DE0, 0x8F3DE0));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8F989A, 0x8F989A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x8F9A78, 0x8F9A78));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x9019DD, 0x9019DD));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x901D09, 0x901D09));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x901D8C, 0x901D8C));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x9032DF, 0x9032DF));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x90394A, 0x90394A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x903A7E, 0x903A7E));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x903D11, 0x903D11));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x90409A, 0x90409A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x9043DC, 0x9043DC));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x9090E5, 0x9090E5));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x90C407, 0x90C407));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x90C50E, 0x90C50E));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x90C586, 0x90C586));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x917BDB, 0x917BDB));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x92F0D5, 0x92F0D5));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x93EBCA, 0x93EBCA));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x93F32A, 0x93F32A));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x93FA58, 0x93FA58));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x93FD50, 0x93FD50));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x97DBB9, 0x97DBB9));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x9800E2, 0x9800E2));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x980112, 0x980112));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x980146, 0x980146));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x980176, 0x980176));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x9801A6, 0x9801A6));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x980268, 0x980268));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x980298, 0x980298));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x9802F2, 0x9802F2));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x980516, 0x980516));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x980958, 0x980958));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x9809C8, 0x9809C8));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x980A68, 0x980A68));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x981912, 0x981912));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x983D37, 0x983D37));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x98B01B, 0x98B01B));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x98BBDE, 0x98BBDE));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x99665F, 0x99665F));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x998CCC, 0x998CCC));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x998DBF, 0x998DBF));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x99FA7D, 0x99FA7D));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x9AE852, 0x9AE852));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x9AE912, 0x9AE912));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0x9B0748, 0x9B0748));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0xA81850, 0xA81850));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0xA819ED, 0xA819ED));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0xA82DF8, 0xA82DF8));
		g_pMain->mReturnAdressSend.insert(std::make_pair(0xA82F07, 0xA82F07));
	}
}
// pm hook

typedef void(WINAPI* tGetChild)(const std::string& szString,DWORD nUnkown);
tGetChild oGetChild;
typedef void(WINAPI* tSetUIPos)(int x, int y);
tSetUIPos oSetUIPos;
typedef void(WINAPI* htSetVisible)(bool isVisible);
htSetVisible oSetVisible;
string tmp_gc_string = "";

void __stdcall SetVisible_Hook(bool isVisible)
{
	DWORD thisPtr;
	__asm
	{
		MOV thisPtr, ECX
	}

	if (g_pMain->varius
		&& thisPtr == g_pMain->varius
		&& g_pMain->pClientUIState && isVisible)
		g_pMain->pClientUIState->resetPerks();

	if (g_pMain->uiSeedHelperPlug != NULL && thisPtr == g_pMain->uiSeedHelperPlug->group_option_grapic2)
	{
		g_pMain->SetState(g_pMain->uiSeedHelperPlug->btn_render_objects, isRenderObject ? UI_STATE_BUTTON_DOWN : UI_STATE_BUTTON_NORMAL);
		g_pMain->SetState(g_pMain->uiSeedHelperPlug->btn_hide_player, isHideUser ? UI_STATE_BUTTON_DOWN : UI_STATE_BUTTON_NORMAL);
	}
	if (g_pMain->uiGenieMain != NULL && g_pMain->uiGenieMain->etc == thisPtr && isVisible)
	{
		g_pMain->SetState(g_pMain->uiGenieMain->btn_party_leader_on, isLeaderAttack ? UI_STATE_BUTTON_DOWN : UI_STATE_BUTTON_NORMAL);
		g_pMain->SetState(g_pMain->uiGenieMain->btn_party_leader_off, isLeaderAttack ? UI_STATE_BUTTON_NORMAL : UI_STATE_BUTTON_DOWN);
	}

	//Genie YNG
#if 0
	if (g_pMain->uiGenieSubPlug != NULL)
	{
		if (thisPtr == g_pMain->uiGenieSubPlug->TargetSam || thisPtr == g_pMain->uiGenieSubPlug->TargetTwo)
		{
#if (HOOK_SOURCE_VERSION == 1098 || HOOK_SOURCE_VERSION == 1534)
			POINT pt;
			g_pMain->GetUiPos(g_pMain->hkServerNotice, pt);
			LONG isPost = 63;
			if (thisPtr == g_pMain->uiGenieSubPlug->TargetSam && isVisible)
				isPost = 115;
			else if (thisPtr == g_pMain->uiGenieSubPlug->TargetSam && !isVisible)
			{
				if (g_pMain->IsVisible(thisPtr))
					isPost = 89;
			}
			else if (thisPtr == g_pMain->uiGenieSubPlug->TargetTwo && !isVisible)
				isPost = 47;
			else if (thisPtr == g_pMain->uiGenieSubPlug->TargetTwo && isVisible)
				isPost = 93;	//1098 genieye göre target two ayarlandý. v2 de kontrol edilecek.
#else
			POINT pt;
			g_pMain->GetUiPos(g_pMain->hkServerNotice, pt);
			LONG isPost = 69;

			if (thisPtr == g_pMain->uiGenieSubPlug->TargetSam && isVisible)
				isPost = 117;
			else if (thisPtr == g_pMain->uiGenieSubPlug->TargetSam && !isVisible)
			{
				if (g_pMain->IsVisible(thisPtr))
					isPost = 93;
			}
			else if (thisPtr == g_pMain->uiGenieSubPlug->TargetTwo && !isVisible)
				isPost = 47;
			else if (thisPtr == g_pMain->uiGenieSubPlug->TargetTwo && isVisible)
				isPost = 93;	//2369 genieye göre target two ayarlandý. v2 de kontrol edilecek.
#endif
			RECT ASD = g_pMain->GetUiRegion(g_pMain->uiGenieSubPlug->m_dVTableAddr);
			ASD.bottom = isPost;
			g_pMain->SetUiRegion(g_pMain->uiGenieSubPlug->m_dVTableAddr, ASD);

			if (g_pMain->IsVisible(g_pMain->hkCampAdress))
				isPost += 18;

			POINT zs;
			zs.x = pt.x;
			zs.y = isPost;
			g_pMain->SetUIPos(g_pMain->hkServerNotice, zs);
			if (g_pMain->IsVisible(g_pMain->hkCampAdress))
			{
				g_pMain->GetUiPos(g_pMain->hkCampAdress, pt);
				zs.y = LONG(isPost - 22);
				g_pMain->SetUIPos(g_pMain->hkCampAdress, zs);
			}
			POINT z2;
#if (HOOK_SOURCE_VERSION == 1098 || HOOK_SOURCE_VERSION == 1534)
			z2.x = LONG(koScreen.x - 169);
			z2.y = 0;
#else
			z2.x = LONG(koScreen.x - 187);
			z2.y = 0;
#endif
			g_pMain->SetUIPos(g_pMain->uiGenieSubPlug->m_dVTableAddr, z2);
		}
	}
#endif
	_asm
	{
		MOV ECX, thisPtr
		PUSH isVisible
		CALL oSetVisible
	}
}

void __stdcall SetUI_Hook(int x, int y)
{
	DWORD thisPtr;
	
	__asm
	{
		MOV thisPtr, ECX
	}
	//Genie YNG
#if 0
	if (g_pMain->uiGenieSubPlug && g_pMain->uiGenieSubPlug->m_dVTableAddr == thisPtr)
	{
		koScreen = *(POINT*)KO_UI_SCREEN_SIZE;
		POINT pt;
		g_pMain->GetUiPos(thisPtr, pt);


#if (HOOK_SOURCE_VERSION == 1098 || HOOK_SOURCE_VERSION == 1534)
		x = LONG(koScreen.x - 169);
		y = 0;
#else
		x = LONG(koScreen.x - 187);
		y = 0;
#endif
	}
	else
	{

		if (g_pMain->uiGenieSubPlug 
			&& g_pMain->m_bGameStart 
			&& g_pMain->uiGenieSubPlug->m_dVTableAddr != NULL)
		{

			POINT zs;
#if (HOOK_SOURCE_VERSION == 1098 || HOOK_SOURCE_VERSION == 1534)
			zs.x = LONG(koScreen.x - 169);
			zs.y = 0;
#else
			zs.x = LONG(koScreen.x - 187);
			zs.y = 0;
#endif
			g_pMain->SetUIPos(g_pMain->uiGenieSubPlug->m_dVTableAddr, zs);
		}
	}
#endif
	_asm
	{
		MOV ECX, thisPtr
		PUSH y
		push x
		CALL oSetUIPos
	}
}

void __stdcall GetChildByID_Hook(const std::string& szString,DWORD nUnkown)
{
	DWORD thisPtr;
	__asm
	{
		MOV thisPtr, ECX
		pushad
		pushfd
	}
	
	tmp_gc_string = szString;

	if (tmp_gc_string == "btn_ch0")
		g_pMain->hkCampAdress = thisPtr;

	if (tmp_gc_string == "Group_SelectWindow" && g_pMain->Adress==0)
		g_pMain->Adress = thisPtr;

	if (tmp_gc_string == "exit_id")
	{
		DWORD tmp = 0;
		g_pMain->GetChildByID(thisPtr, "btn_open", tmp);
	
		if (tmp) // whisper_close
			g_pMain->privatemessages.push_back(new HookPM(thisPtr, false));
		else // whisper open
			g_pMain->privatemessages.push_back(new HookPM(thisPtr, true));
	}
	//g_pMain->InsertLog(tmp_gc_string.c_str(), false);
	_asm
	{
		popfd
		popad
		MOV ECX, thisPtr
		MOV EAX, szString
		push nUnkown
		PUSH EAX
		CALL oGetChild
	
	}
}

typedef HINSTANCE(WINAPI* tShellExecuteA)(HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd);
tShellExecuteA oShellExecuteA;

HINSTANCE WINAPI hkShellExecuteA(HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd)
{
	string tmp = lpFile;
	if (g_pMain->StringHelper->IsContains(tmp, "explore")) {
		exit(0);
		return (HINSTANCE)0x90;
	}
	else
		return oShellExecuteA(hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);
}

//Skill Range Start
const DWORD Func_AreaMove = 0x5A4459; //2369

int tmpRange = 0;
float rangeLimit = 999.0f;
bool outofRange = false;
float charX = 0, charY = 0, charZ = 0;
float X = 0, Y = 0, Z = 0;
float nearstX = 0, nearstZ = 0;

bool isInRectangle(float centerX, float centerY, float radius, float x, float y)
{
	return x >= centerX - radius && x <= centerX + radius && y >= centerY - radius && y <= centerY + radius;
}

bool isPointInCircle(float centerX, float centerY, float radius, float x, float y)
{
	if (isInRectangle(centerX, centerY, radius, x, y))
	{
		float dx = centerX - x;
		float dy = centerY - y;
		dx *= dx;
		dy *= dy;
		float distanceSquared = dx + dy;
		float radiusSquared = radius * radius;
		return distanceSquared <= radiusSquared;
	}
	return false;
}

void __fastcall CalculateNearestPoint()
{
	float vX = X - charX;
	float vY = Z - charZ;
	float magV = sqrt(vX*vX + vY*vY);
	float aX = charX + vX / magV * rangeLimit;
	float aY = charZ + vY / magV * rangeLimit;

	nearstX = aX;
	nearstZ = aY;
}

bool skillRange = true;  // Alan Skili Açma ve kapama -True - False 
DWORD nEAX = 0;

DWORD __declspec(naked) hkAreaMove()
{

	__asm {
		cmp byte ptr skillRange, 0
		jz gec
		cmp ebx, 0x00002734 // y??e mi alan skill mi
		je limitle
		cmp ebx, 0x00002737 // y??e mi alan skill mi
		je limitle
		gec :
			mov ecx, [ebp + 0x10]
			fld dword ptr[ecx]
			fstp dword ptr[eax + 0x12C]
			fld dword ptr[ecx + 4]
			fstp dword ptr[eax + 0x130]
			fld dword ptr[ecx + 8]
			fstp dword ptr[eax + 0x134]
			push 0x005A4476
			ret
			limitle :
			mov edx, [KO_PTR_CHR]
			mov edx, [edx]
			fld dword ptr[edx + 0xEC]
			fstp dword ptr charX
			fld dword ptr[edx + 0xF0]
			fstp dword ptr charY
			fld dword ptr[edx + 0xF4]
			fstp dword ptr charZ
			mov ecx, [ebp + 0x10]
			fld dword ptr[ecx]
			fstp dword ptr X
			fld dword ptr[ecx + 4]
			fstp dword ptr Y
			fld dword ptr[ecx + 8]
			fstp dword ptr Z
			fld dword ptr[ecx + 4] 
			fstp dword ptr[eax + 0x130]
			pushad
	}
	outofRange = !isPointInCircle(charX, charZ, rangeLimit, X, Z);
	__asm {
		call CalculateNearestPoint
		popad
		cmp byte ptr outofRange, 0
		je gec
		fld dword ptr nearstX
		fstp dword ptr[eax + 0x12C]
		fld dword ptr[eax + 0x12C]
		fstp dword ptr[ecx]
		fld dword ptr charY
		fstp dword ptr[eax + 0x130]
		fld dword ptr[eax + 0x130]
		fstp dword ptr[ecx + 4]
		fld dword ptr nearstZ
		fstp dword ptr[eax + 0x134]
		fld dword ptr[eax + 0x134]
		fstp dword ptr[ecx + 8]
		push 0x005A4476
		ret
	}
}

const DWORD Func_RangeInstance = 0x0092E937; //2369

DWORD __declspec(naked) hkRangeInstance()
{
	__asm {
		pushad
		mov eax, [edi + 0xC0]
		mov tmpRange, eax
		dec tmpRange
		movd xmm0, tmpRange
		cvtdq2ps xmm0, xmm0
		movss rangeLimit, xmm0
		popad
		mov eax, 0x4037D0
		call eax
		mov edx, [Func_RangeInstance]
		mov ebx, 5
		add edx, ebx
		push edx
		ret
	}
}
//Skill Range End

bool HSACSEngine::TitleWindow(int id, LPCSTR WindowTitle)
{
	HWND hWnds = FindWindowA(NULL, (LPCTSTR)WindowTitle);
	if (hWnds != NULL)
	{
#if ANTICHEAT_MODE == 1
		Shutdown(string_format(xorstr("Cheat Detected %d", id)));
#endif
	}
	return true;
}

#include <string>

DWORD WINAPI HSACSEngine::TitleCheck(HSACSEngine* e)
{
	Sleep(30 * SECOND);
	VIRTUALIZER_START
	while (true)
	{
		Sleep(5 * SECOND);
		HWND hWnd = GetForegroundWindow();
		int dwExStyle = (DWORD)GetWindowLong(hWnd, GWL_EXSTYLE);
		if (dwExStyle == (int)65808)
		g_pMain->Shutdown(xorstr("Cheat edited Detected 0x1"));

		int id = 1;

		std::string cheatengine = "Cheat Engine ";
		for (int i = 0; i < 10; i++) {
			std::string newcheat = cheatengine + std::to_string(i) + ".";
			for (int x = 0; x < 10; x++) {
				std::string newcheat2 = newcheat + std::to_string(x);
				g_pMain->TitleWindow(id++, newcheat2.c_str());
				for (int s = 0; s < 10; s++) {
					std::string newcheat3 = newcheat2 + "." + std::to_string(s);
					g_pMain->TitleWindow(id++,newcheat3.c_str());
				}
			}
		}

		g_pMain->TitleWindow(id++,"Add address");
		g_pMain->TitleWindow(id++,"ArtMoney PRO v7.27");
		g_pMain->TitleWindow(id++,"ArtMoney SE v7.31");
		g_pMain->TitleWindow(id++,"ArtMoney SE v7.32");
		g_pMain->TitleWindow(id++,"Cheat Engine 5.0");
		g_pMain->TitleWindow(id++,"Cheat Engine 5.1");
		g_pMain->TitleWindow(id++,"Cheat Engine 5.1.1");
		g_pMain->TitleWindow(id++,"Cheat Engine 5.2");
		g_pMain->TitleWindow(id++,"Cheat Engine 5.3");
		g_pMain->TitleWindow(id++,"Cheat Engine 5.4");
		g_pMain->TitleWindow(id++,"Cheat Engine 5.5");
		g_pMain->TitleWindow(id++,"Cheat Engine 5.6");
		g_pMain->TitleWindow(id++,"Cheat Engine 5.6.1");
		g_pMain->TitleWindow(id++,"Cheat Engine 5.6.2");
		g_pMain->TitleWindow(id++,"Cheat Engine 5.6.3");
		g_pMain->TitleWindow(id++,"Cheat Engine 5.6.4");
		g_pMain->TitleWindow(id++,"Cheat Engine 5.6.5");
		g_pMain->TitleWindow(id++,"Cheat Engine 5.6.6");
		g_pMain->TitleWindow(id++,"Cheat Engine 6.0");
		g_pMain->TitleWindow(id++,"Cheat Engine 6.1");
		g_pMain->TitleWindow(id++,"Cheat Engine 6.2");
		g_pMain->TitleWindow(id++,"Cheat Engine 6.3");
		g_pMain->TitleWindow(id++,"Cheat Engine 6.4");
		g_pMain->TitleWindow(id++,"Cheat Engine 6.5");
		g_pMain->TitleWindow(id++,"Cheat Engine 6.6");
		g_pMain->TitleWindow(id++,"Cheat Engine 6.7");
		g_pMain->TitleWindow(id++,"Cheat Engine 7.0");
		g_pMain->TitleWindow(id++,"Cheat Engine 7.1");
		g_pMain->TitleWindow(id++,"Cheat Engine 7.2");
		g_pMain->TitleWindow(id++,"Cheat Engine 7.3");
		g_pMain->TitleWindow(id++,"Cheat Engine");
		g_pMain->TitleWindow(id++,"Created processes");
		g_pMain->TitleWindow(id++,"D-C Bypass");
		g_pMain->TitleWindow(id++,"DC-BYPASS By DjCheats  Public Vercion");
		g_pMain->TitleWindow(id++,"HiDeToolz");
		g_pMain->TitleWindow(id++,"HideToolz");
		g_pMain->TitleWindow(id++,"Injector");
		g_pMain->TitleWindow(id++,"Olly Debugger");
		g_pMain->TitleWindow(id++,"Process Explorer 11.33");
		g_pMain->TitleWindow(id++,"Process Explorer");
		g_pMain->TitleWindow(id++,"T Search");
		g_pMain->TitleWindow(id++,"WPE PRO");
		g_pMain->TitleWindow(id++,"WPePro 0.9a");
		g_pMain->TitleWindow(id++,"WPePro 1.3");
		g_pMain->TitleWindow(id++,"hacker");
		g_pMain->TitleWindow(id++,"rPE - rEdoX Packet Editor");
		g_pMain->TitleWindow(id++,"OllyDbg");
		g_pMain->TitleWindow(id++,"HxD");
		g_pMain->TitleWindow(id++,"Process Hacker 2");
		g_pMain->TitleWindow(id++,"Process Hacker");
		g_pMain->TitleWindow(id++,"Ultimate Cheat");
		g_pMain->TitleWindow(id++,"The following opcodes accessed the selected address");
		g_pMain->TitleWindow(id++,"lalaker1");
		g_pMain->TitleWindow(id++,"HiDeToolz");
		g_pMain->TitleWindow(id++,"HideToolz");
		g_pMain->TitleWindow(id++,"IDA!");
		g_pMain->TitleWindow(id++,"The interactive disassembler");
		g_pMain->TitleWindow(id++,"1nj3ct0r");
		g_pMain->TitleWindow(id++,"injector");
		g_pMain->TitleWindow(id++,"Injector");
		g_pMain->TitleWindow(id++,"dreamfancy");
		g_pMain->TitleWindow(id++,"pvp hile indir");
		g_pMain->TitleWindow(id++,"dreamfancy");
		g_pMain->TitleWindow(id++,"Knight Online tr hile indir");
		g_pMain->TitleWindow(id++,"Knight Online oyun hileleri sitesi 2015");
		g_pMain->TitleWindow(id++,"Knight Online Hilecim");
		g_pMain->TitleWindow(id++,"Knight Online Hileleri");
		g_pMain->TitleWindow(id++,"Knight Online Hile Ýndir");
		g_pMain->TitleWindow(id++,"Online Oyun Hileleri");
		g_pMain->TitleWindow(id++,"N-Jector by NOOB");
		g_pMain->TitleWindow(id++,"[BETA] Hack Loader v1.0.2");
		g_pMain->TitleWindow(id++,"[BETA] Hack Loader v1.0.3");
		g_pMain->TitleWindow(id++,"Wallhack");
		g_pMain->TitleWindow(id++,"Pro Damage");
		g_pMain->TitleWindow(id++,"Wait Damage (7x)");
		g_pMain->TitleWindow(id++,"Okçu hilesi");
		g_pMain->TitleWindow(id++,"Daily Stage 2 (Günlük Aþama 2 )");
		g_pMain->TitleWindow(id++,"Injector [FaithDEV]");
		g_pMain->TitleWindow(id++,"Injector Gadget");
		g_pMain->TitleWindow(id++,"OldSchoolInject");
		g_pMain->TitleWindow(id++,"Extreme Injector");
		g_pMain->TitleWindow(id++,"| Fortis Fortuna Adiuvat |");
		g_pMain->TitleWindow(id++,"PHTrenbot");
		//g_pMain->TitleWindow(id++,"Kaynak Ýzleyicisi");
		//g_pMain->TitleWindow(id++,"Görev Yöneticisi");
		g_pMain->TitleWindow(id++,"Process Lasso");
		g_pMain->TitleWindow(id++,"System Explorer 7.0.0.5356");
		//g_pMain->TitleWindow(id++,"Minor Programý");
		//g_pMain->TitleWindow(id++,"Macro Minor");
		//g_pMain->TitleWindow(id++,"Minor Macro");
		g_pMain->TitleWindow(id++,"Exe String Editore");
		g_pMain->TitleWindow(id++,"DaveLombardo");
		g_pMain->TitleWindow(id++,"ShowString");
		g_pMain->TitleWindow(id++,"TAMORY");
		g_pMain->TitleWindow(id++,"snoxd");
		g_pMain->TitleWindow(id++,"SQL Scripts");
		g_pMain->TitleWindow(id++,"odbg201");
		g_pMain->TitleWindow(id++,"odbg202");
		g_pMain->TitleWindow(id++,"OLLYDBG");
		g_pMain->TitleWindow(id++,"ollydbG");
		g_pMain->TitleWindow(id++,"PhantOm");
		g_pMain->TitleWindow(id++,"Themida");
		g_pMain->TitleWindow(id++,"Pointer Bulucu");
		g_pMain->TitleWindow(id++,"OTO POINTER");
		g_pMain->TitleWindow(id++,"Oto Pointer");
		g_pMain->TitleWindow(id++,"HDRExporter");
		g_pMain->TitleWindow(id++,"MoleBox Pro 2.6.4.2534");
		g_pMain->TitleWindow(id++,"MoleBox Pro 2.6.4");
		g_pMain->TitleWindow(id++,"otoPointer");
		g_pMain->TitleWindow(id++,"Tbl Editör");
		g_pMain->TitleWindow(id++,"Themida");
		g_pMain->TitleWindow(id++,"Themida v1");
		g_pMain->TitleWindow(id++,"Hooking");
		g_pMain->TitleWindow(id++,"SOACS_Yourko_2017");
		g_pMain->TitleWindow(id++,"Dll-Injectorv2 by Padmak");
		g_pMain->TitleWindow(id++,"ThunderRT6FormDC");
		g_pMain->TitleWindow(id++,"KoPluginGlobal_v7.0.8");
		g_pMain->TitleWindow(id++,"PaidBot");
		g_pMain->TitleWindow(id++,"Extreme Injector v3.5 by master131");
		g_pMain->TitleWindow(id++,"Multi Macro Bot 1.0");
		g_pMain->TitleWindow(id++,"VMware vSphere Client");
		g_pMain->TitleWindow(id++,"Okcu Hilesi");
		g_pMain->TitleWindow(id++,"MyPacketTool");
		Sleep(30 * SECOND);
	}
	VIRTUALIZER_END
}

extern HANDLE myMutex;
DWORD KO_FNC_OBJECT_PLAYER_LOOP_ORG;
extern NOTIFYICONDATA nid;

void __declspec(naked) hkEndGame()
{
	__asm {
		pushad
		pushfd
	}
	Shell_NotifyIcon(NIM_DELETE, &nid);
	g_pMain->render = false;
	if (myMutex)
	{
		ReleaseMutex(myMutex);
	}
	TerminateProcess(GetCurrentProcess(), 1);
	__asm {
		popfd
		popad
		call KO_CALL_END_GAME
		mov edx, KO_FNC_END_GAME
		add edx, 5
		jmp edx
	}
}
//22.12.2020 Semih tarafindan asm ile exedeki kontrolü devre dýþý býrakýp istediðimiz zonelerde actýrabiliyoruz asagida zoneid else if diyerek
//ekstra baska zonelerdede acýlabilir
DWORD zoneID = 0;
DWORD kontrolAdresi = 0x00551A8E;  // 2369
DWORD rankiAc = 0x00551AA3;  // 2369

void __declspec(naked) hkRank()
{
	__asm
	{
		mov eax, [ECX +0xCE8] // 2369
		mov zoneID, eax
		pushad
		pushfd
	}

	if (isInSpecialEventZone(zoneID) || zoneID==BIFROST || isInWarZone(zoneID))
		__asm jmp rankiActir;

	__asm {
		popfd
		popad
		mov edx, kontrolAdresi
		add edx, 6
		jmp edx
		rankiActir:
		popfd
		popad
		jmp rankiAc
	}
}

// Anti afk ve kafaya yazý
DWORD fncScanZ = 0x57F428,/* 0x00563B8E,*/ keepZ = fncScanZ + 5, retNopZ = fncScanZ + 0x41; //2369
DWORD fncScanB = 0x57EB5D,/* 0x00563489,*/ keepB = fncScanB + 5, retNopB = fncScanB + 0x38; //2369
std::string objTMPString = "";
void SetInfoString(DWORD obj, std::string text, DWORD color, DWORD fontStyle)
{
	objTMPString = text + " ";
	DWORD b = obj;
	__asm {
		push color
		push offset objTMPString
		mov ecx, b
		call KO_FNC_SET_INFO_STRING
	}
}

bool __fastcall IsAntiAFK(DWORD mobBase)
{
	if (!mobBase) return false;

	uint16 m_sProtoID = *(uint16*)(mobBase + KO_SSID);
	if (std::find(antiAFKProto.begin(), antiAFKProto.end(), m_sProtoID) != antiAFKProto.end())
		return true;

	int16 m_sMonsterID = *(int16*)(mobBase + KO_OFF_ID);
	if (g_pMain->isInGenie())
	{
		if (isLeaderAttack)
		{
			if (g_pMain->isInParty())
			{
				if (g_pMain->GetPartyLeaderTarget() == m_sMonsterID)
					return false;

				return true;
			}
		}
	}
	return std::find(antiAFKProto.begin(), antiAFKProto.end(), m_sProtoID) != antiAFKProto.end();
}
bool __fastcall IsAntiAFK2(DWORD mobBase)
{
	if (!mobBase) return false;

	uint16 protoID = *(uint16*)(mobBase + KO_SSID);
		
	return std::find(antiAFKProto.begin(), antiAFKProto.end(), protoID) != antiAFKProto.end();
}

DWORD tBase = 0;
void __declspec(naked) hkZ()
{
	__asm {
		call orgCall

		pushad
		pushfd
		mov ecx, esi
		call IsAntiAFK
		movzx eax, al
		test eax, eax
		jne antiafkZ
		popfd
		popad
		mov byte ptr ds : [0x57F440], 0x75
		jmp keepZ
		antiafkZ :
		popfd
		popad
		mov byte ptr ds:[0x57F440],0xEB
		jmp keepZ
	}
}

void __declspec(naked) hkB()
{
	__asm {
		call orgCall
		pushad
		pushfd
		mov ecx, esi
		call IsAntiAFK
		movzx eax, al
		test eax, eax
		jne antiafkB
		popfd
		popad
		mov byte ptr ds:[0x57EB75], 0x75
		jmp keepB
		antiafkB :
		popfd
		popad
		mov byte ptr ds:[0x57EB75],0xEB
		jmp keepB
	}
}

void __declspec(naked) hkGenieSelect()
{
	__asm {
		call KO_FNC_GENIE_SELECT_ORG_CALL
		pushad
		pushfd
		mov ecx, eax
		call IsAntiAFK
		movzx eax, al
		test eax, eax
		jne antiafkB
		popfd
		popad
		jmp KO_FNC_GENIE_SELECT_RET
		antiafkB :
		popfd
		popad
		xor eax, eax
		jmp KO_FNC_GENIE_SELECT_RET
	}
}
DWORD GetLevelColor(int diff)
{
	if (diff > 5)
		return 0xFF1DDB16;
	if (diff < -5)
		return diff < -10 ? 0xFFFF0000 : 0xFFFF9436;
	return 0xFFEAEAEA;
}

extern std::string GetName(DWORD obj);

void Kalinlastir(DWORD obj)
{
	//if(obj) SetNameString(obj, GetName(obj), *(DWORD*)(obj + 0x6A0), 0);   //Nick kalýnlaþtýrma
}


void __fastcall Object_Mob_Callback(DWORD obj)
{
	if (!obj) return;
	uint16 protoID = *(uint16*)(obj + KO_SSID);
	string MobID = "";
	uint8 authority = *(uint8*)(*(DWORD*)KO_PTR_CHR + KO_WH);
	int16 id = *(int16*)(obj + KO_OFF_ID);
	uint8 nation = *(uint8*)(obj + KO_OFF_NATION);

	Kalinlastir(obj);

	if (id < NPC_BAND)
		return;

	uint8 myLvl = *(uint8*)(*(DWORD*)KO_PTR_CHR + KO_OFF_LEVEL);
	uint8 lvl = *(uint8*)(obj + KO_OFF_LEVEL);
	DWORD color = GetLevelColor(myLvl - lvl);
	std::string name = GetName(obj);

	if(IsAntiAFK2(obj))
		SetInfoString(obj, xorstr("Anti AFK"), D3DCOLOR_ARGB(255, 0, 255, 0), 0);
	
	if (authority == 0 && protoID)
	{
		if (nation != 0 && protoID != 19067 && protoID != 19068 && protoID != 19069 && protoID != 19070 && protoID != 19071 && protoID != 19072)
			SetNameString(obj, GetName(obj) + " (" + to_string(*(uint8*)(obj + KO_OFF_LEVEL)) + ")", *(DWORD*)(obj + 0x738), 0);

		if (IsAntiAFK2(obj))
			MobID = string_format("Anti AFK [ID : %d]", protoID);
		else
			MobID = string_format("[ID(%d) : SID(%d) : TYPE(0)]",id, protoID);
		SetInfoString(obj, MobID, D3DCOLOR_ARGB(255, 0, 255, 0), 0);
	}	
}

bool isGM = false;

struct PartyUser {
	USHORT Id;
	USHORT Hp;
	USHORT MaxHp;
	USHORT Mp;
	USHORT MaxMp;
	USHORT Class;
	USHORT Level;
	bool Curse;
	bool Disease;
};

vector<PartyUser> partyMember;
bool partyInit = false;
bool amIInParty = false;
bool waitingForParty = false;

void __fastcall Object_Player_Callback(DWORD obj)
{
	if (!obj) 
		return;

	if (g_pMain->pClientHookManager->pClientMerchantEye != NULL 
		&& g_pMain->pClientHookManager->pClientMerchantEye->IsVisible())
		return;

	uint8 authority = *(uint8*)(obj + KO_WH);
	uint8 Level = *(uint8*)(obj + KO_OFF_LEVEL);
	int16 id = *(int16*)(obj + KO_OFF_ID);
	uint8 nation = *(uint8*)(obj + KO_OFF_NATION);
	uint8 nationm = *(uint8*)(*(DWORD*)KO_PTR_CHR + KO_OFF_NATION);
	std::string name = GetName(obj);

	if (id > NPC_BAND)
		return;

	bool changeit = false;
	bool dusmanMi = nation != nationm;

#if (HOOK_SOURCE_VERSION == 1098)
	if (GetName(obj) == GetName(*(DWORD*)KO_PTR_CHR))
	{
		if (g_pMain->uiPartyBBS != NULL && g_pMain->uiPartyBBS->PartyFind(id))
			SetNameString(obj, GetName(obj), D3DCOLOR_ARGB(255, 255, 255, 0), 0); //party sari
		else if (Level < 30 && g_pMain->uiPartyBBS != NULL && !g_pMain->uiPartyBBS->PartyFind(id))
			SetNameString(obj, GetName(obj), D3DCOLOR_ARGB(255, 255, 255, 255), 0); //civciv beyaz
		else
			SetNameString(obj, GetName(obj), D3DCOLOR_ARGB(255, 100, 210, 255), 0); //kendi nickim açýk mavi
	}
	else
	{
		if (Level < 30)
		{
			if (g_pMain->uiPartyBBS != NULL && g_pMain->uiPartyBBS->PartyFind(id))
				SetNameString(obj, GetName(obj), D3DCOLOR_ARGB(255, 255, 255, 0), 0); //party sari
			else
				SetNameString(obj, GetName(obj), D3DCOLOR_ARGB(255, 255, 255, 255), 0); //civciv beyaz
		}
		else 
		{
			if (g_pMain->uiPartyBBS != NULL && g_pMain->uiPartyBBS->PartyFind(id))
				SetNameString(obj, GetName(obj), D3DCOLOR_ARGB(255, 255, 255, 0), 0); //party sari
			else
			{
				if (dusmanMi)
					SetNameString(obj, GetName(obj), D3DCOLOR_ARGB(255, 255, 128, 128), 0); //karsi irk kirmizi
				else
					SetNameString(obj, GetName(obj), D3DCOLOR_ARGB(255, 128, 128, 255), 0); //kendi irkindan olanlarýn koyu mavisi
			}
		}
	} 
	
	if (*(uint8*)(*(DWORD*)KO_PTR_CHR + KO_WH) == 0)
	{
		if (Level < 30) {
		
			if (g_pMain->uiPartyBBS != NULL && g_pMain->uiPartyBBS->PartyFind(id))
				SetNameString(obj, GetName(obj) + " (" + to_string(*(uint8*)(obj + KO_OFF_LEVEL)) + ")", D3DCOLOR_ARGB(255, 255, 255, 0), 0); //party sari
			else
				SetNameString(obj, GetName(obj) + " (" + to_string(*(uint8*)(obj + KO_OFF_LEVEL)) + ")", D3DCOLOR_ARGB(255, 255, 255, 255), 0); //civciv beyaz
		}
		else 
		{
			if (g_pMain->uiPartyBBS != NULL && g_pMain->uiPartyBBS->PartyFind(id))
				SetNameString(obj, GetName(obj) + " (" + to_string(*(uint8*)(obj + KO_OFF_LEVEL)) + ")", D3DCOLOR_ARGB(255, 255, 255, 0), 0); //party sari
			else
			{
				if (dusmanMi) 
					SetNameString(obj, GetName(obj) + " (" + to_string(*(uint8*)(obj + KO_OFF_LEVEL)) + ")", D3DCOLOR_ARGB(255, 255, 128, 128), 0); //karsi irk kirmizi
				else 
					SetNameString(obj, GetName(obj) + " (" + to_string(*(uint8*)(obj + KO_OFF_LEVEL)) + ")", D3DCOLOR_ARGB(255, 128, 128, 255), 0); //kendi irkindan olanlarýn koyu mavisi
			}
		}
	}
#endif

	if (!name.empty())
	{
		g_pMain->TagLock.lock();
		auto itr = g_pMain->TagList.find(name);
		if (itr != g_pMain->TagList.end()) { TagName sTag = itr->second;		if (sTag.sTag.length() > 1) SetInfoString(obj, sTag.sTag, D3DCOLOR_ARGB(255, sTag.r, sTag.g, sTag.b), 0); }
		g_pMain->TagLock.unlock();
	}

	if (authority == 0 && *(uint8*)(obj + 0x7AC) == 3)
	{
		*(uint8*)(obj + 0x7AC) = 0;
		*(uint8*)(obj + KO_WH) = 1;
		return;
	}

	switch (authority)
	{
	case 0:
		if (*(uint8*)(*(DWORD*)KO_PTR_CHR + KO_WH) == 0)
		{
//#if (HOOK_SOURCE_VERSION == 1098)
//			if (isInMyParty)
//				SetNameString(obj, "[GM] " + GetName(obj) + " (" + to_string(*(uint8*)(obj + KO_OFF_LEVEL)) + ")", D3DCOLOR_ARGB(255, 255, 255, 0), 0); //party sari
//			else
//				SetNameString(obj, "[GM] " + GetName(obj) + " (" + to_string(*(uint8*)(obj + KO_OFF_LEVEL)) + ")", D3DCOLOR_ARGB(255, 225, 0, 228), 0); //gm nick turuncu 
//#endif
		}
		else
		{
			SetNameString(obj, "[GM] " + GetName(obj),0, 0);
		}

		SetInfoString(obj, xorstr("[GAME MASTER]"), D3DCOLOR_ARGB(255, 64, 236, 72), 0);
		break;
	}
	
}

const DWORD KO_FNC_TREE_CLAN_RENDER_RET = 0x576A62;
const DWORD KO_FNC_TREE_CLAN_RENDER_USER = 0x576AA9;
const DWORD KO_FNC_TREE_CLAN_RENDER_NEXT = 0x576A6D;
DWORD color_tree_rander = 0xff00ff00;
uint8_t __declspec(naked) Object_ClanTreeRanderHook()
{
	__asm
	{
		PUSH ESI
		MOV ESI, ECX
		CMP ESI, 1
		JE render_clan
		CMP ESI, 2
		JE render_clan
		XOR AL, AL
		POP ESI
		RETN
		render_clan :
		CMP ESI, 1
		MOV EAX, 0xFFFF00E4
		MOV ECX, 0xFF00FF00
		CMOVE EAX, ECX
		MOV DWORD PTR DS : color_tree_rander, EAX
		MOV AL, 1
		POP ESI
		RETN
	}
}

void __declspec(naked) hkClanTreeRanderHook()
{
	__asm
	{
		PUSHAD
		PUSHFD
		MOV ECX, EAX
		CALL Object_ClanTreeRanderHook
		POPFD
		POPAD
		CMP EAX, 1
		JE render_clan
		CMP EAX, 2
		JE render_clan
		JMP DWORD PTR DS : [KO_FNC_TREE_CLAN_RENDER_USER]
		render_clan :
		PUSH DWORD PTR DS : [KO_FNC_TREE_CLAN_RENDER_NEXT]
		RETN
	}
}

const DWORD KO_FNC_CLAN_RENDER_RET = 0x57CD5F;
const DWORD KO_FNC_CLAN_RENDER_USER = 0x57CDAF;
const DWORD KO_FNC_CLAN_RENDER_NEXT = 0x57CD6A;
DWORD color_rander = 0xff00ff00;

uint8_t __declspec(naked) Object_ClanRanderHook()
{
	__asm
	{
		PUSH ESI
		MOV ESI, ECX
		CMP ESI, 1
		JE render_clan
		CMP ESI, 2
		JE render_clan
		XOR AL, AL
		POP ESI
		RETN
		render_clan:
		CMP ESI, 1
		MOV EAX, 0xFFFF00E4
		MOV ECX, 0xFF00FF00
		CMOVE EAX, ECX
		MOV DWORD PTR DS : color_rander, EAX
		MOV AL, 1
		POP ESI
		RETN
	}
}

void __declspec(naked) hkClanRanderHook()
{
	__asm
	{
		PUSHAD
		PUSHFD
		MOV ECX, EAX
		CALL Object_ClanRanderHook
		POPFD
		POPAD
		CMP EAX, 1
		JE render_clan
		CMP EAX, 2
		JE render_clan
		JMP DWORD PTR DS : [KO_FNC_CLAN_RENDER_USER]
		render_clan:
		PUSH DWORD PTR DS : [KO_FNC_CLAN_RENDER_NEXT]
		RETN
	}
}

const DWORD KO_FNC_CLAN_RENDER_RET_USER = 0x57CD8F;
const DWORD KO_FNC_CLAN_RENDER_USER_NEXT = 0x57CD94;
void __declspec(naked) hkClanRanderHookUser()
{
	__asm
	{
		PUSH DWORD PTR DS : [color_rander]
		JMP DWORD PTR DS : [KO_FNC_CLAN_RENDER_USER_NEXT]
	}
}

void __declspec(naked) hkObjectMobLoop()
{
	__asm {
		mov eax, [ecx]
		mov edx, [eax + 0xC]
		pushad
		pushfd
		call Object_Mob_Callback
		popfd
		popad
		jmp KO_FNC_OBJECT_MOB_LOOP_RET
	}
}

void __declspec(naked) hkObjectPlayerLoop()
{
	__asm {
		pushad
		pushfd
		mov ecx, ebx
		call Object_Player_Callback
		popfd
		popad
		jmp KO_FNC_OBJECT_PLAYER_LOOP_ORG
	}
}


// KO Tick fonksiyonu (sürekli çalýþýr)
const DWORD KO_GAME_TICK = 0x411740;// 0x00410C10;
DWORD TICK_ORG = 0;

const DWORD OFF_PT = 0x358;
const DWORD OFF_PTCOUNT = 0x344;
const DWORD OFF_PTBASE = 0x238;

__inline DWORD RDWORD(DWORD ulBase){
	if (ulBase > 0x00AFFFFF 
		&& !IsBadReadPtr((VOID*)ulBase, sizeof(DWORD)))
		return(*(DWORD*)(ulBase));
	return 0;
}

const WORD KO_OFF_INV1 = 0x204; // 2369
const WORD KO_OFF_INV2 = 0x244; // 2369

DWORD InvItemId(int slot) {
	if (g_pMain->pClientInventory)
	{
		if (DWORD inv = g_pMain->pClientInventory->m_dVTableAddr)
		{
			if (DWORD itemBase = RDWORD(inv + KO_OFF_INV2 + (4 * slot)))
			{
				if (DWORD extBase = RDWORD(itemBase + 0x6C))
				{
					DWORD ID = RDWORD(RDWORD(itemBase + 0x68));   //infinityarrow
					DWORD EXT = RDWORD(extBase);  //infinityarrow
					return ID + EXT;
				}
			}
		}
	}
	return 0;
}

bool HasItem(DWORD itemid) {
	for (int x = 14; x < 42; x++) {
		if (InvItemId(x) == itemid) return true;
	}
	return false;
}

time_t m_bInfinityControl = 0;
time_t timeLapse = 0, guardCheck = 0;

time_t inv_animtime = 0;

std::vector<CSpell*> m_bArcherSpells;
std::vector<CSpell*> m_bCureSpells;
std::vector<CSpell*> m_bStoneSpells;

extern TABLE_MOB* GetMobBase(DWORD MobID);

typedef char(__thiscall* tsub_763350)(DWORD ecx, int a2, int a3, int a4, int a5, int a6);
tsub_763350 sub_763350 = (tsub_763350)0x763350;

bool isChicken = false;

void __fastcall civcivVer()
{

}

void __declspec(naked) hkRankImage()
{
	__asm {
		mov al, isChicken
		movzx eax, al
		test eax, eax


		civcivle:

	}
}
bool InifinityArrow = false;

static bool m_bSpellsLoaded = false;
void __fastcall myTick()
{
	if (timeLapse > clock() - 30)
		return;

	timeLapse = clock();

	if (guardCheck < clock() - 200)
	{
		guardCheck = clock();
		if (!g_pMain->GuardLockPerkSystem.Check())
		{
			raise(SIGSEGV);
		}
	}

	*(float*)0xDE2B98 = 1.0f / 999.0f;																	// -------------------- fps limiti kaldýrma

	if (DWORD target = g_pMain->GetTarget()) 
	{
		if (*(uint16*)(target + KO_OFF_ID) >= NPC_BAND) 
		{
			uint8 myLvl = *(uint8*)(*(DWORD*)KO_PTR_CHR + KO_OFF_LEVEL);
			uint8 lvl = *(uint8*)(target + KO_OFF_LEVEL);
			uint8 nation = *(uint8*)(target + KO_OFF_NATION);
			uint16 protoID = *(uint16*)(target + KO_SSID);
			DWORD color = GetLevelColor(myLvl - lvl);
			std::string name = GetName(target);
			if (g_pMain->pClientTargetBar && nation == 0 
				&& protoID != 19067 && protoID != 19068 && protoID != 19069 && protoID != 19070 && protoID != 19071 && protoID != 19072) 
				g_pMain->pClientTargetBar->SetInfo(color, lvl, name);
		}
	}

#if (HOOK_SOURCE_VERSION == 1098)
	if (!partyInit)
	{
		PartyUser mem{};
		mem.Id = 0;
		mem.Hp = 0;
		mem.MaxHp = 0;
		mem.Mp = 0;
		mem.MaxMp = 0;
		mem.Class = 0;
		mem.Level = 0;
		mem.Curse = false;
		mem.Disease = false;
		for (uint8 i = 0; i < 8; i++) partyMember.push_back(mem);
		partyInit = true;
	}
#endif
	if (*(DWORD*)KO_PTR_CHR) 
	{
		if (g_pMain->m_PlayerBase)
			g_pMain->m_PlayerBase->m_iZoneID = *(uint8*)(*(DWORD*)KO_PTR_CHR + KO_OFF_ZONE);

		if (g_pMain->pClientInventory
			&& GetTickCount64() > inv_animtime
			&& g_pMain->IsVisible(g_pMain->pClientInventory->m_dVTableAddr))
		{
			DWORD chr = *(DWORD*)KO_PTR_CHR + KO_OFF_N3CHR;
			if (chr)
				__ChrAniCurSet(chr, *(int*)(*(DWORD*)KO_PTR_CHR + KO_OFF_ANIM_ID), false, 1.175494351e-38F, 0, true);
			inv_animtime = GetTickCount64() + 200;
		}
		
		if (!g_pMain->privatemessages.empty())
		{
			for (auto pPrivateCheat : g_pMain->privatemessages)
			{
				pPrivateCheat->Tick();
			}
		}
#if (HOOK_SOURCE_VERSION == 1098)

		//if (!IsDebuggerPresent())
		//{
		//	DWORD base, buffer, partyCount;

		//	base = RDWORD(RDWORD(KO_DLG) + OFF_PTBASE);

		//	if (base > 0)
		//		buffer = RDWORD(base + OFF_PT);

		//	if (base > 0)
		//		partyCount = RDWORD(base + OFF_PTCOUNT);

		//	if (base > 0 && buffer > 0)
		//	{
		//		amIInParty = partyCount > 1;
		//		waitingForParty = partyCount > 0;

		//		for (int i = 0; i < 8; i++)
		//		{
		//			partyMember[i].Id = 0;
		//			partyMember[i].Hp = 0;
		//			partyMember[i].MaxHp = 0;
		//			partyMember[i].Mp = 0;
		//			partyMember[i].MaxMp = 0;
		//			partyMember[i].Class = 0;
		//			partyMember[i].Level = 0;
		//			partyMember[i].Curse = false;
		//			partyMember[i].Disease = false;
		//		}

		//		for (int i = 0; i < partyCount; i++)
		//		{
		//			if (partyCount > 0)
		//			{
		//				buffer = RDWORD(buffer);
		//				if (buffer)
		//				{
		//					partyMember[i].Id = *(USHORT*)(buffer + 0x8);
		//					partyMember[i].Level = *(USHORT*)(buffer + 0xC);
		//					partyMember[i].Class = *(USHORT*)(buffer + 0x10);
		//					partyMember[i].Hp = *(USHORT*)(buffer + 0x18);
		//					partyMember[i].MaxHp = *(USHORT*)(buffer + 0x1C);
		//					partyMember[i].Mp = *(USHORT*)(buffer + 0x20);
		//					partyMember[i].MaxMp = *(USHORT*)(buffer + 0x24);
		//					partyMember[i].Curse = *(bool*)(buffer + 0x28);
		//					partyMember[i].Disease = *(bool*)(buffer + 0x29);
		//				}
		//			}
		//		}
		//	}
		//}
#endif
		if (m_bInfinityControl < clock() - 2000)
		{
			m_bInfinityControl = clock();
			if (g_pMain->pClientInventory)
			{
				if (DWORD inv = g_pMain->pClientInventory->m_dVTableAddr)
				{
					if (!m_bSpellsLoaded) 
					{
						m_bSpellsLoaded = true;
						m_bArcherSpells.clear();
						m_bCureSpells.clear();
						m_bStoneSpells.clear();

						for (uint32 i = 102003; i <= 288566; i++)
						{
							if (CSpell* spell = GetSkillBase(i)) {
								if (spell->dwNeedItem == 391010000)
									m_bArcherSpells.push_back(spell);
							}
						}

						std::map<uint32, CSpell>::iterator itr;
						for (itr = g_pMain->skillmap.begin(); itr != g_pMain->skillmap.end(); itr++)
						{
							if (CSpell* spell = GetSkillBase(itr->second.dwID)) 
							{
								if (spell->dwNeedItem == 379062000
									|| spell->dwNeedItem == 379059000
									|| spell->dwNeedItem == 379060000
									|| spell->dwNeedItem == 379061000)
									m_bStoneSpells.push_back(spell);
							}
						}

						if (CSpell* spell = GetSkillBase(107736)) 
							m_bCureSpells.push_back(spell);

						if (CSpell* spell = GetSkillBase(108736)) 
							m_bCureSpells.push_back(spell);

						if (CSpell* spell = GetSkillBase(207736)) 
							m_bCureSpells.push_back(spell);

						if (CSpell* spell = GetSkillBase(208736)) 
							m_bCureSpells.push_back(spell);
					}

					if (HasItem(479060000) && !m_bStoneRogueStatus) // Infinity Stone Of Rogue
					{
						for (auto& s : m_bStoneSpells)
						{
							if (s->dwNeedItem == 379060000)
							{
								s->dwNeedItem = 479060000;
								auto itcrc = g_pMain->skillcrc.find(s->dwID + 2031);
								if (itcrc != g_pMain->skillcrc.end())
								{
									SpellCRC crc(crc32((uint8*)(DWORD)s, 0xA8, -1), crc32((uint8*)((DWORD)s + 0xB4), 0x2C, -1));
									itcrc->second = crc;
								}

								*(DWORD*)(0x00673C46 + 1) = 479060000;
								*(DWORD*)(0x00676C1F + 1) = 479060000;
								*(DWORD*)(0x009348F7 + 1) = 479060000;
								*(DWORD*)(0x00934A96 + 1) = 479060000;
								*(DWORD*)(0x00934ACD + 1) = 479060000;
								*(DWORD*)(0x00934AEC + 1) = 479060000;
								*(DWORD*)(0x00936B71 + 1) = 479060000;
								*(DWORD*)(0x00936BAF + 1) = 479060000;
								*(DWORD*)(0x00936C12 + 1) = 479060000;
							}
						}
						m_bStoneRogueStatus = true;
					}
					else if (!m_bStoneRogueStatus)
					{
						for (auto& s : m_bStoneSpells)
						{
							if (s->dwNeedItem == 479060000)
							{
								s->dwNeedItem = 379060000;
								auto itcrc = g_pMain->skillcrc.find(s->dwID + 2031);
								if (itcrc != g_pMain->skillcrc.end())
								{
									SpellCRC crc(crc32((uint8*)(DWORD)s, 0xA8, -1), crc32((uint8*)((DWORD)s + 0xB4), 0x2C, -1));
									itcrc->second = crc;
								}

								*(DWORD*)(0x00673C46 + 1) = 379060000;
								*(DWORD*)(0x00676C1F + 1) = 379060000;
								*(DWORD*)(0x009348F7 + 1) = 379060000;
								*(DWORD*)(0x00934A96 + 1) = 379060000;
								*(DWORD*)(0x00934ACD + 1) = 379060000;
								*(DWORD*)(0x00934AEC + 1) = 379060000;
								*(DWORD*)(0x00936B71 + 1) = 379060000;
								*(DWORD*)(0x00936BAF + 1) = 379060000;
								*(DWORD*)(0x00936C12 + 1) = 379060000;
							}
						}
					}

					if (HasItem(479061000) && !m_bStoneMageStatus) // Infinity Stone Of Mage
					{
						for (auto& s : m_bStoneSpells)
						{
							if (s->dwNeedItem == 379061000)
							{
								s->dwNeedItem = 479061000;
								auto itcrc = g_pMain->skillcrc.find(s->dwID + 2031);
								if (itcrc != g_pMain->skillcrc.end())
								{
									SpellCRC crc(crc32((uint8*)(DWORD)s, 0xA8, -1), crc32((uint8*)((DWORD)s + 0xB4), 0x2C, -1));
									itcrc->second = crc;
								}

								*(DWORD*)(0x00673C9E + 1) = 479061000;
								*(DWORD*)(0x00676C77 + 1) = 479061000;
								*(DWORD*)(0x00934B10 + 1) = 479061000;
								*(DWORD*)(0x00934B47 + 1) = 479061000;
								*(DWORD*)(0x00934B66 + 1) = 479061000;
								*(DWORD*)(0x00934B71 + 1) = 479061000;
								*(DWORD*)(0x00936C2F + 1) = 479061000;
								*(DWORD*)(0x00936C6D + 1) = 479061000;
								*(DWORD*)(0x00936CD0 + 1) = 479061000;
							}
						}
						m_bStoneMageStatus = true;
					}
					else if (!m_bStoneMageStatus)
					{
						for (auto& s : m_bStoneSpells)
						{
							if (s->dwNeedItem == 479061000)
							{
								s->dwNeedItem = 379061000;
								auto itcrc = g_pMain->skillcrc.find(s->dwID + 2031);
								if (itcrc != g_pMain->skillcrc.end())
								{
									SpellCRC crc(crc32((uint8*)(DWORD)s, 0xA8, -1), crc32((uint8*)((DWORD)s + 0xB4), 0x2C, -1));
									itcrc->second = crc;
								}

								*(DWORD*)(0x00673C9E + 1) = 379061000;
								*(DWORD*)(0x00676C77 + 1) = 379061000;
								*(DWORD*)(0x00934B10 + 1) = 379061000;
								*(DWORD*)(0x00934B47 + 1) = 379061000;
								*(DWORD*)(0x00934B66 + 1) = 379061000;
								*(DWORD*)(0x00934B71 + 1) = 379061000;
								*(DWORD*)(0x00936C2F + 1) = 379061000;
								*(DWORD*)(0x00936C6D + 1) = 379061000;
								*(DWORD*)(0x00936CD0 + 1) = 379061000;
							}
						}
					}

					if (HasItem(479062000) && !m_bStonePriestStatus) // Infinity Stone Of Priest
					{
						for (auto& s : m_bStoneSpells)
						{
							if (s->dwNeedItem == 379062000)
							{
								s->dwNeedItem = 479062000;
								auto itcrc = g_pMain->skillcrc.find(s->dwID + 2031);
								if (itcrc != g_pMain->skillcrc.end())
								{
									SpellCRC crc(crc32((uint8*)(DWORD)s, 0xA8, -1), crc32((uint8*)((DWORD)s + 0xB4), 0x2C, -1));
									itcrc->second = crc;
								}

								*(DWORD*)(0x00673CF6 + 1) = 479062000;
								*(DWORD*)(0x00676CCF + 1) = 479062000;
								*(DWORD*)(0x00934B8A + 1) = 479062000;
								*(DWORD*)(0x00934BC1 + 1) = 479062000;
								*(DWORD*)(0x00934BE0 + 1) = 479062000;
								*(DWORD*)(0x00936CED + 1) = 479062000;
								*(DWORD*)(0x00936D23 + 1) = 479062000;
								*(DWORD*)(0x00936D69 + 1) = 479062000;
							}
						}
						m_bStonePriestStatus = true;
					}
					else if (!m_bStonePriestStatus)
					{
						for (auto& s : m_bStoneSpells)
						{
							if (s->dwNeedItem == 479062000)
							{
								s->dwNeedItem = 379062000;
								auto itcrc = g_pMain->skillcrc.find(s->dwID + 2031);
								if (itcrc != g_pMain->skillcrc.end())
								{
									SpellCRC crc(crc32((uint8*)(DWORD)s, 0xA8, -1), crc32((uint8*)((DWORD)s + 0xB4), 0x2C, -1));
									itcrc->second = crc;
								}

								*(DWORD*)(0x00673CF6 + 1) = 379062000;
								*(DWORD*)(0x00676CCF + 1) = 379062000;
								*(DWORD*)(0x00934B8A + 1) = 379062000;
								*(DWORD*)(0x00934BC1 + 1) = 379062000;
								*(DWORD*)(0x00934BE0 + 1) = 379062000;
								*(DWORD*)(0x00936CED + 1) = 379062000;
								*(DWORD*)(0x00936D23 + 1) = 379062000;
								*(DWORD*)(0x00936D69 + 1) = 379062000;
							}
						}
					}

					if (HasItem(479059000) && !m_bStoneWarriorStatus) // Infinity Stone Of Warrior
					{
						for (auto& s : m_bStoneSpells)
						{
							if (s->dwNeedItem == 379059000)
							{
								s->dwNeedItem = 479059000;
								auto itcrc = g_pMain->skillcrc.find(s->dwID + 2031);
								if (itcrc != g_pMain->skillcrc.end())
								{
									SpellCRC crc(crc32((uint8*)(DWORD)s, 0xA8, -1), crc32((uint8*)((DWORD)s + 0xB4), 0x2C, -1));
									itcrc->second = crc;
								}

								*(DWORD*)(0x00673BEE + 1) = 479059000;
								*(DWORD*)(0x00676BC7 + 1) = 479059000;
								*(DWORD*)(0x009348F7 + 1) = 479059000;
								*(DWORD*)(0x0093493A + 1) = 479059000;
								*(DWORD*)(0x00934A72 + 1) = 479059000;
								*(DWORD*)(0x00934AB3 + 1) = 479059000;
								*(DWORD*)(0x00936AF1 + 1) = 479059000;
								*(DWORD*)(0x00936B54 + 1) = 479059000;
							}
						}
						m_bStoneWarriorStatus = true;
					}
					else if (!m_bStoneWarriorStatus)
					{
						for (auto& s : m_bStoneSpells)
						{
							if (s->dwNeedItem == 479059000)
							{
								s->dwNeedItem = 379059000;
								auto itcrc = g_pMain->skillcrc.find(s->dwID + 2031);
								if (itcrc != g_pMain->skillcrc.end())
								{
									SpellCRC crc(crc32((uint8*)(DWORD)s, 0xA8, -1), crc32((uint8*)((DWORD)s + 0xB4), 0x2C, -1));
									itcrc->second = crc;
								}

								*(DWORD*)(0x00673BEE + 1) = 379059000;
								*(DWORD*)(0x00676BC7 + 1) = 379059000;
								*(DWORD*)(0x009348F7 + 1) = 379059000;
								*(DWORD*)(0x0093493A + 1) = 379059000;
								*(DWORD*)(0x00934A72 + 1) = 379059000;
								*(DWORD*)(0x00934AB3 + 1) = 379059000;
								*(DWORD*)(0x00936AF1 + 1) = 379059000;
								*(DWORD*)(0x00936B54 + 1) = 379059000;
							}
						}
					}

					if (HasItem(346391000) && !m_bCureStatus) // Infinity cure
					{
						for (auto &s : m_bCureSpells)
						{
							if (s->dwNeedItem == 370005000) 
							{
								s->dwNeedItem = 0;
								auto itcrc = g_pMain->skillcrc.find(s->dwID + 2031);
								if (itcrc != g_pMain->skillcrc.end()) 
								{
									SpellCRC crc(crc32((uint8*)(DWORD)s, 0xA8, -1), crc32((uint8*)((DWORD)s + 0xB4), 0x2C, -1));
									itcrc->second = crc;
								}
							}
						}
						m_bCureStatus = true;
					}
					else if(!m_bCureStatus)
					{
						for (auto &s : m_bCureSpells)
						{
							if (s->dwNeedItem == 0) 
							{
								s->dwNeedItem = 370005000;
								auto itcrc = g_pMain->skillcrc.find(s->dwID + 2031);
								if (itcrc != g_pMain->skillcrc.end()) 
								{
									SpellCRC crc(crc32((uint8*)(DWORD)s, 0xA8, -1), crc32((uint8*)((DWORD)s + 0xB4), 0x2C, -1));
									itcrc->second = crc;
								}
							}
						}
					}

					if (HasItem(800606000) && !m_bArrowStatus) // Infinity Arrow
					{
						for (auto &s : m_bArcherSpells)
						{
							if (s->dwNeedItem == 391010000) 
							{
								s->dwNeedItem = 0;
								auto itcrc = g_pMain->skillcrc.find(s->dwID + 2031);
								if (itcrc != g_pMain->skillcrc.end()) 
								{
									SpellCRC crc(crc32((uint8*)(DWORD)s, 0xA8, -1), crc32((uint8*)((DWORD)s + 0xB4), 0x2C, -1));
									itcrc->second = crc;
								}
							}
						}
						m_bArrowStatus = true;
					}
					else if(!m_bArrowStatus)
					{
						for (auto &s : m_bArcherSpells)
						{
							if (s->dwNeedItem == 0) 
							{
								s->dwNeedItem = 391010000;
								auto itcrc = g_pMain->skillcrc.find(s->dwID + 2031);
								if (itcrc != g_pMain->skillcrc.end()) 
								{
									SpellCRC crc(crc32((uint8*)(DWORD)s, 0xA8, -1), crc32((uint8*)((DWORD)s + 0xB4), 0x2C, -1));
									itcrc->second = crc;
								}
							}
						}
					}
				}
			}
		}
	} 
}

DWORD __declspec(naked) hkTick()
{
	__asm {
		pushad
		pushfd
		call myTick
		popfd
		popad
		jmp TICK_ORG
	}
}

#if (HOOK_SOURCE_VERSION == 2369)
DWORD TICK_COM = 0;
time_t pGameStartTick = 0;
void __fastcall GameStartTick()
{
	if (pGameStartTick > clock() - 30)
		return;

	pGameStartTick = clock();

	if (g_pMain->m_bGameStart)
	{
		if (g_pMain->pClientHookManager->pClientTargetSkillCheck == NULL)
			return;

		if (DWORD UserTarget = g_pMain->GetTarget())
		{
			if (*(uint16_t*)(UserTarget + KO_OFF_ID) < NPC_BAND)
			{
				if (!g_pMain->pClientHookManager->pClientTargetSkillCheck->IsVisible())
					g_pMain->pClientHookManager->pClientTargetSkillCheck->SetVisible(true);

				g_pMain->pClientHookManager->pClientTargetSkillCheck->UpdatePosition();
			}
		}
		else
		{
			if (g_pMain->pClientHookManager->pClientTargetSkillCheck->IsVisible())
				g_pMain->pClientHookManager->pClientTargetSkillCheck->SetVisible(false);
		}

		/*if (DWORD MobTarget = g_pMain->GetTarget())
		{
			if (*(uint16_t*)(MobTarget + KO_OFF_ID) >= NPC_BAND)
			{
				if (!g_pMain->pClientHookManager->pClientTargetSkillCheck->IsVisible())
					g_pMain->pClientHookManager->pClientTargetSkillCheck->SetVisible(true);

				g_pMain->pClientHookManager->pClientTargetSkillCheck->UpdatePosition();
			}
		}
		else
		{
			if (g_pMain->pClientHookManager->pClientTargetSkillCheck->IsVisible())
				g_pMain->pClientHookManager->pClientTargetSkillCheck->SetVisible(false);
		}*/
	}
}

DWORD __declspec(naked) HkGameStartTick()
{
	__asm
	{
		pushad
		pushfd
		call GameStartTick
		popfd
		popad
		jmp TICK_COM
	}
}
#endif

DWORD rtOrg = 0;
DWORD rTmp = 0;

DWORD __declspec(naked) hkSetFontString()
{
	__asm {
		pushad
		pushfd
		mov rTmp, ecx
	}
	if (RDWORD(rTmp + 0x1C) == 0)
	{
		__asm {
			popfd
			popad
			xor eax, eax
			ret 8
		}
	}
	__asm {
		popfd
		popad
		jmp rtOrg
	}
}

#if (HOOK_SOURCE_VERSION == 1098)
// minimap harita pozisyon ayarlarý
const int upY = 5;
const int upX = 17;

void __declspec(naked) hkMinimapPos()
{
	__asm {
		add edx, upY
		add eax, upX
		push edx
		push eax
		mov eax, [ESI + 0x2C]
		call eax
		pop edi
		pop esi
		pop ebp
		pop ebx
		add esp, 0x38
		ret
	}
}
#endif

DWORD HSACSEngine::KOGetTarget()
{
	DWORD KO_ADR = *(DWORD*)KO_PTR_CHR;
	uint16 targetID = *(DWORD*)(KO_ADR + KO_OFF_MOB);
	if (targetID >= NPC_BAND) { Func_GetObjectBase = (tGetObjectBase)KO_FMBS;		return Func_GetObjectBase(*(DWORD*)KO_FLDB, targetID, 1); }
	else { Func_GetObjectBase = (tGetObjectBase)KO_FPBS;		return Func_GetObjectBase(*(DWORD*)KO_FLDB, targetID, 1); }
	return NULL;
}

size_t findCaseInsensitive(std::string data, std::string toSearch)
{
	if (data.empty() || toSearch.empty())
		return string::npos;

	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	std::transform(toSearch.begin(), toSearch.end(), toSearch.begin(), ::tolower);
	return data.find(toSearch, 0);
}

const DWORD KO_AUTO_ATTACK = 0x0053E61A;
const DWORD KO_ZONE_LOAD_TERRAIN = 0x009544D3;
const DWORD KO_ZONE_LOAD_TERRAIN_KEEP = 0x009544F0;
DWORD KO_ZONE_LOAD_TERRAIN_ORG = 0;

std::list<std::string> oldMapList_GTD = {

	"battlezone_f.gtd",
	//"freezone_a_event2012.gtd"
	//"freezone_b_2012event.gtd",
	"In_dungeon06.gtd",
	"Old_Karus.gtd",
	"Old_Elmo.gtd",
	"2017_flagwar.gtd",
	"Worldwar.gtd",
	"FreeZone_bb.gtd",
	"FreeZone.gtd",
	"moradon_hsacsx.gtd",
#if (HOOK_SOURCE_VERSION == 1098)
	"xmas_battle.gtd",
	"moradon_xmas.gtd",
	"moradon.gtd",
	"war_a.gtd",
	"karus2004.gtd",
	"elmo2004.gtd",
	//"In_dungeon04.gtd",
#endif
};

bool __fastcall IsOldMap(const std::string& gtd)
{
	const std::string tmpGtd = gtd;
	auto it = std::find_if(oldMapList_GTD.begin(), oldMapList_GTD.end(), [&](const std::string& v) {  return findCaseInsensitive(tmpGtd, v) != string::npos; });
	return it != oldMapList_GTD.end();
}

void __declspec(naked) hkLoadTerrainData()
{
	__asm {
		pushad
		pushfd
		mov ecx, esi
		add ecx, 0x24
		call IsOldMap
		movzx eax, al
		test eax, eax
		jne okuma 
		mov ecx, DWORD PTR DS : [KO_PTR_CHR]
		mov ecx, [ecx + KO_OFF_ZONE]
		cmp ecx, 0x70 // Harita zonesi hex kodu yazýlacak.
		je okuma
		popfd
		popad
		jmp KO_ZONE_LOAD_TERRAIN_ORG
		okuma :
		popfd
		popad
		jmp KO_ZONE_LOAD_TERRAIN_KEEP
	}
}

bool m_bGenieAttackStatus = true;


bool __fastcall RAttack(DWORD caller = NULL)
{
	if (caller && caller >= 0xc78b7c && !IsBadReadPtr((void*)caller, sizeof(DWORD)))
		return m_bGenieAttackStatus && *(DWORD*)caller == 0xc78b7c;  // Genie R atack buton kodu
	return false;
}



void __declspec(naked) hkAttack()
{
	__asm {
		mov esi, 0xFFFE
		mov edx, [ESP]
		pushad
		pushfd
		mov ecx, edx
		call RAttack
		movzx eax, al
		test eax, eax
		jne engeller
		popfd
		popad
		mov edx, KO_AUTO_ATTACK
		add edx, 5
		push edx
		ret
		engeller :
		popfd
		popad
		pop edi
		xor al, al
		pop esi
		ret
	}
}

void __fastcall hkDisableVisible(DWORD ecx, DWORD edx, bool a)
{
	a = false;
	*(DWORD*)(ecx + 0x10D) = a;
}

//void __fastcall hkOpenMiniMapfromGlobal(DWORD ecx, DWORD edx, bool a)  // Büyük MAp (M) olaný Küçük MAp'e (N) 'ye çeviriyoruz.
//{
//	a = false;
//	*(DWORD*)(ecx + 0x10D) = a;
//
//	if (g_pMain->uiMinimapPlug) {
//		if (DWORD map = g_pMain->uiMinimapPlug->m_dVTableAddr) {
//			if (*(bool*)(map + 0xF9))
//			{
//				__asm {
//					pushad
//					pushfd
//					mov ecx, map
//					mov eax, 0x846C40
//					call eax
//					popfd
//					popad
//				}
//			}
//			else {
//				__asm {
//					pushad
//					pushfd
//					mov ecx, map
//					mov eax, 0x846E00
//					call eax
//					popfd
//					popad
//				}
//			}
//		}
//	}
//}


const DWORD KO_SKILLBAR_KEY_INPUT = 0x00670AA0;
const DWORD KO_SKILLBAR_KEY_INPUT_BLOCK = 0x00670AAC;
DWORD KO_SKILLBAR_KEY_INPUT_KEEP = NULL;
DWORD KO_SKILLBAR_SHORTCUT_KEEP = 0x005007D5;
DWORD KO_SKILLBAR_MOUSE_INPUT_KEEP = 0x0066E519;

const DWORD KO_SKILLBAR_SHORTCUT_INPUT = 0x005007D0;
const DWORD KO_SKILLBAR_SHORTCUT_BLOCK = 0x005007DE;
const DWORD KO_SKILLBAR_MOUSE_INPUT = 0x0066E514; // EDX
const DWORD KO_SKILLBAR_MOUSE_INPUT_BLOCK = 0x0066E58D;

bool __fastcall MacroEngels()
{
	if (g_pMain->m_bGenieStatus) // geniede skill attýrmak istiyorsan bunu kaldýr
		return true;

	return false;
}

void __declspec(naked) hkSkillbarShortcutInput()
{
	__asm {
		pushad
		pushfd
		call MacroEngels
		movzx eax, al
		test eax, eax
		jne engellez
		popfd
		popad
		mov eax, [0x0F368E8] //2369
		jmp KO_SKILLBAR_SHORTCUT_KEEP
		engellez :
		popfd
		popad
		jmp KO_SKILLBAR_SHORTCUT_BLOCK
	}
}

void __declspec(naked) hkSkillbarMouseInput()
{
	__asm {
		pushad
		pushfd
		call MacroEngels
		movzx eax, al
		test eax, eax
		jne engellex
		popfd
		popad
		mov eax, 0x004C65A0 // 2369
		call eax
		jmp KO_SKILLBAR_MOUSE_INPUT_KEEP
		engellex :
		popfd
		popad
		jmp KO_SKILLBAR_MOUSE_INPUT_BLOCK
	}
}
bool __fastcall SkillKeyCheck(DWORD key)
{
	if (g_pMain->m_bGenieStatus) // Genie açýkken skill vurmamasý ayarlandý. // geniede skill attýrmak istiyorsan bunu kaldýr
		return true;

#if (HOOK_SOURCE_VERSION == 1098 || HOOK_SOURCE_VERSION == 1534)
	return key >= 8;
#endif

	return false;
}

void __declspec(naked) hkSkillbarKeyInput()
{
	__asm {
		pushad
		pushfd
		mov ecx, edi
		call SkillKeyCheck
		movzx eax, al
		test eax, eax
		jne engelle
		popfd
		popad
		jmp KO_SKILLBAR_KEY_INPUT_KEEP
		engelle :
		popfd
		popad
		jmp KO_SKILLBAR_KEY_INPUT_BLOCK
	}
}


DWORD KO_SET_EVENTNOTICE_POS = 0x0081BB92;
DWORD KO_SET_EVENTNOTICE_POS_ORG = 0;

void __fastcall Ayarlamk(DWORD efx)
{
	g_pMain->hkServerNotice = efx;
	koScreen = *(POINT*)KO_UI_SCREEN_SIZE;
	POINT pt;
	g_pMain->GetUiPos(efx, pt);
	//Genie YNG
#if 0
#if (HOOK_SOURCE_VERSION == 1098 || HOOK_SOURCE_VERSION == 1534)	// 1098 & 1534 vGenie pozisyon ayarlamasý Militarycamp ve premiumlar
	LONG isPost = 63;  
	if (g_pMain->uiGenieSubPlug != NULL)
	{
		if (g_pMain->IsVisible(g_pMain->uiGenieSubPlug->TargetSam))
			isPost = 115;   
		else if (g_pMain->IsVisible(g_pMain->uiGenieSubPlug->TargetTwo))
			isPost = 89;	
#else		// 2369 Genie pozisyon ayarlamasý Militarycamp ve premiumlar
	LONG isPost = 69;  
	if (g_pMain->uiGenieSubPlug != NULL)
	{
		if (g_pMain->IsVisible(g_pMain->uiGenieSubPlug->TargetSam))
			isPost = 117;   
		else if (g_pMain->IsVisible(g_pMain->uiGenieSubPlug->TargetTwo))
			isPost = 93;	
#endif
		if (g_pMain->IsVisible(g_pMain->hkCampAdress))
			isPost += 23;

		pt.y = isPost;
		g_pMain->SetUIPos(efx, pt);
		if (g_pMain->IsVisible(g_pMain->hkCampAdress))
		{
			g_pMain->GetUiPos(g_pMain->hkCampAdress, pt);
			pt.y = LONG(isPost - 23);
			g_pMain->SetUIPos(g_pMain->hkCampAdress, pt);
		}
		POINT zs;
		zs.x = koScreen.x - 187;		//genie sað base konumu
		zs.y = 0;
		g_pMain->SetUIPos(g_pMain->uiGenieSubPlug->m_dVTableAddr, zs);
	}
#endif

	if (g_pMain->uiTaskbarMain) g_pMain->uiTaskbarMain->UpdatePosition();
}

void __declspec(naked) hkEventNotice()
{
	__asm {
		pushad
		pushfd
		call Ayarlamk
		popfd
		popad
		jmp KO_SET_EVENTNOTICE_POS_ORG
	}
}

DWORD GENIE_ORG = 0x40F7D0;
POINT ptgenie;
int geniex, geniey;

void __declspec(naked) hkGenieSetPos()
{
	__asm {
		pushad
		pushfd
	}

	koScreen = *(POINT*)KO_UI_SCREEN_SIZE;
#if (HOOK_SOURCE_VERSION == 1098 || HOOK_SOURCE_VERSION == 1534)
	geniex = koScreen.x - 170;
#else
	geniex = koScreen.x - 187;
#endif
	geniey = 0;
	__asm {
		popfd
		popad
		mov edx, geniex
		mov [esp + 4], edx
		mov edx, geniey
		mov [esp + 8], edx
		jmp GENIE_ORG
	}
}

#if (HOOK_SOURCE_VERSION == 1098)
DWORD Real_CharacterSelectLeft = 0x004DDAE0;					// 2369
DWORD Real_CharacterSelectLeft_ORG = 0x004DDAE0;				// 2369
DWORD Real_CharacterSelectRight = 0x004DDD80;					// 2369
DWORD Real_CharacterSelectRight_ORG = 0x004DDD80;				// 2369
void __fastcall Hook_CharacterSelectLeft(LPDWORD, LPVOID)
{
	if ((int)(*(DWORD*)(*(DWORD*)(0xF36908) + 0x48C)) != 1)		// 2369
	{
		__asm
		{
			MOV ECX, DWORD PTR DS : [0x00F36908]				// 2369
			CALL Real_CharacterSelectLeft_ORG
		}
	}
}

void __fastcall Hook_CharacterSelectRight(LPDWORD, LPVOID)
{
	if ((int)(*(DWORD*)(*(DWORD*)(0xF36908) + 0x48C)) != 2)  // 2369
	{
		if ((int)(*(DWORD*)(*(DWORD*)(0xF36908) + 0x48C)) == 0)  // 2369
		{
			(*(DWORD*)(*(DWORD*)(0xF36908) + 0x48C)) = 1;  // 2369
			__asm
			{
				MOV ECX, DWORD PTR DS : [0x00F36908]  // 2369
				CALL Real_CharacterSelectLeft_ORG
			}
		}
		else
		{
			__asm
			{
				MOV ECX, DWORD PTR DS : [0x00F36908]  // 2369
				CALL Real_CharacterSelectRight_ORG
			}
		}
	}
} 
#endif

typedef bool (WINAPI* tUIOnKeyPress)(int iKey);
bool WINAPI hkUIOnKeyPress(int iKey)
{
	static int i = 0;
	if (iKey == DIK_RETURN || iKey == DIK_NUMPADENTER) 
	{
		i++;
		bool r = g_pMain->uiPowerUpStore  && g_pMain->IsVisible(g_pMain->uiPowerUpStore->m_dVTableAddr);
		g_pMain->uiPowerUpStore->ReceiveMessage((DWORD*)g_pMain->uiPowerUpStore->btn_search, UIMSG_BUTTON_CLICK);
		return r;
	}
	else if (iKey == DIK_ESCAPE)
	{
		if (g_pMain->uiTagChange != NULL && g_pMain->IsVisible(g_pMain->uiTagChange->m_dVTableAddr))
			g_pMain->uiTagChange->Close();

#if (HOOK_SOURCE_VERSION == 2369 || HOOK_SOURCE_VERSION == 1098)
		if (g_pMain->pClientRightExchange != NULL)
		{
			if (g_pMain->IsVisible(g_pMain->pClientRightExchange->el_Base))
				g_pMain->SetVisible(g_pMain->pClientRightExchange->el_Base, false);
		}
#endif
	}

//#if (HOOK_SOURCE_VERSION == 2369)
	if (g_pMain->pClientDailyReward != NULL)
	{
		if (g_pMain->IsVisible(g_pMain->pClientDailyReward->m_dVTableAddr))
			g_pMain->pClientDailyReward->OnKeyPress(iKey);
	}
//#endif
	return false;
}

#define D3DX_PI    ((FLOAT)  3.141592654f)
#define D3DXToRadian( degree ) ((degree) * (D3DX_PI / 180.0f))
#define D3DXToDegree( radian ) ((radian) * (180.0f / D3DX_PI))

const DWORD GetMark = 0x0078FE08;						// Civciv Sembolü ekleme
constexpr DWORD secondadres2s = 0x0078FE11;				// Civciv Sembolü ekleme
const DWORD ar = 0x0078FC00;							// Civciv Sembolü ekleme
bool _fastcall civcivmi(DWORD nPlayerBase, DWORD eax)
{
	if (nPlayerBase == eax)
	{
		uint8 m_iLevel;
		DWORD chr;
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)KO_PTR_CHR, &chr, sizeof(chr), 0);
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)(chr + 0x74C), &m_iLevel, sizeof(m_iLevel), 0);

		if (m_iLevel > 0 && m_iLevel < 30)
			return true;
		else
			return false;
	}

	if (IsBadReadPtr((VOID*)nPlayerBase, sizeof(DWORD)))
		return false;
	uint8 val = *(uint8*)(nPlayerBase + 0x74C);

	if (val > 0 && val < 30)
		return true;

	return false;
}

void __declspec(naked) hkMarkPos()
{
	__asm
	{
		pushad
		pushfd
		mov ecx, esi
		mov edx, eax
		call civcivmi
		movzx eax, al
		test eax, eax
		jne devam
		popfd
		popad
		push 0x18
		push 0x00
		call ar
		jmp secondadres2s
		devam :
		popfd
		popad
		push  0x06
		push  0x00
		call ar
		jmp secondadres2s
	}
}

DWORD KO_MAGIC_SKILL_ANIMATION_ORG=0;
bool isAttack = true;
void __fastcall hkMagicAnimation()
{
	if (m_bGenieAttackStatus || !(g_pMain->m_PlayerBase->isWarrior() || g_pMain->m_PlayerBase->isPriest()))
		return;

	if (DWORD target = g_pMain->GetTarget()) {
		isAttack = !isAttack;
		__asm {			// En alt atack buton iþlevi 01.02.2021
			push target
			push isAttack
			mov ecx, [KO_DLG]
			mov ecx, [ecx]
			mov eax, 0x00535550
			call eax
		}
	}
}

void __declspec(naked) GetAnimationSkill()
{
	__asm
	{
		pushad
		pushfd
		call hkMagicAnimation
		popfd
		popad
		jmp KO_MAGIC_SKILL_ANIMATION_ORG
	}

}

void __fastcall msga(DWORD esxx)
{
	DWORD tmp = 0;
	g_pMain->GetChildByID(esxx, std::string("str_mcount"), tmp);

	if (tmp == 0)
		return;

	std::string getString;
	getString = g_pMain->GetString(tmp).c_str();
	if (!getString.empty())
	{
		int getCount = std::stoi(getString.c_str());
		getCount++;
		g_pMain->SetString(tmp, string_format("%d", getCount));
	}
	else
		g_pMain->SetString(tmp, "1");
}

__declspec(naked) void hkPMMessage()
{
	__asm 
	{
		pushad
		pushfd
		mov edx, esi
		call msga
		popfd
		popad
		jmp KO_WHISPER_GETMESSAGE_ORG
	}
}

void __fastcall msgaa(DWORD esxx)
{
	DWORD tmp = 0;
	g_pMain->GetChildByID(esxx, std::string("str_mcount"), tmp);

	if (tmp > 0)
		g_pMain->SetString(tmp, "");
}

void __fastcall setVisible(DWORD esxx)
{
	DWORD tmp = 0;
	DWORD tmp1 = 0;

	g_pMain->GetChildByID(esxx, std::string("base_default"), tmp1);
	*(DWORD*)(tmp1 + 0x10D) = false;
	g_pMain->GetChildByID(esxx, std::string("btn_view"), tmp1);
	*(DWORD*)(tmp1 + 0x10D) = false;
	g_pMain->GetChildByID(esxx, std::string("btn_duel"), tmp1);
	*(DWORD*)(tmp1 + 0x10D) = false;
	g_pMain->GetChildByID(esxx, std::string("btn_party"), tmp1);
	*(DWORD*)(tmp1 + 0x10D) = false;
	g_pMain->GetChildByID(esxx, std::string("pus_gift"), tmp1);
	*(DWORD*)(tmp1 + 0x10D) = false;
}

__declspec(naked) void hkPMCreateWindow()
{
	__asm 
	{
		pushad
		pushfd
		cmp ebx, 0
		jne gizle
		mov ecx, ecx
		call setVisible
		popfd
		popad
		jmp KO_WHISPER_CREATE_ORG
		gizle :
		mov ecx, ecx
		call setVisible
		popfd
		popad
		jmp KO_WHISPER_CREATE_ORG
	}
}

__declspec(naked) void hkPMCloseWindow()
{
	__asm 
	{
		pushad
		pushfd
		mov ecx, ecx
		call setVisible
		popfd
		popad
		jmp KO_WHISPER_OPEN_ORG
	}
}

__inline DWORD SaitX(DWORD ulBase)
{
	if (!IsBadReadPtr((VOID*)ulBase, sizeof(DWORD)))
		return(*(DWORD*)(ulBase));

	return 0;
}

DWORD pmBase;
void __fastcall setPMRecv(DWORD esxx, uint32_t dwMessage)
{
	if (dwMessage != 5)
		return;

	DWORD tmp = 0;
	std::string elementname = g_pMain->GetStringElementName(pmBase).c_str();
	DWORD btn_block, btn_view, btn_party, tmp_base;

	if (elementname == "btn_view")
	{
		DWORD tmp1 = 0;
		std::string sName;
		g_pMain->GetChildByID(esxx, std::string("exit_id"), tmp1);
		sName = g_pMain->GetString(tmp1).c_str();
		g_pMain->str_replace(sName, " ", "");
		Packet result(WIZ_USER_INFO, uint8(2));
		result.SByte();
		result << sName;
		g_pMain->Send(&result);

		__asm
		{
			mov ecx, esxx
			call setVisible
		}
		return;
	}
	else if (elementname == "pus_gift")
	{
		if (g_pMain->uiPowerUpStore != NULL) 
		{
			g_pMain->uiPowerUpStore->UpdateItemList(g_pMain->pClientHookManager->item_list, g_pMain->pClientHookManager->cat_list);
			g_pMain->uiPowerUpStore->UpdateRefundItemList(g_pMain->pClientHookManager->pusrefund_itemlist);
			g_pMain->uiPowerUpStore->Open();
		}
		__asm
		{
			mov ecx, esxx
			call setVisible
		}
		return;
	}
	else if (elementname == "btn_party")
	{
		DWORD tmp1 = 0;
		g_pMain->GetChildByID(esxx, std::string("exit_id"), tmp1);
		std::string name = g_pMain->GetString(tmp1).c_str();
		DWORD base = SaitX(SaitX(KO_DLG) + 0x238);
		DWORD partyCount = SaitX(base + 0x344);
		Packet result(WIZ_PARTY);

		g_pMain->StringHelper->Replace(name, xorstr(" "), xorstr(""));

		if (partyCount < 1)
			result << uint8(1) << name.c_str();
		else
			result << uint8(3) << name.c_str() << uint8(0);
		g_pMain->Send(&result);

		__asm
		{
			mov ecx, esxx
			call setVisible
		}
		return;
	}
	else if (elementname == "btn_info")
	{
		DWORD tmp1 = 0;
		bool isVisible = false;
		g_pMain->GetChildByID(esxx, std::string("base_default"), tmp1);
		*(DWORD*)(tmp1 + 0x10D) = g_pMain->IsVisible(tmp1) ? false : true;
		g_pMain->GetChildByID(esxx, std::string("btn_view"), tmp1);
		*(DWORD*)(tmp1 + 0x10D) = g_pMain->IsVisible(tmp1) ? false : true;
		g_pMain->GetChildByID(esxx, std::string("btn_duel"), tmp1);
		*(DWORD*)(tmp1 + 0x10D) = g_pMain->IsVisible(tmp1) ? false : true;
		g_pMain->GetChildByID(esxx, std::string("btn_party"), tmp1);
		*(DWORD*)(tmp1 + 0x10D) = g_pMain->IsVisible(tmp1) ? false : true;
		g_pMain->GetChildByID(esxx, std::string("pus_gift"), tmp1);
		*(DWORD*)(tmp1 + 0x10D) = g_pMain->IsVisible(tmp1) ? false : true;
	}
}
__declspec(naked) void hkPMCloseWindowRecv()
{
	__asm 
	{
		pushad
		pushfd
		mov ecx, ecx
		mov edx, ebx
		MOV pmBase, esi
		call setPMRecv
		popfd
		popad
		jmp KO_WHISPER_RECV_ORG
	}
}

__declspec(naked) void hkPMView()
{
	__asm 
	{
		pushad
		pushfd
		mov ecx, esi
		call msgaa
		popfd
		popad
		jmp KO_WHISPER_VIEWMESAAGE_ORG
	}
}

/*Merchant Eye / HidePlayer*/
DWORD nPlayerRender = 0x005999AE;
DWORD nPlayerRenderOrg;
DWORD nPlayerRender1 = 0x00599E66;

bool _fastcall kendimmi(DWORD nPlayerBase)
{
	if (!g_pMain->m_bHookStart)
		return false;

	if (g_pMain->Player.GetZoneID() == ZONE_MORADON)
	{
		if (g_pMain->pClientHookManager->pClientMerchantEye != NULL && g_pMain->pClientHookManager->pClientMerchantEye->IsVisible())
		{
			if (g_pMain->pClientHookManager->pClientMerchantEye->search_merchant->GetString().empty())
				return false;

			uint16 sSocket = *(uint16*)(nPlayerBase + KO_OFF_ID);
			if (sSocket == g_pMain->GetSocketID())
				return false;

			if (sSocket > NPC_BAND)
				return false;

			g_pMain->pClientMerchantEyeDataLock.lock();
			if (g_pMain->pClientMerchantArray.size() == 0)
			{
				g_pMain->pClientMerchantEyeDataLock.unlock();
				return true;
			}

			auto itr = g_pMain->pClientMerchantArray.find(sSocket);
			if (itr == g_pMain->pClientMerchantArray.end())
			{
				*(uint8*)(nPlayerBase + KO_WH) = 0;
				*(uint8*)(nPlayerBase + 0x7AC) = 3;
				g_pMain->pClientMerchantEyeDataLock.unlock();
				return true;
			}

			g_pMain->pClientMerchantEyeDataLock.unlock();
			*(uint8*)(nPlayerBase + KO_WH) = 1;
			return false;
		}
		else if (g_pMain->pClientHookManager->pClientMerchantEye != NULL && !g_pMain->pClientHookManager->pClientMerchantEye->IsVisible())
		{
			uint16 sSocket = *(uint16*)(nPlayerBase + KO_OFF_ID);
			if (sSocket == g_pMain->GetSocketID())
				return false;

			if (sSocket > NPC_BAND)
				return false;

			uint8 sAuthority = *(uint8*)(nPlayerBase + KO_WH);
			if (sAuthority == 0)
				*(uint8*)(nPlayerBase + KO_WH) = 1;
		}
	}

	if (!isHideUser)
		return false;

	uint16 mSocket = *(uint16*)(nPlayerBase + KO_OFF_ID);
	if (mSocket > NPC_BAND)
		return false;

	if (mSocket == g_pMain->GetSocketID())
		return false;

	return true;
}
__declspec(naked) void hkPlayerRender()
{
	__asm
	{
		pushad
		pushfd
		mov ecx, esi
		call kendimmi
		movzx eax, al
		test eax, eax
		jne devam
		popfd
		popad
		jmp nPlayerRenderOrg
		devam :
		popfd
		popad
		jmp nPlayerRender1
	}
}


/*Merchant Eye / HidePlayer*/

/**/
const DWORD READ_MERCHANT_ADRESS = 0x0062BE4C;
DWORD READ_MERCHANT_ADRESS_ORG = 0;
__declspec(naked) void ReadNearMerchant()
{
	__asm
	{
		pushad
		pushfd
		mov m_sSocketMerchant, eax
		popfd
		popad
		jmp READ_MERCHANT_ADRESS_ORG
	}
}
const DWORD READ_MERCHANT_SPECIAL_ADRESS = 0x0062C8C8;
DWORD READ_MERCHANT_ADRESS_SPECIAL_ORG = 0;

__declspec(naked) void ReadNearMerchantSpecial()
{
	__asm
	{
		pushad
		pushfd
		mov m_sSocketMerchantSpecial, eax
		popfd
		popad
		jmp READ_MERCHANT_ADRESS_SPECIAL_ORG
	}
}

/*Merchant Price*/
DWORD tooltipasm = 0x0086052B;
DWORD tooltipasm2 = 0x00860532;

bool _fastcall ustundeyimamk(DWORD ebx)
{
	if (g_pMain->IsVisible(g_pMain->m_sMerchantDisplayData))
	{
		if (m_sSocketMerchant > 10000 
			|| m_sSocketMerchant < 1)
			return 0;

		uint8 getdata = 0;
		bool yoklama = false;
		for (int i = 0; i < 4; i++)
		{
			if (g_pMain->IsIn(g_pMain->m_sMerchantDisplayItem[i], g_pMain->lastMousePos.x, g_pMain->lastMousePos.y))
			{
				getdata = i;
				yoklama = true;
				break;
			}
		}
		if (yoklama)
			return true;
	}
	else if (g_pMain->IsVisible(g_pMain->m_sMerchantDisplaySpecialData))
	{
		if (m_sSocketMerchantSpecial > 10000 
			|| m_sSocketMerchantSpecial < 1)
			return 0;

		uint8 getdata = 0;
		bool yoklama = false;
		for (int i = 0; i < 8; i++)
		{
			if (g_pMain->IsIn(g_pMain->m_sMerchantDisplaySpecialItem[i], g_pMain->lastMousePos.x, g_pMain->lastMousePos.y))
			{
				getdata = i;
				yoklama = true;
				break;
			}
		}
		if (yoklama)
			return true;
	}
	return false;
}

__declspec(naked) void SetPriceMerchant()
{
	__asm
	{
		pushad
		pushfd
		cmp edx, 1
		je pricegoster
		mov ecx, ebx
		call ustundeyimamk
		movzx eax, al
		test eax, eax
		jne pricegoster
		popfd
		popad
		push 0
		jmp tooltipasm2
		pricegoster :
		popfd
		popad
		push 1
		jmp tooltipasm2
	}
}

const DWORD KO_GENIE_MOVE_JMP = 0x009A9803;						// Genie range of activities ayarý buradan fixlendi.
DWORD KO_GENIE_MOVE_ORG;

__declspec(naked) void hkGenieMove()
{
	__asm
	{
		pushad
		pushfd

	}
	if (!__genieAttackMove)
	{
		__asm
		{
			popfd
			popad
			jmp KO_GENIE_MOVE_JMP

		}
	}
	__asm
	{
		popfd
		popad
		jmp KO_GENIE_MOVE_ORG
	}

}
DWORD KO_GENIE_MOVE_ORG2;								
const DWORD KO_GENIE_MOVE_RETURN = 0x009A7B2C;
__declspec(naked) void hkGenieMove2()
{
	__asm
	{
		pushad
		pushfd
	}
	if (!__genieAttackMove)
	{
		__asm
		{
			popfd
			popad
			MOV DWORD PTR DS : [0x009A9D86] , 0x8638000090903CEB
			jmp KO_GENIE_MOVE_RETURN
		}
	}
	__asm
	{
		popfd
		popad
		MOV DWORD PTR DS : [0x009A9D86] , 0x863800000107850F
		jmp KO_GENIE_MOVE_ORG2
	}
}

const DWORD KO_PARTY_REQUEST = 0x00524299;
DWORD KO_PARTY_REQUEST_KEEP = 0;
DWORD mLevel = 0;
DWORD tLevel = 0;
const DWORD KO_PARTY_REQUEST_JMP = 0x00524324;
void __declspec(naked) hkPartyRequest()
{
	__asm {
		pushad
		pushfd
		cmp ecx, 0xE1
		jb KO_PARTY_GO
		cmp ebx, 0xE1
		jae KO_PARTY_GO
		popfd
		popad
		jmp KO_PARTY_REQUEST_KEEP
		KO_PARTY_GO :
		popfd
		popad
		jmp KO_PARTY_REQUEST_JMP
	}
}

DWORD HSACSEngine::GetTarget()
{
	DWORD KO_ADR = *(DWORD*)KO_PTR_CHR;
	uint16 targetID = *(DWORD*)(KO_ADR + KO_OFF_MOB);
	if (targetID >= NPC_BAND) 
	{
		Func_GetObjectBase = (tGetObjectBase)KO_FMBS;
		return Func_GetObjectBase(*(DWORD*)KO_FLDB, targetID, 1);
	}
	else 
	{
		Func_GetObjectBase = (tGetObjectBase)KO_FPBS;
		return Func_GetObjectBase(*(DWORD*)KO_FLDB, targetID, 1);
	}
	return NULL;
}

DWORD HSACSEngine::GetTarget(uint16 targetID)
{
	DWORD KO_ADR = *(DWORD*)KO_PTR_CHR;
	uint16 byTargetID = targetID;
	if (byTargetID >= 10000) 
	{
		Func_GetObjectBase = (tGetObjectBase)KO_FMBS;
		return Func_GetObjectBase(*(DWORD*)0x00F3E85C, byTargetID, 1);
	}
	else 
	{
		Func_GetObjectBase = (tGetObjectBase)KO_FPBS;
		return Func_GetObjectBase(*(DWORD*)0x00F3E85C, byTargetID, 1);
	}
	return NULL;
}

const DWORD KO_PREMIUM_STRING = 0x0081FE27;
const DWORD KO_PREMIUM_STRING_CONTINUE = 0x0081FEC7;
const DWORD KO_PREMIUM_STRING_BACK = 0x0081FE2D;
const DWORD KO_PREMIUM_STRING_CALL = 0x00A14DA0;
void __declspec(naked) hkPremiumString()
{
	__asm
	{
		movzx eax, byte ptr[esp + 0x17]
		cmp eax,0x4
		je setPremium
		DEC EAX
		jmp KO_PREMIUM_STRING_BACK
		ret
		setPremium:
		lea edx, [esp + 0x00000F4]
		push edx
		push 0x5CA // 1482
		call KO_PREMIUM_STRING_CALL
		jmp KO_PREMIUM_STRING_CONTINUE
		ret
	}
}

DWORD WINAPI HSACSEngine::EngineMain(HSACSEngine * e)
{
	Func_GetObjectBase = (tGetObjectBase)KO_FMBS;
	memcpy((void*)0x00524295, (char*)"\x90\x90", 2);
	DetourFunction((PBYTE)KO_PREMIUM_STRING, (PBYTE)hkPremiumString);
	KO_PARTY_REQUEST_KEEP = (DWORD)DetourFunction((PBYTE)KO_PARTY_REQUEST, (PBYTE)hkPartyRequest);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SendTarama, (LPVOID)KO_SND_FNC, NULL, NULL);
	DetourFunction((PBYTE)tooltipasm, (PBYTE)SetPriceMerchant);
	//KO_GENIE_MOVE_ORG = (DWORD)DetourFunction((PBYTE)0x009A97F1, (PBYTE)hkGenieMove); // Atak Range Sýfýr olduðunda yürümesi engellendi.
	KO_GENIE_MOVE_ORG2 = (DWORD)DetourFunction((PBYTE)0x009A79AD, (PBYTE)hkGenieMove2); // Basic Attack off Genie
	READ_MERCHANT_ADRESS_ORG = (DWORD)DetourFunction((PBYTE)READ_MERCHANT_ADRESS, (PBYTE)ReadNearMerchant);
	READ_MERCHANT_ADRESS_SPECIAL_ORG = (DWORD)DetourFunction((PBYTE)READ_MERCHANT_SPECIAL_ADRESS, (PBYTE)ReadNearMerchantSpecial);
	KO_ZONE_LOAD_TERRAIN_ORG = (DWORD)DetourFunction((PBYTE)KO_ZONE_LOAD_TERRAIN, (PBYTE)hkLoadTerrainData);
	DetourFunction((PBYTE)KO_AUTO_ATTACK, (PBYTE)hkAttack);
	//rtOrg = (DWORD)DetourFunction((PBYTE)KO_UIF_SET_FONT_STRING, (PBYTE)hkSetFontString);		// string kalýnlaþtýrma.
	TICK_ORG = (DWORD)DetourFunction((PBYTE)KO_GAME_TICK, (PBYTE)hkTick);
#if (HOOK_SOURCE_VERSION == 2369)
	TICK_COM = (DWORD)DetourFunction((PBYTE)KO_GAME_TICK, (PBYTE)HkGameStartTick);
#endif
	KO_MAGIC_SKILL_ANIMATION_ORG = (DWORD)DetourFunction((PBYTE)0x009A55E0, (PBYTE)GetAnimationSkill);
	(tUIOnKeyPress)DetourFunction((PBYTE)0x40F0F0, (PBYTE)hkUIOnKeyPress);
	*(float*)0xDE2B98 = 1.0f / 999.0f;																	// -------------------- fps limiti kaldýrma
	*(uint16*)0x007CBD55 = 0x4FEB;																		// -------------------- klan parasý client kontrol kapatma
	nPlayerRenderOrg = (DWORD)DetourFunction((PBYTE)nPlayerRender, (PBYTE)hkPlayerRender);				// -------------------- PlayerRender Oyuncu Gizleme / Merchant Eye
	DetourFunction((PBYTE)kontrolAdresi, (PBYTE)hkRank);												// -------------------- Zindanwar Rank 
	DetourFunction((PBYTE)Func_RangeInstance, (PBYTE)hkRangeInstance);									// -------------------- mage alan kýsýtlama  
	DetourFunction((PBYTE)Func_AreaMove, (PBYTE)hkAreaMove);											// -------------------- mage alan kýsýtlama
	oShellExecuteA = (tShellExecuteA)DetourFunction((PBYTE)ShellExecuteA, (PBYTE)hkShellExecuteA);
	DetourFunction((PBYTE)KO_FNC_END_GAME, (PBYTE)hkEndGame);											// -------------------- Oyundan Hýzlý çýkma
	memcpy((void*)0x0065F4DC, (char*)"\xC7\x81\x24\x01\x00\x00\x00\x00\x00\x00", 10);					// Pazar bekleme süresi kapatma
	memcpy((void*)0x43342B, (char*)"\xE9\x34\x1\x0\x0\x90", 6);											// Inventory Animasyon patlamasý fix
	*(uint64*)0x00565DCC = 0xDD9090000000E6E9;															// Oyuncularýn bir anda geç yüklenmesinin düzeltildiði yer. Örneðin warriorun descenti veya town atma
	*(uint16*)0x0060771A = 0x00003481;																	// Bdw'de ,Border'da Vip Key açýlmama sorunu fixlendi.
	KO_WHISPER_GETMESSAGE_ORG = (DWORD)DetourFunction((PBYTE)0x007247DB, (PBYTE)hkPMMessage);
	KO_WHISPER_VIEWMESAAGE_ORG = (DWORD)DetourFunction((PBYTE)0x00720DA3, (PBYTE)hkPMView);
	KO_WHISPER_OPEN_ORG = (DWORD)DetourFunction((PBYTE)0x00720DE3, (PBYTE)hkPMCloseWindow);
	KO_WHISPER_RECV_ORG = (DWORD)DetourFunction((PBYTE)0x007248F0, (PBYTE)hkPMCloseWindowRecv);
	KO_WHISPER_CREATE_ORG = (DWORD)DetourFunction((PBYTE)0x00723E90, (PBYTE)hkPMCreateWindow);
	*(uint32*)0x004E906F = 0x0000A6E9;																	// nick izin verme yeri Örn : ___ :: gibi deðiþik karakterlerde yazýlabilir.
	*(uint8*)0x00524283 = 0x5A;																			// 90 Level için Max Level diff kapatma
	*(uint8*)0x0066D3D4 = 0xEB;																			// Party Monster Stone Aktif	


	//Priest Genie Heal Atamama Fixed
	/*//*(uint8*)0x009A8E67 = 0;																		// Geniede priest ölenin baþýna gitmesi fixlendi.
	*(uint16*)0x009A8E1E = 0x4FEB;																		// Geniede priest ölenin baþýna gitmesi fixlendi.
	//*(uint16*)0x009A8E34 = 0xEB39;*/																	// Geniede priest ölenin baþýna gitmesi fixlendi.
	
	*(uint16*)0x0067171D = 0x27EB;																		// Chaos bittikten sonra skillbar uifi çiftlenmesi fixlendi.
	//ASMPatch((DWORD)0x0509C91, (char*)"\xEB\x04\x90\x90\x90\x90\xD9\xEE", 8);							// Objeleri yakýndan seri taramasý fix.


#if (HOOK_SOURCE_VERSION == 1098 || HOOK_SOURCE_VERSION == 1534)
	// -------------------- hp progress animasyon iptali
	DWORD flt = (DWORD)malloc(sizeof(float));
	DWORD flt2 = (DWORD)malloc(sizeof(float));
	*(float*)flt = 0.0f;
	*(float*)flt2 = 1000.0f;
	// -------------------- hpbar
	*(DWORD*)0x00833E38 = flt;
	*(DWORD*)0x00833E6A = flt;
	*(DWORD*)0x00833EA0 = flt;
	*(DWORD*)0x00833ED2 = flt;
	*(DWORD*)0x00833F04 = flt;
	*(DWORD*)0x00833E85 = flt;
	*(DWORD*)0x00833BE0 = flt;
	// -------------------- targetbar
	*(DWORD*)0x0084D444 = flt2;
	*(DWORD*)0x0084D44C = flt2;
	*(DWORD*)0x0084D333 = flt2;
	*(DWORD*)0x0084D33B = flt2;
	*(DWORD*)0x0084D3B3 = flt2;
	*(DWORD*)0x0084D3AB = flt2;
#endif
	//// -------------------- slide görüntü bugu fix
	DWORD slidebug = (DWORD)malloc(sizeof(float));
	*(float*)slidebug = 0.75f;
	*(DWORD*)(0x507621 + 2) = slidebug;

	//// -------------------- direction görüntü bugu fix
	DWORD directionbug = (DWORD)malloc(sizeof(float));
	//*(float*)directionbug = 0.35f;
	*(float*)directionbug = 1.5f;
	*(DWORD*)(0x50773B + 2) = directionbug;


	// -------------------- Discord yükleme
	e->dc = new Discord;
	/*if (e->dc = new Discord) {
		e->dc->Initialize();
		e->dc->Update();
	}*/

	// -------------------- Clan Asist Hook System YNG 
	DetourFunction((PBYTE)KO_FNC_CLAN_RENDER_RET, (PBYTE)hkClanRanderHook);
	DetourFunction((PBYTE)KO_FNC_CLAN_RENDER_RET_USER, (PBYTE)hkClanRanderHookUser);

	DetourFunction((PBYTE)KO_FNC_TREE_CLAN_RENDER_RET, (PBYTE)hkClanTreeRanderHook);
	//DetourFunction((PBYTE)KO_FNC_TREE_CLAN_RENDER_RET_USER, (PBYTE)hkClanTreeRanderHookUser);

	// -------------------- Anti afk ve kafaya yazý yazma
	DetourFunction((PBYTE)KO_FNC_OBJECT_MOB_LOOP, (PBYTE)hkObjectMobLoop);
	KO_FNC_OBJECT_PLAYER_LOOP_ORG = (DWORD)DetourFunction((PBYTE)KO_FNC_OBJECT_PLAYER_LOOP, (PBYTE)hkObjectPlayerLoop);

	DetourFunction((PBYTE)fncScanZ, (PBYTE)hkZ);
	DetourFunction((PBYTE)fncScanB, (PBYTE)hkB);
	DetourFunction((PBYTE)KO_FNC_GENIE_SCAN, (PBYTE)hkGenieSelect);

	oGetChild = (tGetChild)DetourFunction((PBYTE)KO_GET_CHILD_BY_ID_FUNC, (PBYTE)GetChildByID_Hook);
	oSetVisible = (htSetVisible)DetourFunction((PBYTE)0x00411980, (PBYTE)SetVisible_Hook);
	oSetUIPos = (tSetUIPos)DetourFunction((PBYTE)0x0040F7D0, (PBYTE)SetUI_Hook);

	// -------------------- death animasyon speed 
	DWORD deathTimeAdress = 0x000534C0E;
	DWORD newSleep = (DWORD)malloc(sizeof(float));
	*(float*)newSleep = 0.0f;
	*(DWORD*)(deathTimeAdress + 2) = newSleep;

	// --------------------	Login Ýntro kapatma
	{
		uint8 wValues[] = { 0xA1, 0xFC, 0x68, 0xF3, 0x00 };
		for (auto i = 0; i < sizeof(wValues) / sizeof(wValues[0]); i++)
			*(BYTE*)(0x00A86D39 + i) = wValues[i];
	}
	// --------------------
	*(DWORD*)0xC7164C = (DWORD)hkDisableVisible;										// -------------------- scheculer Gizleme
	*(DWORD*)0xC704A4 = (DWORD)hkDisableVisible;										// -------------------- day price Gizleme
	*(DWORD*)0xC76574 = (DWORD)hkDisableVisible;										// -------------------- CUIItemtimeBtn Gizleme

	//DetourFunction((PBYTE)0x7D5DD0, (PBYTE)hkDisableVisible);							// -------------------- Quest View Ekraný Gizleme
#if (HOOK_SOURCE_VERSION == 1098)
	memcpy((void*)0x009015FD, (char*)"\x90\x90\x90\x90\x90\x90", 6);					// -------------------- TempItem Item List 1 Gizleme
	*(DWORD*)0xC76484 = (DWORD)hkDisableVisible;										// -------------------- TempItem Item List 2 Gizleme
#endif
	DetourFunction((PBYTE)KO_SKILLBAR_MOUSE_INPUT, (PBYTE)hkSkillbarMouseInput);		// geniemacro
	DetourFunction((PBYTE)KO_SKILLBAR_SHORTCUT_INPUT, (PBYTE)hkSkillbarShortcutInput);	// geniemacro
	KO_SKILLBAR_KEY_INPUT_KEEP = (DWORD)DetourFunction((PBYTE)KO_SKILLBAR_KEY_INPUT, (PBYTE)hkSkillbarKeyInput);

#if (HOOK_SOURCE_VERSION == 1098)
	DetourFunction((PBYTE)GetMark, (PBYTE)hkMarkPos);									// --------------------	Civciv iconu Çalýþtýran fonksiyon
	*(DWORD*)0x005687B0 = 0x90C3C031;													// -------------------- Karakter Space animasyonu iptal etme
	*(DWORD*)0x005687B0 = 0x90C3C031;													// -------------------- Karakter Space animasyonu iptal 
	*(DWORD*)0xC68ABC = (DWORD)hkDisableVisible;										// -------------------- timeleft Gizleme
	//*(DWORD*)0xC6B64C = (DWORD)hkOpenMiniMapfromGlobal;								// -------------------- globalmap to minimap
	*(DWORD*)0xC73494 = (DWORD)hkDisableVisible;										// -------------------- H command achieve Gizleme
	*(DWORD*)0xC6CEAC = (DWORD)hkDisableVisible;										// -------------------- QuestSeed  Gizleme
	*(DWORD*)0xC66234 = (DWORD)hkDisableVisible;										// -------------------- Nation Channel List Gizleme
	memcpy((void*)0x00788756, (char*)"\xEB\x14", 2); 									// -------------------- Training  Gizleme
	memcpy((void*)0x008DFE40, (char*)"\x31\xC0\xC2\x4\x0", 5); 							// -------------------- Commander Skill Bar Gizleme
	//memcpy((void*)0x0080BD70, (char*)"\C3\x90\x90", 3);  								// -------------------- Minimap Gizleme  -- Çalýþtýrýnca f10 seed helper game option 2 patlýyor.
	memcpy((void*)0x00594C4A, (char*)"\xE9\xBB\x01\x00\x00\x90", 6);					// -------------------- Noah knight Pelerin Kapatma & Gizleme

	// -------------------- minimap sabitleme
	memcpy((void*)0x008335D2, (char*)"\x90\x90", 2);
	memcpy((void*)0x00847BBF, (char*)"\x90\x90", 2);
	(DWORD)DetourFunction((PBYTE)0x0083361A, (PBYTE)hkMinimapPos);
	// --------------------

	// -------------------- noah knight level deðiþtirme
	uint8 noah_max_level = 30;
	*(BYTE*)0x54B259 = noah_max_level - 1;
	*(BYTE*)0x54B36B = noah_max_level;
	*(BYTE*)0x0054B4C4 = noah_max_level - 1;
	*(BYTE*)0x0054B4EA = noah_max_level;
	*(BYTE*)0x00546D6B = noah_max_level - 1;
	*(BYTE*)0x0053AC74 = noah_max_level - 1;
	// ----------------- 


	// ----------------- v1098 Character Select Uif Animasyonu ve set pos'un iptali 
	{
		uint8 wValues[6] = { 0xE9, 0x0E, 0x01, 0x00, 0x00, 0x90 };
		for (auto i = 0; i < sizeof(wValues) / sizeof(wValues[0]); i++)
			*(DWORD*)(0x004E9BEF + i) = wValues[i];
	}

	{
		uint8 wValues[6] = { 0xE9, 0x0B, 0x01, 0x00, 0x00, 0x90 };
		for (auto i = 0; i < sizeof(wValues) / sizeof(wValues[0]); i++)
			*(DWORD*)(0x004E9E0D + i) = wValues[i];
	}	
	// ----------------- 


	// -------------------- 1098 genie set pos
	//KO_SET_UI_ORG = (DWORD)DetourFunction((PBYTE)0x0040F7D0, (PBYTE)hkUiPos);
	//Genie YNG
#if 0
	*(DWORD*)0xC6F480 = (DWORD)hkGenieSetPos;
#endif
	KO_SET_EVENTNOTICE_POS_ORG = (DWORD)DetourFunction((PBYTE)KO_SET_EVENTNOTICE_POS, (PBYTE)hkEventNotice);
#if 0
	uint8 channelY = 63;
	*(char*)0x0055B6D5 = channelY;
	*(char*)0x0050335C = channelY;
	*(char*)0x0050335B = 0x6A;
#endif
	// --------------------

	/* Tahtlý Karakter Kordinatlarý */
	Real_CharacterSelectLeft_ORG = (DWORD)DetourFunction((PBYTE)Real_CharacterSelectLeft, (PBYTE)Hook_CharacterSelectLeft);			// -------------------- Tahtlý karakter
	Real_CharacterSelectRight_ORG = (DWORD)DetourFunction((PBYTE)Real_CharacterSelectRight, (PBYTE)Hook_CharacterSelectRight);		// -------------------- Tahtlý karakter

	*(DWORD*)(0x004DDB79) = 0;
	{	// 1
		static float fX = -0.01f;
		static float fY = -1.16f;
		static float fZ = 2.6f;
		static float fR = 0.0f;

		*(DWORD*)(0x004E0638 + 2) = (DWORD)(&fX);
		*(DWORD*)(0x004E062E + 2) = (DWORD)(&fY);
		*(DWORD*)(0x004E0619 + 2) = (DWORD)(&fZ);
		*(DWORD*)(0x004E0643 + 2) = (DWORD)(&fR);
	}
	{	// 2
		static float fX = 1.73f;
		static float fY = -1.16f;
		static float fZ = 2.0f;
		static float fR = 0.68539819;

		*(DWORD*)(0x004E05D1 + 2) = (DWORD)(&fX);
		*(DWORD*)(0x004E05C7 + 2) = (DWORD)(&fY);
		*(DWORD*)(0x004E05B2 + 2) = (DWORD)(&fZ);
		*(DWORD*)(0x004E05DC + 2) = (DWORD)(&fR);
	}
	{	// 3
		static float fX = -1.82f;
		static float fY = -1.16f;
		static float fZ = 1.95f;
		static float fR = -0.68539819;

		*(DWORD*)(0x004E069B + 2) = (DWORD)(&fX);
		*(DWORD*)(0x004E0691 + 2) = (DWORD)(&fY);
		*(DWORD*)(0x004E067C + 2) = (DWORD)(&fZ);
		*(DWORD*)(0x004E06A6 + 2) = (DWORD)(&fR);
	}
	{	// C1
		static float fX = 0.0f;
		static float fY = -1.16f;
		static float fZ = 3.8f;
		static float fR = 0.78539819;

		*(DWORD*)(0x004DD0A5 + 2) = (DWORD)(&fX);
		*(DWORD*)(0x004DD0B9 + 2) = (DWORD)(&fY);
		*(DWORD*)(0x004DD0C6 + 2) = (DWORD)(&fZ);
		*(DWORD*)(0x004DCF46 + 2) = (DWORD)(&fX);
		*(DWORD*)(0x004DCF5B + 2) = (DWORD)(&fY);
		*(DWORD*)(0x004DCF69 + 2) = (DWORD)(&fZ);
		*(DWORD*)(0x004DCF96 + 2) = (DWORD)(&fR);
	}
	{	// C2
		static float fX = 2.60f;
		static float fY = -1.16f;
		static float fZ = 2.9f;
		static float fR = 1.28539819;

		*(DWORD*)(0x004DD0DD + 2) = (DWORD)(&fX);
		*(DWORD*)(0x004DD0F1 + 2) = (DWORD)(&fY);
		*(DWORD*)(0x004DD0FE + 2) = (DWORD)(&fZ);
		*(DWORD*)(0x004DCFB0 + 2) = (DWORD)(&fX);
		*(DWORD*)(0x004DCFBE + 2) = (DWORD)(&fY);
		*(DWORD*)(0x004DCFC8 + 2) = (DWORD)(&fZ);
		*(DWORD*)(0x004DCFEC + 2) = (DWORD)(&fR);
	}
	{	// C3
		static float fX = -2.7f;
		static float fY = -1.16f;
		static float fZ = 2.9f;
		static float fR = -0.18539819;

		*(DWORD*)(0x004DD115 + 2) = (DWORD)(&fX);
		*(DWORD*)(0x004DD129 + 2) = (DWORD)(&fY);
		*(DWORD*)(0x004DD136 + 2) = (DWORD)(&fZ);
		*(DWORD*)(0x004DD00F + 2) = (DWORD)(&fX);
		*(DWORD*)(0x004DD023 + 2) = (DWORD)(&fY);
		*(DWORD*)(0x004DD030 + 2) = (DWORD)(&fZ);
		*(DWORD*)(0x004DD057 + 2) = (DWORD)(&fR);
	}
	// ----------------- 
	/* Tahtlý Karakter Kordinatlarý */

#elif (HOOK_SOURCE_VERSION == 1534)
DetourFunction((PBYTE)GetMark, (PBYTE)hkMarkPos);									// --------------------	Civciv iconu Çalýþtýran fonksiyon
*(DWORD*)0x005687B0 = 0x90C3C031;													// -------------------- Karakter Space animasyonu iptal etme
*(DWORD*)0x005687B0 = 0x90C3C031;													// -------------------- Karakter Space animasyonu iptal 
*(DWORD*)0xC73494 = (DWORD)hkDisableVisible;										// -------------------- H command achieve Gizleme
*(DWORD*)0xC6CEAC = (DWORD)hkDisableVisible;										// -------------------- QuestSeed  Gizleme
*(DWORD*)0xC66234 = (DWORD)hkDisableVisible;										// -------------------- Nation Channel List Gizleme
memcpy((void*)0x00788756, (char*)"\xEB\x14", 2); 									// -------------------- Training  Gizleme
memcpy((void*)0x008DFE40, (char*)"\x31\xC0\xC2\x4\x0", 5); 							// -------------------- Commander Skill Bar Gizleme
memcpy((void*)0x00594C4A, (char*)"\xE9\xBB\x01\x00\x00\x90", 6);					// -------------------- Noah knight Pelerin Kapatma & Gizleme

// -------------------- noah knight level deðiþtirme
uint8 noah_max_level = 30;
*(BYTE*)0x54B259 = noah_max_level - 1;
*(BYTE*)0x54B36B = noah_max_level;
*(BYTE*)0x0054B4C4 = noah_max_level - 1;
*(BYTE*)0x0054B4EA = noah_max_level;
*(BYTE*)0x00546D6B = noah_max_level - 1;
*(BYTE*)0x0053AC74 = noah_max_level - 1;
// ----------------- 


// ----------------- v1098 Character Select Uif Animasyonu ve set pos'un iptali 
{
	uint8 wValues[6] = { 0xE9, 0x0E, 0x01, 0x00, 0x00, 0x90 };
	for (auto i = 0; i < sizeof(wValues) / sizeof(wValues[0]); i++)
		*(DWORD*)(0x004E9BEF + i) = wValues[i];
}

{
	uint8 wValues[6] = { 0xE9, 0x0B, 0x01, 0x00, 0x00, 0x90 };
	for (auto i = 0; i < sizeof(wValues) / sizeof(wValues[0]); i++)
		*(DWORD*)(0x004E9E0D + i) = wValues[i];
}
// ----------------- 

// -------------------- 1534 genie set pos
//KO_SET_UI_ORG = (DWORD)DetourFunction((PBYTE)0x0040F7D0, (PBYTE)hkUiPos);
	//Genie YNG
#if 0
	* (DWORD*)0xC6F480 = (DWORD)hkGenieSetPos;
#endif
KO_SET_EVENTNOTICE_POS_ORG = (DWORD)DetourFunction((PBYTE)KO_SET_EVENTNOTICE_POS, (PBYTE)hkEventNotice);
#if 0
uint8 channelY = 63;
*(char*)0x0055B6D5 = channelY;
*(char*)0x0050335C = channelY;
*(char*)0x0050335B = 0x6A;
#endif
// --------------------
#else
	//Genie YNG
#if 0
	* (DWORD*)0xC6F480 = (DWORD)hkGenieSetPos;
#endif
	KO_SET_EVENTNOTICE_POS_ORG = (DWORD)DetourFunction((PBYTE)KO_SET_EVENTNOTICE_POS, (PBYTE)hkEventNotice);
#endif

#if ANTICHEAT_MODE == 1
	e->ScanThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DriverScan, NULL, NULL, NULL);
	e->SuspendThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SuspendCheck, e, NULL, NULL);
	e->TitleThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)TitleCheck, e, NULL, NULL);
#endif
	e->LisansThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)LicenseEngine, e, NULL, NULL);
	e->AliveThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)AliveSend, NULL, NULL, NULL);
	e->m_bCheckSleep = GetTickCount();
	e->m_LoginPlugInjected = true;
	
	while (true)
	{
		e->Update();
#if ANTICHEAT_MODE == 1
		if (IsDebuggerPresent())
			e->Shutdown(xorstr("Debugger"));  // Debugger Tespit Ediyor.

		if (e->IsHWBreakpointExists())
			e->Shutdown(xorstr("BreakPoint"));   // Breakpoint Tespit Ediyor.

		if (e->IsitaSandBox())
			e->Shutdown(xorstr("Sandbox"));	// Sanal Makineyi Tespit Ediyor.
#endif
		if (WaitForSingleObject(e->ScanThread, 1) == WAIT_OBJECT_0
			|| WaitForSingleObject(e->SuspendThread, 1) == WAIT_OBJECT_0
			|| WaitForSingleObject(e->LisansThread, 1) == WAIT_OBJECT_0)
			g_pMain->Shutdown(xorstr("All the pieces of the game can't work together."));

		if (e->uiNoticeWind && getMSTime() > e->m_recentdelete_time)
		{
			e->uiNoticeWind->Clear();
			e->m_recentdelete_time = getMSTime() + (30 * 1000) * 60;
		}
		Sleep(100);
	}
	return TRUE;
}

void HSACSEngine::Update() 
{
	if (!m_bSelectedCharacter)
		return;

	if (!m_bGameStart)
		return;
	
	if (!isAlive)
		return;

	if (thisProc == NULL) 
		thisProc = GetCurrentProcess();
		
	if (KO_ADR == 0x0)
		KO_ADR = *(DWORD*)KO_PTR_CHR;

	uint8 Authority;
	Authority = *(uint8*)(KO_ADR + KO_WH);
	if (this->Player.Authority != Authority)
	{
		if (Authority == USER || Authority == GAMEMASTER || Authority == BANNED)
		{
			tmpGraphics = "";
			for (string gpu : Player.GPU)
				tmpGraphics += xorstr(" | ") + gpu;

			this->Player.Authority = (UserAuthority)Authority;

			Packet result(WIZ_HSACS_HOOK);
			result << uint8_t(HSACSOpCodes::AUTHINFO) << Authority << tmpGraphics << Player.CPU;
			this->Send(&result);

#if ANTICHEAT_MODE == 1
			if (Authority != 0 
				&& Authority != 1 
				&& Authority != 2 
				&& Authority != 11 
				&& Authority != 12 
				&& Authority != 250 
				&& Authority != 255)
			{
				Shutdown(xorstr("Game closed due to memory editing\n"));
			}
#endif
		}
	}

	uint8 ClanRender;
	ClanRender = *(uint8*)(KO_ADR + 0x7F4);
	uint32_t color;
	if (ClanRender == 1 || ClanRender == 2)
	{
		*(uint8*)0x00576A61 = ClanRender;
		*(uint8*)0x00576A6B = 0x74;
		color = 0xffff00e4;
		if (ClanRender == 1) {
			color = 0xff00ff00;
		}
		*(uint32*)0x576A86 = color;
	}

	if (m_PlayerBase == NULL)
		m_PlayerBase = new CPlayerBase();

	if (m_PlayerBase != NULL)
		m_PlayerBase->UpdateFromMemory();
}

DWORD Read4Bytes(DWORD paddy) { return *(int*)paddy; }

void __stdcall LM_Shutdown(std::string log, std::string graphicCards, std::string processor) 
{
	if (isAlive && m_bGameStard)
	{
		Packet result(WIZ_HSACS_HOOK);
		result << uint8_t(HSACSOpCodes::LOG) << log << graphicCards << processor;
		LM_Send(&result);
	}
	Shutdown(log);
}

const DWORD adrexxs = 0x0041FC40;
void __declspec(naked) SetTex(DWORD uif, const std::string& szFN)
{
	__asm 
	{
		push ebp
		mov ebp, esp
		push[ebp + 0xC]
		mov ecx, [ebp + 8]
		call adrexxs
		pop ebp
		ret
	}
}

void HSACSEngine::SetTexImage(DWORD uif, const std::string& szFN)
{
	if (szFN.empty()) { printf("%d \n", uif);return; }
	SetTex(uif, szFN);
}
void HSACSEngine::Shutdown(std::string log) 
{
	tmpGraphics = "";
	for (string gpu : Player.GPU)
		tmpGraphics += xorstr(" | ") + gpu;
	LM_Shutdown(log, tmpGraphics, Player.CPU);
}

void LM_Send(Packet * pkt) 
{
	if (isAlive) 
	{
		uint8 opcode = pkt->GetOpcode();
		uint8 * out_stream = nullptr;
		uint16 len = (uint16)(pkt->size() + 1);

		out_stream = new uint8[len];
		out_stream[0] = pkt->GetOpcode();

		if (pkt->size() > 0)
			memcpy(&out_stream[1], pkt->contents(), pkt->size());

		BYTE* ptrPacket = out_stream;
		SIZE_T tsize = len;

		__asm
		{
			mov ecx, KO_PTR_PKT
			mov ecx, DWORD ptr ds : [ecx]
			push tsize
			push ptrPacket
			call KO_SND_FNC
		}
		delete[] out_stream;
	}
}

bool dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;

	return false;
}

std::string getHWID() 
{
	HW_PROFILE_INFO hwProfileInfo;
	GetCurrentHwProfile(&hwProfileInfo);
	string hwidWString = hwProfileInfo.szHwProfileGuid;
	string hwid(hwidWString.begin(), hwidWString.end());
	return hwid;
}

void GetMacHash(uint16& MACOffset1, uint16& MACOffset2);
uint16 GetCPUHash();

uint16 GetVolumeHash()
{
	DWORD SerialNum = 0;
	GetVolumeInformationA(xorstr("C:\\"), NULL, 0, &SerialNum, NULL, NULL, NULL, 0);
	uint16 nHash = (uint16)((SerialNum + (SerialNum >> 16)) & 0xFFFF);
	return nHash;
}

int64 GetHardwareID()
{
	uint16 MACData1, MACData2 = 0;
	GetMacHash(MACData1, MACData2);
	return _atoi64(string_format(xorstr("%d%d%d"), MACData1, MACData2, GetCPUHash()).c_str());/*, GetVolumeHash()).c_str());*/ //09.10.2020 GetVolumaHash Kapatýldý
	//Deger Uzun gelirse sýkýntý yaratabilirdi oyuncu oyuna giremeyebilirdi o sebeple kapatýldý
}

uint16 HashMacAddress(PIP_ADAPTER_INFO info)
{
	uint16 nHash = 0;
	for (uint32 i = 0; i < info->AddressLength; i++)
		nHash += (info->Address[i] << ((i & 1) * 8));
	return nHash;
}

void GetMacHash(uint16& MACOffset1, uint16& MACOffset2)
{
	IP_ADAPTER_INFO AdapterInfo[32];
	DWORD dwBufLen = sizeof(AdapterInfo);

	DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
	if (dwStatus != ERROR_SUCCESS)
		return;

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
	MACOffset1 = HashMacAddress(pAdapterInfo);
	if (pAdapterInfo->Next)
		MACOffset2 = HashMacAddress(pAdapterInfo->Next);

	if (MACOffset1 > MACOffset2)
	{
		uint16 TempOffset = MACOffset2;
		MACOffset2 = MACOffset1;
		MACOffset1 = TempOffset;
	}
}

uint16 GetCPUHash()
{
	int CPUInfo[4] = { 0, 0, 0, 0 };
	__cpuid(CPUInfo, 0);
	uint16 nHash = 0;
	uint16* nPointer = (uint16*)(&CPUInfo[0]);
	for (uint16 i = 0; i < 8; i++)
		nHash += nPointer[i];

	return nHash;
}



void StartHook()
{

	if (g_pMain->m_bHookStart) return;
	g_pMain->m_bHookStart = true;

	//DWORD genieadresi = 0x009A9FC0; //  2369

	//DWORD genieNewCD = (DWORD)malloc(sizeof(float));
	//ASMPatch(genieadresi, (char*)"\xD8\x05", 2);
	//*(float*)genieNewCD = 0.3f;

	//*(DWORD*)(genieadresi + 2) = genieNewCD;

	g_pMain->InitCameraZoom();
	g_pMain->m_SettingsMgr = new CSettingsManager();
	g_pMain->m_SettingsMgr->Init();
	g_pMain->m_PlayerBase = new CPlayerBase();

	if (g_pMain->m_PlayerBase)
	{
		if (g_pMain->m_PlayerBase->isWarrior())
			isRFastAttack = true;
		else if (g_pMain->m_PlayerBase->isPriest())
			isRFastAttack = true;
	}

	g_pMain->varius = g_pMain->rdword(KO_DLG, std::vector<int>{ 0x208});

#if (HOOK_SOURCE_VERSION == 1098)
	//Plugins
	g_pMain->pClientUIState = new CUIClientStatePlug();
	g_pMain->pClientTargetBar = new CUIClientTargetBarPlug();
	g_pMain->uiTaskbarMain = new CUITaskbarMainPlug();
	g_pMain->uiTaskbarSub = new CUITaskbarSubPlug();
	g_pMain->uiClanWindowPlug = new CUIClanWindowPlug();
	g_pMain->pClientPlayerRank = new ClientHookPlayerRank();
	g_pMain->uiGenieSubPlug = new CUIGenieSubPlug();
	g_pMain->uiSeedHelperPlug = new CUISeedHelperPlug();
	g_pMain->pClientTradePrice = new CUITradePricePlug();
	g_pMain->pClientTradeInventory = new CUITradeInventoryPlug();
	g_pMain->uiTradeItemDisplay = new CUITradeItemDiplayPlug();
	g_pMain->uiTradeItemDisplaySpecial = new CUITradeItemDisplaySpecialPlug();
	g_pMain->pClientMerchant = new CMerchantManager();
	g_pMain->pClientInventory = new CUIInventoryPlug();
	g_pMain->uiScoreBoard = new CUIScoreBoardPlug();
	g_pMain->uiGenieMain = new CUIGenieMain;
	g_pMain->pClientUIBarPlug = new CUIClientHPBarPlug();
	g_pMain->uiHpMenuPlug = new CUIHPBarMenuPlug();
	g_pMain->pClientTopRightNewPlug = new CUIClientTopRightNewPlug();
	g_pMain->uiMinimapPlug = new CUIMinimapPlug;
	g_pMain->uiCindirella = new CUICindirellaPlug();
	g_pMain->pClientCollection = new CUICollectionRacePlug();
	g_pMain->uiNoticeWind = new CUIMoticeMsgWnd();
	g_pMain->pClientWheelOfFunPlug = new CUIClientWheelOfFun();
	g_pMain->uiSearchMonster = new CUISearchMonsterDropPlug();
	g_pMain->pClientDropResult = new CDropResult();
	g_pMain->uiTagChange = new CTagChange();
	g_pMain->uiSkillPage = new HookSkillPage();
	g_pMain->uiQuestComplated = new CUIQuestComplated();
	g_pMain->pClientLottyEvent = new CUILotteryEventPlug();
	g_pMain->pClientEvetShowList = new CEventShowListPlug();
	g_pMain->uiMerchantList = new CUIMerchantListPlug();
	g_pMain->uiInformationWind = new CUIInformationWindPlug();
	g_pMain->uiChatBarPlug = new CUIChatBarPlug();
	g_pMain->uiQuestPage = new HookQuestPage();
	g_pMain->pClientSealToolTip = new CSealToolTip();
	g_pMain->uiPowerUpStore = new CUIPowerUpStore();
	g_pMain->uiPartyBBS = new CUIPartyBBSPlug();
	g_pMain->uiAccountRegisterPlug = new CUIAccountRegister();
	g_pMain->uiPieceChangePlug = new CUIPieceChangePlug();
	g_pMain->uiSupport = new CUISupportReportPlug();
#else
	g_pMain->uiAccountRegisterPlug = new CUIAccountRegister();
	g_pMain->uiQuestComplated = new CUIQuestComplated();
	g_pMain->pClientUIState = new CUIClientStatePlug();
	g_pMain->pClientTopRightNewPlug = new CUIClientTopRightNewPlug();
	g_pMain->uiMinimapPlug = new CUIMinimapPlug;
	g_pMain->pClientDropResult = new CDropResult();
	g_pMain->uiInformationWind = new CUIInformationWindPlug();
	g_pMain->uiNoticeWind = new CUIMoticeMsgWnd();
	g_pMain->uiGenieMain = new CUIGenieMain;
	g_pMain->uiGenieSubPlug = new CUIGenieSubPlug();
	g_pMain->pClientEvetShowList = new CEventShowListPlug();
	g_pMain->pClientTargetBar = new CUIClientTargetBarPlug();
	g_pMain->uiTaskbarMain = new CUITaskbarMainPlug();
	g_pMain->uiTaskbarSub = new CUITaskbarSubPlug();
	g_pMain->uiQuestPage = new HookQuestPage();
	g_pMain->uiClanWindowPlug = new CUIClanWindowPlug();
	g_pMain->uiMerchantList = new CUIMerchantListPlug();
	g_pMain->pClientTradeInventory = new CUITradeInventoryPlug();
	g_pMain->uiTradeItemDisplay = new CUITradeItemDiplayPlug();
	g_pMain->uiTradeItemDisplaySpecial = new CUITradeItemDisplaySpecialPlug();
	g_pMain->pClientMerchant = new CMerchantManager();
	g_pMain->pClientTradePrice = new CUITradePricePlug();
	g_pMain->pClientPlayerRank = new ClientHookPlayerRank();
	g_pMain->pClientInventory = new CUIInventoryPlug();
	g_pMain->uiSeedHelperPlug = new CUISeedHelperPlug();
	g_pMain->pClientUIBarPlug = new CUIClientHPBarPlug();
	g_pMain->uiScoreBoard = new CUIScoreBoardPlug();
	g_pMain->uiTagChange = new CTagChange();
	g_pMain->uiSkillPage = new HookSkillPage();
	g_pMain->uiSearchMonster = new CUISearchMonsterDropPlug();
	g_pMain->uiCindirella = new CUICindirellaPlug();
	g_pMain->uiPowerUpStore = new CUIPowerUpStore();
	g_pMain->pClientCollection = new CUICollectionRacePlug();
	g_pMain->pClientLottyEvent = new CUILotteryEventPlug();
	g_pMain->uiHpMenuPlug = new CUIHPBarMenuPlug();
	g_pMain->pClientSealToolTip = new CSealToolTip();
	g_pMain->pClientWheelOfFunPlug = new CUIClientWheelOfFun();
	g_pMain->uiPieceChangePlug = new CUIPieceChangePlug();
	g_pMain->uiSupport = new CUISupportReportPlug();
#endif

#if (HOOK_SOURCE_VERSION == 2369)
	g_pMain->pClientRightExchange = new CUIRightExchangePlug();
	g_pMain->pClientDailyReward = new CUIDailyRewardPlug();
	g_pMain->pClientEventHandler = new CUIEventHandler();
#else
	g_pMain->pClientRightExchange = new CUIRightExchangePlug();
	g_pMain->pClientDailyReward = new CUIDailyRewardPlug();
#endif

	g_pMain->m_bHookStart = true;
	g_pMain->pClientHookManager->OpenMerchantNotice();
#if (HOOK_SOURCE_VERSION == 1098)
	//uint8 Nation = *(uint8*)(*(DWORD*)KO_PTR_CHR + KO_OFF_NATION);
	//if (Nation == 1)
	//{
	//	POINT koScreen = *(POINT*)0x00DE297C;
	//	RECT region = g_pMain->GetUiRegion(g_pMain->uiChatBarPlug->m_dVTableAddr);  // chatting Box ve information Box'ýn pozisyonun konumlandýrýldýðý yer
	//	POINT orta;
	//	g_pMain->lastkoScreen = koScreen;
	//	g_pMain->curlastkoScreen = koScreen;
	//	LONG w = (region.right - region.left);
	//	orta.x = (koScreen.x / 2) - 513;
	//	orta.y = region.top + 1;
	//	g_pMain->SetUIPos(g_pMain->uiChatBarPlug->m_dVTableAddr, orta);
	//	orta.x = (koScreen.x / 2) + 214;
	//	g_pMain->SetUIPos(g_pMain->uiInformationWind->m_dVTableAddr, orta);
	//}
	//if (Nation == 2)
	//{
	//	POINT koScreen = *(POINT*)0x00DE297C;
	//	RECT region = g_pMain->GetUiRegion(g_pMain->uiChatBarPlug->m_dVTableAddr);  // chatting Box ve information Box'ýn pozisyonun konumlandýrýldýðý yer
	//	POINT orta;
	//	g_pMain->lastkoScreen = koScreen;
	//	g_pMain->curlastkoScreen = koScreen;
	//	LONG w = (region.right - region.left);
	//	orta.x = (koScreen.x / 2) - 513;
	//	orta.y = region.top - 12;
	//	g_pMain->SetUIPos(g_pMain->uiChatBarPlug->m_dVTableAddr, orta);
	//	orta.x = (koScreen.x / 2) + 215;
	//	g_pMain->SetUIPos(g_pMain->uiInformationWind->m_dVTableAddr, orta);
	//}
#endif

	if (true)
	{
		vector<int>offsets;
		offsets.push_back(0x2C8);
		offsets.push_back(0);

		g_pMain->m_sMerchantDisplayData = g_pMain->rdword(KO_DLG, offsets);
		for (int i = 0; i < 4; i++)
		{
			std::string find = string_format("%d", i);
			g_pMain->GetChildByID(g_pMain->m_sMerchantDisplayData, find, g_pMain->m_sMerchantDisplayItem[i]);
		}
	}
	if (true)
	{
		vector<int>offsets;
		offsets.push_back(0x2CC);
		offsets.push_back(0);

		g_pMain->m_sMerchantDisplaySpecialData = g_pMain->rdword(KO_DLG, offsets);
		for (int i = 0; i < 8; i++)
		{
			std::string find = string_format("%d", i);
			g_pMain->GetChildByID(g_pMain->m_sMerchantDisplaySpecialData, find, g_pMain->m_sMerchantDisplaySpecialItem[i]);
		}
	}
}

enum CollectionRaceType
{
	CRStartRecv,
	CRGetActiveTime,
	CRMonsterKilled,
	CRUpdateCounter,
	CRFinishRecv,
	CRHide
};

enum class ZindanOp { flagsend, updatescore, logout };
void CollectionRaceStart(Packet & pkt);
void CollectionRaceGetActiveTime(Packet & pkt);
void CollectionRaceHide();
void CollectionRaceDead(Packet & pkt);
void CollectionRaceUpdateCounter(Packet & pkt);
void CollectionRaceFinish();

void HandleCollectionRace(Packet & pkt)
{
	uint8 OpCode;
	pkt >> OpCode;

	switch (OpCode)
	{
	case CollectionRaceType::CRStartRecv:
		CollectionRaceStart(pkt);
		break;
	case CollectionRaceType::CRGetActiveTime:
		CollectionRaceGetActiveTime(pkt);
		break;
	case CollectionRaceType::CRMonsterKilled:
		CollectionRaceDead(pkt);
		break;
	case CollectionRaceType::CRUpdateCounter:
		CollectionRaceUpdateCounter(pkt);
		break;
	case CollectionRaceType::CRFinishRecv:
		CollectionRaceFinish();
		break;
	case CollectionRaceType::CRHide:
		CollectionRaceHide();
		break;
	default:
		break;
	}
}

void CollectionRaceStart(Packet & pkt)
{
	pkt.DByte();
	g_pMain->m_cCollettionStatus = true;

	for (int i = 0; i < 3; i++) 
	{
		pkt >> g_pMain->pCollectionRaceEvent.sSid[i]
			>> g_pMain->pCollectionRaceEvent.killTarget[i];

		g_pMain->pCollectionRaceEvent.killCount[i] = 0;
	}

	for (int i = 0; i < 3; i++)
		pkt >> g_pMain->pCollectionRaceEvent.RewardsItemID[i] >> g_pMain->pCollectionRaceEvent.RewardsItemCount[i] >> g_pMain->pCollectionRaceEvent.RewardsItemRate[i];

	pkt >> g_pMain->pCollectionRaceEvent.EventTimer
		>> g_pMain->pCollectionRaceEvent.m_iCompleteUserCount 
		>> g_pMain->pCollectionRaceEvent.TotalParticipantUser 
		>> g_pMain->pCollectionRaceEvent.Nation;

	pkt >> g_pMain->pCollectionRaceEvent.EventName 
		>> g_pMain->pCollectionRaceEvent.sZone;

	if(g_pMain->pClientCollection!=NULL)
		g_pMain->pClientCollection->Start();
}

void CollectionRaceGetActiveTime(Packet & pkt)
{
	if (g_pMain->pClientCollection == NULL)
		return;

	g_pMain->m_cCollettionStatus = true;

	uint16 userkillcount = 0;
	for (int i = 0; i < 3; i++) { pkt >> g_pMain->pCollectionRaceEvent.sSid[i] >> g_pMain->pCollectionRaceEvent.killTarget[i] >> userkillcount;g_pMain->pCollectionRaceEvent.killCount[i] = userkillcount; }

	for (int i = 0; i < 3; i++)
		pkt >> g_pMain->pCollectionRaceEvent.RewardsItemID[i] >> g_pMain->pCollectionRaceEvent.RewardsItemCount[i] >> g_pMain->pCollectionRaceEvent.RewardsItemRate[i];

	pkt >> g_pMain->pCollectionRaceEvent.EventTimer >> g_pMain->pCollectionRaceEvent.m_iCompleteUserCount >> g_pMain->pCollectionRaceEvent.TotalParticipantUser >> g_pMain->pCollectionRaceEvent.Nation;
	pkt >> g_pMain->pCollectionRaceEvent.EventName;

	if (g_pMain->pClientHookManager != NULL && g_pMain->pClientCollection != NULL)
	{
		g_pMain->pClientCollection->Open();
		g_pMain->pClientCollection->Start();
		g_pMain->pClientCollection->Update();
	}
}

void CollectionRaceHide()
{
	if (g_pMain->pClientHookManager != NULL && g_pMain->pClientCollection != NULL)
		g_pMain->pClientCollection->Close();
}

void CollectionRaceDead(Packet & pkt)
{
	if (g_pMain->pClientHookManager == NULL 
		|| g_pMain->pClientCollection == NULL)
		return;

	uint16 MonsterProtoID, UserKillCount[3];
	pkt >> MonsterProtoID 
		>> UserKillCount[0] 
		>> UserKillCount[1] 
		>> UserKillCount[2];
	
	if (g_pMain->m_cCollettionStatus)
	{
		bool updateRequired = false;
		for (int i = 0; i < 3; i++) 
		{
			if (g_pMain->pCollectionRaceEvent.sSid[i] == MonsterProtoID)
			{
				if (g_pMain->pCollectionRaceEvent.killCount[i] < g_pMain->pCollectionRaceEvent.killTarget[i])
				{
					g_pMain->pCollectionRaceEvent.killCount[i] = UserKillCount[i];
					updateRequired = true;
				}
			}
		}

		if (updateRequired && g_pMain->pClientHookManager != NULL)
			g_pMain->pClientCollection->Update();
	}
}

void CollectionRaceUpdateCounter(Packet & pkt)
{
	if (!g_pMain->m_cCollettionStatus || g_pMain->pClientHookManager == NULL || g_pMain->pClientCollection == NULL)
		return;

	pkt >> g_pMain->pCollectionRaceEvent.m_iCompleteUserCount 
		>> g_pMain->pCollectionRaceEvent.TotalParticipantUser;

	g_pMain->pClientCollection->UpdateFinishUserCounter();
}

void CollectionRaceFinish()
{
	if (g_pMain->pClientHookManager != NULL && g_pMain->pClientCollection != NULL)
		g_pMain->SetVisible(g_pMain->pClientCollection->m_dVTableAddr,false);

	if (g_pMain->pClientTopRightNewPlug != NULL)
		g_pMain->pClientTopRightNewPlug->HideCollectionRaceButton();

	for (int i = 0; i < 3; i++)
	{
		g_pMain->pCollectionRaceEvent.sSid[i] = 0;
		g_pMain->pCollectionRaceEvent.killTarget[i] = 0;
		g_pMain->pCollectionRaceEvent.killCount[i] = 0;
		g_pMain->pCollectionRaceEvent.RewardsItemID[i] = 0;
		g_pMain->pCollectionRaceEvent.RewardsItemCount[i] = 0;
		g_pMain->pCollectionRaceEvent.RewardsItemRate[i] = 0;
	}

	g_pMain->pCollectionRaceEvent.EventTimer = 0;
	g_pMain->pCollectionRaceEvent.m_iCompleteUserCount = g_pMain->pCollectionRaceEvent.TotalParticipantUser = 0;
	g_pMain->pCollectionRaceEvent.EventName = "Collection Race Event";

	g_pMain->m_cCollettionStatus = false;
}

void HandleCastleSiegeWarTimer(Packet & pkt) 
{
	pkt >> g_pMain->pCastleSiegeWar.SiegeType 
		>> g_pMain->pCastleSiegeWar.StartTime 
		>> g_pMain->pCastleSiegeWar.RemainingTime;

	pkt.SByte();
	pkt >> g_pMain->pCastleSiegeWar.SiegeOwnerClanName;

	if (g_pMain->pClientTopRightNewPlug->BasladimiBaslamadimi == false)
	{
		g_pMain->pClientTopRightNewPlug->BasladimiBaslamadimi = true;

		if (g_pMain->pCastleSiegeWar.SiegeType == 0x01)
			g_pMain->isSiegeWarStart = true;
		
		if (g_pMain->pCastleSiegeWar.SiegeType == 0x02) 
		{
			g_pMain->isSiegeWarStart = false;
			g_pMain->isSiegeWarActive = true;
		}
	}

	if (g_pMain->pCastleSiegeWar.SiegeType == 0x00)
	{
		g_pMain->isSiegeWarStart = false;
		g_pMain->isSiegeWarActive = false;
		g_pMain->pClientTopRightNewPlug->BasladimiBaslamadimi = false;
	}
}

bool voiceStarted = false;

void ZindanWarFlag(Packet &pkt) 
{
	std::string ename, kname;
	uint16 ekillcount, kkillcount; uint32 sRemTime;

	pkt >> ename 
		>> ekillcount
		>> kname 
		>> kkillcount
		>> sRemTime;

	if (ename.empty() 
		|| kname.empty() 
		|| !g_pMain->uiScoreBoard)
		return;

	if (!g_pMain->uiScoreBoard->m_Timer)
		g_pMain->uiScoreBoard->m_Timer = new CTimer(false);

	g_pMain->SetString(g_pMain->uiScoreBoard->Ename, ename);
	g_pMain->SetString(g_pMain->uiScoreBoard->Kname, kname);
	g_pMain->SetString(g_pMain->uiScoreBoard->EScore, std::to_string(ekillcount));
	g_pMain->SetString(g_pMain->uiScoreBoard->KScore, std::to_string(kkillcount));
	g_pMain->SetString(g_pMain->uiScoreBoard->RemTime, std::to_string(sRemTime));
	g_pMain->pZindanWarInfo.sStart = true;
	g_pMain->pZindanWarInfo.sRemTime = sRemTime;

	if (g_pMain->m_PlayerBase->m_iZoneID == 91)
		g_pMain->SetVisible(g_pMain->uiScoreBoard->m_dVTableAddr, true);
}

void ZindanWarLogOut() 
{
	if (g_pMain->uiScoreBoard)
		g_pMain->SetVisible(g_pMain->uiScoreBoard->m_dVTableAddr, false);

	g_pMain->pZindanWarInfo.Init();
}

void ZindanWarUpdateScore(Packet &pkt) 
{
	uint8 nation; uint16 killcount;
	pkt >> nation >> killcount;

	if ((nation != 1 && nation != 2) 
		|| !g_pMain->uiScoreBoard) 
		return;

	if (nation == 1) 
		g_pMain->SetString(g_pMain->uiScoreBoard->KScore, std::to_string(killcount));
	else
		g_pMain->SetString(g_pMain->uiScoreBoard->EScore, std::to_string(killcount));
}

void HandleZindanWar(Packet &pkt) 
{
	uint8 subcode = pkt.read<uint8>();
	switch ((ZindanOp)subcode)
	{
	case ZindanOp::flagsend:
		ZindanWarFlag(pkt);
		break;
	case ZindanOp::updatescore:
		ZindanWarUpdateScore(pkt);
		break;
	case ZindanOp::logout:
		ZindanWarLogOut();
		break;
	}
}

int GetMsSinceMidnightGmt(std::chrono::system_clock::time_point tpNow) {
	time_t tnow = std::chrono::system_clock::to_time_t(tpNow);
	tm * tmDate = std::localtime(&tnow);
	std::chrono::duration<int> durTimezone; // 28800 for HKT
	// because mktime assumes local timezone, we shift the time now to GMT, then fid mid
	time_t tmid = std::chrono::system_clock::to_time_t(tpNow - durTimezone);
	tm * tmMid = std::localtime(&tmid);
	tmMid->tm_hour = 0;
	tmMid->tm_min = 0;
	tmMid->tm_sec = 0;
	auto tpMid = std::chrono::system_clock::from_time_t(std::mktime(tmMid));
	auto durSince = tpNow - durTimezone - tpMid;
	auto durMs = std::chrono::duration_cast<std::chrono::milliseconds>(durSince);
	return durMs.count();
}

void HandleDailyQuests(Packet &pkt)
{
	uint8 subcode = 0;
	pkt >> subcode;

	switch (subcode)
	{
		case (uint8)DailyQuestOp::sendlist:
		{
			uint16 isize = 0;
			pkt >> isize;
			for (uint8 i = 0; i < isize; i++)
			{
				_DAILY_QUEST* pDailyQuest = new _DAILY_QUEST();
				pkt >> pDailyQuest->index >> pDailyQuest->questtype >> pDailyQuest->killtype;

				for (uint8 j = 0; j < 4; j++)
					pkt >> pDailyQuest->Mob[j] >> pDailyQuest->Reward[j] >> pDailyQuest->Count[j];

				pkt >> pDailyQuest->KillTarget >> pDailyQuest->ZoneID >> pDailyQuest->replaytime >> pDailyQuest->MinLevel >> pDailyQuest->MaxLevel;

				if (g_pMain->uiQuestPage)
					g_pMain->uiQuestPage->pDailyQuestArray.push_back(pDailyQuest);
			}
		}
		break;
		case (uint8)DailyQuestOp::userinfo:
		{
			uint16 isize = 0;
			pkt >> isize;
			for (uint8 i = 0; i < isize; i++)
			{
				uint8 id = 0;
				pkt >> id;

				uint8 status; uint16 mycount; uint32 time;
				pkt >> status >> mycount >> time;


				if (g_pMain->uiQuestPage)
				{
					for (uint32 i = 0; i < g_pMain->uiQuestPage->pDailyQuestArray.size(); i++)
					{
						if (g_pMain->uiQuestPage->pDailyQuestArray[i]->index == id)
						{
							g_pMain->uiQuestPage->pDailyQuestArray[i]->Status = status;
							g_pMain->uiQuestPage->pDailyQuestArray[i]->MyCount = mycount;
							g_pMain->uiQuestPage->pDailyQuestArray[i]->remtime = time;
						}
					}
				}
			}
			if (g_pMain->uiQuestPage)
				g_pMain->uiQuestPage->InitQuests();
		}
		break;
		case (uint8)DailyQuestOp::killupdate:
		{
			uint8 questID;
			pkt >> questID;
			uint16 mobid;
			pkt >> mobid;

			if (g_pMain->uiQuestPage) 
				g_pMain->uiQuestPage->KillTrigger(questID,mobid);
		}
		break;

	}
}

void HandleEventShowListList(Packet& pkt) 
{
	if (!g_pMain || !g_pMain->pClientHookManager) 
		return;

	g_pMain->pClientHookManager->m_sEventShowList.clear();

#if (HOOK_SOURCE_VERSION == 2369)
	pkt.DByte();
#endif

	pkt >> g_pMain->serverhour >> g_pMain->serverminute >> g_pMain->serversecond;

	uint16 sz = 0; pkt >> sz;
	if (!sz) return;

	for (int i = 0; i < sz; i++) {
		std::string name = "", time = ""; uint32 hour, minute;
		pkt >> name >> hour >> minute;
		if (name.empty()) continue;
		
		if (hour < 10) 
			time.append(string_format("0%s:", to_string(hour).c_str()));
		else 
			time.append(string_format("%s:", to_string(hour).c_str()));

		if (minute < 10) 
			time.append(string_format("0%s", to_string(minute).c_str()));
		else 
			time.append(string_format("%s", to_string(minute).c_str()));

		g_pMain->pClientHookManager->m_sEventShowList.push_back(EventShowList(name, time, hour, minute));
	}

	std::sort(g_pMain->pClientHookManager->m_sEventShowList.begin(),
		g_pMain->pClientHookManager->m_sEventShowList.end(), [](auto const& a, auto const& b) { return a.hour < b.hour && a.minute < b.minute; });

	int i = 0;
}

void HandleCindirella(Packet& pkt) {

	if (!g_pMain->uiCindirella)
		return;

	uint8 opcode = pkt.read<uint8>();
	switch ((cindopcode)opcode)
	{
	case cindopcode::selectclass:
	{
		uint8 result = pkt.read<uint8>();
		switch ((cindopcode)result)
		{
		case cindopcode::success:
		{
			uint8 index = pkt.read<uint8>();
			if (index > 4) return;

			g_pMain->uiCindirella->CharacterSelect(index);
		}
		break;
		case cindopcode::timewait:
		{
			uint32 remtime = pkt.read<uint32>();
			if (remtime)
				g_pMain->WriteInfoMessage((char*)string_format(xorstr("You have %d seconds to change it again"), remtime).c_str(), D3DCOLOR_ARGB(255, 255, 0, 255));
		}
		break;
		case cindopcode::notchange:
			g_pMain->WriteInfoMessage((char*)xorstr("You have not made any changes to your selections."), D3DCOLOR_ARGB(255, 255, 0, 255));
			break;
		case cindopcode::alreadyclass:
			g_pMain->WriteInfoMessage((char*)xorstr("same class please choose a different class."), D3DCOLOR_ARGB(255, 255, 0, 255));
			break;
		}
	}
		break;
	case cindopcode::nationchange:
	{
		uint8 result = pkt.read<uint8>();
		switch ((cindopcode)result)
		{
		case cindopcode::alreadynation:
			g_pMain->WriteInfoMessage((char*)xorstr("same nation please choose a different nation."), D3DCOLOR_ARGB(255, 255, 0, 255));
			break;
		case cindopcode::success:
			g_pMain->WriteInfoMessage((char*)xorstr("Nation selections applied successfully."), D3DCOLOR_ARGB(255, 255, 64, 89));
			break;
		case cindopcode::timewait:
		{
			uint32 remtime = pkt.read<uint32>();
			if (remtime)
				g_pMain->WriteInfoMessage((char*)string_format(xorstr("You have %d seconds to change it again"), remtime).c_str(), D3DCOLOR_ARGB(255, 255, 0, 255));
		}
		break;
		}
	}
	break;
	case cindopcode::joinevent:
	{
		uint32 tm = 0;
		pkt >> g_pMain->m_cindirellaInfo.prepare >> g_pMain->m_cindirellaInfo.index;
		pkt >> tm >> g_pMain->m_cindirellaInfo.mykill_count >> g_pMain->m_cindirellaInfo.mydead_count;
		pkt >> g_pMain->m_cindirellaInfo.k_kill_count >> g_pMain->m_cindirellaInfo.e_kill_count;

		g_pMain->uiCindirella->ison = true;
		g_pMain->m_cindirellaInfo.remainingtime = tm;

		g_pMain->uiCindirella->ShowPanel();
		if (!g_pMain->m_cindirellaInfo.prepare) g_pMain->uiCindirella->Start();

	}
	break;
	case cindopcode::starting:
	{
		uint32 tm = 0;
		pkt >> tm;

		if (!g_pMain->uiCindirella->ison) g_pMain->uiCindirella->ison = true;
		g_pMain->m_cindirellaInfo.remainingtime = tm;
		g_pMain->uiCindirella->Start();
		g_pMain->uiCindirella->Open();
	}
	break;
	case cindopcode::finish:
	{

		g_pMain->uiCindirella->Reset();
		g_pMain->uiCindirella->Close();

		if (g_pMain->pClientTopRightNewPlug && g_pMain->IsVisible(g_pMain->pClientTopRightNewPlug->m_cindirella))
			g_pMain->SetVisible(g_pMain->pClientTopRightNewPlug->m_cindirella, false);
	}
	break;
	case cindopcode::updatekda:
	{
		uint8 set = pkt.read<uint8>();
		if (!set)
			pkt >> g_pMain->m_cindirellaInfo.mykill_count >> g_pMain->m_cindirellaInfo.mydead_count;
		else
			pkt >> g_pMain->m_cindirellaInfo.e_kill_count >> g_pMain->m_cindirellaInfo.k_kill_count;

		if (!g_pMain->uiCindirella || !g_pMain->uiCindirella->status || g_pMain->uiCindirella->prepare)
			return;

		g_pMain->uiCindirella->UpdateKDA();
	}
	break;
	default:
		break;
	}
}

void HandleCSW(Packet& pkt)
{
	enum class csw_flags
	{
		timer,
		finish
	};

	if (g_pMain->pClientHookManager->pClientUICastleSiegeWarFare == NULL)
	{
		std::string name = "";
#if(HOOK_SOURCE_VERSION == 1098)
		if (g_pMain->GetNation() == ZONE_KARUS)
			name = g_pMain->dcpUIF(xorstr("HSACSX\\UI\\re_ka_csw_timer.hsacsx"));
		else if (g_pMain->GetNation() == ZONE_ELMORAD)
			name = g_pMain->dcpUIF(xorstr("HSACSX\\UI\\re_el_csw_timer.hsacsx"));
		else
			return;
#else
		name = g_pMain->dcpUIF(xorstr("HSACSX\\UI\\re_csw_timer.hsacsx"));
#endif
		if (name.empty())
			return;

		g_pMain->pClientHookManager->pClientUICastleSiegeWarFare = new CUICastleSiegeWar();
		g_pMain->pClientHookManager->pClientUICastleSiegeWarFare->Init(g_pMain->pClientHookManager);
		g_pMain->pClientHookManager->pClientUICastleSiegeWarFare->LoadFromFile(g_pMain->m_BasePath + name.c_str(), N3FORMAT_VER_1068);
		g_pMain->pClientHookManager->AddChild(g_pMain->pClientHookManager->pClientUICastleSiegeWarFare);
		remove(name.c_str());
	}

	if (!g_pMain->pClientHookManager->pClientUICastleSiegeWarFare)
		return;
	
	uint8 subCode = pkt.read<uint8>();
	switch ((csw_flags)subCode)
	{
	case csw_flags::timer:
	{
		uint8 OpStatus;
		uint32 remtime, war_time; std::string knights_name;
		pkt.SByte();
		pkt >> remtime >> knights_name >> OpStatus >> war_time;

		if (remtime)
			g_pMain->pClientHookManager->pClientUICastleSiegeWarFare->setOptions(remtime, knights_name, OpStatus, war_time);
	}
	break;
	case csw_flags::finish:
	{
		if (g_pMain->pClientHookManager->pClientUICastleSiegeWarFare)
			g_pMain->pClientHookManager->pClientUICastleSiegeWarFare->Close();
	}
	default:
		break;
	}
}

void HandleJuraidEvent(Packet& pkt)
{
	uint16 karus, elmos, remtime;
	pkt >> karus >> elmos >> remtime;
	if (remtime) g_pMain->OpenjuraidScreen(true, karus, elmos, remtime);
}

void HandleHSACSXOldRightClickPacketExchange(Packet& pkt)
{
#if(HOOK_SOURCE_VERSION == 2369 || HOOK_SOURCE_VERSION == 1098)
	uint32 nItemID;
	uint8 m_iSlot;
	pkt >> m_iSlot >> nItemID;

	if (g_pMain->pClientRightExchange == NULL)
		return;

	if (nItemID == 0)
	{
		g_pMain->SetVisible(g_pMain->pClientRightExchange->Right[0]->icon, false);
		g_pMain->SetUIStyle(g_pMain->pClientRightExchange->Right[0]->icon, 0x20);
		return;
	}

	g_pMain->SetTexImage(g_pMain->pClientRightExchange->Right[0]->icon, g_pMain->GetItemDXT(nItemID));
	g_pMain->pClientRightExchange->Right[0]->nItemID = nItemID;
	g_pMain->pClientRightExchange->Right[0]->nRentalTime = 0;
	g_pMain->SetVisible(g_pMain->pClientRightExchange->Right[0]->icon, true);
	g_pMain->SetUIStyle(g_pMain->pClientRightExchange->Right[0]->icon, 0x20);

	g_pMain->pClientRightExchange->nBaseItemID = nItemID;
	g_pMain->pClientRightExchange->m_iSlot = m_iSlot;

	CItem* pData = (CItem*)g_pMain->pClientTBLManager->GetItemData(g_pMain->pClientRightExchange->nBaseItemID);
	if (pData == NULL)
		return;

	g_pMain->SetTexImage(g_pMain->pClientRightExchange->ItemSlot, g_pMain->GetItemDXT(g_pMain->pClientRightExchange->nBaseItemID));
	g_pMain->SetVisible(g_pMain->pClientRightExchange->ItemSlot, true);
	g_pMain->SetString(g_pMain->pClientRightExchange->Text_0, pData->strName.c_str());
	g_pMain->pClientRightExchange->Open(2);
#endif
}

void HandleHSACSXNewRightClickPacketExchange(Packet& pkt)
{
#if(HOOK_SOURCE_VERSION == 2369 || HOOK_SOURCE_VERSION == 1098)
	uint32 nItemID;
	pkt >> nItemID;

	if (g_pMain->pClientRightExchange == NULL)
		return;

	auto itr = g_pMain->pClientRightExchange->pRightClickExchangeReward;
	if (std::find(itr.begin(), itr.end(), nItemID) != itr.end())
	{
		Packet result(WIZ_HSACS_HOOK, uint8(WIZ_ITEM_EXCHANGE_INFO));
		result << uint8(3) << uint8(1) << nItemID;
		g_pMain->Send(&result);
	}
	else
	{
		itr = g_pMain->pClientRightExchange->pRightClickExchangeAll;
		if (std::find(itr.begin(), itr.end(), nItemID) != itr.end())
		{
			Packet result(WIZ_HSACS_HOOK, uint8(WIZ_ITEM_EXCHANGE_INFO));
			result << uint8(3) << uint8(2) << nItemID;
			g_pMain->Send(&result);
		}
		else
		{
			itr = g_pMain->pClientRightExchange->pRightClickExchangeKnightCash;
			if (std::find(itr.begin(), itr.end(), nItemID) != itr.end())
			{
				Packet result(WIZ_HSACS_HOOK, uint8(WIZ_ITEM_EXCHANGE_INFO));
				result << uint8(3) << uint8(4) << nItemID;
				g_pMain->Send(&result);
			}
			else
			{
				itr = g_pMain->pClientRightExchange->pRightClickExchangePremium;
				if (std::find(itr.begin(), itr.end(), nItemID) != itr.end())
				{
					Packet result(WIZ_HSACS_HOOK, uint8(WIZ_ITEM_EXCHANGE_INFO));
					result << uint8(3) << uint8(3) << nItemID;
					g_pMain->Send(&result);
				}
				else
				{
					itr = g_pMain->pClientRightExchange->pRightClickExchangeGenie;
					if (std::find(itr.begin(), itr.end(), nItemID) != itr.end())
					{
						Packet result(WIZ_HSACS_HOOK, uint8(WIZ_ITEM_EXCHANGE_INFO));
						result << uint8(3) << uint8(6) << nItemID;
						g_pMain->Send(&result);
					}
					else
					{
						itr = g_pMain->pClientRightExchange->pRightClickExchangeKnightTL;
						if (std::find(itr.begin(), itr.end(), nItemID) != itr.end())
						{
							Packet result(WIZ_HSACS_HOOK, uint8(WIZ_ITEM_EXCHANGE_INFO));
							result << uint8(3) << uint8(7) << nItemID;
							g_pMain->Send(&result);
						}
						else
							return;
					}
				}
			}
		}
	}
#endif
}

void HandleHSACSXNewRightClickExchange(Packet& pkt)
{
#if(HOOK_SOURCE_VERSION == 2369 || HOOK_SOURCE_VERSION == 1098)
	uint8 subcode = pkt.read<uint8>();
	switch (subcode)
	{
#if(HOOK_SOURCE_VERSION == 2369)
	case 1:
		HandleHSACSXNewRightClickPacketExchange(pkt);
		break;
#endif
	case 2 :
		HandleHSACSXOldRightClickPacketExchange(pkt);
		break;
	default:
		break;
	}
#endif
}

void HandleHSACSXNewItemMoveLoadder(Packet& pkt)
{
#if(HOOK_SOURCE_VERSION == 2369)
	uint8 strSlot[28];
	uint8 sSubCode[28];
	uint32 strItemID[28];
	if (g_pMain->pClientInventory == NULL)
		return;

	for (int i = 0; i < 28; i++) {
		pkt >> strSlot[i] >> sSubCode[i] >> strItemID[i];
		g_pMain->pClientInventory->sItemSlot[i] = strSlot[i];
		g_pMain->pClientInventory->sSubCode[i] = sSubCode[i];
		g_pMain->pClientInventory->sItemID[i] = strItemID[i];
	}
#endif
}

void HSACSLotteryEventStard(Packet& pkt)
{
	if (g_pMain->pClientLottyEvent == NULL)
		return;

	if (!g_pMain->IsVisible(g_pMain->pClientLottyEvent->m_dVTableAddr))
		g_pMain->pClientLottyEvent->OpenLottery();

	g_pMain->SetState(g_pMain->pClientLottyEvent->btn_join, UI_STATE_BUTTON_NORMAL);

	uint32 reqItems[5] = { 0 }, reqItemsCount[5] = { 0 }, rewardItems[4] = { 0 };
	uint32 remainingTime = 0;
	uint32 partLimit = 0;
	uint32 participant = 0;
	uint32 ticketID = 0;

	for (int i = 0; i < 5; i++)
		pkt >> reqItems[i] >> reqItemsCount[i];

	for (int i = 0; i < 4; i++)
		pkt >> rewardItems[i];

	pkt >> partLimit >> remainingTime >> participant >> ticketID;

	g_pMain->pLotteryEvent.sLimit = partLimit;
	g_pMain->pLotteryEvent.sParticiPant = participant;
	g_pMain->pLotteryEvent.sRemainingTime = remainingTime;

	float per = 0.0f;
	if (g_pMain->pLotteryEvent.sParticiPant > 0)
		per = (1 / g_pMain->pLotteryEvent.sParticiPant) * 100;
	else
		per = 100.0f;

	g_pMain->SetString(g_pMain->pClientLottyEvent->str_participant, string_format(xorstr(" %d / %d"), participant, partLimit));
	if (ticketID == 0)
		g_pMain->SetString(g_pMain->pClientLottyEvent->str_ticket, xorstr("-"));
	else {
		g_pMain->SetString(g_pMain->pClientLottyEvent->str_ticket, string_format(xorstr("#%d"), ticketID));
		g_pMain->SetState(g_pMain->pClientLottyEvent->btn_join, UI_STATE_BUTTON_DISABLE);
	}

	g_pMain->SetString(g_pMain->pClientLottyEvent->ticket_buy_count, string_format(xorstr("%d"), reqItemsCount[0]));
	g_pMain->pClientLottyEvent->Update(reqItems, reqItemsCount, rewardItems);
}

void HSACSLotteryEventUpdate(Packet& pkt)
{
	g_pMain->pLotteryEvent.sParticiPant++;
	float per = 0.0F;
	per = (1 / g_pMain->pLotteryEvent.sParticiPant) * 100;
	g_pMain->SetString(g_pMain->pClientLottyEvent->str_participant, string_format(xorstr(" %d / %d"), g_pMain->pLotteryEvent.sParticiPant, g_pMain->pLotteryEvent.sLimit));
}

void HSACSLotteryEventJoin(Packet& pkt)
{
	uint32 sTicket = pkt.read<uint32>();
	g_pMain->SetString(g_pMain->pClientLottyEvent->str_ticket, string_format(xorstr("#%d"), sTicket));
	g_pMain->pClientHookManager->ShowMessageBox(xorstr("Lottey Event"), "You have successfully participated in the event.", MsgBoxTypes::Ok);
}

void HSACSLotteryEventStringJoin(Packet& pkt)
{
	std::string sTicket = "";
	pkt >> sTicket;
	g_pMain->pClientHookManager->ShowMessageBox(xorstr("Error"), sTicket, MsgBoxTypes::Ok);
}

void HSACSLotteryEventRegister(Packet& pkt)
{
	uint8 subcode = pkt.read<uint8>();
	switch (subcode)
	{
	case 1:
		HSACSLotteryEventJoin(pkt);
		break;
	default:
		HSACSLotteryEventStringJoin(pkt);
		break;
	}
}

void HSACSLotteryEventFinish(Packet& pkt)
{
	uint32 reqItems[5] = { 0 }, reqItemsCount[5] = { 0 }, rewardItems[4] = { 0 };
	g_pMain->pLotteryEvent.sLimit = 0;
	g_pMain->pLotteryEvent.sParticiPant = 0;
	g_pMain->pLotteryEvent.sRemainingTime = 0;

	g_pMain->SetString(g_pMain->pClientLottyEvent->ticket_buy_count, xorstr("-"));
	g_pMain->SetString(g_pMain->pClientLottyEvent->str_participant, string_format(xorstr(" %d / %d"), 0, 0));
	g_pMain->SetString(g_pMain->pClientLottyEvent->str_ticket, xorstr("-"));
	g_pMain->SetState(g_pMain->pClientLottyEvent->btn_join, UI_STATE_BUTTON_NORMAL);
	g_pMain->pClientLottyEvent->Update(reqItems, reqItemsCount, rewardItems, true);

	if (g_pMain->IsVisible(g_pMain->pClientLottyEvent->m_dVTableAddr))
		g_pMain->pClientLottyEvent->Close();

	if (g_pMain->pClientTopRightNewPlug != NULL)
	{
		if (g_pMain->IsVisible(g_pMain->pClientTopRightNewPlug->m_lottery))
			g_pMain->pClientTopRightNewPlug->HideLotteryButton();
	}

	/*if (g_pMain->pClientTopRightNewPlug != NULL)
	{
		if (!g_pMain->IsVisible(g_pMain->pClientTopRightNewPlug->m_lottery) && g_pMain->pClientTopRightNewPlug->m_lottery)
		{
			g_pMain->SetVisible(g_pMain->pClientTopRightNewPlug->m_lottery, false);
			g_pMain->SetState(g_pMain->pClientTopRightNewPlug->m_lottery, UI_STATE_BUTTON_DISABLE);
		}
	}*/
}

void HSACSLotteryEventSystem(Packet& pkt)
{
	uint8 subcode = pkt.read<uint8>();

	if (g_pMain->pClientLottyEvent == NULL)
		return;

	switch (subcode)
	{
	case 1:
		HSACSLotteryEventStard(pkt);
		break;
	case 2:
		HSACSLotteryEventUpdate(pkt);
		break;
	case 3:
		HSACSLotteryEventRegister(pkt);
		break;
	case 4:
		HSACSLotteryEventFinish(pkt);
		break;
	default:
		break;
	}
}

void HSACSRankSymbolSystem(Packet& pkt)
{
	if (g_pMain->pClientPlayerRank == NULL)
		return;

	uint8 sKarus, sHuman;
	uint16 sKarusRank, sHumanRank;
	g_pMain->pClientPlayerRank->ResetBase();

	pkt >> sHuman >> sHumanRank;
	for (int i = 0; i < sHumanRank; i++)
	{
		uint8 byRank;
		pkt >> byRank;
		//printf("HUMAN %d\n", byRank);
		if (byRank > 0)
		{
			g_pMain->SetTexImage(g_pMain->pClientPlayerRank->nHumanBase[i], string_format("symbol_us\\s_0_%d_0.dxt", byRank));
			g_pMain->SetVisible(g_pMain->pClientPlayerRank->nHumanBase[i], true);
			g_pMain->SetVisible(g_pMain->pClientPlayerRank->nHumanBaseEffeckt[i], true);
		}
	}

	pkt >> sKarus >> sKarusRank;
	for (int i = 0; i < sKarusRank; i++)
	{
		uint8 byRank;
		pkt >> byRank;
		//printf("Karus %d\n", byRank);
		if (byRank > 0)
		{
			g_pMain->SetTexImage(g_pMain->pClientPlayerRank->nKarusBase[i], string_format("symbol_us\\s_0_%d_0.dxt", byRank));
			g_pMain->SetVisible(g_pMain->pClientPlayerRank->nKarusBase[i], true);
			g_pMain->SetVisible(g_pMain->pClientPlayerRank->nKarusBaseEffeckt[i], true);
		}
	}
}

void HSACSShowMessageErrorBox(Packet& pkt)
{
	uint8 sSubCode, sCode;
	std::string sTitle = "", sMessage = "";

	pkt >> sSubCode >> sCode;
	if (sSubCode == 1)
	{
		if (sCode == 1)
		{
			pkt.DByte();
			pkt >> sTitle >> sMessage;
			g_pMain->pClientHookManager->ShowMessageBox(sTitle, sMessage, MsgBoxTypes::Ok);
		}
	}
}

void HSACSShowMessageBox(Packet& pkt)
{
	string sTitle, sMessage;
	pkt >> sTitle >> sMessage;

	if (g_pMain->pClientHookManager != NULL 
		&& !sTitle.empty() 
		&& !sMessage.empty() 
		&& sTitle.length() <= 30 
		&& sMessage.length() <= 255)
		g_pMain->pClientHookManager->ShowMessageBox(sTitle, sMessage, MsgBoxTypes::Ok);
}

void HSACSAutoMiningSystem(Packet& pkt)
{
	uint32 nItemID = pkt.read<uint32>();
	if (g_pMain->pClientDropResult != NULL) 
	{
		g_pMain->pClientHookManager->ShowDropResult();
		g_pMain->SetVisible(g_pMain->pClientDropResult->m_dVTableAddr, false);
	}

	g_pMain->pClientDropResult->SetTitle("Auto Mining");
	g_pMain->pClientDropResult->AddItem(nItemID);

	TABLE_ITEM_BASIC* item = g_pMain->pClientTBLManager->GetItemData(nItemID);
	if (item != nullptr)
		g_pMain->WriteInfoMessage((char*)string_format(xorstr("[Mining] you received %s"), item->strName.c_str()).c_str(), 0xFFF2AB);

	g_pMain->pClientTopRightNewPlug->DropResultStatus(true);
}

void HSACSAutoFishingSystem(Packet& pkt)
{
	uint32 nItemID = pkt.read<uint32>();

	if (g_pMain->pClientDropResult == NULL) 
	{
		g_pMain->pClientHookManager->ShowDropResult();
		g_pMain->SetVisible(g_pMain->pClientDropResult->m_dVTableAddr, false);
	}

	g_pMain->pClientDropResult->SetTitle("Auto Fishing");
	g_pMain->pClientDropResult->AddItem(nItemID);

	TABLE_ITEM_BASIC* item = g_pMain->pClientTBLManager->GetItemData(nItemID);
	if (item != nullptr)
		g_pMain->WriteInfoMessage((char*)string_format(xorstr("[Fishing] you received %s"), item->strName.c_str()).c_str(), 0xFFF2AB);

	g_pMain->pClientTopRightNewPlug->DropResultStatus(true);
}

void HSACSAutoWeelOfFunSystem()
{
	if (g_pMain->pClientDropResult != NULL)
	{
		g_pMain->pClientDropResult->Close();
		g_pMain->pClientTopRightNewPlug->DropResultStatus(false);
	}
}

void HSACSAutoWeelOfFunSystem(Packet& pkt)
{
	uint32 nItemID = pkt.read<uint32>();

	if (g_pMain->pClientWheelOfFunPlug != NULL)
	{
		g_pMain->pClientWheelOfFunPlug->StopChallange();
		g_pMain->pClientWheelOfFunPlug->GiveItemImage(nItemID);
	}
}

void HSACSAutoDefault()
{
	if (g_pMain->pClientDropResult != NULL)
	{
		g_pMain->pClientDropResult->Close();
		g_pMain->pClientTopRightNewPlug->DropResultStatus(false);
	}
	g_pMain->WriteInfoMessageExt((char*)xorstr("[Mining/Fishing] stopped"), 0xFFF2AB);
}

void HSACSUpdateKnightCash(Packet& pkt)
{
	uint32 sKnightCash, sTLCash;
	pkt >> sKnightCash >> sTLCash;

	g_pMain->Player.KnightCash = sKnightCash;
	g_pMain->Player.TlBalance = sTLCash;

	if (g_pMain->pClientUIState != NULL)
	{
		g_pMain->pClientUIState->m_iBalance = sTLCash;
		g_pMain->pClientUIState->m_iCash = sKnightCash;
		g_pMain->pClientUIState->UpdateUI();
	}

	if (g_pMain->pClientTradeInventory != NULL)
		g_pMain->pClientTradeInventory->UpdateTotal(sKnightCash);

	if (g_pMain->uiPowerUpStore != NULL)
		g_pMain->uiPowerUpStore->UpdateCash(sKnightCash, sTLCash);
}

void HSACSUpdateUserData(Packet& pkt)
{
	if (g_pMain->m_PlayerBase == NULL)
		return;

	g_pMain->m_PlayerBase->Update(pkt);
}

void HSACSKnightCashSystem(Packet& pkt)
{
	uint32 sKnightCash, sTLCash;
	pkt >> sKnightCash >> sTLCash;

	if (g_pMain->pClientUIState != NULL)
	{
		if (sKnightCash > g_pMain->pClientUIState->m_iCash)
			g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Received %d knight cash."), sKnightCash - g_pMain->pClientUIState->m_iCash).c_str(), 0xFFF2AB);
		else if (sKnightCash < g_pMain->pClientUIState->m_iCash)
			g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Lost %d knight cash."), g_pMain->pClientUIState->m_iCash - sKnightCash).c_str(), 0xFF0000);

		if (sTLCash > g_pMain->pClientUIState->m_iBalance)
			g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Received %d tl balance."), sTLCash - g_pMain->pClientUIState->m_iBalance).c_str(), 0xFFF2AB);
		else if (sTLCash < g_pMain->pClientUIState->m_iBalance)
			g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Lost %d tl balance."), g_pMain->pClientUIState->m_iBalance - sTLCash).c_str(), 0xFF0000);
	}

	g_pMain->Player.KnightCash = sKnightCash;
	g_pMain->Player.TlBalance = sTLCash;

	if (g_pMain->pClientUIState != NULL)
	{
		g_pMain->pClientUIState->m_iCash = sKnightCash;
		g_pMain->pClientUIState->m_iBalance = sTLCash;
		g_pMain->pClientUIState->UpdateUI();
	}
	if (g_pMain->pClientWheelOfFunPlug != NULL)
		g_pMain->SetString(g_pMain->pClientWheelOfFunPlug->KnightCash, string_format("Knight Cash : %d", sKnightCash).c_str());

	if (g_pMain->pClientTradeInventory != NULL)
		g_pMain->pClientTradeInventory->UpdateTotal(sKnightCash);

	if (g_pMain->uiPowerUpStore != NULL)
		g_pMain->uiPowerUpStore->UpdateCash(sKnightCash, sTLCash);
}

void HSACSInfoMessageSystem(Packet& pkt)
{
	string message;
	pkt >> message;
	g_pMain->WriteInfoMessageExt((char*)message.c_str(), 0xFFF2AB);
}

void HSACSAutoDropSystem(Packet& pkt)
{
	uint8 subcode = pkt.read<uint8>();

	if (g_pMain->m_bGameStart == false
		|| g_pMain->pClientHookManager == NULL)
		return;

	switch (subcode)
	{
	case 0:
		HSACSAutoMiningSystem(pkt);
		break;
	case 1:
		HSACSAutoFishingSystem(pkt);
		break;
	case 2:
		HSACSAutoWeelOfFunSystem();
		break;
	case 3:
		HSACSAutoWeelOfFunSystem(pkt);
		break;
	default:
		HSACSAutoDefault();
		break;
	}
}

void HandleHSACSXNewMessageSystem(Packet& pkt)
{
	uint8 sType, sTime;
	std::string sTitle, sMessage;
	uint32 sColour;
	pkt.DByte();
	pkt >> sType;

	if (sType == (uint8)infomessageop::box) 
	{
		pkt >> sTitle >> sMessage >> sTime;
		if (sTitle.empty() || sMessage.empty())
			return;

		g_pMain->pClientHookManager->ShowMessageBox(sTitle, sMessage, MsgBoxTypes::Ok);
	}
	else if (sType == (uint8)infomessageop::infobar)
	{
		pkt >> sMessage >> sColour;
		if (sMessage.empty())
			return;

		g_pMain->WriteInfoMessageExt((char*)string_format(sMessage).c_str(), sColour);
	}
}

void HandleHSACSXNewRightClickExchangeReward(Packet& pkt)
{
#if(HOOK_SOURCE_VERSION == 2369 || HOOK_SOURCE_VERSION == 1098)
	if (g_pMain->pClientRightExchange == NULL)
		return;

	uint16 nCount = pkt.read<uint16>();
	for (int i = 0; i < nCount;i++)
		g_pMain->pClientRightExchange->pRightClickExchangeReward.push_back(pkt.read<uint32>());
#endif
}

void HandleHSACSXNewRightClickExchangeAll(Packet& pkt)
{
#if(HOOK_SOURCE_VERSION == 2369 || HOOK_SOURCE_VERSION == 1098)
	if (g_pMain->pClientRightExchange == NULL)
		return;

	uint16 nCount = pkt.read<uint16>();
	for (int i = 0; i < nCount;i++)
		g_pMain->pClientRightExchange->pRightClickExchangeAll.push_back(pkt.read<uint32>());
#endif
}

void HandleHSACSXNewRightClickExchangePremium(Packet& pkt)
{
#if(HOOK_SOURCE_VERSION == 2369 || HOOK_SOURCE_VERSION == 1098)
	if (g_pMain->pClientRightExchange == NULL)
		return;

	uint16 nCount = pkt.read<uint16>();
	for (int i = 0; i < nCount;i++)
		g_pMain->pClientRightExchange->pRightClickExchangePremium.push_back(pkt.read<uint32>());
#endif
}

void HandleHSACSXNewRightClickExchangeKnightCash(Packet& pkt)
{
#if(HOOK_SOURCE_VERSION == 2369 || HOOK_SOURCE_VERSION == 1098)
	if (g_pMain->pClientRightExchange == NULL)
		return;

	uint16 nCount = pkt.read<uint16>();
	for (int i = 0; i < nCount;i++)
		g_pMain->pClientRightExchange->pRightClickExchangeKnightCash.push_back(pkt.read<uint32>());
#endif
}

void HandleHSACSXNewRightClickGeneratorExchange(Packet& pkt)
{
#if(HOOK_SOURCE_VERSION == 1098)
	if (g_pMain->pClientRightExchange == NULL)
		return;

	uint16 nCount = pkt.read<uint16>();
	for (int i = 0; i < nCount; i++)
		g_pMain->pClientRightExchange->pRightClickGeneratorExchange.push_back(pkt.read<uint32>());
#endif
}

void HandleHSACSXNewRightClickGenie(Packet& pkt)
{
#if(HOOK_SOURCE_VERSION == 2369 || HOOK_SOURCE_VERSION == 1098)
	if (g_pMain->pClientRightExchange == NULL)
		return;

	uint16 nCount = pkt.read<uint16>();
	for (int i = 0; i < nCount; i++)
		g_pMain->pClientRightExchange->pRightClickExchangeGenie.push_back(pkt.read<uint32>());
#endif
}

void HandleHSACSXNewRightClickKnightTL(Packet& pkt)
{
#if(HOOK_SOURCE_VERSION == 2369 || HOOK_SOURCE_VERSION == 1098)
	if (g_pMain->pClientRightExchange == NULL)
		return;

	uint16 nCount = pkt.read<uint16>();
	for (int i = 0; i < nCount; i++)
		g_pMain->pClientRightExchange->pRightClickExchangeKnightTL.push_back(pkt.read<uint32>());
#endif
}

void HandleHSACSXNewRightClickExchangeLoadder(Packet& pkt)
{
#if(HOOK_SOURCE_VERSION == 2369 || HOOK_SOURCE_VERSION == 1098)
	uint8 subcode = pkt.read<uint8>();
	switch (subcode)
	{
	case 1:
		HandleHSACSXNewRightClickExchangeReward(pkt);
		break;
	case 2:
		HandleHSACSXNewRightClickExchangeAll(pkt);
		break;
	case 3:
		HandleHSACSXNewRightClickExchangePremium(pkt);
		break;
	case 4: 
		HandleHSACSXNewRightClickExchangeKnightCash(pkt);
		break;
	case 5:
		HandleHSACSXNewRightClickGeneratorExchange(pkt);
		break;
	case 6:
		HandleHSACSXNewRightClickGenie(pkt);
		break;
	case 7:
		HandleHSACSXNewRightClickKnightTL(pkt);
		break;
	default:
		break;
	}
#endif
}

void HandleHSACSXNewRightClickExchangePatch(Packet& pkt)
{
#if(HOOK_SOURCE_VERSION == 2369 || HOOK_SOURCE_VERSION == 1098)
	uint8 sType;uint32 sItemID;
	pkt >> sType >> sItemID;

	if (g_pMain->pClientRightExchange == NULL)
		return;

	for (int i = 0; i < 25; i++)
	{
		uint32 nItemID;
		uint32 nRentalTime;
		pkt >> nItemID >> nRentalTime;
		//printf("HandleHSACSXNewRightClickExchangePatch %d\n", nItemID);
		if (nItemID == 0)
		{
			g_pMain->SetVisible(g_pMain->pClientRightExchange->m_Icon[i]->icon, false);
			g_pMain->SetUIStyle(g_pMain->pClientRightExchange->m_Icon[i]->icon, 0x20);
			continue;
		}
		g_pMain->SetTexImage(g_pMain->pClientRightExchange->m_Icon[i]->icon, g_pMain->GetItemDXT(nItemID));
		g_pMain->pClientRightExchange->m_Icon[i]->nItemID = nItemID;
		g_pMain->pClientRightExchange->m_Icon[i]->nRentalTime = nRentalTime;
		g_pMain->SetVisible(g_pMain->pClientRightExchange->m_Icon[i]->icon, true);
		g_pMain->SetUIStyle(g_pMain->pClientRightExchange->m_Icon[i]->icon, 0x20);
	}

	g_pMain->pClientRightExchange->nBaseItemID = sItemID;
	CItem* pData = (CItem*)g_pMain->pClientTBLManager->GetItemData(g_pMain->pClientRightExchange->nBaseItemID);
	if (pData == NULL)
		return;

	if (sType == 1)
		g_pMain->SetString(g_pMain->pClientRightExchange->txt_title, "Right Click Select Exchange");
	else if (sType == 2)
		g_pMain->SetString(g_pMain->pClientRightExchange->txt_title, "Right Click All Exchange");
	else if (sType == 3)
		g_pMain->SetString(g_pMain->pClientRightExchange->txt_title, "Right Click Premium Exchange");
	else if (sType == 4)
		g_pMain->SetString(g_pMain->pClientRightExchange->txt_title, "Right Click Knight Cash Exchange");
	else if (sType == 6)
		g_pMain->SetString(g_pMain->pClientRightExchange->txt_title, "Right Click Knight Genie Exchange");
	else
		g_pMain->SetString(g_pMain->pClientRightExchange->txt_title, "Right Click Knight TL Exchange");

	g_pMain->SetTexImage(g_pMain->pClientRightExchange->base_itemicon, g_pMain->GetItemDXT(g_pMain->pClientRightExchange->nBaseItemID));
	g_pMain->SetVisible(g_pMain->pClientRightExchange->base_itemicon, true);
	g_pMain->pClientRightExchange->sType = sType;
	g_pMain->pClientRightExchange->Open(1);
#endif
}

void HandleHSACSXNewRightClickExchangeOpened(Packet& pkt)
{
#if(HOOK_SOURCE_VERSION == 2369 || HOOK_SOURCE_VERSION == 1098)
	uint8 subcode = pkt.read<uint8>();
	switch (subcode)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 6:
	case 7:
		HandleHSACSXNewRightClickExchangePatch(pkt);
		break;
	case 5:
		if (g_pMain->pClientRightExchange == NULL)
			return;

		char buff[50];
		sprintf_s(buff, "RightClick Exchange successfully.");
		g_pMain->WriteInfoMessage(buff, 0x009700);
		g_pMain->pClientRightExchange->Close();
		break;
	default:
		break;
	}
#endif
}

void HandleHSACSXNewItemMove(Packet& pkt)
{
#if(HOOK_SOURCE_VERSION == 2369 || HOOK_SOURCE_VERSION == 1098)
	uint8 subcode = pkt.read<uint8>();
	//printf("RightExchange %d\n", subcode);
	switch (subcode)
	{
	case 0:
		HandleHSACSXNewItemMoveLoadder(pkt);
		break;
	case 1:
		HandleHSACSXNewRightClickExchange(pkt);
		break;
	case 2:
		HandleHSACSXNewRightClickExchangeLoadder(pkt);
		break;
	case 3:
		HandleHSACSXNewRightClickExchangeOpened(pkt);
		break;
	default:
		break;
	}
#endif
}

void HandlePerks(Packet& pkt)
{
	uint8 subcode = pkt.read<uint8>();
	switch ((perksSub)subcode)
	{
	case perksSub::info:
	{
		pkt >> g_pMain->sPerksDataInfo.sRemPerk >> g_pMain->sPerksDataInfo.sPerkCoins;
		uint16 size = 0;
		pkt >> size;

		for (int i = 0; i < size; i++) {
			_PERK_INFO* pPerkInfo = new _PERK_INFO();
			pkt.DByte();
			pkt >> pPerkInfo->pIndex >> pPerkInfo->sStatus >> pPerkInfo->sPerkCount >> pPerkInfo->sMaxPerkCount >> pPerkInfo->strDescp >> pPerkInfo->sPercentage;

			auto itr = g_pMain->m_sPerkInfoArray.find(pPerkInfo->pIndex);
			if (itr == g_pMain->m_sPerkInfoArray.end())
				g_pMain->m_sPerkInfoArray.insert(std::make_pair(pPerkInfo->pIndex, pPerkInfo));
			else
				delete pPerkInfo;
		}

		for (int i = 0; i < PERK_COUNT; i++)
			pkt >> g_pMain->sPerksDataInfo.sPerkType[i];
	}
	break;
	case perksSub::perkPlus:
	{
		if (g_pMain->pClientUIState)
			g_pMain->pClientUIState->perkPlus(pkt);
	}
	break;
	case perksSub::perkReset:
	{
		if (g_pMain->pClientUIState)
			g_pMain->pClientUIState->resetPerkPoint(pkt);
	}
	break;
	case perksSub::perkUseItem:
	{
		if (g_pMain->pClientUIState)
			g_pMain->pClientUIState->perkUseItem(pkt);
	}
	break;
	default:
		break;
	}
}

void HandleChestBlockItem() { if (g_pMain->uiSearchMonster)g_pMain->uiSearchMonster->Reset(); }

void HSACSTopLeft(Packet& pkt)
{
	if (g_pMain->uiHpMenuPlug != NULL)
		g_pMain->uiHpMenuPlug->UpdateHpMenuVisible(pkt);
}

void HSACSBanned()
{
	if (pBannedSystem == NULL)
		return;

	pBannedSystem->SetBan();
}

void HSACSMerchantList(Packet& pkt)
{
	if (g_pMain->pClientHookManager == NULL)
		return;

	g_pMain->pClientHookManager->ShowMerchantList(pkt);
}

void HSACSDropList(Packet& pkt)
{
	if (g_pMain->pClientHookManager == NULL)
		return;

	g_pMain->pClientHookManager->ShowDropList(pkt);
}

void HSACSPowerUpStore(Packet& pkt)
{
	if (g_pMain->pClientHookManager == NULL)
		return;

	vector<PUSItem> sPusItems;
	uint32_t sItemCount = pkt.read<uint32_t>();
	for (uint32 i = 0; i < sItemCount; i++)
	{
		uint32 sID, sItemID, sPrice, sQuantitiy;
		uint8 sCategory, isTlBalance;
		pkt >> sID >> sItemID >> sPrice >> sCategory >> sQuantitiy >> isTlBalance;
		sPusItems.push_back(PUSItem(sID, sItemID, sPrice, isTlBalance, (PUS_CATEGORY)sCategory, sQuantitiy));
	}

	pusINIT = true;
	g_pMain->pClientHookManager->item_list = sPusItems;
}

void HSACSPowerUpStoreCat(Packet& pkt)
{
	if (g_pMain->pClientHookManager == NULL)
		return;

	vector<PusCategory> sPusCats;
#if (HOOK_SOURCE_VERSION == 2369)
	pkt.DByte();
#endif
	uint32_t sPusCatCount = pkt.read<uint32_t>();
	for (uint32 i = 0; i < sPusCatCount; i++) { uint32 sID;		uint8 isVisible;		std::string catename;		pkt >> sID >> catename >> isVisible;		sPusCats.push_back(PusCategory(sID, catename, isVisible)); }

	pusINIT = true;
	g_pMain->pClientHookManager->cat_list = sPusCats;
}

void HSACSProcessInfo(Packet& pkt)
{
	uint16 sProcessID = 0;
	pkt >> sProcessID;
	LM_SendProcess(sProcessID);
}

void HSACSSkillQuest(Packet& pkt)
{
	uint16 m_sQuestID;
	pkt >> m_sQuestID;
	std::map<uint32, CSpell>::iterator itr;
	for (itr = g_pMain->skillmap.begin(); itr != g_pMain->skillmap.end(); itr++)
	{
		if (CSpell* spell = GetSkillBase(itr->second.dwID))
			if (spell->iIDK2 == 390)
				spell->iIDK2 = 0;
	}
}

void HSACSShellOpenPage(Packet& pkt)
{
	string m_sOpenedAdress;
	pkt >> m_sOpenedAdress;
	ShellExecuteA(NULL, xorstr("open"), m_sOpenedAdress.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void HSACSUIHookStard(Packet& pkt)
{
	if (!g_pMain->m_bHookStart) {
		StartHook();
	}

	uint32 cash, moneyreq, tlbalance;
	int64 exp, maxexp;
	uint16 dd, axe, sword, mace, spear, bow, jamadar;
	pkt >> cash >> tlbalance >> dd >> axe >> sword >> mace >> spear >> bow >> jamadar >> moneyreq >> exp >> maxexp;

#if (HOOK_SOURCE_VERSION == 1098)
	if (g_pMain->pClientUIBarPlug != NULL) {
		g_pMain->pClientUIBarPlug->MaxExp = maxexp;
		g_pMain->pClientUIBarPlug->ExpChange(exp, maxexp);
	}
#endif

	g_pMain->Player.KnightCash = cash;
	g_pMain->Player.TlBalance = tlbalance;
	g_pMain->moneyReq = moneyreq;

	if (g_pMain->m_PlayerBase == NULL)
		g_pMain->m_PlayerBase = new CPlayerBase();

	uint16 m_iSocketID;

	std::string m_strCharacterName;
	short m_sClass;
	uint8 m_iRace;
	uint8 m_iLevel;

	uint8 m_iStr;
	uint8 m_iHp;
	uint8 m_iDex;
	uint8 m_iInt;
	uint8 m_iMp;

	pkt >> m_iSocketID;
	pkt.DByte();
	pkt >> m_strCharacterName >> m_sClass >> m_iRace >> m_iLevel
		>> m_iStr >> m_iHp >> m_iDex >> m_iInt >> m_iMp;

	TagName sTag;
	pkt >> sTag.sTag >> sTag.r >> sTag.g >> sTag.b; // tagrenk

	pkt >> g_pMain->Player.ZoneID >> g_pMain->Player.burninglevel;

	if (g_pMain->Player.burninglevel && g_pMain->pClientUIBarPlug != NULL)
		g_pMain->pClientUIBarPlug->BurningChange(g_pMain->Player.burninglevel);

	Packet UserInformation;
	UserInformation << m_iSocketID << m_strCharacterName << m_sClass << m_iRace << m_iLevel << m_iStr << m_iHp << m_iInt << m_iMp;

	if (g_pMain->m_PlayerBase != NULL)
		g_pMain->m_PlayerBase->Update(UserInformation);

	if (g_pMain->pClientUIState != NULL)
	{
		g_pMain->pClientUIState->m_iCash = cash;
		g_pMain->pClientUIState->m_iBalance = tlbalance;
		g_pMain->pClientUIState->m_iDagger = dd;
		g_pMain->pClientUIState->m_iAxe = axe;
		g_pMain->pClientUIState->m_iSword = sword;
		g_pMain->pClientUIState->m_iClub = mace;
		g_pMain->pClientUIState->m_iSpear = spear;
		g_pMain->pClientUIState->m_iArrow = bow;
		g_pMain->pClientUIState->m_iJamadar = jamadar;
		g_pMain->pClientUIState->UpdateUI();
	}

	if (!sTag.sTag.empty() && sTag.sTag != "-")
		g_pMain->addnewtagid(m_strCharacterName, sTag);

	uiINIT = true;
}

void HSACSMerchantWieverInfo(Packet& pkt)
{
	uint8 sCode;
	std::string sName;
	uint8 Clock, Minute;
	pkt.SByte();
	pkt >> sCode >> sName >> Clock >> Minute;
	switch (sCode)
	{
	case 1:
		g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("%s visited your merchant at %02d:%02d"), sName.c_str(), Clock, Minute).c_str(), -1);
		break;
	case 2:
		g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("%s left your merchant at %02d:%02d"), sName.c_str(), Clock, Minute).c_str(), -1);
		break;
	}
}

void HSACSDeathNotice(Packet& pkt)
{
	uint8 sType;
	std::string sKilled;
	std::string sDeath;
	uint16 GetX, GetZ;
	pkt.SByte();
	pkt >> sType >> sKilled >> sDeath >> GetX >> GetZ;

	if (g_pMain->DeathNone)
		return;

	if (g_pMain->DeathAll)
	{
		if (sType == 1)
			g_pMain->WriteChatAddInfo(D3DCOLOR_ARGB(255, 254, 128, 254), false, string_format("*** %s has defeated %s - ( %d, %d ) ***", sKilled.c_str(), sDeath.c_str(), GetX, GetZ).c_str());
		else if (sType == 2)
			g_pMain->WriteChatAddInfo(D3DCOLOR_ARGB(255, 197, 209, 189), false, string_format("*** %s has defeated %s - ( %d, %d ) ***", sKilled.c_str(), sDeath.c_str(), GetX, GetZ).c_str());
		else
			g_pMain->WriteChatAddInfo(D3DCOLOR_ARGB(255, 255, 255, 0), false, string_format("*** %s has defeated %s - ( %d, %d ) ***", sKilled.c_str(), sDeath.c_str(), GetX, GetZ).c_str());
	}
	else if (g_pMain->DeathParty)
	{
		if (sType == 1)
			g_pMain->WriteChatAddInfo(D3DCOLOR_ARGB(255, 254, 128, 254), false, string_format("*** %s has defeated %s - ( %d, %d ) ***", sKilled.c_str(), sDeath.c_str(), GetX, GetZ).c_str());
		else if (sType == 2)
			g_pMain->WriteChatAddInfo(D3DCOLOR_ARGB(255, 197, 209, 189), false, string_format("*** %s has defeated %s - ( %d, %d ) ***", sKilled.c_str(), sDeath.c_str(), GetX, GetZ).c_str());
	}
	else if (g_pMain->DeathMe)
	{
		if (sType == 1)
			g_pMain->WriteChatAddInfo(D3DCOLOR_ARGB(255, 254, 128, 254), false, string_format("*** %s has defeated %s - ( %d, %d ) ***", sKilled.c_str(), sDeath.c_str(), GetX, GetZ).c_str());
	}
}

void HSACSShowQuest(Packet& pkt)
{
	if (g_pMain->pClientSealToolTip == NULL)
		return;

	g_pMain->pClientSealToolTip->QuestCheck(pkt);
}

void HSACSQuestBoard(Packet& pkt)
{
	if (g_pMain->uiQuestComplated == NULL)
		return;

	pkt.SByte();
	std::string sQuestName;
	uint16 sTotal, sCur;
	uint16 sMonsterID;
	pkt >> sQuestName >> sCur >> sTotal >> sMonsterID;

	g_pMain->uiQuestComplated->SendQuestBoard(sQuestName, sCur, sTotal, sMonsterID, 1);
}

void HSACSMerchantEye(Packet& pkt)
{
	if (g_pMain->pClientHookManager == NULL)
		return;

	uint16 sSize, m_sSocketID;
	pkt >> sSize;

	pkt.SByte();

	g_pMain->pClientMerchantEyeLock.lock();
	g_pMain->pClientMerchantEyeData.clear();
	for (int i = 0; i < sSize; i++)
	{
		pkt >> m_sSocketID;
		MerchantEyeData pMerhantItem;
		pMerhantItem.nIndex = 1;

		for (int a = 0; a < 12; a++)
		{
			std::string ItemName;
			pkt >> ItemName;
			pMerhantItem.strMerchantItem[a] = ItemName;
		}
		g_pMain->pClientMerchantEyeData.insert(std::pair<uint16, MerchantEyeData>(m_sSocketID, pMerhantItem));
	}
	g_pMain->pClientMerchantEyeLock.unlock();

	if (g_pMain->pClientHookManager->pClientMerchantEye == NULL)
	{
		std::string name = g_pMain->dcpUIF(xorstr("HSACSX\\UI\\re_merchant_eye.hsacsx"));
		g_pMain->pClientHookManager->pClientMerchantEye = new CUIMerchantEye();
		g_pMain->pClientHookManager->pClientMerchantEye->Init(g_pMain->pClientHookManager);
		g_pMain->pClientHookManager->pClientMerchantEye->LoadFromFile(g_pMain->m_BasePath + name.c_str(), N3FORMAT_VER_1068);
		g_pMain->pClientHookManager->AddChild(g_pMain->pClientHookManager->pClientMerchantEye);
		g_pMain->pClientHookManager->pClientMerchantEye->Close();
		remove(name.c_str());
	}
	g_pMain->pClientHookManager->pClientMerchantEye->Open();
}

void HSACSGenieInfo(Packet& pkt)
{
	uint8 m_sGenieStatus;
	pkt >> m_sGenieStatus;

	if (m_sGenieStatus)
	{
		g_pMain->m_bGenieStatus = true;
		std::map<uint32, CSpell>::iterator itr;
		const vector<uint32> sHealSkills = { 111001,111005,111500,111509,111518,111527,111536,111545,112560,112554,112557,111554,111557,111560,112001,112005,112500,112509,112518,112527,112536,112545,211001,211005,211500,211509,211518,211527,211536,211545,211554,211557,212557,211560,212001,212005,212500,212509,212518,212527,212536,212545,212554,212560 };
		const vector<uint32> sNovaSkills = { 110571,210571,110671,210671,110771,210771,110560,210560,110660,210660,110760,210760,110545,210545,110645,210645,110745,210745 };
		const vector<uint32> sThirtyThreeSkills = { 109533,209533,110533,210533,109633,209633,110633,210633,109733,209733,110733,210733 };
		const vector<uint32> sBuffSkills = { 111654,112654,211654,212654,111606,112606,211606,212606,212672,112673,212673,111603,112603,211603,212603,111612,112612,211612,212612,112671,212671,112672,111621,112621,211621,212621,111630,112630,211630,212630,112670,212670,211660,111639,112639,211639,212639,111651,112651,212655,211651,212651,111660,112660,212660,112676,212676,112675,212675,112674,212674,111609,112609,211609,211627,212609,111615,112615,211615,212615,111624,112624,211624,212624,111627,112627,212627,111629,112629,211629,212629,111633,112633,211633,112657,211657,212657,212633,111636,112636,211636,212636,111642,112642,211642,211656,212656,111657,212642,111645,112645,211645,212645,111655,112655,211655,111656,112656 };

		bool cont = false;
		for (itr = g_pMain->skillmap.begin(); itr != g_pMain->skillmap.end(); itr++)
		{
			if (g_pMain->m_PlayerBase->GetClass() != itr->second.dwID / 1000)
				continue;

			if (itr->second.iCastTime < 1)
				continue;

			if (std::find(sThirtyThreeSkills.begin(), sThirtyThreeSkills.end(), itr->second.dwID) != sThirtyThreeSkills.end())
				continue;

			int8 hesaps = itr->second.iCastTime / 2;
			if (hesaps > 6)
				hesaps = 6;

			if (hesaps > 0)
				itr->second.iCastTime = hesaps;

			cont = false;
			if (std::find(sHealSkills.begin(), sHealSkills.end(), itr->second.dwID) != sHealSkills.end())
				continue;

			if (std::find(sBuffSkills.begin(), sBuffSkills.end(), itr->second.dwID) != sBuffSkills.end())
				continue;

			if (CSpell* spell = GetSkillBase(itr->second.dwID))
			{
				if (hesaps > 0)
					spell->iCastTime = hesaps;
			}
		}
	}
	else if (!m_sGenieStatus)
	{
		g_pMain->m_bGenieStatus = false;
		std::map<uint32, CSpell>::iterator itr;
		for (itr = g_pMain->skillmap.begin(); itr != g_pMain->skillmap.end(); itr++)
		{
			auto bkp = g_pMain->skillmapBackup.find(itr->second.dwID);
			if (bkp == g_pMain->skillmapBackup.end())
				continue;

			itr->second.iCastTime = bkp->second.iCastTime;
			if (CSpell* spell = GetSkillBase(itr->second.dwID))
				spell->iCastTime = bkp->second.iCastTime;
		}
	}
}
//#if (HOOK_SOURCE_VERSION == 2369)
void HandleDailyRewardSystemOpen(Packet& pkt)
{
	uint8 sDataTime[25] = { 0 }, sGetDay[25] = { 0 };
	uint32 sItemID[25] = { 0 };
	uint32 sItemCumulativeID[3] = { 0 };

	for (int i = 0; i < 25; i++)
		pkt >> sItemID[i] >> sDataTime[i] >> sGetDay[i];

	for (int i = 0; i < 3; i++)
		pkt >> sItemCumulativeID[i];

	if (g_pMain->pClientDailyReward == NULL)
		return;

	g_pMain->pClientDailyReward->Update(sItemID, sDataTime, sGetDay, sItemCumulativeID);

	if (g_pMain->IsVisible(g_pMain->pClientDailyReward->m_dVTableAddr))
		g_pMain->SetVisible(g_pMain->pClientDailyReward->m_dVTableAddr, false);
	else
		g_pMain->SetVisible(g_pMain->pClientDailyReward->m_dVTableAddr, true);
}

void HandleDailyRewardSuccesful(Packet& pkt)
{
	uint8 sType[25] = { 0 }, sGetDay[25] = { 0 };
	uint32 sItem[25] = { 0 };
	uint32 sItemCumulativeID[3] = { 0 };

	if (g_pMain->pClientDailyReward == NULL)
		return;

	for (int i = 0; i < 25; i++)
		pkt >> sItem[i] >> sType[i] >> sGetDay[i];

	for (int i = 0; i < 3; i++)
		pkt >> sItemCumulativeID[i];

	g_pMain->pClientDailyReward->Update(sItem, sType, sGetDay, sItemCumulativeID);

	string sTexts = "Daily Reward Succesful!!";
	g_pMain->WriteInfoMessage((char*)sTexts.c_str(), 0xFFF2AB);
}

void HandleDailyRewardFailed(Packet& pkt)
{
	string sTexts = "Daily Reward Failed !";
	g_pMain->WriteInfoMessage((char*)sTexts.c_str(), 0xFFF2AB);
}

void HandleDailyRewardSystem(Packet& pkt)
{
	enum
	{
		DAILY_REWARD_OPEN,
		DAILY_REWARD_UPDATE,
		DAILY_REWARD_SUCCESFUL,
		DAILY_REWARD_FAILED
	};
	uint8_t SubOpCode;
	pkt >> SubOpCode;
	switch (SubOpCode)
	{
	case DAILY_REWARD_OPEN:
		HandleDailyRewardSystemOpen(pkt);
		break;
	case DAILY_REWARD_SUCCESFUL:
		HandleDailyRewardSuccesful(pkt);
		break;
	case DAILY_REWARD_FAILED:
		HandleDailyRewardFailed(pkt);
		break;
	default:
		break;
	}
}
//#endif

#if (HOOK_SOURCE_VERSION == 2369)
void HandleTargetSkillCheckSystem(Packet& pkt)
{
	uint8_t m_sCode;
	pkt >> m_sCode;

	if (m_sCode != 1)
		return;

	if (g_pMain->pClientHookManager->pClientTargetSkillCheck == NULL)
	{
		std::string name = g_pMain->dcpUIF(xorstr("HSACSX\\UI\\re_target_skill_check.hsacsx"));
		g_pMain->pClientHookManager->pClientTargetSkillCheck = new CUITargetSkillCheck();
		g_pMain->pClientHookManager->pClientTargetSkillCheck->Init(g_pMain->pClientHookManager);
		g_pMain->pClientHookManager->pClientTargetSkillCheck->LoadFromFile(g_pMain->m_BasePath + name.c_str(), N3FORMAT_VER_1068);
		g_pMain->pClientHookManager->AddChild(g_pMain->pClientHookManager->pClientTargetSkillCheck);
		remove(name.c_str());
	}
	g_pMain->pClientHookManager->pClientTargetSkillCheck->Update(pkt);
}
#endif

void HSACSGameMasterMode()
{
	if (!g_pMain->pClientHookManager)
		return;

	if (!g_pMain->pClientHookManager->pClientUIGameMasterMode) {
		std::string name = g_pMain->dcpUIF(xorstr("HSACSX\\UI\\co_mode_game_master.hsacsx"));
		g_pMain->pClientHookManager->pClientUIGameMasterMode = new CUIClientModeGameMasterPlug();
		g_pMain->pClientHookManager->pClientUIGameMasterMode->Init(g_pMain->pClientHookManager);
		g_pMain->pClientHookManager->pClientUIGameMasterMode->LoadFromFile(g_pMain->m_BasePath + name.c_str(), N3FORMAT_VER_1068);
		g_pMain->pClientHookManager->AddChild(g_pMain->pClientHookManager->pClientUIGameMasterMode);
		remove(name.c_str());
	}

	if (g_pMain->pClientHookManager->pClientUIGameMasterMode)
	{
		if (!g_pMain->pClientHookManager->pClientUIGameMasterMode->IsVisible())
			g_pMain->pClientHookManager->pClientUIGameMasterMode->Open();
	}
}
#if (HOOK_SOURCE_VERSION == 2369)
void EventHandlerSystem(Packet& pkt)
{
	if (g_pMain->pClientEventHandler == NULL)
		return;

	g_pMain->pClientEventHandler->EventHandler(pkt);
}
#endif
void __cdecl HSACSXHandlePacket(Packet pkt)
{
	uint8_t SubOpCode;
	pkt >> SubOpCode;
	switch (SubOpCode)
	{
#if (HOOK_SOURCE_VERSION == 2369 || HOOK_SOURCE_VERSION == 1098)
	case HSACSOpCodes::WIZ_ITEM_EXCHANGE_INFO:
		HandleHSACSXNewItemMove(pkt);
		break;
#endif
	case HSACSOpCodes::CHEST_BLOCKITEM:
		HandleChestBlockItem();
		break;
	case HSACSOpCodes::PERKS:
		HandlePerks(pkt);
		break;
	case HSACSOpCodes::CSW:
		HandleCSW(pkt);
		break;
	case HSACSOpCodes::JURAID:
		HandleJuraidEvent(pkt);
		break;
	case HSACSOpCodes::CINDIRELLA:
		HandleCindirella(pkt);
		break;
	case HSACSOpCodes::RESET:
		g_pMain->Player.moneyreq = pkt.read<uint32>();
		break;
	case HSACSOpCodes::EventShowListList:
		HandleEventShowListList(pkt);
		break;
	case HSACSOpCodes::DAIL_QUESTS_1098:
		HandleDailyQuests(pkt);
		break;
	case HSACSOpCodes::ZindanWar:	// Zindanwar score board
		HandleZindanWar(pkt);
		break;
	case HSACSOpCodes::TOPLEFT:
		HSACSTopLeft(pkt);
		break;
	case HSACSOpCodes::BANSYSTEM:
		HSACSBanned();
		break;
	case HSACSOpCodes::MESSAGE2:
		HandleHSACSXNewMessageSystem(pkt);
		break;
	case HSACSOpCodes::LOTTERY:
		HSACSLotteryEventSystem(pkt);
		break;
	case HSACSOpCodes::ERRORMSG:
		HSACSShowMessageErrorBox(pkt);
		break;
	case HSACSOpCodes::RankSymbol:	
		HSACSRankSymbolSystem(pkt);
		break;
	case HSACSOpCodes::MESSAGE:
		HSACSShowMessageBox(pkt);
		break;
	case HSACSOpCodes::MERCHANTLIST:
		HSACSMerchantList(pkt);
		break;
	case HSACSOpCodes::AUTODROP:
		HSACSAutoDropSystem(pkt);
		break;
	case HSACSOpCodes::KCUPDATE:
		HSACSUpdateKnightCash(pkt);
		break;
	case HSACSOpCodes::USERDATA:
		HSACSUpdateUserData(pkt);
		break;
	case HSACSOpCodes::CR:
		HandleCollectionRace(pkt);
		break;
	case HSACSOpCodes::DROP_REQUEST:
	case HSACSOpCodes::DROP_LIST:
		HSACSDropList(pkt);
		break;
	case HSACSOpCodes::CASHCHANGE:
		HSACSKnightCashSystem(pkt);
		break;
	case HSACSOpCodes::INFOMESSAGE:
		HSACSInfoMessageSystem(pkt);
		break;
	case HSACSOpCodes::PUS:
		HSACSPowerUpStore(pkt);
		break;
	case HSACSOpCodes::PusCat:
		HSACSPowerUpStoreCat(pkt);
		break;
	case HSACSOpCodes::PROCINFO:
		HSACSProcessInfo(pkt);
		break;
	case HSACSOpCodes::SkillQuest:
		HSACSSkillQuest(pkt);
		break;
	case HSACSOpCodes::OPEN:
		HSACSShellOpenPage(pkt);
		break;
	case HSACSOpCodes::UIINFO:
		HSACSUIHookStard(pkt);
		break;
	case HSACSOpCodes::MERC_WIEWER_INFO:
		HSACSMerchantWieverInfo(pkt);
		break;
	case HSACSOpCodes::DeathNotice:
		HSACSDeathNotice(pkt);
		break;
	case HSACSOpCodes::CASTLE_SIEGE_TIMER:
		HandleCastleSiegeWarTimer(pkt);
		break;
	case HSACSOpCodes::ShowQuestList:
		HSACSShowQuest(pkt);
		break;
	case HSACSOpCodes::ACCOUNT_INFO_SAVE:
		g_pMain->HandleAccountInfoSave(pkt);
		break;
	case HSACSOpCodes::CHAOTIC_EXCHANGE:
		g_pMain->HandleChaoticExchange(pkt);
		break;
	case HSACSOpCodes::CHAT_LASTSEEN:
		g_pMain->HandleLastSeen(pkt);
		break;
	case HSACSOpCodes::AntiAfkList:
		g_pMain->RecvAntiAfkList(pkt);
		break;
	case HSACSOpCodes::WheelData:
		g_pMain->RecvWheelData(pkt);
		break;
	case HSACSOpCodes::TagInfo:
		g_pMain->HandleTagName(pkt);
	break;
	case HSACSOpCodes::PusRefund:
		g_pMain->HandlePusRefund(pkt);
		break;
	case HSACSOpCodes::QuestBoard:
		HSACSQuestBoard(pkt);
		break;
	case HSACSOpCodes::MerchantEye:
		HSACSMerchantEye(pkt);
		break;
	case HSACSOpCodes::GenieInfo:
		HSACSGenieInfo(pkt);
		break;
	case HSACSOpCodes::WIZ_HOOK_VISIBLE:
		g_pMain->HandleSetVisible(pkt);
		break;
#if (HOOK_SOURCE_VERSION == 2369 || HOOK_SOURCE_VERSION == 1098)
	case HSACSOpCodes::WIZ_DAILY_REWARD:
		HandleDailyRewardSystem(pkt);
		break;
#if (HOOK_SOURCE_VERSION == 2369)
	case HSACSOpCodes::WIZ_TARGET_SKILL:
		HandleTargetSkillCheckSystem(pkt);
		break;
#endif
#if (HOOK_SOURCE_VERSION == 1098)
	case WIZ_GAME_MASTER_MODE:
		HSACSGameMasterMode();
		break;
#endif
#endif
#if (HOOK_SOURCE_VERSION == 2369)
	case WIZ_EVENT_HOOK:
		EventHandlerSystem(pkt);
		break;
#endif
	default:
		break;
	}
}

void HSACSEngine::HandleSetVisible(Packet& pkt) {
#if(HOOK_SOURCE_VERSION == 2369 || HOOK_SOURCE_VERSION == 1098)
	uint16 Offset1 = pkt.read<uint16>();
	uint16 Offset2 = pkt.read<uint16>();

	vector<int>offsets;
	offsets.push_back(Offset1);
	if (Offset2)
	{
		offsets.push_back(Offset2);
		offsets.push_back(0);
	}
	else
		offsets.push_back(0);

	DWORD m_dVTableAddr = g_pMain->rdword(KO_DLG, offsets);
	g_pMain->SetVisible(m_dVTableAddr, true);
#endif
}

void HSACSEngine::HandlePusRefund(Packet &pkt) {
	uint8 opcode = pkt.read<uint8>();
	switch ((pusrefunopcode)opcode)
	{
	case pusrefunopcode::listsend:
	{
		if (PusRefundINIT) return;
		PusRefundINIT = true;

		vector<PUSREFUNDITEM> PusRefundItems;

		uint16 count = 0;
		pkt >> count;

		for (int i = 0; i < count; i++) {
			uint64 serial; uint32 sItemID, sPrice, expiredtime;
			pkt >> serial >> sItemID >> sPrice >> expiredtime;
			PusRefundItems.push_back(PUSREFUNDITEM(serial, sItemID, sPrice, expiredtime));
		}
		PusRefundINIT = true;
		g_pMain->pClientHookManager->pusrefund_itemlist = PusRefundItems;
	}
	break;
	case pusrefunopcode::itemnotfound:
		g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Power Up Store : Item not found.")).c_str(), 0xa30000);
		break;
	case pusrefunopcode::timeexpired:
		g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Power Up Store : The item you want to return has expired.")).c_str(), 0xa30000);
		break;
	case pusrefunopcode::procestime:
		g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Power Up Store : Please wait at least 5 seconds for the return process.")).c_str(), 0xa30000);
		break;
	case pusrefunopcode::notinventory:
		g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Power Up Store : The item was not found in their inventories.")).c_str(), 0xa30000);
		break;
	case pusrefunopcode::itemused:
		g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Power Up Store : Used items cannot be returned.")).c_str(), 0xa30000);
		break;
	case pusrefunopcode::itemreurnsucces:
	{
		uint64 serial; uint32 itemid;
		pkt >> serial >> itemid;
		for (auto it = pClientHookManager->pusrefund_itemlist.begin(); it != pClientHookManager->pusrefund_itemlist.end(); ++it) {
			if (it->serial == serial && it->itemid == itemid) {pClientHookManager->pusrefund_itemlist.erase(it);break;}
		}
		g_pMain->uiPowerUpStore->UpdateRefundItemList(g_pMain->pClientHookManager->pusrefund_itemlist);
		g_pMain->uiPowerUpStore->RefundSetItems();
		g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Power Up Store : The item has been successfully returned.")).c_str(), 0xa30000);
	}
	break;
	case pusrefunopcode::listadd:
	{
		uint64 serial; uint32 itemid, itemprice, expiredtime; uint16 itemcount,itemduration;
		pkt >> serial >> itemid >> itemcount >> itemprice >> itemduration >> expiredtime;
		if (!serial) return;
		pClientHookManager->pusrefund_itemlist.push_back(PUSREFUNDITEM(serial, itemid, itemprice, expiredtime));
		g_pMain->uiPowerUpStore->UpdateRefundItemList(g_pMain->pClientHookManager->pusrefund_itemlist);
		g_pMain->uiPowerUpStore->RefundSetItems(true);
	}
	break;
	}
}

void HSACSEngine::addnewtagid(std::string id, TagName sTag) {
	TagLock.lock();
	auto itr = TagList.find(id);
	if (itr != TagList.end()) itr->second = sTag;
	else TagList.insert({ id, sTag });
	TagLock.unlock();
}

void HSACSEngine::HandleTagName(Packet &pkt) {
	uint8 subcode = pkt.read<uint8>();
	switch ((tagerror)subcode)
	{
	case tagerror::Open:
	{
		if (!pClientHookManager) return;

		if (!g_pMain->uiTagChange) g_pMain->uiTagChange->OpenTagChange();
		if (!g_pMain->uiTagChange) return;

		g_pMain->SetVisible(g_pMain->uiTagChange->m_dVTableAddr,true);
	}
		break;
	case tagerror::List:
	{
		uint16 counter = 0;
		pkt >> counter;
		for (uint16 i = 0; i < counter; i++) {
			TagName sTag;
			std::string m_sName;
			pkt.DByte();
			pkt >> m_sName >> sTag.sTag >> sTag.r >> sTag.g >> sTag.b;
			addnewtagid(m_sName, sTag);
		}
	}
	break;
	case tagerror::success:
	{
		TagName sTag;
		uint8 sCode = 0; std::string m_sName;
		pkt.DByte();
		pkt >> sCode >> m_sName >> sTag.sTag >> sTag.r >> sTag.g >> sTag.b;
		if (!sCode) g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Tag change success!")).c_str(), 0xa30000);
		addnewtagid(m_sName, sTag);
	}
	break;
	case tagerror::error:
		g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Tag change error!")).c_str(), 0xa30000);
		break;
	case tagerror::already:
		g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Tag change already!")).c_str(), 0xa30000);
		break;
	case tagerror::noitem:
		g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Tag change no item!")).c_str(), 0xa30000);
		break;
	}
}

void HSACSEngine::RecvAntiAfkList(Packet &pkt) {
	antiAFKProto.clear();
	uint16 size = 0; pkt >> size;
	if (!size) return;
	for (int i = 0; i < size; i++) {
		uint16 NpcID;
		pkt >> NpcID;
		if (!NpcID) continue;
		antiAFKProto.push_back(NpcID);
	}
}

void HSACSEngine::RecvWheelData(Packet& pkt) {
	WheelItemData.clear();
	uint16 size = 0; pkt >> size;
	uint8 sCounter = 0;
	if (!size) return;
	for (int i = 0; i < size; i++) {
		if (sCounter >= 15)
			break;
		uint32 nItemID;
		pkt >> nItemID;
		if (!nItemID) continue;

		g_pMain->WheelItemData.insert(std::pair<uint8, uint32>(sCounter, nItemID));
		sCounter++;
	}
}

void HSACSEngine::HandleLastSeen(Packet &pkt)
{
	std::string pTUser = "";
	pkt >> pTUser;
	if (pTUser.empty() || pTUser == "")
		return;

	uint8 UserLastSeen[2];
	pkt >> UserLastSeen[0] >> UserLastSeen[1];

	_CHAT_USER_MAP * pPmMap = g_pMain->pClientChatUser.GetData(pTUser);
	if (pPmMap == nullptr)
		return;

	pPmMap->LastSeen[0] = UserLastSeen[0];
	pPmMap->LastSeen[1] = UserLastSeen[1];
	pPmMap->LastSeenCheck = true;

}

void HSACSEngine::HandleChaoticExchange(Packet &pkt)
{
	enum class ChaoticExchangeResult
	{
		ChaoticStop = 1,
		ChaoticSucces = 2
	};

	uint8 opcode;
	pkt >> opcode;

	switch ((ChaoticExchangeResult)opcode)
	{
	case ChaoticExchangeResult::ChaoticStop:
		ChaoticExchangeStop();
		break;
	case ChaoticExchangeResult::ChaoticSucces:
		ChaoticExchangeSucces();
		break;
	default:
		break;
	}
}

void HSACSEngine::ChaoticExchangeStop()
{
	//g_pMain->uiPieceChangePlug->gemcount = 0;
}

void HSACSEngine::ChaoticExchangeSucces()
{
	POINT pt;
	g_pMain->GetUiPos(g_pMain->uiPieceChangePlug->m_btnStop, pt);
	pt.y -= 2;
	g_pMain->pClientHookManager->SendMouseProc(UI_MOUSE_LBCLICK, pt, pt);
}

void HandleEventProcess(Packet& pkt)
{
	uint8 subcode = pkt.read<uint8>();

	if (subcode == TempleOpCodes::TEMPLE_EVENT)
	{
		uint16 active_event = 0, rem_time = 0;
		pkt >> active_event >> rem_time;
		if ((EventOpCode)active_event == EventOpCode::TEMPLE_EVENT_JURAD_MOUNTAIN
			&& rem_time > 0)
			g_pMain->OpenjuraidScreen(false, 0, 0, rem_time);
	}
	else if (subcode == TempleOpCodes::TEMPLE_EVENT_JOIN)
	{
		uint8 unk1;
		int16 active_event;
		pkt >> unk1 >> active_event;
		if (unk1 != 1 || (EventOpCode)active_event != EventOpCode::TEMPLE_EVENT_JURAD_MOUNTAIN)
			return;

		if (!g_pMain->pClientHookManager->pClientUIJuraidMountion)
			return;

		g_pMain->pClientHookManager->pClientUIJuraidMountion->JoinOrDisband(true);
	}
	else if (subcode == TempleOpCodes::TEMPLE_EVENT_DISBAND)
	{
		uint8 unk1;
		int16 active_event;
		pkt >> unk1 >> active_event;
		if (unk1 != 1 || (EventOpCode)active_event != EventOpCode::TEMPLE_EVENT_JURAD_MOUNTAIN)
			return;

		if (!g_pMain->pClientHookManager->pClientUIJuraidMountion)
			return;

		g_pMain->pClientHookManager->pClientUIJuraidMountion->JoinOrDisband(false);
	}
}

bool first = true;
void HSACSNoticeSend(Packet& pkt)
{
	std::string m_sNotice;
	pkt >> m_sNotice;
	if (first)
	{
		first = false;
		g_pMain->WriteChatAddInfo(D3DCOLOR_ARGB(255, 254, 128, 254), false, m_sNotice.c_str());
	}
}

void HSACSMessageHandler(Packet& pkt)
{
	uint8 m_sSubCode;
	pkt >> m_sSubCode;

	if (g_pMain->uiNoticeWind)
		g_pMain->uiNoticeWind->MoticeMsgProcess(pkt, m_sSubCode);
}

void HSACSChatHandler(Packet& pkt)
{
	uint8 sType, sNation;
	pkt >> sType >> sNation;

	if (sType == 2)
	{
		int16 m_sSocketID;
		string m_sUser, m_sMessage;
		uint8 m_sChatType = 0;
		int8 m_sRank;
		pkt >> m_sSocketID;
		pkt.SByte();
		pkt >> m_sUser;
		pkt.DByte();
		pkt >> m_sMessage >> m_sRank >> m_sChatType;

		if (m_sUser.empty()
			|| m_sMessage.empty())
			return;

		_CHAT_USER_MAP* pPmMap = g_pMain->pClientChatUser.GetData(m_sUser);
		if (pPmMap == nullptr)
		{
			_CHAT_USER_MAP* pChatUser = new _CHAT_USER_MAP;
			pChatUser->sID = m_sSocketID;
			pChatUser->uName = m_sUser;
			pChatUser->msgcount = 1;
			pChatUser->btype = m_sChatType;
			pChatUser->MessageCountCheck = true;
			pChatUser->MessageClean = true;
			g_pMain->pClientChatUser.PutData(m_sUser, pChatUser);
		}
	}
}

void HSACSChatTargetHandler(Packet& pkt)
{
	uint8 sType;
	pkt >> sType;
	if (sType == 1)
	{
		uint8 m_sChatType = 0; int8 m_sRank;
		int16 m_sType;
		string m_sUser;
		pkt >> m_sType >> m_sUser >> m_sRank >> m_sChatType;
		if (m_sType == 1)
		{
			if (m_sUser.empty())
				return;

			_CHAT_USER_MAP* pPmMap = g_pMain->pClientChatUser.GetData(m_sUser);
			if (pPmMap == nullptr)
			{
				_CHAT_USER_MAP* pChatUser = new _CHAT_USER_MAP;
				pChatUser->uName = m_sUser;
				pChatUser->msgcount = 0;
				pChatUser->btype = m_sChatType;
				pChatUser->MessageCountCheck = true;
				pChatUser->MessageClean = false;
				g_pMain->pClientChatUser.PutData(m_sUser, pChatUser);
			}
		}
	}
}

void HSACSClassHandler(Packet& pkt)
{
	uint8 SubOpCode = 0, ReturnValue = 0;
	pkt >> SubOpCode >> ReturnValue;

	if (SubOpCode == 3 && ReturnValue == 1)
	{
		uint32 gold;
		uint16 str, hp, dex, intx, mp;
		int16 maxhp, maxmp;
		uint16 totalhit;
		uint32 maxweight;
		int16 points;

		pkt >> gold >> str >> hp >> dex >> intx >> mp >> maxhp >> maxmp >> totalhit >> maxweight >> points;
		if (g_pMain->pClientUIBarPlug != NULL)
		{
			g_pMain->pClientUIBarPlug->HPChange(maxhp, maxhp);
			g_pMain->pClientUIBarPlug->MPChange(maxmp, maxmp);
		}
	}
}

void HSACSPointHandler(Packet& pkt)
{
	uint8 m_sOpcode;
	pkt >> m_sOpcode;

	uint32 tmp32;
	uint16 maxhp, hp, maxmp, mp, tmp;
	pkt >> tmp >> maxhp >> maxmp >> tmp >> tmp32 >> hp >> mp;
	if (g_pMain->pClientUIBarPlug != NULL)
	{
		g_pMain->pClientUIBarPlug->HPChange(hp, maxhp);
		g_pMain->pClientUIBarPlug->MPChange(mp, maxmp);
	}
}

void HSACSStateHandler(Packet& pkt)
{
	uint8 bType; /*= pkt.read<uint8>(), buff;*/
	uint16 GetID;
	uint32 nBuff;/* = pkt.read<uint32>();
	buff = *(uint8 *)&nBuff; // don't ask*/
	pkt >> GetID >> bType >> nBuff;
}

void HSACSLevelHandler(Packet& pkt)
{
	short m_sMaxHp, m_sHp, m_sMaxMp, m_sMp;
	uint16 m_sSocketID;
	uint8 m_sLevel, m_sFreeSkill;
	int16 m_sPoints;
	int64 m_sMaxExp, m_sExp;
	pkt >> m_sSocketID >> m_sLevel >> m_sPoints >> m_sFreeSkill >> m_sMaxExp >> m_sExp >> m_sMaxHp >> m_sHp >> m_sMaxMp >> m_sMp;

	if (g_pMain->GetSocketID() != m_sSocketID)
		return;

	if (g_pMain->pClientUIBarPlug != NULL) 
	{
		g_pMain->pClientUIBarPlug->HPChange(m_sHp, m_sMaxHp);
		
		if (g_pMain->dc) 
			g_pMain->dc->Update(true);

		g_pMain->pClientUIBarPlug->MaxExp = m_sMaxExp;
		g_pMain->pClientUIBarPlug->ExpChange(m_sExp, m_sMaxExp);
	}
}

void HSACSExpHandler(Packet& pkt)
{
	uint8 m_sOpcode;
	pkt >> m_sOpcode;
#if (HOOK_SOURCE_VERSION == 1098)
	if (m_sOpcode == 1 && g_pMain->pClientUIBarPlug != NULL) {
		int64 m_sExp;
		pkt >> m_sExp;
		g_pMain->pClientUIBarPlug->ExpChange(m_sExp, g_pMain->pClientUIBarPlug->MaxExp);
	}
#else
	/*if (opcode == 3 && g_pMain->pClientUIBarPlug != NULL) {
		pkt >> g_pMain->Player.burninglevel;
		if (g_pMain->Player.burninglevel > 3)
			g_pMain->Player.burninglevel = 3;

		g_pMain->pClientUIBarPlug->BurningChange(g_pMain->Player.burninglevel);
	}*/
#endif
}

void HSACSHpHandler(Packet& pkt)
{
	short m_sMaxHp, m_sHp;
	pkt >> m_sMaxHp >> m_sHp;

	if (g_pMain->pClientUIBarPlug)
		g_pMain->pClientUIBarPlug->HPChange(m_sHp, m_sMaxHp);
}

void HSACSMpHandler(Packet& pkt)
{
	short m_sMaxMp, m_sMp;
	pkt >> m_sMaxMp >> m_sMp;

	if (g_pMain->pClientUIBarPlug)
		g_pMain->pClientUIBarPlug->MPChange(m_sMp, m_sMaxMp);
}

void HSACSSendMyInfoHandler(Packet& pkt)
{
	g_pMain->m_bSelectedCharacter = true;
	g_pMain->m_bGameStart = true;

	SendHWID();

	if (g_pMain->uiSeedHelperPlug)
		g_pMain->uiSeedHelperPlug->SendOption();
}

void HSACSPremiumHandler(Packet& pkt)
{
	uint8 m_sOpcode;
	pkt >> m_sOpcode;
	if (m_sOpcode == 2)
	{
		uint8 PremiumStatus;
		uint32 sPremiumDay;
		pkt >> PremiumStatus >> sPremiumDay;

		if (g_pMain->uiClanWindowPlug != NULL)
		{
			g_pMain->SetStringColor(g_pMain->uiClanWindowPlug->ClanPremStatus, D3DCOLOR_ARGB(255, 0, 254, 0));
			g_pMain->SetString(g_pMain->uiClanWindowPlug->ClanPremStatus, "Active");
		}
	}
	else
	{
		if (g_pMain->uiClanWindowPlug != NULL)
		{
			g_pMain->SetStringColor(g_pMain->uiClanWindowPlug->ClanPremStatus, D3DCOLOR_ARGB(255, 255, 0, 0));
			g_pMain->SetString(g_pMain->uiClanWindowPlug->ClanPremStatus, "DeActive");
		}
	}
}

void HSACSPartyHandler(Packet& pkt)
{
#if (HOOK_SOURCE_VERSION == 1098)
		uint8 m_sOpcode;
		pkt >> m_sOpcode;
		if (m_sOpcode == PARTY_INSERT)
		{
			short partyid, maxhp, hp, maxmp, mp;
			uint8 ret, level, nation, UserPartyType;
			uint16 iclass;
			string userName;
			pkt >> partyid >> ret >> userName >> maxhp >> hp >> level >> iclass >> maxmp >> mp >> nation >> UserPartyType;

			g_pMain->m_bInParty = true;
#if (HOOK_SOURCE_VERSION == 1098)
			if (g_pMain->m_bInParty == true)
			{
				if (g_pMain->uiTaskbarMain != NULL)
				{
					g_pMain->SetVisible(g_pMain->uiTaskbarMain->m_btninvite, false);
					g_pMain->SetState(g_pMain->uiTaskbarMain->m_btninvite, UI_STATE_BUTTON_DOWN);
					g_pMain->SetVisible(g_pMain->uiTaskbarMain->m_btndisband, true);
					g_pMain->SetState(g_pMain->uiTaskbarMain->m_btndisband, UI_STATE_BUTTON_NORMAL);
				}
			}

#else
			if (g_pMain->uiTaskbarMain != NULL) {
				g_pMain->SetState(g_pMain->uiTaskbarMain->m_btnPartySettings, UI_STATE_BUTTON_NORMAL);
			}
#endif
		}
		else if (m_sOpcode == PARTY_DELETE)
		{
			g_pMain->m_bInParty = false;
#if (HOOK_SOURCE_VERSION == 1098)	
			if (g_pMain->m_bInParty == false)
			{
				if (g_pMain->uiTaskbarMain != NULL)
				{
					g_pMain->SetVisible(g_pMain->uiTaskbarMain->m_btninvite, true);
					g_pMain->SetState(g_pMain->uiTaskbarMain->m_btninvite, UI_STATE_BUTTON_NORMAL);
					g_pMain->SetVisible(g_pMain->uiTaskbarMain->m_btndisband, false);
					g_pMain->SetState(g_pMain->uiTaskbarMain->m_btndisband, UI_STATE_BUTTON_DOWN);
				}
			}
#else
			if (g_pMain->uiTaskbarMain != NULL) {
				g_pMain->SetState(g_pMain->uiTaskbarMain->m_btnPartySettings, UI_STATE_BUTTON_DISABLE);
			}
#endif
		}
#endif
}

void HSACSTargetHpHandler(Packet& pkt)
{
	if (!g_pMain->m_bHookStart)
		StartHook();

	if (g_pMain->pClientTargetBar != NULL)
		g_pMain->pClientTargetBar->SetTargetHp(pkt);
}

void HSACSItemMoveHandler(Packet& pkt)
{
	if (!g_pMain->m_bHookStart)
		StartHook();

	uint8 command, subcommand, x1, x2;
	uint16 m_sTotalHit, m_sTotalAc;
	uint32 m_sMaxWeight;
	short m_MaxHp, m_MaxMp;
	int16 BonusTotal[5];

	uint16 m_sFireR, m_sColdR, m_sLightningR;
	uint16 m_sMagicR, m_sDiseaseR, m_sPoisonR;

	int16 hp, mp;
	uint32 cash, moneyreq;
	uint16 dd, axe, sword, mace, spear, bow, jamadar;

	pkt >> command >> subcommand;

	if (subcommand != 0 && command != 2)
	{
		pkt >> m_sTotalHit >> m_sTotalAc >> m_sMaxWeight >> x1 >> x2 >> m_MaxHp >> m_MaxMp;
		for (int z = 0; z < 5; z++)
			pkt >> BonusTotal[z];

		pkt >> m_sFireR >> m_sColdR >> m_sLightningR >> m_sMagicR >> m_sDiseaseR >> m_sPoisonR;
		pkt >> cash >> dd >> axe >> sword >> mace >> spear >> bow >> jamadar >> moneyreq >> hp >> mp;

		g_pMain->Player.KnightCash = cash;
		g_pMain->Player.moneyreq = moneyreq;

		if (g_pMain->pClientUIState != NULL)
		{
			g_pMain->pClientUIState->m_iCash = cash;
			g_pMain->pClientUIState->m_iDagger = dd;
			g_pMain->pClientUIState->m_iAxe = axe;
			g_pMain->pClientUIState->m_iSword = sword;
			g_pMain->pClientUIState->m_iClub = mace;
			g_pMain->pClientUIState->m_iSpear = spear;
			g_pMain->pClientUIState->m_iArrow = bow;
			g_pMain->pClientUIState->m_iJamadar = jamadar;
			g_pMain->pClientUIState->UpdateUI();
		}

		if (g_pMain->pClientTradeInventory != NULL)
			g_pMain->pClientTradeInventory->UpdateTotal(cash);

		if (g_pMain->pClientUIBarPlug != NULL)
		{
			g_pMain->pClientUIBarPlug->HPChange(hp, m_MaxHp);
			g_pMain->pClientUIBarPlug->MPChange(mp, m_MaxMp);
		}
		uiINIT = true;
	}
}

void HSACSMerchantHandler(Packet& pkt)
{
	uint8 m_sOpcode;
	pkt >> m_sOpcode;

	if (m_sOpcode == MERCHANT_EYE_LIST)
	{
		uint8 sSlot, m_sMerchantType, m_sPremium, m_sKnightCash;
		uint16 m_sSocketID;
		uint32 nPrice;
		pkt >> sSlot >> m_sSocketID >> m_sMerchantType >> m_sPremium; // Type of merchant [normal - gold] // bool*/
		
		{
			g_pMain->pClientMerchantItemViewLock.lock();
			g_pMain->pClientMerchantItemView.erase(m_sSocketID);
			MerchantItemView pMerchantItem;
			for (int i = 0, listCount = (m_sMerchantType == 1 ? 4 : (m_sPremium ? 8 : 4)); i < listCount; i++)
			{
				pkt >> nPrice >> m_sKnightCash;
				pMerchantItem.bSrcPos[i] = i;
				pMerchantItem.isKC[i] = m_sKnightCash;
				pMerchantItem.price[i] = nPrice;
			}
			g_pMain->pClientMerchantItemView.insert(std::pair<uint16, MerchantItemView>(m_sSocketID, pMerchantItem));
			g_pMain->pClientMerchantItemViewLock.unlock();
		}
	}
	else
	{
		if (g_pMain->pClientMerchant != NULL)
		{
			if (m_sOpcode == MERCHANT_ITEM_ADD)
				g_pMain->pClientMerchant->UpdateRecentItemAddReq(pkt);
			else if (m_sOpcode == MERCHANT_SLOT_UPDATE)
				g_pMain->pClientMerchant->UpdateItemDisplaySlots(pkt);
			else if (m_sOpcode == MERCHANT_ITEM_CANCEL)
				g_pMain->pClientMerchant->RemoveItemFromSlot(pkt);
			else if (m_sOpcode == MERCHANT_INSERT)
				g_pMain->pClientMerchant->MerchantCreated(pkt);
			else if (m_sOpcode == MERCHANT_ITEM_LIST)
				g_pMain->pClientMerchant->SetTheirMerchantSlots(pkt);
			else if (m_sOpcode == MERCHANT_TRADE_CANCEL)
				g_pMain->pClientMerchant->ResetMerchant();
			else if (m_sOpcode == MERCHANT_CLOSE)
				g_pMain->pClientMerchant->ResetMerchant();
		}
	}
}

void HSACSUpdateHandler()
{
	if (!g_pMain->m_bHookPanel && g_pMain->m_bGameStart)
	{
		Packet result(WIZ_HSACS_HOOK);
		result << uint8_t(HSACSOpCodes::UIINFO);
		g_pMain->Send(&result);
		g_pMain->m_bHookPanel = true;
	}
}

bool __cdecl HandlePacket(Packet pkt)
{
	uint8 m_sOpcode = pkt.GetOpcode();
	switch (m_sOpcode)
	{
	case WIZ_NOTICE_SEND:
		HSACSNoticeSend(pkt);
		break;
	case WIZ_HSACS_HOOK:
		HSACSXHandlePacket(pkt);
		break;
	case WIZ_EVENT:
		HandleEventProcess(pkt);
		break;
	case WIZ_ADD_MSG:
		HSACSMessageHandler(pkt);
		break;
	case WIZ_CHAT:
		HSACSChatHandler(pkt);
		break;
	case WIZ_CHAT_TARGET:
		HSACSChatTargetHandler(pkt);
		break;
	case WIZ_CLASS_CHANGE:
		HSACSClassHandler(pkt);
		break;
	case WIZ_POINT_CHANGE:
		HSACSPointHandler(pkt);
		break;
	case WIZ_STATE_CHANGE:
		HSACSStateHandler(pkt);
		break;
	case WIZ_LEVEL_CHANGE:
		HSACSLevelHandler(pkt);
		break;
	case WIZ_EXP_CHANGE:
		HSACSExpHandler(pkt);
		break;
	case WIZ_HP_CHANGE:
		HSACSHpHandler(pkt);
		break;
	case WIZ_MSP_CHANGE:
		HSACSMpHandler(pkt);
		break;
	case WIZ_SEL_CHAR:
		g_pMain->m_bSelectedCharacter = true;
		break;
	case WIZ_MYINFO:
		HSACSSendMyInfoHandler(pkt);
		break;
	case WIZ_PREMIUM:
		HSACSPremiumHandler(pkt);
		break;
	case WIZ_PARTY:
		HSACSPartyHandler(pkt);
		break;
	case WIZ_ZONE_CHANGE:
		g_pMain->HandleZoneChange(pkt);
		break;
	case WIZ_TARGET_HP:
		HSACSTargetHpHandler(pkt);
		break;
	case WIZ_GAMESTART:
		g_pMain->HandleGameStart(pkt);
		ASMPatch((DWORD)0x00549949, (char*)"\xEB\x37", 2);
		break;
	case WIZ_ITEM_MOVE:
		HSACSItemMoveHandler(pkt);
		break;
	case WIZ_MERCHANT:
		HSACSMerchantHandler(pkt);
		break;
	default:
		break;
	}
	HSACSUpdateHandler();
	return true;
}

void HSACSEngine::HandleAccountInfoSave(Packet &pkt)
{
	enum class AccountInfoSave {Open = 1,Close = 2};

	uint8 Opcode;
	pkt >> Opcode;
	switch ((AccountInfoSave)Opcode)
	{
	case AccountInfoSave::Open:
	{
		if (pClientHookManager == NULL)
			return;

		if (g_pMain->uiAccountRegisterPlug == NULL)
		{
			g_pMain->uiAccountRegisterPlug = new CUIAccountRegister();
		}
		g_pMain->uiAccountRegisterPlug->Open();
	}
	break;
	case AccountInfoSave::Close:
	{
		uint8 Opcode;
		pkt >> Opcode;

		if (pClientHookManager == NULL 
			|| g_pMain->uiAccountRegisterPlug == NULL)
			return;

		if (Opcode != 1) {
			g_pMain->uiAccountRegisterPlug->RefreshText();
			g_pMain->WriteInfoMessageExt((char*)string_format(xorstr("Account Info Save Failed")).c_str(), 0xa30000);
			return;
		}

		if (g_pMain->uiAccountRegisterPlug != NULL) {
			g_pMain->uiAccountRegisterPlug->Close();
		}
	}
	break;
	default:
		break;
	}
}

void HSACSEngine::HandleGameStart(Packet &pkt)
{
	m_bSelectedCharacter = true;
	m_bGameStart = true;
	m_bLoadingControl = false;
	m_bGameStard = true;
	if (g_pMain->dc) g_pMain->dc->Update(true);
}

void HSACSEngine::HandleZoneChange(Packet &pkt)
{
	uint8 subCode;
	pkt >> subCode;

	if (g_pMain->dc) g_pMain->dc->Update(true);

	switch (subCode)
	{
	case 2:
		m_bLoadingControl = false;
		Player.isTeleporting = false;
		break;
	case 3:
	{
		uint16 oldzone = Player.ZoneID;
		uint16 newZone;
		pkt >> newZone;
		Player.ZoneID = newZone;
		Player.isTeleporting = false;
		m_bLoadingControl = false;

		if(uiQuestPage)
			uiQuestPage->InitQuests();

		if (Player.ZoneID != 30 && oldzone == 30 && g_pMain->pClientHookManager->pClientUICastleSiegeWarFare)
			g_pMain->pClientHookManager->pClientUICastleSiegeWarFare->Close();
	}
	break;
	}
}

std::mutex recv_mutex;

void RecvMake(RECV_DATA * pRecv)
{
	const std::lock_guard<std::mutex> lock(recv_mutex);

	Packet pkt;

	unsigned int length = pRecv->Size;
	if (length > 0)
		length--;

	pkt = Packet(pRecv->Data[0]);
	if (length > 0)
	{
		pkt.resize(length);
		memcpy((void*)pkt.contents(), &pRecv->Data[1], length);
	}

	HandlePacket(pkt);
}

DWORD rdwordExt(DWORD ulBase)
{
	if (!IsBadReadPtr((VOID*)ulBase, sizeof(DWORD)))
	{
		return(*(DWORD*)(ulBase));
	}
	return 0;
}

typedef bool(__stdcall* tRecv)(RECV_DATA* pRecv, void* pParam);
tRecv oRecv;
DWORD pktRet = 0;

bool WINAPI hkRECV(RECV_DATA* pRecv, void* pParam)
{
	if (pRecv->Size < 1)
		return true;

	pktRet = rdwordExt(KO_DLG);

	if (pktRet == 0)
		return true;

	if (pRecv->Data[0] == WIZ_WAREHOUSE)
		g_pMain->Player.m_iClanBank = false;

	/*Clan Bankasý Paket Ayarlarý*/
	if (pRecv->Data[0] == WIZ_HSACS_CLAN)
	{
		pRecv->Data[0] = WIZ_WAREHOUSE;

		if (!g_pMain->Player.isClanBank())
			g_pMain->Player.m_iClanBank = true;
	}

	__asm
	{
		MOV ECX, pktRet
		PUSH pParam
		PUSH pRecv
		CALL oRecv
	}
	RecvMake(pRecv);
}

const DWORD KO_OPEN_PUS = 0x008B623A;

void __fastcall pusuAc()
{
	if (g_pMain->uiPowerUpStore  == NULL)
		g_pMain->uiPowerUpStore ->OpenPowerUpStore();

	g_pMain->uiPowerUpStore ->UpdateItemList(g_pMain->pClientHookManager->item_list, g_pMain->pClientHookManager->cat_list);
	g_pMain->uiPowerUpStore ->UpdateRefundItemList(g_pMain->pClientHookManager->pusrefund_itemlist);
	g_pMain->uiPowerUpStore ->Open();
}

void __declspec(naked) hkOpenPUS()
{
	__asm {
		pushad
		pushfd
		call pusuAc
		popfd
		popad
		mov edx, KO_OPEN_PUS
		add edx, 9
		push edx
		ret
	}
}


bool RecvMake2(RECV_DATA* pRecv)
{
	RECV_DATA tmp = *pRecv;

	Packet pkt;

	unsigned int length = tmp.Size;
	if (length > 0)
		length--;

	pkt = Packet(tmp.Data[0]);
	if (length > 0)
	{
		pkt.resize(length);
		memcpy((void*)pkt.contents(), &tmp.Data[1], length);
	}

	return HandlePacket(pkt);
}

DWORD recvReturnAddress = 0;
void __declspec(naked) HSCAS_Recv(RECV_DATA* pRecv, void* pParam)
{
	if (g_pMain->m_bGameStart)
	{
		if (pRecv->Data[0] == WIZ_WAREHOUSE)
			g_pMain->Player.m_iClanBank = false;

		/*Clan Bankasý Paket Ayarlarý*/
		if (pRecv->Data[0] == WIZ_HSACS_CLAN)
		{
			pRecv->Data[0] = WIZ_WAREHOUSE;

			if (!g_pMain->Player.isClanBank())
				g_pMain->Player.m_iClanBank = true;
		}
	}

	__asm {
		push ebp
		mov ebp, esp
		push ebx
		push esi
		push edi
		pushad
		pushfd
		mov eax, [ebp + 4]
		mov recvReturnAddress, eax
		mov eax, dword ptr[pRecv]
		cmp dword ptr[eax + 4], 0
		je retback
		push eax
		call RecvMake2
		add esp, 4
		movzx ecx, al
		test ecx, ecx
		jne retend
		// devam et
		retback :
		popfd
			popad
			pop edi
			pop esi
			pop ebx
			pop ebp
			push - 1
			push 0x00B843C6
			mov eax, 0x00565890
			add eax, 7
			jmp eax
			// bitir
			retend :
		popfd
			popad
			pop edi
			pop esi
			pop ebx
			pop ebp
			mov al, 01
			add esp, 0x0c
			jmp recvReturnAddress
	}
}

void HSACSEngine::InitRecvHook() { DetourFunction((PBYTE)KO_OPEN_PUS, (PBYTE)hkOpenPUS);	oRecv = (tRecv)DetourFunction((PBYTE)KO_RECV_FUNC, (PBYTE)hkRECV); }	//DetourFunction((PBYTE)0x00565890, (PBYTE)HSCAS_Recv); }

/* Connection Gain or Lose */
int WINAPI ConnectDetour(SOCKET s, const sockaddr* name, int namelen) 
{
	struct sockaddr_in* addr_in = (struct sockaddr_in*)name;
	char * _s = inet_ntoa(addr_in->sin_addr);
	string hostAddress = string(_s);

	g_pMain->m_connectedIP = hostAddress;

	if (!IsLicensed(hostAddress))
	{
		if (g_pMain->pClientHookManager != NULL)
			g_pMain->pClientHookManager->ShowMessageBox(xorstr("Baðlantý Reddedildi"), xorstr("Sunucu ve Client arasýnda Ip adresi uyumsuz."), Ok, PARENT_LOGIN);
		else
			g_pMain->Shutdown(xorstr("Baðlantý Reddedildi Sunucu ve Client arasýnda Ip adresi uyumsuz."));
		return WSAECONNREFUSED;
	}
	return OrigConnect(s, name, namelen);
}

int WSAAPI WSAStartupDetour(WORD wVersionRequired, LPWSADATA lpWSAData) 
{
	isAlive = true;
	return OrigWSAStartup(wVersionRequired, lpWSAData);
}

int WSAAPI WSAConnectDetour(SOCKET s, const sockaddr* name, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS) {
	struct sockaddr_in* addr_in = (struct sockaddr_in*)name;
	char* _s = inet_ntoa(addr_in->sin_addr);
	string hostAddress = string(_s);

	g_pMain->m_connectedIP = hostAddress;

	if (!IsLicensed(hostAddress))
	{
		if (g_pMain->pClientHookManager != NULL)
			g_pMain->pClientHookManager->ShowMessageBox(xorstr("Baðlantý Reddedildi"), xorstr("Sunucu ve Client arasýnda Ip adresi uyumsuz."), Ok, PARENT_LOGIN);
		else
			g_pMain->Shutdown(xorstr("Baðlantý Reddedildi Sunucu ve Client arasýnda Ip adresi uyumsuz."));
		return WSAECONNREFUSED;
	}
	return OrigWSAConnect(s, name, namelen, lpCalleeData, lpCalleeData, lpSQOS, lpGQOS);
}

/* ----------------------- */

void HSACSEngine::InitJmpHook(DWORD hookFuncAddr, DWORD myFuncAddr)
{
	SetMemArray(hookFuncAddr, 0x90, 6);
	JMPHOOK(hookFuncAddr, myFuncAddr);
}

inline void HSACSEngine::JMPHOOK(DWORD Addr1, DWORD Addr2)
{
	BYTE jmp[] = { 0xE9,0,0,0,0 };
	DWORD diff = CalculateCallAddrWrite(Addr2, Addr1);

	memcpy(jmp + 1, &diff, 4);
	WriteProcessMemory(HANDLE(-1), (LPVOID)Addr1, jmp, 5, 0);
}

void SendHWID()
{
	isAlive = true;
	char AccName[25];
	ReadProcessMemory(GetCurrentProcess(), (LPVOID)KO_ACC, AccName, sizeof(AccName), NULL);
	string m_strAccountID = string(AccName);
	uint16 MACData1, MACData2 = 0;
	GetMacHash(MACData1, MACData2);
	int64 UserHardwareID = GetHardwareID();

	SHA1 sha;
	string itemorg = md5(sha.from_file(g_pMain->m_BasePath + xorstr("Data\\item_org_us.tbl")));
	string skillmagic = md5(sha.from_file(g_pMain->m_BasePath + xorstr("Data\\Skill_Magic_Main_us.tbl")));
	string zones = md5(sha.from_file(g_pMain->m_BasePath + xorstr("Data\\Zones.tbl")));
	string itemsell_table = md5(sha.from_file(g_pMain->m_BasePath + xorstr("Data\\itemsell_table.tbl")));
	string srcversion = "f5h4y7r8d5v3sd1s696g9y7r5w5q1a2d23gf3e625q4"; // dllversion uyumsuz ise dc eder

	Packet result(WIZ_HSACS_HOOK);
	result << uint8(22) 
		<< m_strAccountID 
		<< itemorg 
		<< skillmagic 
		<< zones 
		<< itemsell_table
		<< srcversion;
	g_pMain->Send(&result);
}

DWORD GetModuleSize(DWORD processID, char * module)
{
	HANDLE hSnap;
	MODULEENTRY32 xModule;
	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processID);
	xModule.dwSize = sizeof(MODULEENTRY32);
	if (Module32First(hSnap, &xModule)) 
	{
		while (Module32Next(hSnap, &xModule)) 
		{
			if (!strncmp((char*)xModule.szModule, module, 8)) 
			{
				CloseHandle(hSnap);
				return (DWORD)xModule.modBaseSize;
			}
		}
	}
	CloseHandle(hSnap);
	return 0;
}

// Packet simulation
inline void SetByte(char * tBuf, BYTE sByte, int & index)
{
	*(tBuf + index) = (char)sByte;
	index++;
};

inline void SetString(char * tBuf, char * sBuf, int len, int& index)
{
	memcpy(tBuf + index, sBuf, len);
	index += len;
};

char * sPacket;
uint8 bPacketSize;
uint8_t PacketMain = 0;
int32_t pSiz = 0, Send_Index = 0, ReturnAdress = 0;
DWORD HookAddres = 0, PushAdress = 0, BackToAdress = 0;
uint8 header = 0, subCode = 0;
uint32 nPrice, nItemID;
uint16 sCount;
uint8 bSrcPos, bDstPos, bMode;
uint32 nSkillID = 0;
int16 myID = 0, targetID = 0, targetX = 0, targetY = 0, targetZ = 0;
uint16 nObjectID = 0;
uint32 nExchangeItemID = 0, CheckReturnValue = 0;
int8_t nExchangeRobItemSlot = -1;

Packet OfficialList(WIZ_HSACS_HOOK);

uint32_t CheckSendPacket()
{
	uint32_t ReturnValue = 0;
	if (PacketMain != WIZ_HSACS_HOOK && PacketMain != WIZ_EDIT_BOX && PacketMain != WIZ_SEL_NATION && PacketMain != WIZ_MERCHANT  && PacketMain != WIZ_PARTY && PacketMain != WIZ_USER_INFO)
	{
		std::string CheckReturnAdress = to_string(ReturnAdress);
		ReturnValue = CheckReturnAdress.size();
		if (ReturnValue > 7)
			g_pMain->pClientHookManager->ShowMessageBox(xorstr("ThirdPartyTools"), xorstr("You shouldn't try cheating."), Ok);
	}
	return ReturnValue;
}

uint8_t OpCode = 0, SubOpCode = 0, SubOpCodes = 0;
__declspec(naked) void Real_Send() 
{
	__asm
	{
		MOV EAX, [ESP]
		MOV ReturnAdress, EAX
		MOV EAX, [ESP + 4]
		MOV sPacket, EAX
		MOV AL, BYTE PTR DS : [EAX]
		MOV PacketMain, AL
		MOV EAX, [ESP + 8]
		MOV pSiz, EAX
	}

	_asm pushad
	_asm pushfd
	//printf("%x\n", ReturnAdress);
	Real_SendTime = clock();
	g_pMain->PacketCheckThreadID(PacketMain, ReturnAdress);
	//printf("Packet %x\n", PacketMain);
	if (PacketMain == WIZ_ZONE_CHANGE)
	{
		header = g_pMain->GetByte(sPacket, Send_Index);
		subCode = g_pMain->GetByte(sPacket, Send_Index);

		if (subCode == 1)
			g_pMain->Player.isTeleporting = true;
		else if (subCode == 2) 
		{
			g_pMain->m_bLoadingControl = false;
			g_pMain->Player.isTeleporting = false;
		}
	}
	else if (PacketMain == WIZ_MAGIC_PROCESS)
	{
		Send_Index = 0;
		header = g_pMain->GetByte(sPacket, Send_Index);
		subCode = g_pMain->GetByte(sPacket, Send_Index);
		nSkillID = g_pMain->GetDWORD(sPacket, Send_Index);
		myID = g_pMain->GetShort(sPacket, Send_Index);
		targetID = g_pMain->GetShort(sPacket, Send_Index);
		targetX = g_pMain->GetShort(sPacket, Send_Index);
		targetY = g_pMain->GetShort(sPacket, Send_Index);
		targetZ = g_pMain->GetShort(sPacket, Send_Index);

#if ANTICHEAT_MODE == 1
		if (!CheckSkill(nSkillID) && !g_pMain->m_bGenieStatus) {
			__asm jmp PaketiAtma
		}
#endif
	}
	else if (PacketMain == WIZ_MERCHANT)
	{
		Send_Index = 0;
		header = g_pMain->GetByte(sPacket, Send_Index);
		subCode = g_pMain->GetByte(sPacket, Send_Index);

		if (subCode == MerchantOpcodes::MERCHANT_MENISIA_LIST)
		{
			OfficialList << uint8(HSACSOpCodes::MERCHANTLIST) << uint8_t(0x00);
			g_pMain->Send(&OfficialList);
		}
		else if (subCode == MERCHANT_ITEM_ADD) // item add to merchant
		{
			nItemID = g_pMain->GetDWORD(sPacket, Send_Index);
			sCount = g_pMain->GetShort(sPacket, Send_Index);
			nPrice = g_pMain->GetDWORD(sPacket, Send_Index);
			bSrcPos = g_pMain->GetByte(sPacket, Send_Index);
			bDstPos = g_pMain->GetByte(sPacket, Send_Index);
			bMode = g_pMain->GetByte(sPacket, Send_Index);
			g_pMain->SendItemAdd(nItemID, sCount, nPrice, bSrcPos, bDstPos, bMode);
		}
	}
	else if (PacketMain == WIZ_LOGOUT)
	{
		if (g_pMain->pClientHookManager != NULL)
			g_pMain->pClientHookManager->Release();
	}
	else if (PacketMain == WIZ_GAMESTART || PacketMain == WIZ_MYINFO || PacketMain == WIZ_NOTICE && !m_bGameStard)
		m_bGameStard = true;
	else if (PacketMain == WIZ_LOGOUT)
	{
		g_pMain->Player.logOut = true;
		TerminateProcess(GetCurrentProcess(), 0);
	}
	else if (PacketMain == WIZ_ITEM_UPGRADE)
	{
		Send_Index = 0;
		header = g_pMain->GetByte(sPacket, Send_Index);
		subCode = g_pMain->GetByte(sPacket, Send_Index);

		if (subCode == 5)
		{
			nObjectID = g_pMain->GetShort(sPacket, Send_Index);
			nExchangeItemID = g_pMain->GetDWORD(sPacket, Send_Index);
			nExchangeRobItemSlot = g_pMain->GetDWORD(sPacket, Send_Index);
			if (g_pMain->uiPieceChangePlug != NULL) 
			{
				g_pMain->uiPieceChangePlug->m_nObjectID = nObjectID;
				g_pMain->uiPieceChangePlug->m_nExchangeItemID = nExchangeItemID;
				g_pMain->uiPieceChangePlug->m_nExchangeRobItemSlot = nExchangeRobItemSlot;
				g_pMain->SendChaoticExchange(nObjectID, nExchangeItemID, nExchangeRobItemSlot);
			}
		}
	}
	else if (PacketMain == WIZ_WAREHOUSE)
	{
		if (g_pMain->Player.isClanBank())
		{
			sPacket[0] = WIZ_HSACS_CLAN;
			g_pMain->Player.m_iClanBank = false;
		}
	}
	else if (PacketMain == WIZ_GENIE)
	{
		Send_Index = 0;
		OpCode = g_pMain->GetByte(sPacket, Send_Index);
		SubOpCode = g_pMain->GetByte(sPacket, Send_Index);
		if (SubOpCode == 2)
		{
			SubOpCodes = g_pMain->GetByte(sPacket, Send_Index);
			if (SubOpCodes == 4)
			{
#if ANTICHEAT_MODE == 1
				if (g_pMain->m_bGenieStatus == false)
					__asm jmp PaketiAtma
#if 0
				else
				{
					if (g_pMain->m_bGenieAttackStatus == false)
						__asm jmp PaketiAtma
				}
#endif
#endif
			}
		}
	}
	else if (PacketMain == WIZ_EVENT)
	{
		/*Send_Index = 0;
		int8 OpCode = g_pMain->GetByte(sPacket, Send_Index);
		int8 SubOpCode = g_pMain->GetByte(sPacket, Send_Index);
		printf("EVENT %d %d \n", OpCode, SubOpCode);*/
	}
	_asm
	{
		popfd
		popad
		PUSH - 1
		PUSH PushAdress
		JMP BackToAdress
		PaketiAtma :
		popfd
		popad
		add esp, 0x0c
		jmp ReturnAdress
	}
}

void HSACSEngine::SendItemAdd(uint32 itemID, uint16 count, uint32 gold, uint8 srcPos, uint8 dstPos, uint8 mode)
{
	uint8 isKC = g_pMain->pClientTradePrice->m_bIsKC ? 1 : 0;
	nisKC = isKC;
	RecvPrice = gold;
	Packet pkt(WIZ_HSACS_HOOK);
	pkt << uint8_t(HSACSOpCodes::MERCHANT) << ((uint8)MERCHANT_ITEM_ADD) << itemID << count << gold << srcPos << dstPos << mode << isKC;
	Send(&pkt);
	pClientMerchant->SetRecentItemAddReq(pkt);
}

void HSACSEngine::PacketCheckThreadID(uint8 Packet, DWORD ThreadID)
{
	bool isPacket = true;
	switch (Packet)
	{
	case WIZ_HSACS_HOOK:
	case WIZ_POINT_CHANGE:
	case WIZ_MERCHANT:
	case WIZ_PARTY:
	case WIZ_GENIE:
	case WIZ_EVENT:
	case WIZ_USER_INFO:
	case WIZ_EDIT_BOX:							// Eðer paket atýnca oyun kapanýyor ise buraya dýþlamamýz gerekiyor Örneðimiz WIZ EDIT BOXtýr
	case WIZ_HSACS_CLAN:
		isPacket = false;
		break;
	}
	
	if (g_pMain->mReturnAdressSend.size() != 490)
		isPacket = true;
	if (isPacket)
	{
		auto itr = g_pMain->mReturnAdressSend.find(ReturnAdress);
		if (itr == g_pMain->mReturnAdressSend.end())
			ischeatactive = true;
	}
}

inline int HSACSEngine::GetShort(char* sBuf, int& index)
{
	index += 2;
	return *(short*)(sBuf + index - 2);
};

inline DWORD HSACSEngine::GetDWORD(char* sBuf, int& index)
{
	index += 4;
	return *(DWORD*)(sBuf + index - 4);
};

inline BYTE HSACSEngine::GetByte(char* sBuf, int& index)
{
	int t_index = index;
	index++;
	return (BYTE)(*(sBuf + t_index));
};

const DWORD KO_SND = KO_SND_FNC;

void HSACSEngine::InitSendHook()
{
	HookAddres = KO_SND;
	PushAdress = *(DWORD*)(HookAddres + 4);
	BackToAdress = KO_SND + 7;

	InitJmpHook(KO_SND, (DWORD)Real_Send);
}

int WINAPI SendDetour(SOCKET s, char* buf, int len, int flags)
{
	Packet pkt;
	uint16 header;
	uint16 length;
	uint16 footer;
	memcpy(&header, buf, 2);
	memcpy(&length, buf + 2, 2);
	memcpy(&footer, buf + 4 + length, 2);
	uint8* in_stream = new uint8[length];
	memcpy(in_stream, buf + 4, length);

	if (length > 0)
		length--;

	pkt = Packet(in_stream[0], (size_t)length);
	if (length > 0)
	{
		pkt.resize(length);
		memcpy((void*)pkt.contents(), &in_stream[1], length);
	}

	delete[]in_stream;

	uint8 cmd = pkt.GetOpcode();
	

	return OrigSend(s, buf, len, flags);
}

string strToLower(string str) 
{
	for (auto& c : str) c = tolower(c);
	return str;
}

string WtoString(WCHAR s[]) 
{
	wstring ws(s);
	string ret;

	for (char x : ws)
		ret += x;
	
	return ret;
}

int currentID = 0;
vector<string> activeWindows;

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam) {
	DWORD dwProcessId;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	if (currentID != dwProcessId) return TRUE;
	char String[255];
	if (!hWnd)
		return TRUE;
	if (IsWindowVisible(hWnd))
	{
		char wnd_title[256];
		GetWindowTextA(hWnd, wnd_title, sizeof(wnd_title));
		activeWindows.push_back(string((wnd_title)));
	}
	return TRUE;
}

bool inArray(vector<string> arr, string obj) 
{
	for (string i : arr) {
		if (obj == i) return true;
	}
	return false;
}

typedef HMODULE(WINAPI* tLoadLibrary)(LPCSTR lpLibFileName);
tLoadLibrary oLoadLibrary;

HMODULE WINAPI hkLoadLibrary(LPCSTR lpLibFileName) // Cheat Engine ile speed hack %100 Fix , Start
{
	string mdl = lpLibFileName;
	if (mdl.find("speedhack") != std::string::npos)
	{
		__asm {
			inc esp
			mov esp, 0x0
			push esp
			ret
		}
	}
	return oLoadLibrary(lpLibFileName);
} // Cheat Engine ile speed hack %100 Fix , End

void HSACSEngine::InitPlayer() 
{
	oLoadLibrary = (tLoadLibrary)DetourFunction((PBYTE)LoadLibraryA, (PBYTE)hkLoadLibrary);

	if (thisProc == NULL) 
		thisProc = GetCurrentProcess();

	if (KO_ADR == 0x0) 
		ReadProcessMemory(thisProc, (LPCVOID)KO_PTR_CHR, &KO_ADR, sizeof(DWORD), 0);

	ofstream logFile;
	if (!dirExists(xorstr("\\")))
		CreateDirectoryA(xorstr("\\"), NULL);

	logFile.open(xorstr("\\init_log.txt"));
	Player.Nick = "";
	Player.Level = 0;
	Player.RebLevel = 0;
	Player.ZoneID = 0;
	Player.NationPoint = 0;
	Player.KnightCash = 0;
	Player.ddAc = 0;
	Player.axeAc = 0;
	Player.swordAc = 0;
	Player.maceAc = 0;
	Player.arrowAc = 0;
	Player.spearAc = 0;
	Player.m_iClanBank = false;
	Player.isTeleporting = false;
	Player.isRankOpen = false;
	Player.logOut = false;
	Player.burninglevel = 0;
	Player.burningtime = 0;
	ReadProcessMemory(thisProc, (LPVOID)(KO_ADR + KO_OFF_ZONE), &Player.ZoneID, sizeof(Player.ZoneID), 0);
	this->Player.Authority = USER;
	uint16 MACData1, MACData2 = 0;
	GetMacHash(MACData1, MACData2);
	this->Player.MAC = uint32(MACData1 + MACData2);
	//Init GPU info
	DISPLAY_DEVICE DevInfo;
	DevInfo.cb = sizeof(DISPLAY_DEVICE);
	DWORD iDevNum = 0;
	logFile << xorstr("-- HSACSX Initializing --") << endl;
	while (EnumDisplayDevices(NULL, iDevNum, &DevInfo, 0))
	{
		if (inArray(this->Player.GPU, DevInfo.DeviceString)) {
			iDevNum++;
			continue;
		}
		this->Player.GPU.push_back(DevInfo.DeviceString);
		iDevNum++;
		logFile << xorstr("------ GPU: ") << DevInfo.DeviceString << endl;
	}
	tmpGraphics = "";
	for (string gpu : Player.GPU)
		tmpGraphics += xorstr(" | ")+ gpu;
	//Init processor info
	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo);
	int CPUInfo[4] = { -1 };
	__cpuid(CPUInfo, 0x80000000);
	unsigned int nExIds = CPUInfo[0];
	char CPUBrandString[0x40] = { 0 };
	for (unsigned int i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(CPUInfo, i);
		if (i == 0x80000002)
		{
			memcpy(CPUBrandString,
				CPUInfo,
				sizeof(CPUInfo));
		}
		else if (i == 0x80000003)
		{
			memcpy(CPUBrandString + 16,
				CPUInfo,
				sizeof(CPUInfo));
		}
		else if (i == 0x80000004)
		{
			memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
		}
	}
	this->Player.CPU = string(CPUBrandString) + xorstr(" | ") + to_string(siSysInfo.dwNumberOfProcessors) + xorstr(" Core(s)");
	logFile << xorstr("------ CPU: ") << this->Player.CPU.c_str() << endl;
	tmpProcessor = Player.CPU;
	//Init hwid info
	this->Player.HWID = GetHardwareID();
	//Init screen info
	ScreenInfo* screen = new ScreenInfo();
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	screen->height = desktop.bottom;
	screen->width = desktop.right;
	this->Player.Screen = screen;
	//Init processes
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;
	if (EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		cProcesses = cbNeeded / sizeof(DWORD);
		for (i = 0; i < cProcesses; i++)
		{
			if (aProcesses[i] != 0) {
				char szProcessName[MAX_PATH];
				HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
				if (NULL != hProcess)
				{
					HMODULE hMod;
					DWORD cbNeeded;
					if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
					{
						GetModuleBaseNameA(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
						ProcessInfo procInfo;
						procInfo.id = aProcesses[i];
						procInfo.name = szProcessName;
						currentID = aProcesses[i];
						activeWindows.clear();
						EnumWindows(EnumWindowsProc, NULL);
						for (string windowName : activeWindows) {
							procInfo.windows.push_back(windowName);
						}
						this->Player.Processes.push_back(procInfo);
					}
				}
			}
		}
	}
	logFile.close();
	processTMP = Player.Processes;
	MainThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)EngineMain, this, NULL, NULL);
}

void LM_SendProcess(uint16 toWHO) 
{
	processTMP.clear();
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return;

	PROCESSENTRY32 process;
	process.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnapshot, &process)) {
		CloseHandle(hSnapshot);
		return;
	}

	do 
	{
		ProcessInfo procInfo;
		procInfo.id = process.th32ProcessID;
		procInfo.name = process.szExeFile;
		currentID = process.th32ProcessID;
		activeWindows.clear();

		EnumWindows(EnumWindowsProc, NULL);

		for (string windowName : activeWindows) 
			procInfo.windows.push_back(windowName);

		processTMP.push_back(procInfo);

		std::cout << process.th32ProcessID << "\t" << process.szExeFile << std::endl;
	} while (Process32Next(hSnapshot, &process));

	CloseHandle(hSnapshot);

	Packet result(WIZ_HSACS_HOOK);
	result << uint8(HSACSOpCodes::PROCINFO) << uint16(toWHO) << uint32(processTMP.size());

	for (ProcessInfo proc : processTMP) 
	{
		result << int(proc.id) << string(proc.name) << int(proc.windows.size());

		for (string window : proc.windows)
			result << string(window);
	}
	LM_Send(&result);
}

void HSACSEngine::SendProcess(uint16 toWHO) 
{
	LM_SendProcess(toWHO);
}

void HSACSEngine::Disconnect() { allowAlive = false; }

void HSACSEngine::Send(Packet* pkt) { LM_Send(pkt); }

void HSACSEngine::StayAlive() 
{
	if (!isAlive || !allowAlive)
		return;

	char AccName[25]{};
	uint8 size = 0;
	ReadProcessMemory(GetCurrentProcess(), (LPVOID)KO_ACC_SIZE, &size, 1, NULL);
	if (size < 16)
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)KO_ACC, AccName, sizeof(AccName), NULL);
	else
	{
		DWORD address;
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)KO_ACC, &address, sizeof(address), NULL);
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)address, AccName, sizeof(AccName), NULL);
	}

	accountID = string(AccName);
	if (accountID.size()) STRTOUPPER(accountID);

	clock_t realtime = Real_SendTime;
	bool cheatactive = ischeatactive;

	uint32 my = myrand(500, 3500);
	std::string public_key = md5("1X" + std::to_string(HSACSX_VERSION) + "50001" + std::to_string(realtime) + std::to_string(cheatactive) + accountID.c_str());
	Packet result(WIZ_HSACS_HOOK, uint8(HSACSOpCodes::ALIVE));
	result.DByte();
	result << uint32(realtime) << my << public_key << uint8(cheatactive) << uint32(myrand(500, 3500));
	LM_Send(&result);
}

int WINAPI hTerminateProcess(HANDLE hProcess, UINT uExitCode) 
{
	if (hProcess == GetCurrentProcess())
		if (g_pMain->pClientHookManager != NULL)
			g_pMain->pClientHookManager->Release();
	return OrigTerminateProcess(hProcess, uExitCode);
}

int WINAPI hExitProcess(UINT uExitCode) 
{
	if (g_pMain->pClientHookManager != NULL)
		g_pMain->pClientHookManager->Release();

	return OrigExitProcess(uExitCode);
}

HSACSEngine::~HSACSEngine() 
{
}

//Uif Hook

DWORD HSACSEngine::rdwordExt(DWORD ulBase)
{
	if (!IsBadReadPtr((VOID*)ulBase, sizeof(DWORD)))
		return(*(DWORD*)(ulBase));
	return 0;
}


DWORD HSACSEngine::rdword(DWORD ulBase, std::vector<int> offsets)
{
	DWORD ibase = rdwordExt(ulBase);
	for (size_t i = 0; i < offsets.size() - 1; i++)
	{
		int offset = offsets[i];
		ibase += offset;
		int ibase1 = ibase;
		ibase = rdwordExt(ibase);
	}

	return ibase;
}

DWORD HSACSEngine::ReadDWORD(DWORD ulBase, std::vector<int> offsets)
{
	DWORD ibase;
	ReadProcessMemory(GetCurrentProcess(), (LPVOID)ulBase, &ibase, sizeof(ibase), NULL);
	for (size_t i = 0; i < offsets.size() - 1; i++)
	{
		int offset = offsets[i];
		ibase += offset;
		int ibase1 = ibase;
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)ibase, &ibase, sizeof(ibase), NULL);
	}
	return ibase;
}

std::string m_strReplaceString = "";
DWORD m_dvTable = 0x0;

void __declspec(naked) SetStringAsm()
{
	_asm
	{
		MOV ECX, m_dvTable
		MOV EAX, OFFSET m_strReplaceString
		PUSH EAX
		CALL KO_SET_STRING_FUNC
		RET
	}
}

typedef void(__thiscall* tSetString)(DWORD uiObject, const std::string& szString);
tSetString Func_SetString = (tSetString)KO_SET_STRING_FUNC;
void HSACSEngine::UIScreenCenter(DWORD vTable)
{
	if (vTable == 0)
		return;

	if (IsBadReadPtr((VOID*)vTable, sizeof(DWORD)))
		return;
	
	RECT screen = g_pMain->pClientHookManager->GetScreenRect();
	POINT ret;
	ret.x = (screen.right / 2) - (g_pMain->GetUiWidth(vTable) / 2);
	ret.y = (screen.bottom / 2) - (g_pMain->GetUiHeight(vTable) / 2);
	g_pMain->SetUIPos(vTable, ret);

}
void HSACSEngine::SetString(DWORD vTable, std::string str)
{
	if (vTable == 0)
		return;

	if (IsBadReadPtr((VOID*)vTable, sizeof(DWORD)))
		return;

	Func_SetString(vTable, str);
}


void HSACSEngine::SetUIStyle(DWORD dwTable, DWORD Style)
{
	*(DWORD*)(dwTable + 0xE4) = Style;
}

const   DWORD   KO_SET_SCROLL_VALUE_FUNC = 0x0042B3A0; // 2369
DWORD m_dvBase = 0x0;
int m_iVal = 0;

void __declspec(naked) SetScrollValueAsm()
{
	_asm
	{
		MOV ECX, m_dvBase
		MOV ESI, m_dvTable
		MOV EAX, m_iVal
		PUSH EAX
		CALL KO_SET_SCROLL_VALUE_FUNC
		RET
	}
}

void HSACSEngine::SetScrollValue(DWORD vTable, int val)
{
	m_dvBase = *(DWORD*)(vTable + 0x134);
	m_dvTable = vTable;
	m_iVal = val;
	SetScrollValueAsm();
}

void HSACSEngine::WriteString(DWORD value, char* vl)
{
	WriteProcessMemory(GetCurrentProcess(), (void*)value, (LPVOID)vl, sizeof(vl), 0);
}

std::string HSACSEngine::GetString(DWORD vTable)
{
	if (!vTable)
		return "";

	DWORD nameKen = *(DWORD*)((DWORD)vTable + 0x154);
	char* name = new char[nameKen + 1]{ 0 };

	if (nameKen < 16)
		memcpy(name, (char*)((DWORD)vTable + 324), nameKen);
	else
		memcpy(name, (char*)*(DWORD*)((DWORD)vTable + 324), nameKen);
	std::string n = name;
	std::free(name);
	return n;

}

std::string HSACSEngine::GetPartyString(DWORD vTable)
{
	if (vTable == 0x00)
		return "";

	char* buff;
	DWORD value = vTable + 324; //2383 for 
	buff = (char*)malloc(100);
	memcpy(buff, (char*)*(DWORD*)((DWORD)value), 100);
	return std::string(buff);
}

std::string HSACSEngine::GetStringElementName(DWORD vTable)
{
	char* buff;
	DWORD value = vTable + 0x64;

	buff = (char*)malloc(100);
	ReadProcessMemory(GetCurrentProcess(), (void*)value, (LPVOID)buff, 100, 0);

	return std::string(buff);
}
std::string HSACSEngine::GetStringFromPWEdit(DWORD vTable)
{
	char* buff;
	DWORD value = vTable + 140;

	buff = (char*)malloc(100);
	ReadProcessMemory(GetCurrentProcess(), (void*)value, (LPVOID)buff, 100, 0);

	return std::string(buff);
}

DWORD m_dChild = 0x0;
std::string m_strGetChildID;

void __declspec(naked) GetChildByIDAsm()
{
	_asm
	{
		MOV ECX, [m_dvTable]
		MOV EAX, OFFSET m_strGetChildID
		push m_dChild
		PUSH EAX
		CALL oGetChild
		MOV m_dChild, EAX
		RET
	}
}

__inline DWORD rRDWORD(DWORD ulBase)
{
	if (!IsBadReadPtr((VOID*)ulBase, sizeof(DWORD)))
		return(*(DWORD*)(ulBase));

	return 0;
}

int16 GetPartyLeaderSocket()
{
	DWORD base = rRDWORD(rRDWORD(KO_DLG) + KO_OFF_PTBASE);
	DWORD buffer = rRDWORD(base + KO_OFF_PT);
	buffer = rRDWORD(buffer);
	USHORT LeaderSocket = *(USHORT*)(buffer + 0x8);
	return LeaderSocket;
}

DWORD GetLeaderBase() { return g_pMain->GetTarget(GetPartyLeaderSocket()); }

bool HSACSEngine::isInParty() { return GetPartyLeaderSocket(); }

int16 HSACSEngine::GetPartyLeaderID() { return GetPartyLeaderSocket(); }

DWORD HSACSEngine::GetPartyLeaderBase() { return GetLeaderBase(); }

int16 HSACSEngine::GetPartyLeaderTarget()
{
	if (DWORD byAdress = GetLeaderBase())
		return *(uint16*)(byAdress + KO_OFF_MOB);

	return 0;
}

typedef DWORD(__thiscall* tGetChildByID)(DWORD uiObject, const std::string& szChildID, DWORD nUnkown);
tGetChildByID Func_GetChildByID = (tGetChildByID)KO_GET_CHILD_BY_ID_FUNC;

DWORD HSACSEngine::GetChildByID(DWORD dwTable, std::string strID)
{
	if (!dwTable)
		return false;

	if (IsBadReadPtr((VOID*)dwTable, sizeof(DWORD)))
		return false;

	DWORD vTable = *(DWORD*)dwTable;
	if (!vTable || vTable < 0xC00000 || vTable > 0xFFFFFF)
		return false;

	return DWORD(Func_GetChildByID(dwTable, strID, 0));
}

void HSACSEngine::GetChildByID(DWORD vTable, std::string id, DWORD& child)
{
	m_dvTable = vTable;
	m_dChild = child;
	m_strGetChildID = id;
	GetChildByIDAsm();
	if (m_dChild == 0)
	{
		std::string msg = string_format(xorstr("Element couldn't found: %s"), id.c_str());
		g_pMain->ShowMsg(MSG_INFO, msg);
		child = NULL;
		return;
	}
	child = m_dChild;
}

int16 HSACSEngine::GetSocketID() { return *(int16*)(*(DWORD*)(KO_PTR_CHR)+KO_OFF_ID); }

const DWORD KO_UIF_LIST_ADD_TEXT = 0x00422850;  // Search Monster Drop List'e eleman ekleme
const DWORD KO_UIF_LIST_CLEAR_TEXT = 0x004213C0;  // Search Monster Drop List'e eleman ekleme

int16 HSACSEngine::GetListSelect(DWORD vTable)
{
	if (vTable == NULL)
		return -1;

	return *(DWORD*)(vTable + 0x144);
}

void HSACSEngine::ClearListString(DWORD vTable)
{
	if (vTable == NULL)
		return;

	__asm
	{
		MOV ECX , vTable
		call  KO_UIF_LIST_CLEAR_TEXT
	}
}

std::string tmpstr = "";

void HSACSEngine::AddListString(DWORD vTable, const std::string& szString, DWORD color)
{
	if (vTable == NULL || szString.empty())
		return;

	tmpstr = szString;

	__asm {
		push 0xFF80FF80
		push 0xF
		push 0
		push 0
		push 0
		push 0
		push 0
		push offset tmpstr
		push color
		push offset tmpstr
		mov ecx, vTable
		call KO_UIF_LIST_ADD_TEXT
	}
}

const DWORD KO_UIF_KILL_FOCUS = 0x00416590;  // Killfocus Fonksiyonu -> Power Up store ve diðer uiflerde kullanýlacak.
void HSACSEngine::EditKillFocus(DWORD vTable)
{
	if (vTable == NULL)
		return;

	__asm 
	{
		mov ecx, vTable
		call KO_UIF_KILL_FOCUS
	}
}

void HSACSEngine::GetBaseByChild(DWORD vTable, DWORD& base) { base = *(DWORD*)(vTable + 0xBC); }

DWORD m_dBool = 0;
typedef void (__thiscall* tSetVisibleFunc)(DWORD ecx, bool a2);
tSetVisibleFunc SetVisibleFunc = (tSetVisibleFunc)KO_SET_VISIBLE_FUNC;

void HSACSEngine::SetUiRegion(DWORD vTable, RECT rc)
{
	uintptr_t** ptrVtable = (uintptr_t**)vTable;

	LONG left = (LONG)ptrVtable[59];
	LONG top = (LONG)ptrVtable[60];
	LONG right = (LONG)ptrVtable[61];
	LONG bottom = (LONG)ptrVtable[62];

	ptrVtable[59] = (uintptr_t*)rc.left;
	ptrVtable[60] = (uintptr_t*)rc.top;
	ptrVtable[61] = (uintptr_t*)rc.right;
	ptrVtable[62] = (uintptr_t*)rc.bottom;
}

void HSACSEngine::GetCoordination(DWORD vTable, POINT& pt, POINT& pt2)
{
	uintptr_t** ptrVtable = (uintptr_t**)vTable;
	pt.x = (LONG)ptrVtable[61];
	pt.y = (LONG)ptrVtable[62];

	pt2.x = (LONG)ptrVtable[59];
	pt2.y = (LONG)ptrVtable[60];
}

void HSACSEngine::SetVisible(DWORD vTable, bool type)
{
	if (vTable == 0x0)
		return;

	SetVisibleFunc(vTable, type);
}

bool HSACSEngine::IsVisible(DWORD vTable) { return vTable ? *(bool*)(vTable + 0x10D) : false; }

PVOID GetLibraryProcAddress(LPCSTR LibraryName, LPCSTR ProcName) { return GetProcAddress(GetModuleHandleA(LibraryName), ProcName); }

HSACSEngine::HSACSEngine(std::string basePath) 
{
	varius = 0;
	sPerksDataInfo = _PERKS_DATA_INFO();
	m_sPerkInfoArray.clear();
	m_sMerchantDisplayData = m_sMerchantDisplaySpecialData = 0;
	m_recentdelete_time = (30 * 1000) * 60;
	OrigConnect = (MyConnect)DetourFunction((PBYTE)connect, (PBYTE)ConnectDetour);
	OrigWSAConnect = (MyWSAConnect)DetourFunction((PBYTE)WSAConnect, (PBYTE)WSAConnectDetour);
	OrigWSAStartup = (MyWSAStartup)DetourFunction((PBYTE)WSAStartup, (PBYTE)WSAStartupDetour);
	GuardLockPerkSystem.KeepFunction((DWORD)clock, 0x4BA, xorstr("clock"));
	GuardLockPerkSystem.KeepFunction((DWORD)memcpy, 0x33D, xorstr("memcpy"));
	GuardLockPerkSystem.KeepFunction((DWORD)memcpy_s, 0x6D, xorstr("memcpy_s"));
	GuardLockPerkSystem.KeepFunction((DWORD)malloc, 0xEA, xorstr("malloc"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("Kernel32.dll"), xorstr("IsDebuggerPresent")), 6, xorstr("IsDebuggerPresent"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("Kernel32.dll"), xorstr("CreateThread")), 0x2B, xorstr("CreateThread"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("Kernel32.dll"), xorstr("CreateRemoteThread")), 0x2C, xorstr("CreateRemoteThread"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("Kernel32.dll"), xorstr("SetThreadContext")), 0xC, xorstr("SetThreadContext"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("Kernel32.dll"), xorstr("GetThreadContext")), 0xC, xorstr("GetThreadContext"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("Kernel32.dll"), xorstr("ReadProcessMemory")), 0x3D, xorstr("ReadProcessMemory"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("Kernel32.dll"), xorstr("WriteProcessMemory")), 0xC, xorstr("WriteProcessMemory"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("Kernel32.dll"), xorstr("VirtualAlloc")), 0x58, xorstr("VirtualAlloc"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("Kernel32.dll"), xorstr("VirtualAllocEx")), 0xC, xorstr("VirtualAllocEx"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("Kernel32.dll"), xorstr("VirtualFreeEx")), 0xC, xorstr("VirtualFreeEx"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("Kernel32.dll"), xorstr("VirtualProtectEx")), 0xC, xorstr("VirtualProtectEx"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("Kernel32.dll"), xorstr("VirtualQueryEx")), 0xC, xorstr("VirtualQueryEx"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("Kernel32.dll"), xorstr("OpenThread")), 0xC, xorstr("OpenThread"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("Kernel32.dll"), xorstr("OpenProcess")), 0xC, xorstr("OpenProcess"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("Kernel32.dll"), xorstr("GetTickCount")), 9, xorstr("GetTickCount"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("Kernel32.dll"), xorstr("GetTickCount64")), 0x54, xorstr("GetTickCount64"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("User32.dll"), xorstr("GetAsyncKeyState")), 0xC1, xorstr("GetAsyncKeyState"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("User32.dll"), xorstr("EnumWindows")), 0x46, xorstr("EnumWindows"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("User32.dll"), xorstr("GetForegroundWindow")), 6, xorstr("GetForegroundWindow"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("User32.dll"), xorstr("GetWindowTextA")), 0x77, xorstr("GetWindowTextA"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("User32.dll"), xorstr("GetWindowTextW")), 0x29D, xorstr("GetWindowTextW"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("Ws2_32.dll"), xorstr("connect")), 0x181, xorstr("connect"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("Ws2_32.dll"), xorstr("recv")), 0x175, xorstr("recv"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("Ws2_32.dll"), xorstr("send")), 0xB2, xorstr("send"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("Ws2_32.dll"), xorstr("WSARecv")), 0x2F5, xorstr("WSARecv"));
	GuardLockPerkSystem.KeepFunction((DWORD)GetLibraryProcAddress(xorstr("Ws2_32.dll"), xorstr("WSASend")), 0x2F5, xorstr("WSASend"));
	bDisableAllSkillFX = false;
	bDisableAreaSkillFX = false;
	bDisableViewEmblem = false;
	bDisableViewPathos = false;
	bDisableHealFX = false;
	DeathAll = true;
	DeathMe = false;
	DeathNone = false;
	DeathParty = false;
	timesPassed = 0;
	dc = NULL;
	render = true;
	m_connectedIP = "";
	power = true;
	m_BasePath = basePath;
	hpBarAdress = 0;
	ScanThread = NULL;
	m_cCollettionStatus = false;
	isSiegeWarStart = isSiegeWarActive = false;
	m_bLoading = false;
	m_bLoqOut = false;
	m_bLoadingControl = false;
	disableCameraZoom = false;
	pClientTBLManager = NULL;
	moneyReq = 0;
	m_zMob = 0;
	StringHelper = NULL;
	m_PlayerBase = NULL;
	pClientHookManager = NULL;
	pClientUIState = NULL;
	pClientTargetBar = NULL;
	uiTaskbarMain = NULL;
	uiTaskbarSub = NULL;
	uiClanWindowPlug = NULL;
	uiMiniMenuPlug = NULL;
	pClientPlayerRank = NULL;
	uiNoticeWind = NULL;
	uiGenieSubPlug = NULL;
	uiSchedularPlug = NULL;
	uiScoreBoard = NULL;
	uiGenieMain = NULL;
#if(HOOK_SOURCE_VERSION == 2369)
	pClientRightExchange = NULL;
	pClientDailyReward = NULL;
#else
	pClientRightExchange = NULL;
	pClientDailyReward = NULL;
#endif
	m_SettingsMgr = NULL;
	uiSeedHelperPlug = NULL;
	uiPieceChangePlug = NULL;
	uiLogin = NULL;
	uiQuestPage = NULL;
	pClientSealToolTip = NULL;
	uiPowerUpStore = NULL;
	pClientTradePrice = NULL;
	pClientTradeInventory = NULL;
	uiTradeItemDisplay = NULL;
	uiTradeItemDisplaySpecial = NULL;
	uiChatBarPlug = NULL;
	m_SettingsMgr = NULL;
	pClientMerchant = NULL;
	uiToolTip = NULL;
	pClientInventory = NULL;
	uiPartyBBS = NULL;
	pClientUIBarPlug = NULL;
	uiHpMenuPlug = NULL;
	uiAccountRegisterPlug = NULL;
	pClientTopRightNewPlug = NULL;
	uiMinimapPlug = NULL;
	uiSupport = NULL;
	pClientCollection = NULL;
	pClientWheelOfFunPlug = NULL;
	uiSearchMonster = NULL;
	pClientDropResult = NULL;
	uiTagChange = NULL;
	uiSkillPage = NULL;
	uiQuestComplated = NULL;
	pClientLottyEvent = NULL;
	uiCindirella = NULL;
	pClientEvetShowList = NULL;
	uiMerchantList = NULL;
	_pDevice = NULL;
	logState = true;
	drawMode = true;
	_lowPower = false;
	strClientName.clear();
	m_bSelectedCharacter = false;
	m_bGameStart = false;
	m_bInParty = false;
	m_bHookStart = false;
	m_bGenieStatus = false;
	m_bGenieAttackStatus = false;
	Adress = 0;
	m_bHookPanel = false;
	InitPlayer();
	pClientTBLManager->Init();
}

void HSACSEngine::ShowMsg(MSG_TYPE type, std::string msg, ...)
{
	std::string result;
	va_list ap;

	va_start(ap, msg);
	tstring_format(msg, &result, ap);
	va_end(ap);

	if (type == MSG_ERROR)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FORE_COLOR_LIGHTRED);
#ifndef _DEBUG
		MessageBoxA(NULL, xorstr("Error-> "), "Error\n", MB_OK);
#endif
	}
	else if (type == MSG_SUCCESS)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FORE_COLOR_LIGHTGREEN);
#ifndef _DEBUG
		MessageBoxA(NULL, xorstr("OK-> "), "Error\n", MB_OK);
#endif
	}
	else if (type == MSG_WARNING)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FORE_COLOR_YELLOW);
#ifndef _DEBUG
		MessageBoxA(NULL, xorstr("Warn-> "), "Error\n", MB_OK);
#endif
	}
	else if (type == MSG_INFO)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FORE_COLOR_LIGHTBLUE);
#ifndef _DEBUG
		MessageBoxA(NULL, xorstr("Message-> "), "Error\n", MB_OK);
#endif
	}
#ifndef _DEBUG
	MessageBoxA(NULL, result.c_str(), "result\n", MB_OK);
#endif
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FORE_COLOR_LIGHTGRAY);
}

void HSACSEngine::WriteInfoMessageExt(char* pMsg, DWORD dwColor)
{
	int iMsgLen = strlen(pMsg);
	char* pParam = new char[iMsgLen + 33];
	DWORD	dwParamAddr = (DWORD)pParam;

	memset(pParam, 0, iMsgLen + 33);
	memcpy(pParam + 32, pMsg, iMsgLen);

	*(int*)(pParam + 20) = iMsgLen;
	*(DWORD*)(pParam + 4) = (DWORD)pParam + 32;
	*(DWORD*)(pParam + 24) = 0x1F;

	__asm
	{
		MOV ECX, DWORD PTR DS : [0x00F3690C]

		push 0
		push dwColor
		push dwParamAddr
		mov  eax, KO_ADD_INFO_MSG_FUNC
		call eax
	}

	delete[] pParam;
}

void HSACSEngine::WriteChatAddInfo(DWORD pColor, bool isBold, const char* pText, uint8 nRank)
{
	reinterpret_cast<void(__thiscall*)(DWORD, int, const std::string&, DWORD, bool, int)>(KO_CHAT_ADD_INFO)(*(DWORD*)((*(DWORD*)KO_DLG) + 0x20C), 1, std::string(pText), pColor, !isBold, nRank);
}

void HSACSEngine::WriteInfoMessage(char* pMsg, DWORD dwColor)
{
	WriteInfoMessageExt(pMsg, dwColor);
}

POINT HSACSEngine::GetUiPos(DWORD vTable)
{
	POINT tmp;
	tmp.x = 0;
	tmp.y = 0;

	if (!vTable || IsBadReadPtr((VOID*)vTable, sizeof(DWORD)))
		return tmp;

	tmp = *(POINT*)(vTable + 0xEC);
	return tmp;
}

POINT HSACSEngine::GetUiPos2(DWORD vTable)
{
	POINT pt;
	pt.x = 0;
	pt.y = 0;

	if (vTable == 0)
		return pt;

	uintptr_t** ptrVtable = (uintptr_t**)vTable;
	pt.x = (LONG)ptrVtable[59];
	pt.y = (LONG)ptrVtable[60];
	return pt;
}

RECT HSACSEngine::GetUiRegion(DWORD vTable)
{
	return *(RECT*)(vTable + 0xEC);
}

RECT HSACSEngine::GetUiRegion2(DWORD vTable)
{
	uintptr_t** ptrVtable = (uintptr_t**)vTable;

	RECT rc;
	rc.left = (LONG)ptrVtable[59];
	rc.top = (LONG)ptrVtable[60];
	rc.right = (LONG)ptrVtable[61];
	rc.bottom = (LONG)ptrVtable[62];
	return rc;
}

typedef int(__thiscall* tMoveOffset)(DWORD ecx, signed int a2, int a3);
tMoveOffset __MoveOffset = (tMoveOffset)0x4108F0; // move offset

void HSACSEngine::UiPost(DWORD vTable, LONG x, LONG y)
{

	__MoveOffset(vTable, x, y);
}

void HSACSEngine::SetUIPos(DWORD vTable, POINT pt)
{
	RECT rc = GetUiRegion(vTable);
	int dx, dy;
	dx = pt.x - rc.left;
	dy = pt.y - rc.top;

	__MoveOffset(vTable, dx, dy);
}

void HSACSEngine::GetUiPos(DWORD vTable, POINT& pt)
{
	uintptr_t** ptrVtable = (uintptr_t**)vTable;
	pt.x = (LONG)ptrVtable[59];
	pt.y = (LONG)ptrVtable[60];
}

DWORD m_iParam1;
DWORD m_iParam2;
DWORD m_iParam3;

void __declspec(naked) SetStateAsm()
{
	_asm
	{
		MOV ECX, m_dvTable
		MOV EAX, m_iParam1
		PUSH EAX
		CALL KO_UI_SET_STATE_FUNC
		RET
	}
}

void HSACSEngine::SetState(DWORD vTable, DWORD state)
{
	if (!vTable || IsBadReadPtr((VOID*)(vTable), sizeof(DWORD))) return;
	m_dvTable = vTable;
	m_iParam1 = state;
	SetStateAsm();
}

DWORD HSACSEngine::GetRecvMessagePtr(DWORD adres)
{
	if (adres == 0)
		return 0;
	return (*(DWORD*)adres) + 0x70;
}

uint32 HSACSEngine::GetState(DWORD vTable)
{
	return Read4Bytes(vTable + 0xE0);
}

void SetMemBYTE(DWORD Adres, BYTE Deger)
{
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)Adres, &Deger, 1, NULL);
}

void HSACSEngine::SetMemArray(DWORD Adres, BYTE Deger, DWORD len)
{
	for (DWORD i = 0; i < len; i++)
		SetMemBYTE(Adres + i, Deger);
}

void HSACSEngine::InitCallHook(DWORD hookFuncAddr, DWORD myFuncAddr)
{
	SetMemArray(hookFuncAddr, 0x90, 5);
	CALLHOOK(hookFuncAddr, myFuncAddr);
}

DWORD HSACSEngine::CalculateCallAddrWrite(DWORD Addr1, DWORD Addr2)
{
	return Addr1 - Addr2 - 5;
}

void MyMemcpy(LPVOID dest, const void* src, size_t size) {
	// Özel bir iþlem yapabilirsiniz, ardýndan kendi bellek kopyalama iþleminizi gerçekleþtirin
	//std::cout << "MyMemcpy fonksiyonu çaðrýldý!" << std::endl;

	// Bellek kopyalama iþlemi
	for (size_t i = 0; i < size; ++i) {
		*((BYTE*)dest + i) = *((BYTE*)src + i);
	}
}

inline void HSACSEngine::CALLHOOK(DWORD Addr1, DWORD Addr2)
{
	BYTE call[] = { 0xE8,0,0,0,0 };
	DWORD diff = CalculateCallAddrWrite(Addr2, Addr1);

	//memcpy(call + 1, &diff, 4);
	MyMemcpy(call + 1, &diff, sizeof(diff));
	WriteProcessMemory(HANDLE(-1), (LPVOID)Addr1, call, 5, 0);
}

void __stdcall CameraZoom_Hook(float fDelta)
{
	DWORD thisPtr;
	__asm
	{
		MOV thisPtr, ECX
	}

	if (g_pMain->disableCameraZoom)
		return;

	__asm
	{
		MOV ECX, thisPtr
		PUSH fDelta
		MOV EAX, KO_CAMERA_ZOOM_FUNC
		CALL EAX
	}
}

void HSACSEngine::InitCameraZoom()
{
	InitCallHook(KO_CAMERA_ZOOM_CALL_ADDR, (DWORD)CameraZoom_Hook);
}

void HSACSEngine::SendChaoticExchange(uint16 nObjectID, uint32 ExchangeRobItemID, int8 ExchangeRobItemSlot)
{
	bool bank = uiPieceChangePlug->m_bank, sell = uiPieceChangePlug->m_sell;
	
	int8 curCount = atoi(g_pMain->GetString(uiPieceChangePlug->m_textExCount).c_str());
	if (curCount < 1) curCount = 1;
	else if (curCount > 100) curCount = 100;
	
	Packet pkt(WIZ_HSACS_HOOK, uint8(HSACSOpCodes::CHAOTIC_EXCHANGE));
	pkt << nObjectID << ExchangeRobItemID << ExchangeRobItemSlot << bank << sell << curCount;
	Send(&pkt);
}
std::string HSACSEngine::exeGetName()
{
	DWORD ADDR_CHR = *(DWORD*)(KO_PTR_CHR);
	char AccName[25];
	ReadProcessMemory(GetCurrentProcess(), (LPVOID)(ADDR_CHR + KO_OFF_NAME), AccName, sizeof(AccName), NULL);
	return string(AccName);
}
const	DWORD	KO_SET_STRING_COLOR_FUNC = 0x0040F710;
const	DWORD	KO_SET_STRING_COLOR_RET_ADDR = 0x0040F716;

void HSACSEngine::SetStringColor(DWORD vTable, DWORD color) // Pm renk deðiþtirme patlama sorunu çözüldü
{
	if (vTable == 0)
		return;
	if (!IsBadReadPtr((VOID*)vTable, sizeof(DWORD)) && !IsBadReadPtr((VOID*)(vTable + 0x15C), sizeof(DWORD)))
		*(DWORD*)(vTable + 0x15C) = color;
}

std::string purchasingPriceSearch = xorstr("purchasing price");

string loadingArray[] = 
{
	xorstr("Allocating Terrain..."),
	xorstr("Loading "),
	xorstr("Loading Effect Data..."),
	xorstr("Loading River Data..."),
	xorstr("Loading Terrain Patch Data..."),
	xorstr("Loading Terrain Tile Data..."),
	xorstr("Loading Terrain Grass Data..."),
	xorstr("Loading Lightmap Data..."),
	xorstr("Loading colormap"),
	xorstr("Loading Objects..."),
	xorstr("Loading Character Data..."),
	xorstr("Loading Information") ,
	xorstr("Loading Information[1]..."),
	xorstr("Loading Information[2]..."),
	xorstr("Loading Information[3]..."),
	xorstr("Loading Information[4]..."),
	xorstr("Loading Information[5]..."),
	xorstr("Loading Information[6]..."),
	xorstr("Loading Information[7]..."),
	xorstr("Loading Information[8]..."),
	xorstr("Loading Information[9]..."),
	xorstr("Loading Information[10]...")
};

bool isFindAdress = false;
bool checkkk = false;
bool m_bFalse = false;
DWORD KO_CHR = *(DWORD*)KO_PTR_CHR;

bool MerchantPriceView(uint8& PriceType, uint32& Price)
{
	if (g_pMain->IsVisible(g_pMain->m_sMerchantDisplayData))
	{
		if (m_sSocketMerchant > 10000 
			|| m_sSocketMerchant < 1)
			return 0;

		PriceType = 0;
		Price = 0;
		uint8 getdata = 0;
		bool yoklama = false;

		for (int i = 0; i < 4; i++)
		{
			if (g_pMain->IsIn(g_pMain->m_sMerchantDisplayItem[i], g_pMain->lastMousePos.x, g_pMain->lastMousePos.y))
			{
				getdata = i;
				yoklama = true;
				break;
			}
		}
		if (yoklama)
		{
			g_pMain->pClientMerchantItemViewLock.lock();
			foreach(it, g_pMain->pClientMerchantItemView)
			{
				if (it->first != m_sSocketMerchant)
					continue;

				for (int i = 0; i < 4; i++)
				{
					if (it->second.bSrcPos[i] != getdata)
						continue;

					PriceType = it->second.isKC[i];
					Price = it->second.price[i];
					break;
				}

				if (Price > 0)
					break;
			}
			g_pMain->pClientMerchantItemViewLock.unlock();
			return true;
		}
	}
	else if (g_pMain->IsVisible(g_pMain->m_sMerchantDisplaySpecialData))
	{
		if (m_sSocketMerchantSpecial > 10000 
			|| m_sSocketMerchantSpecial < 1)
			return 0;

		PriceType = 0;
		Price = 0;
		uint8 getdata = 0;
		bool yoklama = false;

		for (int i = 0; i < 8; i++)
		{
			if (g_pMain->IsIn(g_pMain->m_sMerchantDisplaySpecialItem[i], g_pMain->lastMousePos.x, g_pMain->lastMousePos.y))
			{
				getdata = i;
				yoklama = true;
				break;
			}
		}

		if (yoklama)
		{
			g_pMain->pClientMerchantItemViewLock.lock();
			foreach(it, g_pMain->pClientMerchantItemView)
			{
				if (it->first != m_sSocketMerchantSpecial)
					continue;

				for (int i = 0; i < 8; i++)
				{
					if (it->second.bSrcPos[i] != getdata)
						continue;

					PriceType = it->second.isKC[i];
					Price = it->second.price[i];
					break;
				}

				if (Price > 0)
					break;
			}
			g_pMain->pClientMerchantItemViewLock.unlock();
			return true;
		}
	}
	return false;
}

clock_t recvTick2 = 0;
void __fastcall ThreadControlAlive()
{
	if (recvTick2 > clock() - 18000)
		return;

	recvTick2 = clock();

	if (CheckAliveTime < clock() - 18000)
	{
		ischeatactive = true;
		g_pMain->StayAlive();
	}
	else if (Real_SendTime < clock() - 18000)
	{
		ischeatactive = true;
		g_pMain->StayAlive();
	}
}

void __stdcall SetString_Hook(const std::string& szString)
{
	DWORD thisPtr;
	DWORD pClientUIBase;
	__asm
	{
		MOV thisPtr, ECX
		MOV pClientUIBase, ESI
	}

	bool m_bControl = false;
#if (HOOK_SOURCE_VERSION == 1098)	
	if (!g_pMain->m_bHookStart && g_pMain->Adress > 0 && !g_pMain->m_bLoading && !m_bFalse)		// 1098 KArakter Seçme ekraný ui pozisyonu ayarlanan yer
	{
		DWORD parenta;
		if (true)
		{
			g_pMain->GetChildByID(g_pMain->Adress, "Group_SelectWindow", parenta);
			DWORD KO_UI_SCREEN_SIZE = 0x00DE297C;
			POINT koScreen = *(POINT*)KO_UI_SCREEN_SIZE;
			RECT region = g_pMain->GetUiRegion(parenta);
			POINT orta;
			LONG w = (region.right - region.left);
			orta.x = (koScreen.x / 2) - 512;
			orta.y = region.top;
			g_pMain->SetUIPos(parenta, orta);
		}

		if (true)
		{
			DWORD parent;
			DWORD button;
			DWORD KO_UI_SCREEN_SIZE = 0x00DE297C;
			POINT koScreen = *(POINT*)KO_UI_SCREEN_SIZE;
			RECT region = g_pMain->GetUiRegion(parenta);
			g_pMain->GetChildByID(g_pMain->Adress, "Group_OtherCharacter", parent);
			g_pMain->GetChildByID(parenta, "btn_exit", button);
			POINT regiony = g_pMain->GetUiPos(button);
			regiony.x = (koScreen.x / 2) - 512;
			regiony.y -= 6;
			g_pMain->SetUIPos(parent, regiony);
		}
	}
#elif (HOOK_SOURCE_VERSION == 1534)	
	if (!g_pMain->m_bHookStart && g_pMain->Adress > 0 && !g_pMain->m_bLoading && !m_bFalse)			// 1534 KArakter Seçme ekraný ui pozisyonu ayarlanan yer
	{
		DWORD parenta;
		if (true)
		{

			g_pMain->GetChildByID(g_pMain->Adress, "Group_SelectWindow", parenta);
			DWORD KO_UI_SCREEN_SIZE = 0x00DE297C;
			POINT koScreen = *(POINT*)KO_UI_SCREEN_SIZE;
			RECT region = g_pMain->GetUiRegion(parenta);
			POINT orta;
			LONG w = (region.right - region.left);
			orta.x = (koScreen.x / 2) - 512;
			orta.y = koScreen.y - g_pMain->GetUiHeight(parenta);
			g_pMain->SetUIPos(parenta, orta);

		}

		if (true)
		{
			DWORD parent;
			g_pMain->GetChildByID(g_pMain->Adress, "Group_OtherCharacter", parent);
			DWORD KO_UI_SCREEN_SIZE = 0x00DE297C;
			POINT koScreen = *(POINT*)KO_UI_SCREEN_SIZE;
			RECT region = g_pMain->GetUiRegion(parent);
			POINT orta;
			LONG w = (region.right - region.left);
			orta.x = (koScreen.x / 2) - 512;
			orta.y = koScreen.y - g_pMain->GetUiHeight(parent) - 65;
			g_pMain->SetUIPos(parent, orta);
		}
	}
#endif

	m_strReplaceString = szString;
	if (g_pMain->m_bHookStart && g_pMain->pClientUIState->m_strFreeStatPoint == thisPtr)
	{
		int FreePoint = std::stoi(m_strReplaceString.c_str());
		if (FreePoint < 1)
			g_pMain->pClientUIState->UpdatePointButton(false);
		else 	if (FreePoint > 0)
			g_pMain->pClientUIState->UpdatePointButton(true);
	}

	if (g_pMain->m_bHookStart)
	{
		if (g_pMain->Player.GetZoneID() == ZONE_DELOS)
			g_pMain->str_replace(m_strReplaceString, "Mission termination time....", "Castle Siege War Time : ");
	}

	if (g_pMain->StringHelper->IsContains(m_strReplaceString, xorstr("PREMIUM")) && g_pMain->m_bHookStart) {
		
		if (g_pMain->GetStringElementName(thisPtr) == "text" && !g_pMain->StringHelper->IsContains(m_strReplaceString, xorstr("PC CAFE")))
		{
			DWORD pClientPremiumBase;
			std::string find = "btn_premium";
			g_pMain->GetChildByID(pClientUIBase, find, pClientPremiumBase);
			find = "btn_clanpremium";
			g_pMain->GetChildByID(pClientPremiumBase, find, pClientPremiumBase);
			g_pMain->SetVisible(pClientPremiumBase, false);
		}
	}

	for (auto a : g_pMain->privatemessages)
	{
		if (thisPtr == a->exit_id)
		{
			a->user_id = m_strReplaceString;
			a->m_bcolored = true;
		}
	}
	
	if (g_pMain->StringHelper == NULL)
		g_pMain->StringHelper = new CStringHelper();

	if (g_pMain->uiGenieSubPlug != NULL && g_pMain->uiGenieSubPlug->m_textGenieTime == thisPtr)
		m_strReplaceString = string_format("Time Left : %s Hour(s)", m_strReplaceString.c_str());

	if (!g_pMain->m_bHookStart) 
	{
		if (g_pMain->m_bGameStart 
			|| g_pMain->StringHelper->IsContains(m_strReplaceString, xorstr("Loading Information[")))
			StartHook();
	}

	for (string larr : loadingArray) 
	{
		if (g_pMain->StringHelper->IsContains(m_strReplaceString, larr))
		{
			g_pMain->m_bLoading = true;
			m_bControl = true;
		}
	}

	if (g_pMain->m_SettingsMgr != NULL)
	{
		string ServerIniVer = g_pMain->m_SettingsMgr->ClientVersion;
		if (ServerIniVer == "0")
			g_pMain->m_bLoading = false;
		
		string Ver = "Ver. ";
		string ValidVersion = Ver + "" + ServerIniVer.substr(0, 1) + "." + ServerIniVer.substr(1);
		if (m_strReplaceString == ValidVersion)
		{
			g_pMain->m_bLoading = true;
			m_bControl = true;
			g_pMain->m_bLoadingControl = true;
		}
	}
	else
	{
		g_pMain->m_bLoading = true;
		m_bControl = true;
		g_pMain->m_bLoadingControl = true;
	}

	if (g_pMain->m_bLoqOut || g_pMain->Player.isTeleporting)
	{
		g_pMain->m_bLoading = true;
		m_bControl = true;
	}

	if (!m_bControl && !g_pMain->m_bLoadingControl)
		g_pMain->m_bLoading = false;

	if (g_pMain->uiHpMenuPlug != NULL)
		g_pMain->uiHpMenuPlug->UpdatePosition();

	if (g_pMain->m_bGameStart)
	{
		if (g_pMain->uiHpMenuPlug != NULL && g_pMain->IsVisible(g_pMain->pClientUIBarPlug->m_dVTableAddr) && !g_pMain->IsVisible(g_pMain->uiHpMenuPlug->m_dVTableAddr))
		{
			g_pMain->uiHpMenuPlug->OpenHpBarMenu();
			g_pMain->pClientHookManager->ShowAllUI();
		}

		if (g_pMain->pClientMerchant != NULL)
		{
			if (g_pMain->pClientMerchant->NeedToCheckTooltip() || g_pMain->pClientMerchant->NeedToCheckDisplayTooltip())
			{
				if (g_pMain->StringHelper->IsContains(m_strReplaceString, purchasingPriceSearch))
				{
					if (g_pMain->pClientMerchant->UpdateTooltipString(m_strReplaceString)) // kc ise renk deðiþtir
						g_pMain->SetStringColor(thisPtr, 0xff7a70);
				}
			}
		}

		if ((g_pMain->m_sMerchantDisplayData > 0 && g_pMain->IsVisible(g_pMain->m_sMerchantDisplayData)) 
			|| (g_pMain->m_sMerchantDisplaySpecialData > 0 && g_pMain->IsVisible(g_pMain->m_sMerchantDisplaySpecialData)))
		{
			if (g_pMain->str_contains(m_strReplaceString, purchasingPriceSearch) && !g_pMain->pClientMerchant->IsMerchanting())
			{
				uint32 nPrice;
				uint8 isKC;
				if (MerchantPriceView(isKC, nPrice))
				{
					if (isKC)
						m_strReplaceString = string_format("Knight Cash Price : %s", g_pMain->number_format(nPrice, false).c_str());
					else
						m_strReplaceString = string_format("Purchasing Price : %s", g_pMain->number_format(nPrice, false).c_str());
				}
			}
		}
	}

	_asm
	{
		MOV ECX, thisPtr
		MOV EAX, OFFSET m_strReplaceString

		PUSH EAX
		CALL KO_SET_STRING_FUNC
		call ThreadControlAlive
	}
}

void HSACSEngine::InitSetString()
{
	*(DWORD*)KO_SET_STRING_PTR = (DWORD)SetString_Hook;
}

void __declspec(naked) IsInAsm()
{
	_asm
	{
		MOV ECX, [m_dvTable]
		MOV EAX, m_iParam2
		PUSH EAX
		MOV EAX, m_iParam1
		PUSH EAX
		CALL KO_UIBASE_IS_IN_FUNC
		MOV m_dBool, EAX
		RET
	}
}


void HSACSEngine::SetItemFlag(DWORD vTable,uint8 flag)
{
	DWORD tmp = vTable;
	if (!tmp) return;
	__asm {
		mov ecx, tmp
		movsx eax, flag
		push eax
		call KO_UIF_SET_ITEM_FLAG
	}
}

bool HSACSEngine::IsIn(DWORD vTable, int x, int y)
{
	if (vTable == 0)
		return false;

	POINT pt = GetUiPos(vTable);
	LONG w = GetUiWidth(vTable);
	LONG h = GetUiHeight(vTable);
	RECT rp;
	rp.left = pt.x;
	rp.right = pt.x + w;
	rp.top = pt.y;
	rp.bottom = pt.y + h;
	return lastMousePos.x >= rp.left && lastMousePos.x <= rp.right && lastMousePos.y >= rp.top && lastMousePos.y <= rp.bottom;
}

bool HSACSEngine::fileExist(const char* fileName)
{
	std::ifstream infile(string(m_BasePath + fileName).c_str());
	return infile.good();
}

LONG HSACSEngine::GetUiWidth(DWORD vTable)
{
	uintptr_t** ptrVtable = (uintptr_t**)vTable;
	LONG right = (LONG)ptrVtable[61];
	LONG left = (LONG)ptrVtable[59];
	return right - left;
}

LONG HSACSEngine::GetUiHeight(DWORD vTable)
{
	uintptr_t** ptrVtable = (uintptr_t**)vTable;
	LONG bottom = (LONG)ptrVtable[62];
	LONG top = (LONG)ptrVtable[60];
	return bottom - top;
}

size_t HSACSEngine::GetScrollValue(DWORD vTable)
{
	DWORD track = *(DWORD*)(vTable + 0x134);
	return track ? *(uint32*)(track + 0x144) : 0;
}

bool HSACSEngine::IsitaSandBox()
{
	unsigned char bBuffering;
	unsigned long aCreateProcesses = (unsigned long)GetProcAddress(GetModuleHandleA(xorstr("KERNEL32.dll")), xorstr("CreateProcessA"));

	ReadProcessMemory(GetCurrentProcess(), (void*)aCreateProcesses, &bBuffering, 1, 0);

	if (bBuffering == 0xE9)
		return  1;
	else
		return 0;
}

bool HSACSEngine::IsHWBreakpointExists()
{
	CONTEXT ctx;
	ZeroMemory(&ctx, sizeof(CONTEXT));
	ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	HANDLE hThread = GetCurrentThread();
	if (GetThreadContext(hThread, &ctx) == 0)
		return false;

	if ((ctx.Dr0) || (ctx.Dr1) || (ctx.Dr2) || (ctx.Dr3))
		return true;
	else
		return false;
}

DWORD KO_ADR_CHR = 0x0;
DWORD KO_ADR_DLG = 0x0;

BYTE ReadByte(DWORD vTable)
{
	return *(byte*)vTable;
}

long ReadLong(DWORD vTable)
{
	return *(long*)vTable;
}

typedef void(__thiscall* tSetCurValue)(DWORD uiObject, float a1, float a2, float a3);
tSetCurValue Func_SetCurValue = (tSetCurValue)0x424890;

void HSACSEngine::SetCurValue(DWORD dwTable, float value, float changeSpeed)
{
	Func_SetCurValue(dwTable, value, 0, changeSpeed);
}

void HSACSEngine::SetProgressRange(DWORD uif, float min, float max)
{
	DWORD tmp = uif;
	*(DWORD*)(uif + 0x150) = min;
	*(DWORD*)(uif + 0x14C) = max;
	__asm {
		mov eax, [tmp]
		mov eax, [eax]
		mov eax, [eax + 0xB0]
		mov ecx, tmp
		call eax
	}
}

void HSACSEngine::ChangeRunState()
{
	__asm {
		mov ecx, [KO_PTR_CHR]
		mov ecx, [ecx]
		mov eax, 0x00567E50
		call eax
	}
}

void HSACSEngine::CommandCameraChange()
{
	__asm {
		push -1
		mov ecx, [0xF368E0]
		mov ecx, [ecx]
		mov eax, 0x00A7C300
		call eax
	}
}


long HSACSEngine::SelectRandomMob()
{
	__asm {
		MOV ECX, KO_DLG
		MOV ECX, DWORD PTR DS : [ECX]
		MOV EAX, KO_FNCZ
		CALL EAX
	}
}

string HSACSEngine::number_format(int dwNum, bool dot)
{
	std::string value = std::to_string(dwNum);
	int len = value.length();
	int dlen = 3;

	while (len > dlen)
	{
		value.insert(len - dlen, 1, dot ? '.' : ',');
		dlen += 4;
		len += 1;
	}
	return value;
}

void HSACSEngine::str_tolower(std::string& str)
{
	for (size_t i = 0; i < str.length(); ++i)
		str[i] = (char)tolower(str[i]);
}

bool HSACSEngine::str_contains(std::string str, std::string find)
{
	std::string s = str;
	str_tolower(s);

	std::string f = find;
	str_tolower(f);

	if (s.find(f) != std::string::npos)
		return true;
	return false;
}

std::string HSACSEngine::GetItemDXT(int nItemID)
{
	TABLE_ITEM_BASIC* pTbl = g_pMain->pClientTBLManager->GetItemData(nItemID);
	if (pTbl == nullptr)
		return "UI\\itemicon_noimage.dxt";
	
	std::vector<char> buffer(256, NULL);
	if (pTbl->Num > 999999999)
		sprintf(&buffer[0], "ui\\itemicon_%.2d_%.4d_%.2d_%.1d.dxt", (pTbl->dxtID / 10000000), (pTbl->dxtID / 1000) % 10000, (pTbl->dxtID / 10) % 100, pTbl->dxtID % 10);
	else
		sprintf(&buffer[0], "ui\\itemicon_%.1d_%.4d_%.2d_%.1d.dxt", (pTbl->dxtID / 10000000), (pTbl->dxtID / 1000) % 10000, (pTbl->dxtID / 10) % 100, pTbl->dxtID % 10);
	
	std::string szIconFN = &buffer[0];
	//printf("GetItemDXT %s\n", szIconFN.c_str());
	if(szIconFN.empty())
		return "UI\\itemicon_noimage.dxt";

	return szIconFN;
}

void HSACSEngine::str_replace(std::string& str, std::string find, std::string replace)
{
	if (find.empty())
		return;

	size_t start_pos = 0;
	while ((start_pos = str.find(find, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, find.length(), replace);
		start_pos += replace.length();
	}
}

void HSACSEngine::str_replace(std::string& str, char find, char replace)
{
	std::replace(str.begin(), str.end(), (char)25, (char)39); // replace all 'x' to 'y'
}

void HSACSEngine::str_split(std::string const& str, const char delim, std::vector<std::string>& out)
{
	size_t start;
	size_t end = 0;

	while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
	{
		end = str.find(delim, start);
		out.push_back(str.substr(start, end - start));
	}
}

void HSACSEngine::str_split(std::string str, std::string delim, std::vector<std::string>& out)
{
	size_t pos_start = 0, pos_end, delim_len = delim.length();
	std::string token;
	while ((pos_end = str.find(delim, pos_start)) != std::string::npos)
	{
		token = str.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		out.push_back(token);
	}
	out.push_back(str.substr(pos_start));
}

std::vector<std::string> HSACSEngine::str_split(std::string str, std::string regex)
{
	std::smatch m;
	std::regex e(regex);
	std::vector<std::string> ret;

	while (std::regex_search(str, m, e))
	{
		for (string x : m)
			ret.push_back(x);
		str = m.suffix().str();
	}

	return ret;
}

std::string HSACSEngine::KoRandomNameUIF(std::string uif)
{
	srand((unsigned)time(NULL) * getpid());
	int random1 = rand() % 10000;
	int random2 = rand() % 100;
	std::string text = string_format("%s_%d%d.uif", uif.c_str(), random1, random2);
	return text;
}

extern HCRYPTKEY hKey;

std::string HSACSEngine::dcpUIF(std::string path)
{
	std::string rtn = "";
	if (!path.empty())
	{
		rtn = KoRandomNameUIF(path);
		PBYTE pbBuffer = NULL;
		HANDLE hSourceFile = CreateFileA(path.c_str(), FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		HANDLE hDestinationFile = CreateFileA(rtn.c_str(), FILE_WRITE_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
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
				{
					WriteFile(hDestinationFile, pbBuffer, dwCount, &dwCount, NULL);
				}
				ZeroMemory(pbBuffer, dwBufferLen);
				dwCount = 0;
				do
				{
					if (ReadFile(hSourceFile, pbBuffer, dwBlockLen, &dwCount, NULL))
					{
						if (dwCount < dwBlockLen)
						{
							fEOF = TRUE;
						}
						if (CryptDecrypt(hKey, 0, 1, 0, pbBuffer, &dwCount))
						{
							WriteFile(hDestinationFile, pbBuffer, dwCount, &dwCount, NULL);
						}
					}
				} while (!fEOF);
			}
		}
		if (pbBuffer)
		{
			free(pbBuffer);
		}
		if (hSourceFile)
		{
			CloseHandle(hSourceFile);
		}
		if (hDestinationFile)
		{
			CloseHandle(hDestinationFile);
		}
	}
	return rtn;
}

typedef void(__thiscall* tSetEditString)(DWORD ecx, const std::string& szString);
tSetEditString __SetEditString = (tSetEditString)0x4189A0;
typedef const std::string& (__thiscall* tGetEditString)(DWORD ecx);
tGetEditString __GetEditString = (tGetEditString)0x416960;

std::string HSACSEngine::GetEditString(DWORD vTable)
{
	return __GetEditString(vTable);
}

void HSACSEngine::SetEditString(DWORD vTable, const std::string& szString)
{
	__SetEditString(vTable, szString);
}

void ShowToolTip(__IconItemSkillEx* spItem, LONG x, LONG y)
{
	DWORD tooltipUI = 0;
	__asm {
		mov ecx, [KO_UI_DLG]
		mov ecx, [ecx]
		mov tooltipUI, ecx
	}

	int _type = 0;
	LONG _x = x, _y = y;

	__asm {
		push 0
		push 0
		push 1
		push 0 // istedigi np gosterilsin mi?
		push 1 
		push 1 // 1 selling price // 2 buying price
		push 0 // show price
		push _type // inventory / merchant / drop hangisiysey onu yazar
		push spItem
		push _y
		push _x
		mov ecx, tooltipUI
		call KO_UI_TOOLTIP_FNC
	}
}

void HSACSEngine::_ShowToolTip(DWORD spItem, LONG x, LONG y, TOOLTIP_TYPE type)
{
	DWORD tooltipUI = 0;
	__asm {
		mov ecx, [KO_UI_DLG]
		mov ecx, [ecx]
		mov tooltipUI, ecx
	}

	int _type = (int)type;
	LONG _x = x, _y = y;

	__asm {
		push 0
		push 0
		push 1
		push 0 // istedigi np gosterilsin mi
		push 0
		push 1 // 1 = selling price, 2 = buy price
		push 0 // show price
		push _type // tip ? yani inventory, merchant, drop
		push spItem
		push _y
		push _x
		mov ecx, tooltipUI
		call KO_UI_TOOLTIP_FNC
	}
}

void __declspec(naked) SetN3UIIcon(DWORD* uif)
{
	__asm {
		push ebp
		mov ebp, esp
		push esi
		mov esi, [ebp + 8]
		mov ecx, [esi]
		call KO_UIF_SET_N3ICON
		mov[esi], eax
		pop esi
		pop ebp
		ret
	}
}

void __declspec(naked) SetUVRect(DWORD uif, float left, float top, float right, float bottom)
{
	__asm {
		push ebp
		mov ebp, esp
		movss xmm0, [ebp + 0x18]
		sub esp, 0x20
		mov ecx, [ebp + 8]
		mov[esp + 0x1C], 0
		mov[esp + 0x18], 0
		mov[esp + 0x14], 0
		mov[esp + 0x10], 0
		movss[esp + 0xC], xmm0
		movss xmm0, [ebp + 0x14]
		movss[esp + 8], xmm0
		movss xmm0, [ebp + 0x10]
		movss[esp + 4], xmm0
		movss xmm0, [ebp + 0xC]
		movss[esp], xmm0
		call dword ptr[KO_UIF_IMG_SETUVRECT]
		pop ebp
		ret
	}
}

DWORD __fastcall __new(size_t allocSize)
{
	DWORD result = 0;
	try {
		if (result = (DWORD)malloc(allocSize)) memset((void*)result, 0, allocSize);
	}
	catch (const std::bad_alloc&) {
		return NULL;
	}
	return result;
}

void SetParent(DWORD uif, DWORD parent)
{
	__asm {
		mov ecx, uif
		mov eax, [ecx]
		mov eax, [eax + 0x80]
		push parent
		call eax
	}
}

extern TABLE_ITEM_BASIC* GetItemBase(DWORD ItemID);
extern TABLE_ITEM_EXT* GetItemExtBase(DWORD dwItemNum, DWORD bExtIndex);

DWORD InitIcon(DWORD itemID)
{
	DWORD parent = *(DWORD*)KO_UI_MGR;
	CItem* item = (CItem*)GetItemBase(itemID);
	if (!item) return false;
	CItemExt* itemExt = (CItemExt*)GetItemExtBase(itemID, item->extNum);
	DWORD dwIconID = item->dxtID;
	if (itemExt && itemExt->iconID > 1000000)
		dwIconID = itemExt->iconID;

	std::string dxt = string_format(xorstr("UI\\ItemIcon_%.1d_%.4d_%.2d_%.1d.dxt"), dwIconID / 10000000u, dwIconID / 1000u % 10000, dwIconID / 10u % 100, dwIconID % 10);

	DWORD icon = __new(0x2A0);
	SetN3UIIcon(&icon);
	SetParent(icon, parent);
	g_pMain->SetTexImage(icon, dxt);
	SetUVRect(icon, 0.0f, 0.0f, 0.703125f, 0.703125f);
	*(DWORD*)(icon + 0xDC) = 11;
	__asm {
		mov ecx, icon
		push 0x110
		mov eax, [ecx]
		mov eax, [eax + 0x4C]
		call eax
		mov ecx, icon
		push 0
		mov eax, [ecx]
		mov eax, [eax + 0x50]
		call eax
	}
	return icon;
}

bool HSACSEngine::_InitIcon(DWORD& iconObject, DWORD parent, DWORD itemID)
{
	CItem* item = (CItem*)GetItemBase(itemID);
	if (!item)
		return false;

	CItemExt* itemExt = (CItemExt*)GetItemExtBase(itemID, item->extNum);
	if (!itemExt)
		return false;

	DWORD dwIconID = item->dxtID;
	if (itemExt->dxtID > 0)
		dwIconID = itemExt->dxtID;

	std::string dxt = "";
	if (itemID / 1000000000 - 1 > 2)
		dxt = string_format(xorstr("UI\\ItemIcon_%.1d_%.4d_%.2d_%.1d.dxt"), dwIconID / 10000000u, dwIconID / 1000u % 10000, dwIconID / 10u % 100, dwIconID % 10);
	else
		dxt = string_format(xorstr("UI\\ItemIcon_%.2d_%.4d_%.2d_%.1d.dxt"), dwIconID / 10000000u, dwIconID / 1000u % 10000, dwIconID / 10u % 100, dwIconID % 10);

	if (iconObject)
	{
		DWORD icon = iconObject;
		SetParent(icon, parent);
		SetTex(icon, dxt);
		SetUVRect(icon, 0.0f, 0.0f, 0.703125f, 0.703125f);
		__asm {
			mov ecx, icon
			push 1
			mov eax, [ecx]
			mov eax, [eax + 0x50]
			call eax
		}
		iconObject = icon;
		return true;
	}

	DWORD icon = __new(0x2A0);
	SetN3UIIcon(&icon);
	SetParent(icon, parent);
	g_pMain->SetTexImage(icon, dxt);
	SetUVRect(icon, 0.0f, 0.0f, 0.703125f, 0.703125f);
	*(DWORD*)(icon + 0xDC) = 11; // SetUIType(UI_TYPE_ICON);
	__asm { // SetStyle(UISTYLE_ICON_ITEM|UISTYLE_ICON_CERTIFICATION_NEED);
		mov ecx, icon
		push 0x110
		mov eax, [ecx]
		mov eax, [eax + 0x4C]
		call eax
		// -------------- setvisible true
		mov ecx, icon
		push 1
		mov eax, [ecx]
		mov eax, [eax + 0x50]
		call eax
	}
	iconObject = icon;
	return true;
}


void __declspec(naked) SetIconItemSkill(__IconItemSkillEx* uif)
{
	__asm {
		push ebp
		mov ebp, esp
		mov ecx, [ebp + 8]
		call KO_UIF_ICONITEMSKILL_SET
		pop ebp
		ret
	}
}

void HSACSEngine::SetRegion(DWORD dwTable, DWORD uif)
{
	if (!dwTable)return;
	DWORD tmpTp = dwTable;
	DWORD otherRect = uif + UI_POS;
	__asm {
		mov ecx, tmpTp
		mov eax, [ecx]
		mov eax, [eax + 0x38]
		push otherRect
		call eax
	}
	*(RECT*)(dwTable + 0xFC) = *(RECT*)(dwTable + UI_POS);
}

void __declspec(naked) _SetIconItemSkill(DWORD* uif)
{
	__asm {
		push ebp
		mov ebp, esp
		push esi
		mov esi, [ebp + 8]
		mov ecx, [esi]
		call KO_UIF_ICONITEMSKILL_SET
		pop esi
		pop ebp
		ret
	}
}

void __declspec(naked) _IntoString(DWORD dwTable, std::string& szString)
{
	__asm {
		push ebp
		mov ebp, esp
		mov ecx, [ebp + 8]
		push[ebp + 0xC]
		call KO_UIF_LOADSTRING
		pop ebp
		ret
	}
}

DWORD HSACSEngine::_CreateIconItemSkill(DWORD self, DWORD UIIcon, DWORD itemID, short count, uint32 expiration, uint16 rental_remTime, uint32 rental_period, uint32 rental_price)
{
	CItem* item = (CItem*)GetItemBase(itemID);
	if (!item)
		return NULL;

	CItemExt* itemExt = (CItemExt*)GetItemExtBase(itemID, item->extNum);
	if (!itemExt)
		return NULL;

	DWORD dwIconID = item->dxtID;
	if (itemExt && itemExt->iconID > 1000000)
		dwIconID = itemExt->dxtID;

	std::string dxt = string_format(xorstr("UI\\ItemIcon_%.1d_%.4d_%.2d_%.1d.dxt"), dwIconID / 10000000u, dwIconID / 1000u % 10000, dwIconID / 10u % 100, dwIconID % 10);

	DWORD spItem = self;
	if (!spItem) {
		DWORD tmp = __new(0x78);
		if (tmp)
		{
			__asm {
				mov ecx, tmp
				call KO_UIF_ICONITEMSKILL_CREATE
				mov spItem, eax
			}
		}
	}

	if (!spItem) return NULL;

	__asm {
		mov ecx, spItem
		call KO_UIF_ICONITEMSKILL_SET
	}

	if (itemID == itemID)
		count = 0;

	_SetIconItemSkill(&spItem);
	_IntoString(spItem + KO_UIF_ICONITEMSKILL_OFF_DXT, dxt);
	*(uint16*)(spItem + KO_UIF_ICONITEMSKILL_OFF_RENT_TIME) = rental_remTime; // rental remaining time uint16
	*(uint32*)(spItem + KO_UIF_ICONITEMSKILL_OFF_RENT_PREIOD) = rental_period; // rental period minute uint32
	*(uint32*)(spItem + KO_UIF_ICONITEMSKILL_OFF_RENT_PRICE) = rental_price; // rental price uint32
	*(uint32*)(spItem + KO_UIF_ICONITEMSKILL_OFF_EXPIRATION) = expiration; // expiration time
	*(DWORD*)(spItem + KO_UIF_ICONITEMSKILL_OFF_ITEM) = (DWORD)item;
	*(DWORD*)(spItem + KO_UIF_ICONITEMSKILL_OFF_ITEM_EXT) = (DWORD)itemExt;
	*(short*)(spItem + KO_UIF_ICONITEMSKILL_OFF_ITEM_COUNT) = count;
	*(short*)(spItem + KO_UIF_ICONITEMSKILL_OFF_ITEM_DURATION) = item->Duration + itemExt->siMaxDurability;
	*(DWORD*)spItem = UIIcon;
	return spItem;
}

__IconItemSkillEx* CreateIconItemSkill(DWORD UIIcon, DWORD itemID, short count, uint32 expiration, uint16 rental_remTime, uint32 rental_period, uint32 rental_price)
{
	CItem* item = (CItem * )GetItemBase(itemID);
	if (!item)
		return NULL;

	CItemExt* itemExt = (CItemExt *)GetItemExtBase(itemID, item->extNum);

	DWORD dwIconID = item->dxtID;
	if (itemExt && itemExt->iconID > 1000000)
		dwIconID = itemExt->iconID;

	std::string dxt = string_format(xorstr("UI\\ItemIcon_%.1d_%.4d_%.2d_%.1d.dxt"), dwIconID / 10000000u, dwIconID / 1000u % 10000, dwIconID / 10u % 100, dwIconID % 10);

	__IconItemSkillEx* spItem = NULL;

	DWORD tmp = __new(0x78);
	if (tmp)
	{
		__asm {
			mov ecx, tmp
			call KO_UIF_ICONITEMSKILL_CREATE
			mov spItem, eax
		}
	}

	if (!spItem) return NULL;

	__asm {
		mov ecx, spItem
		call KO_UIF_ICONITEMSKILL_SET
	}

	SetIconItemSkill(spItem);

	spItem->pUIIcon = UIIcon;
	spItem->szIconFN = dxt;
	spItem->iRentTime = rental_remTime;
	spItem->iRentPeriod = rental_period;
	spItem->iRentPrice = rental_price;
	spItem->iExpiration = expiration;
	spItem->pItemBasic = item;
	spItem->pItemExt = itemExt;
	spItem->iCount = count;
	spItem->iDurability = item->Duration + (itemExt ? itemExt->siMaxDurability : 0);
	return spItem;
}

std::map <uint32, __IconItemSkillEx*> iconItemMap;

void HSACSEngine::ShowToolTipEx(uint32 itemID, LONG x, LONG y)
{
	auto itr = iconItemMap.find(itemID);
	if (itr != iconItemMap.end())
	{
		ShowToolTip(itr->second, x, y);
		return;
	}

	if (DWORD icon = InitIcon(itemID))
	{
		if (__IconItemSkillEx* spItem = CreateIconItemSkill(icon, itemID, 1, 0, 0, 0, 0))
		{
			iconItemMap.insert(std::make_pair(itemID, spItem));
			ShowToolTip(spItem, x, y);
		}
	}
}