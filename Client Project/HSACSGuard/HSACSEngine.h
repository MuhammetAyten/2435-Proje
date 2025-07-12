#pragma once
#include "stdafx.h"
#include "CItem.h"
#include "Discord.h"
#include "HSACSEngine.h"
#include "PlayerBase.h"
#include "TableManager.h"
#include "SettingsManager.h"
#include "StringHelper.h"
#include "N3BASE/N3Base.h"
#include "N3BASE/N3UIBase.h"
#include "UIManager.h"
#include "LocalInput.h"
#include "UIState.h"
#include "UITargetBar.h"
#include "DropResult.h"
#include "UIHpBarMenu.h"
#include "hPowerUpStore.h"
#include "UITaskbarMain.h"
#include "UITaskbarSub.h"
#include "hEventShowList.h"
#include "UIClanWindow.h"
#include "UITradeItemDisplay.h"
#include "UIMiniMenu.h"
#include "UIGenieSub.h"
#include "CharacterToolTip.h"
#include "CollectionRace.h"
#include "UISeedHelper.h"
#include "UIPieceChange.h"
#include "hMerchantList.h"
#include "hSearchMonsterDrop.h"
#include "UILogin.h"
#include "Support.h"
#include "UITradePrice.h"
#include "UITradeInventory.h"
#include "UITradeItemDisplaySpecial.h"
#include "MerchantManager.h"
#include "UITooltip.h"
#include "UIInventory.h"
#include "CTagChange.h"
#include "hWhellOfFun.h"
#include "UIPartyBBS.h"
#include "UIHPBar.h"
#include "UIPMHook.h"
#include "TopRightReWarIcon.h"
#include "UIMinimap.h"
#include "HookSkillPage.h"
#include "UIScoreBoard.h"
#include "UIDailyQuests.h"
#include "UIInformationWind.h"
#include "CSpell.h"
#include "FunctionGuard.h"
#include "UISchedular.h"
#include "UIGenieMain.h"
#include "UIChatBar.h"
#include "UINoticeMsgWind.h"
#include "PlayerRank.h"
#include "hLotteryEvent.h"
#include "hAccountRegister.h"
#include "QuestComplated.h"

#if(HOOK_SOURCE_VERSION == 2369)
#include "UIRightExchange.h"
#include "UIDailyReward.h"
#include "EventHandler.h"
#else
#include "UIRightExchange.h"
#include "UIDailyReward.h"
#endif

#include "Cindirella.h"
#include <signal.h>

struct _LOTTERY_EVENT_SRGAME
{
	uint32 sRemainingTime;
	uint32 sParticiPant;
	uint32 sLimit;
	_LOTTERY_EVENT_SRGAME()
	{
		sRemainingTime = 0;
		sParticiPant = 0;
		sLimit = 1;
	}
};

#define foreach(itr, arr) \
	for (auto itr = arr.begin(); itr != arr.end(); itr++)

struct MerchantItem
{
	uint32 itemBase;
	uint8 bSrcPos;
	bool isKC;
	uint32 price;
};

struct MerchantItemView
{

	uint8 bSrcPos[8];
	bool isKC[8];
	uint32 price[8];

	MerchantItemView()
	{
		for (int i = 0; i < 8; i++)
		{
			bSrcPos[i] = 0;
			isKC[i] = 0;
			price[i] = 0;
		}

	}

};
struct MerchantEyeData
{
	uint8 nIndex;
	std::string strMerchantItem[12];
	MerchantEyeData()
	{
		for (int i = 0; i < 12; i++)
			strMerchantItem[i] = "";
		nIndex = 0;
	}
};
struct TagName
{
	std::string sTag;
	uint8 r, g, b;
	TagName() : sTag("-"), r(255), g(255), b(255) {};
};

