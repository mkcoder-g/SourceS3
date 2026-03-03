#include "stdafx.h"
#include "Font.h"
#include "Offset.h"
#include "Util.h"

int FontSize;

DWORD FontCharSet;

char FontName[100];

int ReadFontFile(char* path) // OK
{	
	FontSize = GetPrivateProfileInt("FontInfo","FontSize",13,path);
	
	FontCharSet = GetPrivateProfileInt("FontInfo","FontCharSet",1,path);
	
	GetPrivateProfileString("FontInfo","FontName","Tahoma",FontName,sizeof(FontName),path);

	return GetPrivateProfileInt("FontInfo","ChangeFontSwitch",0,path);
}

void InitFont() // OK
{
	if(ReadFontFile(".\\Config.ini") != 0)
	{
		SetCompleteHook(0xE8,0x004DD25E,&FontNormal);
		
		SetCompleteHook(0xE8,0x004DD29E,&FontBool);
		
		SetCompleteHook(0xE8,0x004DD2E0,&FontBig);
		
		SetCompleteHook(0xE8,0x004DD33A,&FontFixed);

		SetByte(0x004DD263,0x90);
		
		SetByte(0x004DD2A3,0x90);
		
		SetByte(0x004DD2E5,0x90);
		
		SetByte(0x004DD33F,0x90);
	}
}

void ReloadFont() // OK
{
	*(HFONT*)0x05875E20 = FontNormal();

	*(HFONT*)0x05875E24 = FontBool();

	*(HFONT*)0x05875E28 = FontBig();

	*(HFONT*)0x05875E2C = FontFixed();

	*(DWORD*)MAIN_FONT_SIZE = FontSize;

	((void(*)())0x006AE752)();
}

HFONT FontNormal() // OK
{
	return CreateFont(FontSize,0,0,0,FW_NORMAL,0,0,0,FontCharSet,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,FontName);
}

HFONT FontBool() // OK
{
	return CreateFont(FontSize,0,0,0,FW_BOLD,0,0,0,FontCharSet,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,FontName);
}

HFONT FontBig() // OK
{
	return CreateFont(FontSize*2,0,0,0,FW_BOLD,0,0,0,FontCharSet,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,FontName);
}

HFONT FontFixed() // OK
{
	return CreateFont(FontSize,0,0,0,FW_NORMAL,0,0,0,FontCharSet,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,FontName);
}