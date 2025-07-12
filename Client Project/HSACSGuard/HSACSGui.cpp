#include "HSACSGui.h"
#include "Timer.h"
#include "resource.h"
#include <future>
#include "CSpell.h"
#include "FunctionGuard.h"
#include "../resource.h"
using namespace std;
extern bool ischeatactive;
HRESULT WINAPI HookCreateDevice();

#define HOOK(func,addy)	o##func = (t##func)DetourFunction((PBYTE)addy,(PBYTE)hk##func)
#define D3D_RELEASE(D3D_PTR) if( D3D_PTR ){ D3D_PTR->Release(); D3D_PTR = NULL; }
#define ES	0
#define DIP	1
#define RES 2
#define FOX 3
HRESULT WINAPI hkReset(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);
typedef HRESULT(WINAPI* tEndScene)(LPDIRECT3DDEVICE9);
typedef HRESULT(WINAPI* tReset)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);
typedef HRESULT(WINAPI* mySendMessage) (HWND, UINT, WPARAM, LPARAM);

typedef HRESULT(WINAPI* CreateDevice_t)(IDirect3D9* Direct3D_Object, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow,
	DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters,
	IDirect3DDevice9** ppReturnedDeviceInterface);
HRESULT WINAPI D3DCreateDevice_hook(IDirect3D9 * Direct3D_Object, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow,
	DWORD BehaviorFlags, D3DPRESENT_PARAMETERS * pPresentationParameters,
	IDirect3DDevice9 * *ppReturnedDeviceInterface);

CreateDevice_t D3DCreateDevice_orig;

typedef HRESULT(WINAPI* tSetScissorRect)(LPDIRECT3DDEVICE9 pDevice, D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9* ppSB);
tSetScissorRect  oSetScissorRect;
#include <tlhelp32.h>
bool IsReshadeDllLoaded() {
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;

	// Take a snapshot of all modules in the current process
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	if (hModuleSnap == INVALID_HANDLE_VALUE) {
		return false;
	}

	// Set the size of the structure before using it
	me32.dwSize = sizeof(MODULEENTRY32);

	// Retrieve information about the first module
	if (!Module32First(hModuleSnap, &me32)) {
		CloseHandle(hModuleSnap);
		return false;
	}

	// Now walk the module list of the process
	do {
		if (_tcsicmp(me32.szModule, _T("dxgi.dll")) == 0) {
			CloseHandle(hModuleSnap);
			return true;
		}
	} while (Module32Next(hModuleSnap, &me32));

	// Clean up the snapshot object
	CloseHandle(hModuleSnap);
	return false;
}

PDWORD IDirect3D9_vtable = NULL;
#define CREATEDEVICE_VTI 16

DWORD VTable[3] = { 0 };
DWORD D3DEndScene;
DWORD D3DReset;
tReset oReset;
tEndScene oPresent;
tEndScene oEndScene;
WNDPROC oWndProc;
static int myfpslimit = 60;
static bool isfpslimit = false;
static bool sendHooked = false;
static bool reset = false;
static bool itsMe = false;
static bool reshadeLoaded = false;

LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;
LPDIRECT3DINDEXBUFFER9  g_pIB = NULL;

HWND window = 0;
NOTIFYICONDATA nid;
bool hiddenWindow = false;

