#include "stdafx.h"
#include "MiniMap.h"
#include "Offset.h"
#include "Util.h"

bool MiniMapTable[MAX_MINI_MAP];

void InitMiniMap() // OK
{
	memset(MiniMapTable,0,sizeof(MiniMapTable));

	SetCompleteHook(0xFF,0x006AC940,&MiniMapCore);

	SetCompleteHook(0xE8,0x006A8D08,&MiniMapLoad);

	SetCompleteHook(0xFF,0x0041315E,&MiniMapCheck);
}

void MiniMapCore() // OK
{
	((void(*)())0x006A79AB)();

	MiniMapLoad();
}

void MiniMapLoad() // OK
{
	char buff[64];

	wsprintf(buff,"World%d\\Map1.jpg",(*(DWORD*)MAIN_CURRENT_MAP)+1);

	if(MiniMapFileCheck(*(DWORD*)MAIN_CURRENT_MAP) != 0)
	{
		MiniMapTable[*(DWORD*)MAIN_CURRENT_MAP] = 1;
		((int(__cdecl*)(char*,int,int,int,int,int))0x006D42FB)(buff,0x898,0x2601,0x2900,0,1);
	}
}

bool MiniMapCheck(int map) // OK
{
	if(MiniMapTable[map] != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool MiniMapFileCheck(int map) // OK
{
	char buff[64];

	wsprintf(buff,".\\Data\\World%d\\Map1.ozj",(map+1));

	FILE* file;

	if(fopen_s(&file,buff,"r") != 0)
	{
		return 0;
	}
	else
	{
		fclose(file);
		return 1;
	}
}
