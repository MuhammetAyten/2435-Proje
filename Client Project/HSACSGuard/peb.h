#ifndef __BLACKMAGIC_HPP__ 
#define __BLACKMAGIC_HPP__ 

#include <windows.h> 

#pragma pack(1) 

// 
// Windows magics - some of the stuff is stolen from wine 
// 
typedef struct _PEB_LDR_DATA
{
	unsigned long               Length;
	unsigned char             Initialized;
	void* SsHandle;
	LIST_ENTRY          InLoadOrderModuleList;
	LIST_ENTRY          InMemoryOrderModuleList;
	LIST_ENTRY          InInitializationOrderModuleList;
} PEB_LDR_DATA, * PPEB_LDR_DATA;
typedef struct _PEBX
{
	unsigned char InheritedAddressSpace; /*00*/
	unsigned char ReadImageFileExecOptions; /*01*/
	unsigned char BeingDebugged;      /*  02 */
	unsigned char BitField; /*03*/
	unsigned char ImageUsesLargePages; /*04*/
	unsigned char IsProtectedProcess; /*05*/
	HMODULE                      ImageBaseAddress;   /*  08 */
	PPEB_LDR_DATA                LdrData;            /*  0c */
	unsigned long						 ProcessParameters;  /*  10 */
	void* __pad_14;           /*  14 */
	HANDLE                       ProcessHeap;        /*  18 */
	unsigned char                         __pad_1c[36];       /*  1c */
	unsigned long		                 TlsBitmap;          /*  40 */
	unsigned long                        TlsBitmapBits[2];   /*  44 */
	unsigned char                         __pad_4c[24];       /*  4c */
	unsigned long                        NumberOfProcessors; /*  64 */
	unsigned char                         __pad_68[128];      /*  68 */
	void* Reserved3[59];      /*  e8 */
	unsigned long                        SessionId;          /* 1d4 */
} PEBX, * PPEBX;
typedef struct _PEB_FREE_BLOCK {
	struct _PEB_FREE_BLOCK* Next;
	unsigned long Size;
} PEB_FREE_BLOCK, * PPEB_FREE_BLOCK;
typedef struct _RTL_DRIVE_LETTER_CURDIR {
	unsigned short Flags;
	unsigned short Length;
	unsigned long					TimeStamp;
	//UNICODE_STRING			DosPath;
} RTL_DRIVE_LETTER_CURDIR, * PRTL_DRIVE_LETTER_CURDIR;
typedef struct _RTL_USER_PROCESS_PARAMETERS {
	unsigned long					MaximumLength;
	unsigned long					Length;
	unsigned long					Flags;
	unsigned long					DebugFlags;
	void* ConsoleHandle;
	unsigned long					ConsoleFlags;
	HANDLE					StdInputHandle;
	HANDLE					StdOutputHandle;
	HANDLE					StdErrorHandle;
	//UNICODE_STRING			CurrentDirectoryPath;
	HANDLE					CurrentDirectoryHandle;
	/*UNICODE_STRING			DllPath;
	UNICODE_STRING			ImagePathName;
	UNICODE_STRING			CommandLine;*/
	void* Environment;
	unsigned long					StartingPositionLeft;
	unsigned long					StartingPositionTop;
	unsigned long					Width;
	unsigned long					Height;
	unsigned long					CharWidth;
	unsigned long					CharHeight;
	unsigned long					ConsoleTextAttributes;
	unsigned long					WindowFlags;
	unsigned long					ShowWindowFlags;
	/*UNICODE_STRING			WindowTitle;
	UNICODE_STRING			DesktopName;
	UNICODE_STRING			ShellInfo;
	UNICODE_STRING			RuntimeData;*/
	RTL_DRIVE_LETTER_CURDIR DLCurrentDirectory[0x20];
} RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;
typedef void(*PPEBLOCKROUTINE)(void* PebLock);
typedef struct _PEB {
	unsigned char                 InheritedAddressSpace;
	unsigned char                 ReadImageFileExecOptions;
	unsigned char                 BeingDebugged;
	unsigned char                 Spare;
	HANDLE                  Mutant;
	void* ImageBaseAddress;
	PPEB_LDR_DATA           LoaderData;
	PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
	void* SubSystemData;
	void* ProcessHeap;
	void* FastPebLock;
	PPEBLOCKROUTINE         FastPebLockRoutine;
	PPEBLOCKROUTINE         FastPebUnlockRoutine;
	unsigned long                   EnvironmentUpdateCount;
	void** KernelCallbackTable;
	void* EventLogSection;
	void* EventLog;
	PPEB_FREE_BLOCK         FreeList;
	unsigned long                   TlsExpansionCounter;
	void* TlsBitmap;
	unsigned long                   TlsBitmapBits[0x2];
	void* ReadOnlySharedMemoryBase;
	void* ReadOnlySharedMemoryHeap;
	void** ReadOnlyStaticServerData;
	void* AnsiCodePageData;
	void* OemCodePageData;
	void* UnicodeCaseTableData;
	unsigned long                   NumberOfProcessors;
	unsigned long                   NtGlobalFlag;
	unsigned char                    Spare2[0x4];
	LARGE_INTEGER           CriticalSectionTimeout;
	unsigned long                   HeapSegmentReserve;
	unsigned long                   HeapSegmentCommit;
	unsigned long                   HeapDeCommitTotalFreeThreshold;
	unsigned long                   HeapDeCommitFreeBlockThreshold;
	unsigned long                   NumberOfHeaps;
	unsigned long                   MaximumNumberOfHeaps;
	void*** ProcessHeaps;
	void* GdiSharedHandleTable;
	void* ProcessStarterHelper;
	void* GdiDCAttributeList;
	void* LoaderLock;
	unsigned long                   OSMajorVersion;
	unsigned long                   OSMinorVersion;
	unsigned short					OSBuildNumber;
	unsigned short					OSCSDVersion;
	unsigned long                   OSPlatformId;
	unsigned long                   ImageSubSystem;
	unsigned long                   ImageSubSystemMajorVersion;
	unsigned long                   ImageSubSystemMinorVersion;
	unsigned long                   GdiHandleBuffer[0x22];
	unsigned long                   PostProcessInitRoutine;
	unsigned long                   TlsExpansionBitmap;
	unsigned char                    TlsExpansionBitmapBits[0x80];
	unsigned long                   SessionId;
} PEB, * PPEB;
typedef struct _TEB
{
	NT_TIB          Tib;                        /* 000 */
	void* EnvironmentPointer;         /* 01c */
	//CLIENT_ID       ClientId;                   /* 020 */
	void* ActiveRpcHandle;            /* 028 */
	void* ThreadLocalStoragePointer;  /* 02c */
	PPEB            Peb;                        /* 030 */
	unsigned long   LastErrorValue;             /* 034 */
	unsigned char   __pad038[140];              /* 038 */
	unsigned long   CurrentLocale;              /* 0c4 */
	unsigned char   __pad0c8[1568];             /* 0c8 */
	int				RealProcessID;				/* 6e8 */
	int				RealThreadID;				/* 6ec */
	unsigned char   __pad6f0[176];             /* 6f0 */
	void* Reserved2[278];             /* 7a0 */
	//UNICODE_STRING  StaticUnicodeString;        /* bf8 used by advapi32 */
	wchar_t			StaticUnicodeBuffer[261];   /* c00 used by advapi32 */
	void* DeallocationStack;          /* e0c */
	void* TlsSlots[64];               /* e10 */
	LIST_ENTRY      TlsLinks;                   /* f10 */
	void* Reserved4[26];              /* f18 */
	void* ReservedForOle;             /* f80 Windows 2000 only */
	void* Reserved5[4];               /* f84 */
	void* TlsExpansionSlots;          /* f94 */
} TEB, * PTEB;
typedef struct _WIN32_START_ADDRESS {
	void* Win32StartAddress;
} WIN32_START_ADDRESS, * PWIN32_START_ADDRESS;


#endif __BLACKMAGIC_HPP__