typedef struct xsettings {
	int authority;
	int nation;
	int mypid;
	int csrsspid;
	int winlogonpid;
	int gamethread;
	int sChaosPopulation;
	int sChaosTimeLeft;
	int sChaosEventStatus;
	int failcount;
	int suspendcount;
	int debugpid;
	int ExplorerPID;
	bool authstatus;
	bool pusopen;
	bool connected;
	bool driverload;
	bool loginstate;
	bool selectnation;
	bool selfpatch;
	bool systemloaded;
	bool threadstate;
	bool activewindow;
	bool speedcheck;
	bool memcheck;
	bool cameracheck;
	bool deviceinfo;
	bool gotSerial;
	bool DrawnUI;
	bool BFTimer;
	bool DrawnRankBoard;
	bool ChaosTimer;
	bool GlitchDetect;
	bool GlitchCheck;
	bool exit;
	bool unwantedload;
	time_t lastfail;
	time_t GlitchDetectTime;
	HANDLE menuthread;
	HANDLE mainthread;
	HANDLE ProcessHandle;
	HANDLE ThreadHandle;
	unsigned long Zone;
	unsigned long CameraView;
	unsigned long RunMode;
	unsigned long CurrentStun;
	unsigned long SendKey;
	unsigned long LastSendCaller;
	float CoordX;
	float CoordZ;
	float NewCoordX;
	float NewCoordZ;
	unsigned long __w64 CHR;
	unsigned long __w64 PKT;
	HINSTANCE MyModule;
	FARPROC pOrig_GetTickCount;
	FARPROC pOrig_LoadLib;
	MODULEINFO ModuleInfo;
	MODULEINFO MainInfo;
	unsigned long MemCRCKey;
	unsigned long ThreadMemCRCKey;
	unsigned long MonitorMemCRCKey;
	unsigned long PacketMemCRCKey;
	unsigned long GamefuncsMemCRCKey;
	unsigned long ModuleCRCKey;
	unsigned long ModuleStart;
	unsigned long LicenseCRCKey;
	unsigned long PeekMessageAddr;

	unsigned long OSMajorVersion;
	unsigned long OSMinorVersion;
	unsigned short OSBuildNumber;
	unsigned short OSCSDVersion;
	unsigned long OSPlatformId;
	unsigned long ImageSubSystem;
	unsigned long ImageSubSystemMajorVersion;
	unsigned long ImageSubSystemMinorVersion;
	char serialNumber[1024];
	char modelNumber[1024];
	char szHWID[33];
	char szReg[33];
	xsettings();
} *PSO_INFO, SO_INFO;

class HSACSEngine
{
public:
	static PSO_INFO Settings;
	FunctionGuard GuardLockPerkSystem;
	DWORD varius;
	DWORD GetTarget(uint16 getid);
	DWORD GetTarget();
	_PERKS_DATA_INFO sPerksDataInfo;
	std::map<uint32, _PERK_INFO*> m_sPerkInfoArray;
	bool bDisableAllSkillFX, bDisableAreaSkillFX, bDisableHealFX;
	bool DeathAll, DeathMe, DeathParty, DeathNone;
	bool bDisableViewEmblem, bDisableViewPathos;
	std::recursive_mutex TagLock;
	std::map<std::string, TagName> TagList;
	void OpenjuraidScreen(bool update, uint16 karus, uint16 elmo, uint16 remtime);
	time_t timesPassed;

	Discord* dc;
	DWORD hkCampAdress;
	DWORD hkServerNotice;
	_LOTTERY_EVENT_SRGAME pLotteryEvent;

	CSTLMap <_CHAT_USER_MAP, std::string> pClientChatUser;
	std::string accountID;
	vector<HookPM*> privatemessages;
	DWORD isAgree;
	std::map<uint32, CSpell> skillmap;
	std::map<uint32, CSpell> skillmapBackup;
	std::map<uint32, SpellCRC> skillcrc;
	bool _InitIcon(DWORD& iconObject, DWORD parent, DWORD itemID);
	DWORD _CreateIconItemSkill(DWORD self, DWORD UIIcon, DWORD itemID, short count, uint32 expiration = 0, uint16 rental_remTime = 0, uint32 rental_period = 0, uint32 rental_price = 0);
	uint32 serverhour, serverminute, serversecond;
	uint8 GetNation();
	HSACSEngine(std::string basePath);
	virtual ~HSACSEngine();
	bool power, render;
	static DWORD WINAPI EngineMain(HSACSEngine* e);
	static DWORD WINAPI SuspendCheck(HSACSEngine* e);
	static DWORD WINAPI TitleCheck(HSACSEngine* e);
	DWORD KOGetTarget();
	uint16 KOGetSSID;
	HANDLE MainThread, ScanThread, SuspendThread, LisansThread, AliveThread;
	HANDLE TitleThread;
	std::string m_connectedIP;
	bool IsHWBreakpointExists();
	bool IsitaSandBox();
	User Player;
	bool m_bLoading;
	bool TitleWindow(int id, LPCSTR WindowTitle);
	DWORD hpBarAdress;
	bool m_bLoqOut;
	bool m_bLoadingControl;
	void SystemLog();
	void SystemSend();
	void __stdcall securesend();
	static void NewSystemLogger(const char* logit, ...);
	static FARPROC ExportGet(const HMODULE Image, const char* Function);
	FARPROC GetPAddress(LPCSTR LibraryName, LPCSTR ProcName) const;
	bool ProtectLocalHooks(char* libName, char* apiName) const;
	void EnableDebugPrivilege();
	void HandleGameStart(Packet &pkt);
	void HandleZoneChange(Packet &pkt);
	void HandleAccountInfoSave(Packet &pkt);
	void HandleChaoticExchange(Packet &pkt);
	void HandleLastSeen(Packet &pkt);
	void ChaoticExchangeStop();
	void ChaoticExchangeSucces();
	void RecvAntiAfkList(Packet &pkt);
	void RecvWheelData(Packet& pkt);
	void HandleTagName(Packet&pkt);
	void addnewtagid(std::string sockid, TagName sTag);
	bool NumberValid(const std::string &str2);
	bool StringValid(const std::string &str2);