HRESULT WINAPI SetScissorRect(LPDIRECT3DDEVICE9 pDevice, D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9* ppSB)
{
	HRESULT ret = oSetScissorRect(pDevice, Type, ppSB);

	if (reshadeLoaded)
		ischeatactive = false;
	else
		ischeatactive = true;

	return ret;
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	

	switch (uMsg)
	{
	case WM_APP + 1:
		switch (lParam)
		{
		case WM_LBUTTONDBLCLK:
			if (g_pMain)
				if (!IsWindowVisible(window)) {
					ShowWindow(window, SW_SHOW);
					hiddenWindow = false;
				}
			break;
		}
		break;
	}
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
	//return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HRESULT WINAPI hkReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* Present)
{
	if (g_pMain->m_SettingsMgr == NULL)
		g_pMain->m_SettingsMgr = new CSettingsManager();

	Present->Windowed = g_pMain->m_SettingsMgr->m_iRealFullScreen == 1 ? false : true;
	Present->PresentationInterval = g_pMain->m_SettingsMgr->m_iVsync == 1 ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
	
	if (g_pMain->m_SettingsMgr->m_iVsync == 1)
		Present->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	reset = true;
	return oReset(pDevice, Present);
}


extern CSpell* GetSkillBase(int iSkillID);
DWORD adresdd = 0x33C;
const DWORD KOKOKO1 = 0x006CA150;					// 2369
const DWORD KOKOKO2 = 0x00F3690C;					// 2369
ULONGLONG thtime = GetTickCount64();

void WriteChatAddInfo(DWORD pColor, bool isBold, const char* pText, uint8 nRank)
{
	reinterpret_cast<void(__thiscall*)(DWORD, const std::string&, DWORD, bool)>(KOKOKO1)(*(DWORD*)((*(DWORD*)KOKOKO2) + 0x1EC), std::string(pText), pColor, !isBold);
}

HRESULT WINAPI hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if(!g_pMain->render)
		return oEndScene(pDevice);

	if (!g_pMain->power)
		return (BYTE)0x90;
	
	if (GetAsyncKeyState(VK_F12) & 1)
	{
		if (GetForegroundWindow() == window) 
		{
			ShowWindow(window, false);
			hiddenWindow = true;
		}
	}

	//if (GetAsyncKeyState(VK_DELETE) & 1)
	//{
	//	//WriteChatAddInfo(0xFFFF3B3B, 0, "TEST", 0);
	//}

	if (hiddenWindow) Sleep(1000 / 60); //f12 basinca cpu düþürme

	if (!sendHooked) 
	{
		sendHooked = true;
		g_pMain->pClientHookManager = new CUIManager();
		g_pMain->pClientHookManager->Init(pDevice);
		g_pMain->InitSendHook();
		g_pMain->InitRecvHook();
		g_pMain->InitSetString();

		for (uint32 i = 101001; i <= 610159; i++)
		{
			CSpell* spell = GetSkillBase(i);
			if (spell) {
				g_pMain->skillmap.insert({ spell->dwID, *spell });
				SpellCRC crc(crc32((uint8*)(DWORD)spell, 0xA8, -1), crc32((uint8*)((DWORD)spell + 0xB4), 0x2C, -1));
				g_pMain->skillcrc.insert({ spell->dwID + 2031, crc });
				g_pMain->skillmapBackup.insert({ spell->dwID, *spell });
			}
		}
#if (HOOK_MODE == 1)	
		reshadeLoaded = IsReshadeDllLoaded(); // Call this once during initialization
#endif
	}

	if (reset)
	{
		reset = false;
		if (g_pMain->uiTaskbarMain) 
			g_pMain->uiTaskbarMain->UpdatePosition();
	}


	if (!g_pMain->drawMode || g_pMain->m_bLoading == true)
		return oEndScene(pDevice);

	if (g_pMain->uiLogin == NULL)
	{
		g_pMain->uiLogin = new CUILogin();
		if (g_pMain->uiLogin->m_bGroupLogin == NULL)
			g_pMain->uiLogin = NULL;
	}

	if (g_pMain->uiSeedHelperPlug != NULL)
		g_pMain->uiSeedHelperPlug->Tick();

	if (g_pMain->pClientUIState != NULL)
		g_pMain->pClientUIState->Tick();

	if (g_pMain->pClientTradePrice != NULL)
		g_pMain->pClientTradePrice->Tick();

	if (g_pMain->uiSkillPage != NULL)
		g_pMain->uiSkillPage->Tick();

	if (g_pMain->uiScoreBoard) g_pMain->uiScoreBoard->Tick();

	if (g_pMain->pClientHookManager != NULL) {
		g_pMain->pClientHookManager->Tick();
		g_pMain->pClientHookManager->Render();
	}

	if (g_pMain->pClientLottyEvent != NULL)
		g_pMain->pClientLottyEvent->Tick();

	if (g_pMain->uiCindirella != NULL)
		g_pMain->uiCindirella->Tick();

	if (g_pMain->pClientUIBarPlug != NULL)
		g_pMain->pClientUIBarPlug->Tick();

	if (g_pMain->pClientWheelOfFunPlug != NULL)
		g_pMain->pClientWheelOfFunPlug->Tick();

	if (g_pMain->pClientCollection != NULL)
	{
		if (g_pMain->m_cCollettionStatus)
			g_pMain->pClientCollection->Tick();
	}
#if (HOOK_SOURCE_VERSION == 2369)
	if (g_pMain->pClientEventHandler != NULL)
	{
		if (g_pMain->pClientEventSystem.m_bStatus)
			g_pMain->pClientEventHandler->Tick();
	}
#endif
	if (GetTickCount64() > thtime) 
	{
		if (g_pMain->MainThread) 
			ResumeThread(g_pMain->MainThread);

		if (g_pMain->AliveThread) 
			ResumeThread(g_pMain->AliveThread);

		if (g_pMain->ScanThread) 
			ResumeThread(g_pMain->ScanThread);

		if (g_pMain->TitleThread) 
			ResumeThread(g_pMain->TitleThread);

		if (g_pMain->LisansThread) 
			ResumeThread(g_pMain->LisansThread);

		thtime = GetTickCount64() + 1000;
	}
	
	if (g_pMain->pClientEvetShowList != NULL)
		g_pMain->pClientEvetShowList->Tick();

	if (g_pMain->uiQuestPage != NULL)
		g_pMain->uiQuestPage->Tick();

	if (g_pMain->uiGenieMain != NULL)
		g_pMain->uiGenieMain->Tick();

//#if(HOOK_SOURCE_VERSION == 2369)
	if (g_pMain->pClientDailyReward != NULL)
		g_pMain->pClientDailyReward->Tick();

	if (GetAsyncKeyState(VK_MENU) & 0x8000) 
	{
		/*Packet pkt = Packet(WIZ_MAGIC_PROCESS);

		pkt
			<< uint8_t(1)
			<< uint32(108010)
			<< uint16(1)
			<< uint16(1)
			<< uint16(0) << uint16(0) << uint16(0) << uint16(0) << uint16(0) << uint16(0) << uint16(0);

		g_pMain->Send(&pkt);*/
	}
//#endif
	return oEndScene(pDevice);
}

