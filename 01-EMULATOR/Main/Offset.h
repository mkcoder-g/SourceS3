#pragma once

#define MAIN_WINDOW				0x05875E10
#define MAIN_CONNECTION_STATUS	0x0587A21C
#define MAIN_SCREEN_STATE		0x007E5ED0
#define MAIN_CHARACTER_STRUCT	0x077A6E48
#define MAIN_VIEWPORT_STRUCT	0x0754D340
#define MAIN_PACKET_SERIAL		0x0587A218
#define MAIN_FONT_SIZE			0x078C30D0
#define MAIN_RESOLUTION			0x05875BD8
#define MAIN_RESOLUTION_X		0x007E5E24
#define MAIN_RESOLUTION_Y		0x007E5E28
#define MAIN_PARTY_MEMBER_COUNT	0x07A25978
#define MAIN_CURRENT_MAP		0x007D4010
#define MAIN_HOOK_RECV			0x007B15BC
#define MAIN_HOOK_SEND			0x007B15CC
#define MAIN_ACTIVE_SOCKET		0x05875FA0
#define CASH_RESOLUTION_X       0x0072DF47
#define CASH_RESOLUTION_Y       0x0072DF51

#define STRUCT_DECRYPT			((void(__thiscall*)(void*,void*))0x0040BD60)((void*)0x05875968,*(void**)0X077A6E4C);
#define STRUCT_ENCRYPT			((void(__thiscall*)(void*,void*))0x0040BE20)((void*)0x05875968,*(void**)0X077A6E4C);

#define ProtocolCore			((BOOL(*)(DWORD,BYTE*,DWORD,DWORD))0x00509C50)
#define pGetPosFromAngle		((void(__cdecl*)(float*,int*,int*))0x006B64F4)
#define pCursorX				*(int*)0x07EB164C
#define pCursorY				*(int*)0x07EB1648
#define DrawInterfaceText		((void(*)(int,int,char*))0x006BA590)
#define pDrawText				((char*(__cdecl*)(int,int,char*,int,int,int))0x0059FD10)
#define pDrawBarForm			((void(__cdecl*)(float,float,float,float,float,int))0x006B7DD4)
#define pSetBlend				((void(__cdecl*)(BYTE))0x006B6949)
#define pGLSwitchBlend			((void(__cdecl*)())0x006B69E4)
#define pGLSwitch				((void(__cdecl*)())0x006B68CA)
#define pDrawBigText			((void(*)(float,float,DWORD,float,float))0x006B7C19)
#define pDrawImage              ((void(*)(DWORD,float,float,float,float,float,float,float,float,int,int,GLfloat))0x006B7EB7)
#define pDrawMessage			((int(__cdecl*)(char*,int))0x005A01C0)
#define pLoadItemModel			((void(*)(int,char*,char*,int))0x0069A213)
#define pLoadItemTexture		((void(*)(int,char*,int,int,int))0x00699D97)
#define pCheckWindow			((bool(__stdcall*)(int))0x006F43B6)
#define pViewportAddress		*(DWORD*)(0x0754D32C)
#define pChaosMixIndex			((DWORD(__thiscall*)(DWORD))0x00465305)(0x90*((DWORD(__thiscall*)(DWORD*))0x0046681D)(&*(DWORD*)(0x008351B8))+0x008351BC)

#define pRenderPartObjectEffect	((void(*)(DWORD,int,float*,float,int,int,int,int,int))0x00692362)
#define pTransformPosition      ((int(__thiscall*)(DWORD,DWORD,float*,float*,bool))0x005155B9)
#define pCreateSprite	        ((int(*)(int,float*,float,float*,DWORD,float,int))0x00592E28)
#define pCreateParticle			((int(__cdecl*)(DWORD,float*,DWORD,float*,DWORD,float,DWORD))(0x0057EB91))
#define pCreateEffect			((void(__cdecl*)(int,float*,DWORD,float*,int,DWORD,short,BYTE,float,BYTE,float*))0x00547780)

#define pCreateMonster			((DWORD(*)(int,int,int,int))0x00453896)
#define pCreateCharacter		((DWORD(*)(int,int,int,int,float))0x0053E2E7)
#define pSettingMonster			((DWORD(*)(int,int))0x0054043D)
#define pSetCharacterScale		((void(*)(DWORD))0x0053E455)