	void HandlePusRefund(Packet &pkt);
	void HandleSetVisible(Packet& pkt);
	std::string GetZoneName(uint16 zoneID);
	DWORD Adress, grp_minimum;
	void Disconnect();
	void Send(Packet* pkt);
	void Update();
	POINT lastkoScreen;
	POINT curlastkoScreen;
	bool InjectLogin();
	void StayAlive();
	void SendProcess(uint16 toWHO);
	bool fileExist(const char* fileName);
	void SetTexImage(DWORD uif, const std::string& szFN);
	void __stdcall Shutdown(std::string log);
	bool WordGuardSystem(std::string Word, uint8 WordStr);
	int beginlog();
	inline int InsertLog(const char* message, bool type = false);
	static unsigned long GetPEB();
	static void SetOSVersion();
	void SetVisible(DWORD vTable, bool type);
	void SetUiRegion(DWORD vTable, RECT rc);
	void SetString(DWORD vTable, std::string str);
	void SetUIStyle(DWORD dwTable, DWORD Style);
	void GetCoordination(DWORD vTable, POINT& pt, POINT& pt2);
	void UIScreenCenter(DWORD vTable);
	void SetStringColor(DWORD vTable, DWORD color);
	std::string GetString(DWORD vTable);
	std::string GetPartyString(DWORD vTable);
	std::string GetStringElementName(DWORD vTable);
	std::string GetStringFromPWEdit(DWORD vTable);
	void WriteString(DWORD value, char* vl);
	bool IsVisible(DWORD vTable);
	void GetChildByID(DWORD vTable, std::string id, DWORD& child);
	DWORD GetChildByID(DWORD vTable, std::string id);
	void AddListString(DWORD vTable, const std::string& szString, DWORD color);
	void ClearListString(DWORD vTable);
	void SetRegion(DWORD dwTable, DWORD uif);
	void _ShowToolTip(DWORD spItem, LONG x, LONG y, TOOLTIP_TYPE type);

