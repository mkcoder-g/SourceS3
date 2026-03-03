#include "stdafx.h"
#include "resource.h"
#include "Main.h"
#include "Camera.h"
#include "CCRC32.H"
#include "ChaosBox.h"
#include "Common.h"
#include "EventEntryLevel.h"
#include "Font.h"
#include "HackCheck.h"
#include "HealthBar.h"
#include "Item.h"
#include "Language.h"
#include "Map.h"
#include "MiniMap.h"
#include "Monster.h"
#include "Offset.h"
#include "PacketManager.h"
#include "PrintPlayer.h"
#include "Protect.h"
#include "Protocol.h"
#include "Reconnect.h"
#include "Resolution.h"
#include "ServerList.h"
#include "Shop.h"
#include "TrayMode.h"
#include "Util.h"

HHOOK HookKB,HookMS;
HINSTANCE hins;

LRESULT CALLBACK KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam) // OK
{
	if(nCode == HC_ACTION)
	{
		if(((DWORD)lParam & (1 << 30)) != 0 && ((DWORD)lParam & (1 << 31)) != 0 && GetForegroundWindow() == *(HWND*)(MAIN_WINDOW))
		{
			if(gProtect.m_MainInfo.KeyCodeHealthBarSwitch != 0 && wParam == gProtect.m_MainInfo.KeyCodeHealthBarSwitch)
			{
				HealthBarToggle();
			}
			else if(gProtect.m_MainInfo.KeyCodeCamera3DSwitch != 0 && wParam == gProtect.m_MainInfo.KeyCodeCamera3DSwitch)
			{
				gCamera.Toggle();
			}
			else if(gProtect.m_MainInfo.KeyCodeCamera3DRestore != 0 && wParam == gProtect.m_MainInfo.KeyCodeCamera3DRestore)
			{
				gCamera.Restore();
			}
			else if(gProtect.m_MainInfo.KeyCodeTrayModeSwitch != 0 && wParam == gProtect.m_MainInfo.KeyCodeTrayModeSwitch)
			{
				gTrayMode.Toggle();
			}
		}
	}

	return CallNextHookEx(HookKB,nCode,wParam,lParam);
}

LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam) // OK
{
	if(nCode == HC_ACTION)
	{
		MOUSEHOOKSTRUCTEX* HookStruct =(MOUSEHOOKSTRUCTEX*)lParam;

		if(GetForegroundWindow() == *(HWND*)(MAIN_WINDOW))
		{
			switch(wParam)
			{
				case WM_MOUSEMOVE:
					gCamera.Move(HookStruct);
					break;
				case WM_MBUTTONDOWN:
					gCamera.SetIsMove(1);
					gCamera.SetCursorX(HookStruct->pt.x);
					gCamera.SetCursorY(HookStruct->pt.y);
					break;
				case WM_MBUTTONUP:
					gCamera.SetIsMove(0);
					break;
				case WM_MOUSEWHEEL:
					gCamera.Zoom(HookStruct);
					break;
				case WM_RBUTTONDOWN:
					ClicMouseProc();
					break;
			}
		}
	}

	return CallNextHookEx(HookMS,nCode,wParam,lParam);
}

SHORT WINAPI KeysProc(int nCode) // OK
{
	if(GetForegroundWindow() != *(HWND*)(MAIN_WINDOW))
	{
		return 0;
	}

	return GetAsyncKeyState(nCode);
}

HICON WINAPI IconProc(HINSTANCE hInstance,LPCSTR lpIconName) // OK
{
	FILE* file;

	if(fopen_s(&file,".\\main.ico","r") != 0)
	{
		gTrayMode.m_TrayIcon = (HICON)LoadImage(hins,MAKEINTRESOURCE(IDI_CLIENT),IMAGE_ICON,GetSystemMetrics(SM_CXSMICON),GetSystemMetrics(SM_CYSMICON),LR_DEFAULTCOLOR);
	}
	else
	{
		fclose(file);
		gTrayMode.m_TrayIcon = (HICON)LoadImage(hins,".\\main.ico",IMAGE_ICON,GetSystemMetrics(SM_CXSMICON),GetSystemMetrics(SM_CYSMICON),LR_LOADFROMFILE | LR_DEFAULTCOLOR);
	}

	return gTrayMode.m_TrayIcon;
}

void WINAPI ReduceConsumeProc() // OK
{
	while(true)
	{
		Sleep(5000);
		SetProcessWorkingSetSize(GetCurrentProcess(),0xFFFFFFFF,0xFFFFFFFF);
		SetThreadPriority(GetCurrentProcess(),THREAD_PRIORITY_LOWEST);
	}
}

