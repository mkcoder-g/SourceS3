	#pragma once

	#define WIN32_LEAN_AND_MEAN

	#define _WIN32_WINNT _WIN32_WINNT_WINXP

	//#define CONNECTSERVER_VERSION "ZTeam - ConnectServer (EX%03d) [%s]"
	#define CONNECTSERVER_VERSION "ZTeam - ConnectServer - [%s]"	

	// System Include
	#include <windows.h>
	#include <winsock2.h>
	#include <iostream>
	#include <stdlib.h>	
	#include <malloc.h>
	#include <memory.h>
	#include <tchar.h>
	#include <time.h>
	#include <math.h>
	#include <map>
	#include <vector>
	#include <queue>
	#include <string>
	#include <Rpc.h>
	#include <dbghelp.h>
	#include <Psapi.h>
	#include <CommCtrl.h>

	#pragma comment(lib,"ws2_32.lib")
	#pragma comment(lib,"Rpcrt4.lib")
	#pragma comment(lib,"dbghelp.lib")
	#pragma comment(lib,"Psapi.lib")

	extern int MaxConnectionPerIP;
	extern int MaxPacketPerSecond;
	extern int MaxConnectionIdle;

	typedef unsigned __int64 QWORD;