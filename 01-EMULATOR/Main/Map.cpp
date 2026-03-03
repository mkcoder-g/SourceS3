#include "stdafx.h"
#include "Map.h"
#include "CustomMap.h"
#include "Offset.h"
#include "Util.h"

void InitMap()  // OK
{
	SetByte(0x006ACA12,0xEB); // Fix Check .map files
	
	SetByte(0x006ACA18,0xEB); // Fix Check .map files
	
	SetByte(0x006ACC05,0xEB); // Fix Check .att files
	
	SetByte(0x006ACC0B,0xEB); // Fix Check .att files
	
	SetByte(0x006ACCA8,0xEB); // Fix Check .obj files
	
	SetByte(0x006ACCAE,0xEB); // Fix Check .obj files

	SetByte(0x006ACA11,0x69); // Increase terrain
	
	SetByte(0x006ACC04,0x69); // Increase terrain
	
	SetByte(0x006ACCA7,0x69); // Increase terrain

	SetByte(0x006F70AC,0x63); // Show title on Silent

	SetCompleteHook(0xE8,0x004E5349,&LoadMapName);

	SetCompleteHook(0xE8,0x004E8598,&LoadMapName);

	SetCompleteHook(0xE8,0x00661916,&LoadMapName);

	SetCompleteHook(0xE8,0x006F710E,&LoadMapTitle);
}

char* LoadMapName(int index) //OK
{
	CUSTOM_MAP_INFO* lpInfo = gCustomMap.GetInfoByNumber(index);

	if(lpInfo != 0)
	{
		return lpInfo->MapName;
	}

	return ((char*(*)(int))0x00661510)(index);
}

int LoadMapTitle(char* path,int index,int c,int d,int e,int f) // OK
{
	CUSTOM_MAP_INFO* lpInfo = gCustomMap.GetInfoByNumber(*(DWORD*)MAIN_CURRENT_MAP);

	if(lpInfo != 0)
	{
		path = lpInfo->TitlePath;
	}

	return ((int(__cdecl*)(char*,int,int,int,int,int))0x006D484C)(path,index,c,d,e,f);
}