#define pGetTextLine(x)			((char*)(0x077E2174+(0x12C*x)))

#define ITEM_BASE_MODEL			634

#define GET_ITEM(x,y)			(((x)*512)+(y))
#define GET_ITEM_MODEL(x,y)		((((x)*512)+(y))+ITEM_BASE_MODEL)
#define GET_ITEM_OPT_LEVEL(x)	((x>>3)&15)
#define GET_ITEM_OPT_EXC(x)		((x)-(x&64))
#define GET_MAX_WORD_VALUE(x)	(((x)>65000)?65000:((WORD)(x)))

enum eWindowsType
{
	WINDOWS_NONE				= 0,
	WINDOWS_FRIEND_LIST			= 1,
	WINDOWS_MOVE_LIST			= 2,
	WINDOWS_PARTY				= 3,
	WINDOWS_QUEST				= 4,
	WINDOWS_GUILD				= 5,
	WINDOWS_TRADE				= 6,
	WINDOWS_WAREHOUSE			= 7,
	WINDOWS_UNKNOWN_1			= 8,
	WINDOWS_CHAOS_MIX			= 9,
	WINDOWS_COMMAND				= 10,
	WINDOWS_PET					= 11,
	WINDOWS_STORE				= 12,
	WINDOWS_DEVIL_SQUARE		= 13,
	WINDOWS_MOVING_SERVER		= 14,
	WINDOWS_BLOOD_CASTLE		= 15,
	WINDOWS_PET_TRAINER			= 16,
	WINDOWS_SHOP				= 17,
	WINDOWS_STORE_OTHER			= 18,
	WINDOWS_GUILD_MASTER		= 19,
	WINDOWS_GUARDMAN_SIEGUE		= 20,
	WINDOWS_SENIOR_MIX			= 21,
	WINDOWS_GUARDMAN_LAND		= 22,
	WINDOWS_CATAPULT_INVADING	= 23,
	WINDOWS_CATAPULT_DEFENDING	= 24,
	WINDOWS_CASTLE_GATE_SWITCH	= 25,
	WINDOWS_CHARACTER			= 26,
	WINDOWS_INVENTORY			= 27,
	WINDOWS_REFINERY			= 28,
	WINDOWS_REFINERY_WARNING	= 29,
	WINDOWS_KANTURU_GATE		= 30,
	WINDOWS_WEREWOLF			= 31,
	WINDOWS_ILLUSION_TEMPLE1	= 32,
	WINDOWS_ILLUSION_TEMPLE2	= 34,
	WINDOWS_CHAOS_CARD			= 35,
};

static const BYTE module0[12] = { 0xB7,0xAA,0xD9,0x92,0xAA,0xC7,0xCF,0xFD,0x85,0x98,0xA3,0xC7 };
static const BYTE module1[11] = { 0xB3,0xBF,0xCE,0x92,0x9F,0xD9,0x93,0xAC,0xCE,0x8F,0xBC };
static const BYTE module2[11] = { 0xB9,0xB7,0xC2,0x88,0x9F,0xD9,0x93,0xAC,0xCE,0x8F,0xBC };
static const BYTE module3[16] = { 0xA8,0xAA,0xD9,0x91,0xA6,0xC5,0x9D,0xBB,0xCE,0xAC,0xBD,0xC4,0x9F,0xAA,0xD8,0x8F };
static const BYTE module4[23] = { 0xAD,0xBA,0xCE,0x8E,0xB6,0xFB,0x99,0xBD,0xCD,0x93,0xBD,0xC6,0x9D,0xA1,0xC8,0x99,0x8C,0xC4,0x89,0xA1,0xDF,0x99,0xBD };
static const BYTE module5[25] = { 0xAD,0xBA,0xCE,0x8E,0xB6,0xFB,0x99,0xBD,0xCD,0x93,0xBD,0xC6,0x9D,0xA1,0xC8,0x99,0x89,0xD9,0x99,0xBE,0xDE,0x99,0xA1,0xC8,0x85 };
static const BYTE module6[12] = { 0xBB,0xAA,0xDF,0xA8,0xA6,0xC8,0x97,0x8C,0xC4,0x89,0xA1,0xDF };