extern "C" _declspec(dllexport) void InitDLL() // OK
{
	if(gProtect.ReadMainFile("load.xmu") != 0)
	{
		SetByte(0x007D0798,0xA0); // Accent
		SetByte(0x004DC941,0xEB); // Crack (mu.exe)
		SetByte(0x004DCCBD,0xE9); // Crack (GameGuard)
		SetByte(0x004DCCBE,0x8A); // Crack (GameGuard)
		SetByte(0x004DCCBF,0x00); // Crack (GameGuard)
		SetByte(0x004DCCC0,0x00); // Crack (GameGuard)
		SetByte(0x004DCCC1,0x00); // Crack (GameGuard)
		SetByte(0x004E52EA,0xEB); // Crack (ResourceGuard)
		SetByte(0x004DCC74,0xEB); // Crack (ResourceGuard)
		SetByte(0x00476ACB,0xEB); // Ctrl Fix
		SetByte(0x00476AD9,0x02); // Ctrl Fix
		SetByte(0x006CCA9C,0x7F); // Fix Enter Select Char
		SetByte(0x006F13EF,0xEB); // Fix Ancient Harmony
		SetByte(0x00613C37,0x1D); // Fix Box of Luck
		SetByte(0x0053EE78,0x0D); // Fix Effect +13
		SetByte(0x007D1AA0,(gProtect.m_MainInfo.ClientVersion[0]+1)); // Version
		SetByte(0x007D1AA1,(gProtect.m_MainInfo.ClientVersion[2]+2)); // Version
		SetByte(0x007D1AA2,(gProtect.m_MainInfo.ClientVersion[3]+3)); // Version
		SetByte(0x007D1AA3,(gProtect.m_MainInfo.ClientVersion[5]+4)); // Version
		SetByte(0x007D1AA4,(gProtect.m_MainInfo.ClientVersion[6]+5)); // Version
		SetWord(0x007E5ECC,(gProtect.m_MainInfo.IpAddressPort)); // IpAddressPort
		SetDword(0x004DBA0D,(DWORD)gProtect.m_MainInfo.WindowName);
		SetDword(0x006D07B4,(DWORD)gProtect.m_MainInfo.ScreenShotPath);
		SetDword(0x007B145C,(DWORD)&KeysProc);
		SetDword(0x007B1500,(DWORD)&IconProc);

		MemorySet(0x004D8B32,0x90,0x0E); // Remove MuError.log

		MemorySet(0x00520583,0x90,0x3F); // Remove Reflect Effect

		MemorySet(0x0062DC6D,0x90,0x02); // Item Move Inventory -> Interface

		MemorySet(0x0062DCE8,0x90,0x02); // Item Move Interface -> Inventory

		MemoryCpy(0x007D0824,gProtect.m_MainInfo.IpAddress,sizeof(gProtect.m_MainInfo.IpAddress)); // IpAddress

		MemoryCpy(0x007D1AA8,gProtect.m_MainInfo.ClientSerial,sizeof(gProtect.m_MainInfo.ClientSerial)); // ClientSerial

		SetCompleteHook(0xE8,0x005FFB97,&DrawNewHealthBar);

		SetCompleteHook(0xFF,0x00509B4D,&ProtocolCoreEx);

		gCustomEffect.Load(gProtect.m_MainInfo.CustomEffectInfo);

		gCustomItem.Load(gProtect.m_MainInfo.CustomItemInfo);

		gCustomJewel.Load(gProtect.m_MainInfo.CustomJewelInfo);

		gCustomMap.Load(gProtect.m_MainInfo.CustomMapInfo);

		gCustomMonster.Load(gProtect.m_MainInfo.CustomMonsterInfo);

		gCustomMonsterSkin.Load(gProtect.m_MainInfo.CustomMonsterSkinInfo);

		gCustomTooltip.Load(gProtect.m_MainInfo.CustomTooltipInfo);

		gCustomWing.Load(gProtect.m_MainInfo.CustomWingInfo);

		gItemStack.Load(gProtect.m_MainInfo.ItemStackInfo);

		gItemValue.Load(gProtect.m_MainInfo.ItemValueInfo);

		gPacketManager.LoadEncryptionKey("Data\\Enc1.dat");

		gPacketManager.LoadDecryptionKey("Data\\Dec2.dat");

		InitChaosBox();

		InitCommon();

		InitEventEntryLevel();

		InitFont();

		InitHackCheck();

		InitItem();

		InitJewel();

		InitLanguage();

		InitWing();

		InitMap();

		InitMiniMap();

		InitMonster();

		InitPrintPlayer();

		InitReconnect();

		InitResolution();

		InitServerList();

		InitShop();

		gProtect.CheckLauncher();

		gProtect.CheckInstance();

		gProtect.CheckClientFile();

		gProtect.CheckPluginFile();

		HookKB = SetWindowsHookEx(WH_KEYBOARD,KeyboardProc,hins,GetCurrentThreadId());

		HookMS = SetWindowsHookEx(WH_MOUSE,MouseProc,hins,GetCurrentThreadId());

		CreateThread(0,0,(LPTHREAD_START_ROUTINE)ReduceConsumeProc,0,0,0);
	}
	else
	{
		ErrorMessageBox("Could not load load.xmu!");
		ExitProcess(0);
	}
}

BOOL APIENTRY DllMain(HANDLE hModule,DWORD ul_reason_for_call,LPVOID lpReserved) // OK
{
	switch(ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			hins = (HINSTANCE)hModule;
			break;
	}

	return 1;
}