	/*Merchant Eye*/
	std::map<uint16, MerchantEyeData> pClientMerchantEyeData;
	std::map<uint16, uint16> pClientMerchantArray;
	std::recursive_mutex pClientMerchantEyeLock;
	std::recursive_mutex pClientMerchantItemViewLock;
	std::recursive_mutex pClientMerchantEyeDataLock;
	std::map<uint8, MerchantItem> pClientMerchantItem;
	std::map<uint16, MerchantItemView> pClientMerchantItemView;
	/*Merchant Eye*/
	int16 GetListSelect(DWORD vTable);
	void EditKillFocus(DWORD vTable);
	void GetBaseByChild(DWORD vTable, DWORD& base);
	void ShowMsg(MSG_TYPE type, std::string msg, ...);
	DWORD rdwordExt(DWORD ulBase);
	DWORD rdword(DWORD ulBase, std::vector<int> offsets);
	DWORD ReadDWORD(DWORD ulBase, std::vector<int> offsets);
	void WriteInfoMessageExt(char* pMsg, DWORD dwColor);
	void WriteChatAddInfo(DWORD pColor, bool isBold, const char* pText, uint8 nRank = 0);
	void WriteInfoMessage(char* pMsg, DWORD dwColor);
	void GetUiPos(DWORD vTable, POINT& pt);
	RECT GetUiRegion(DWORD vTable);
	RECT GetUiRegion2(DWORD vTable);
	POINT GetUiPos(DWORD vTable);
	POINT GetUiPos2(DWORD vTable);
	void SetUIPos(DWORD vTable, POINT pt);
	void UiPost(DWORD vTable, LONG x, LONG y);
	void SetState(DWORD vTable, DWORD state);
	DWORD GetRecvMessagePtr(DWORD adres);
	uint32 GetState(DWORD vTable);
	void InitCallHook(DWORD hookFuncAddr, DWORD myFuncAddr);
	void SetMemArray(DWORD Adres, BYTE Deger, DWORD len);
	inline void CALLHOOK(DWORD Addr1, DWORD Addr2);
	DWORD CalculateCallAddrWrite(DWORD Addr1, DWORD Addr2);
	void SendChaoticExchange(uint16 nObjectID, uint32 ExchangeRobItemID, int8 ExchangeRobItemSlot);
	std::string exeGetName();
	bool disableCameraZoom;
	void InitCameraZoom();
	std::string KoRandomNameUIF(std::string uif);
	void InitJmpHook(DWORD hookFuncAddr, DWORD myFuncAddr);
	inline void JMPHOOK(DWORD Addr1, DWORD Addr2);
	void InitSendHook();
	void InitRecvHook();
	inline BYTE GetByte(char* sBuf, int& index);
	inline DWORD GetDWORD(char* sBuf, int& index);
	inline int GetShort(char* sBuf, int& index);
	void SendItemAdd(uint32 itemID, uint16 count, uint32 gold, uint8 srcPos, uint8 dstPos, uint8 mode);
	void PacketCheckThreadID(uint8 packet, DWORD ThreadID);
	void InitSetString();
	bool IsIn(DWORD vTable, int x, int y);
	void SetItemFlag(DWORD vTable, uint8 flag);
	POINT lastMousePos;
	LONG GetUiWidth(DWORD vTable);
	LONG GetUiHeight(DWORD vTable);
	size_t GetScrollValue(DWORD vTable);
	void SetScrollValue(DWORD vTable, int val);
	static long SelectRandomMob();
	CPlayerBase* m_PlayerBase;
	int16 GetSocketID();
	DWORD GetPartyLeaderBase();
	int16 GetPartyLeaderTarget();
	DWORD m_sMerchantDisplayData;
	int16 GetPartyLeaderID();
	bool isInParty();
	std::map<DWORD, DWORD> mReturnAdressSend;
	DWORD m_sMerchantDisplayItem[4];
	DWORD m_sMerchantDisplaySpecialData;
	DWORD m_sMerchantDisplaySpecialItem[8];
	CTableManager* pClientTBLManager;
	CSettingsManager* m_SettingsMgr;
	std::string m_BasePath;
	bool logState;
	bool drawMode;
	bool _lowPower;
	LPDIRECT3DDEVICE9 _pDevice;
	bool m_cCollettionStatus;
	COLLECTION_RACE_EVENT pCollectionRaceEvent;
	HSACSX_NEW_EVENT_SYSTEM pClientEventSystem;
	ZINDAN_WARINFO pZindanWarInfo;

	bool isSiegeWarStart, isSiegeWarActive;
	_CASTLE_SIEGE_WAR_TIMER pCastleSiegeWar;

	//INLINE bool isInited() { return m_bSelectedCharacter; }
	/*INLINE bool isHookStart() { return m_bHookStart; }
	INLINE bool isGameStart() { return m_bGameStart; }*/

	void SetCurValue(DWORD dwTable, float value, float changeSpeed);
	void SetProgressRange(DWORD uif, float min, float max);
	void ChangeRunState();
	void CommandCameraChange();
	std::map<uint8, uint32> WheelItemData;
	bool m_bHookPanel;
	bool m_bSelectedCharacter;
	bool m_bGameStart;
	bool m_bHookStart;

	bool m_bGenieStatus;
	INLINE bool isInGenie(){ return m_bGenieStatus; }

	bool m_bGenieAttackerStatus;
	INLINE bool isInGenieAttacker() { return m_bGenieAttackerStatus; }

	bool m_bInParty;
	bool m_LoginPlugInjected;
	ULONGLONG m_recentdelete_time;