LRESULT CALLBACK MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void DX_Init(DWORD * table)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, xorstr("DX"), NULL };
	RegisterClassEx(&wc);
	HWND hWnd = CreateWindow(xorstr("DX"), NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, GetDesktopWindow(), NULL, wc.hInstance, NULL);
	LPDIRECT3D9 pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	LPDIRECT3DDEVICE9 pd3dDevice;
	itsMe = true;
	pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, &pd3dDevice);
	itsMe = false;
	DWORD * pVTable = (DWORD*)pd3dDevice;
	pVTable = (DWORD*)pVTable[0];

	table[FOX] = pVTable[17];
	table[ES]  = pVTable[42];
	table[DIP] = pVTable[82];
	table[RES] = pVTable[16];

	DestroyWindow(hWnd);
}
DWORD FindDevice(DWORD Len)
{
	DWORD dwObjBase = 0;

	dwObjBase = (DWORD)LoadLibraryA("d3d9.dll");
	while (dwObjBase++ < dwObjBase + Len)
		if ((*(WORD*)(dwObjBase + 0x00)) == 0x06C7 && (*(WORD*)(dwObjBase + 0x06)) == 0x8689 && (*(WORD*)(dwObjBase + 0x0C)) == 0x8689) {
			dwObjBase += 2; break;
		}

	return(dwObjBase);
}