	time_t m_bCheckSleep;
	std::string GetItemDXT(int nItemID);
	std::string number_format(int dwNum, bool dot = true);
	void str_replace(std::string& str, std::string find, std::string replace);
	void str_replace(std::string& str, char find, char replace);
	bool str_contains(std::string str, std::string find);
	void str_split(std::string const& str, const char delim, std::vector<std::string>& out);
	void str_split(std::string str, std::string delim, std::vector<std::string>& out);
	std::vector<std::string> str_split(std::string str, std::string regex);
	void str_tolower(std::string& str);
	std::string dcpUIF(std::string path);
	std::string GetEditString(DWORD vTable);
	void SetEditString(DWORD vTable, const std::string& szString);
	void ShowToolTipEx(uint32 itemID, LONG x, LONG y);
	
	void maptest();//sait
	std::string strClientName;

	uint32 moneyReq;
	DWORD m_zMob;
	CStringHelper* StringHelper;
	CUIManager* pClientHookManager;
	CDropResult* pClientDropResult;
	CUIClientStatePlug* pClientUIState;
	CUIClientTargetBarPlug* pClientTargetBar;
	CUITaskbarMainPlug* uiTaskbarMain;
	CUITaskbarSubPlug* uiTaskbarSub;
	CUIClanWindowPlug* uiClanWindowPlug;
	CUIMiniMenuPlug* uiMiniMenuPlug;
	ClientHookPlayerRank* pClientPlayerRank;
	CUIMoticeMsgWnd* uiNoticeWind;
	CUIGenieSubPlug* uiGenieSubPlug;
	CUISeedHelperPlug* uiSeedHelperPlug;
	CUIPieceChangePlug* uiPieceChangePlug;
	CUIAccountRegister* uiRegister;
	CUILogin* uiLogin;
	CUITradePricePlug* pClientTradePrice;
	CUITradeInventoryPlug* pClientTradeInventory;
	CUITradeItemDiplayPlug* uiTradeItemDisplay;
	CUITradeItemDisplaySpecialPlug* uiTradeItemDisplaySpecial;
	CMerchantManager* pClientMerchant;
	CUITooltip* uiToolTip;
	CUIInventoryPlug* pClientInventory;
	CUIPartyBBSPlug* uiPartyBBS;
	CUIClientHPBarPlug* pClientUIBarPlug;
	CEventShowListPlug* pClientEvetShowList;
	CUIHPBarMenuPlug* uiHpMenuPlug;
	CUIAccountRegister* uiAccountRegisterPlug;
	CUIClientTopRightNewPlug* pClientTopRightNewPlug;
	CUISchedular* uiSchedularPlug;
	CUIMinimapPlug* uiMinimapPlug;
	CUIChatBarPlug* uiChatBarPlug;
	CUIScoreBoardPlug* uiScoreBoard;
	CUIGenieMain* uiGenieMain;
	CUIMerchantListPlug* uiMerchantList;
	HookQuestPage* uiQuestPage;
	CSealToolTip* pClientSealToolTip;
	CUIPowerUpStore* uiPowerUpStore;
	CUIInformationWindPlug* uiInformationWind;
	CUISupportReportPlug* uiSupport;
	CUICollectionRacePlug* pClientCollection;
	CUILotteryEventPlug* pClientLottyEvent;
	CUICindirellaPlug* uiCindirella;
	CUIClientWheelOfFun* pClientWheelOfFunPlug;
	CUISearchMonsterDropPlug* uiSearchMonster;
	CTagChange* uiTagChange;
	HookSkillPage* uiSkillPage;
	CUIQuestComplated* uiQuestComplated;

#if(HOOK_SOURCE_VERSION == 2369)
	CUIRightExchangePlug* pClientRightExchange;
	CUIDailyRewardPlug* pClientDailyReward;
	CUIEventHandler* pClientEventHandler;
#else
	CUIRightExchangePlug* pClientRightExchange;
	CUIDailyRewardPlug* pClientDailyReward;
#endif 
	struct CindirellaInfo {
		bool prepare;
		int8 index;
		uint16 mykill_count, mydead_count, e_kill_count, k_kill_count;
		uint64 remainingtime, selected_remtime;
		CindirellaInfo() {
			Initialize();
		}
		void Initialize() {
			prepare = false;
			index = -1;
			mykill_count = mydead_count = e_kill_count = k_kill_count = 0;
			remainingtime = selected_remtime = 0;
		}
	};
	CindirellaInfo m_cindirellaInfo;
private:
	void InitPlayer();
	
};

extern HSACSEngine* g_pMain;