DWORD GetDeviceAddress(int VTableIndex)
{
	PDWORD VTable;
	*(DWORD*)&VTable = *(DWORD*)FindDevice(0x128000);
	return VTable[VTableIndex];
}
DWORD WINAPI InitGUI()
{
	while (GetModuleHandle(xorstr("d3d9.dll")) == NULL) 
		Sleep(250);

	DX_Init(VTable);

	HOOK(EndScene, VTable[ES]);
	HOOK(Reset, VTable[RES]);

	char buff[50];
	ZeroMemory(buff, 50);
	sprintf_s(buff, "HSACSX Client[%d]", GetCurrentProcessId()); //old version client name change	
	oSetScissorRect = (tSetScissorRect)DetourFunction((PBYTE)GetDeviceAddress(59), (PBYTE)SetScissorRect);
	window = NULL;
	while (window == NULL) {
		Sleep(100);
		window = FindWindowA(NULL, buff); //old version client name change

	}

	oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc);
	std::memset(&nid, 0, sizeof(nid));
	nid.cbSize = sizeof(nid);
	nid.hWnd = window;
	nid.uID = 0;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_APP + 1;
	nid.hIcon = LoadIcon(GetModuleHandle("HSACSX.dll"), MAKEINTRESOURCE(IDI_ICON1));
	lstrcpy(nid.szTip, buff);
	Shell_NotifyIcon(NIM_ADD, &nid);
	Shell_NotifyIcon(NIM_SETVERSION, &nid);

	return 0;
}


void UIMain() 
{
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InitGUI, NULL, NULL, NULL);
}


BYTE ByteExt(DWORD ulBase)
{
	if (!IsBadReadPtr((VOID*)ulBase, sizeof(BYTE)))
	{
		return(*(BYTE*)(ulBase));
	}
	return 0;
}

const DWORD KO_MULTI_CAP = 0xC7C25C;// 0x00bb4c64;
const DWORD KO_MUTEX = 0xA84C91;// 0x00009DFE01;
DWORD WINAPI MultiPatch()
{
	char buff[50];
	sprintf_s(buff, "HSACSX Client[%d]", GetCurrentProcessId());
	memcpy((LPVOID)KO_MULTI_CAP, buff, 50);
	
	return 1;
}

void PatchMulti()
{
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MultiPatch, NULL, NULL, NULL);
}

HRESULT WINAPI HookCreateDevice()
{
	IDirect3D9 * device = Direct3DCreate9(D3D_SDK_VERSION);
	if (!device)
		return D3DERR_INVALIDCALL;

	IDirect3D9_vtable = (DWORD*)*(DWORD*)device;
	device->Release();
	DWORD protectFlag;
	if (VirtualProtect(&IDirect3D9_vtable[CREATEDEVICE_VTI], sizeof(DWORD), PAGE_READWRITE, &protectFlag))
	{
		*(DWORD*)&D3DCreateDevice_orig = IDirect3D9_vtable[CREATEDEVICE_VTI];
		*(DWORD*)&IDirect3D9_vtable[CREATEDEVICE_VTI] = (DWORD)D3DCreateDevice_hook;
		if (!VirtualProtect(&IDirect3D9_vtable[CREATEDEVICE_VTI], sizeof(DWORD), protectFlag, &protectFlag))
		{
			return D3DERR_INVALIDCALL;
		}
	}
	else
		return D3DERR_INVALIDCALL;

	return D3D_OK;
}

HRESULT WINAPI D3DCreateDevice_hook(IDirect3D9* Direct3D_Object, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow,DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice9** ppReturnedDeviceInterface)
{
	if (!itsMe) 
	{
		if (g_pMain->m_SettingsMgr == NULL)
			g_pMain->m_SettingsMgr = new CSettingsManager();

		pPresentationParameters->Windowed = g_pMain->m_SettingsMgr->m_iRealFullScreen == 1 ? false : true;
		pPresentationParameters->PresentationInterval = g_pMain->m_SettingsMgr->m_iVsync == 1 ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
		if (g_pMain->m_SettingsMgr->m_iVsync == 1)
			pPresentationParameters->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	}

	return D3DCreateDevice_orig(Direct3D_Object, Adapter, DeviceType, hFocusWindow, BehaviorFlags | D3DCREATE_MULTITHREADED, pPresentationParameters, ppReturnedDeviceInterface);
}
