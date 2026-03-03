#include "stdafx.h"
#include "Resolution.h"
#include "Offset.h"
#include "Util.h"

float Resolution_ClipX1 = 2000.0f;
float Resolution_ClipX2 = 2000.0f;

void InitResolution() // OK
{
	SetCompleteHook(0xE9,0x004DBFB6,&ResolutionSwitch);

	SetCompleteHook(0xE9,0x004DD18B,&ResolutionSwitchFont);

	SetCompleteHook(0xE9,0x00468368,&ResolutionMoveList);

	SetCompleteHook(0xE9,0x00467D23,&ResolutionMoveList2);

	SetCompleteHook(0xE8,0x006F39BA,&ResolutionKanturu);
}

void ResolutionKanturu() // OK
{
	if(*(DWORD*)(MAIN_RESOLUTION) >= 4)
	{
		*(WORD*)(*(DWORD*)(0x07ED7CD0)+0x0C) = 0xD7;
	}

	((bool(__thiscall*)(void*))0x0043517B)((void*)*(DWORD*)(0x07ED7CD0));
}

__declspec(naked) void ResolutionSwitch() // OK
{
	static DWORD ResolutionSwitchAddress1 = 0x004DC044;

	_asm
	{
		Mov Edx,Dword Ptr Ds:[MAIN_RESOLUTION]
		Mov Dword Ptr Ss:[Ebp-0x338],Edx
		Cmp Dword Ptr Ss:[Ebp-0x338],0x00
		Jnz NEXT1
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_X],640
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_Y],480
		Jmp EXIT
		NEXT1:
		Cmp Dword Ptr Ss:[Ebp-0x338],0x01
		Jnz NEXT2
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_X],800
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_Y],600
		Jmp EXIT
		NEXT2:
		Cmp Dword Ptr Ss:[Ebp-0x338],0x02
		Jnz NEXT3
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_X],1024
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_Y],768
		Jmp EXIT
		NEXT3:
		Cmp Dword Ptr Ss:[Ebp-0x338],0x03
		Jnz NEXT4
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_X],1280
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_Y],1024
		Jmp EXIT
		NEXT4:
		Cmp Dword Ptr Ss:[Ebp-0x338],0x04
		Jnz NEXT5
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_X],1360
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_Y],768
		Mov Dword Ptr Ds:[CASH_RESOLUTION_X],41
		Mov Dword Ptr Ds:[CASH_RESOLUTION_Y],-121
		Jmp EXIT
		NEXT5:
		Cmp Dword Ptr Ss:[Ebp-0x338],0x05
		Jnz NEXT6
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_X],1440
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_Y],900
		Mov Dword Ptr Ds:[CASH_RESOLUTION_X],81
		Mov Dword Ptr Ds:[CASH_RESOLUTION_Y],-55
		Jmp EXIT
		NEXT6:
		Cmp Dword Ptr Ss:[Ebp-0x338],0x06
		Jnz NEXT7
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_X],1600
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_Y],900
		Mov Dword Ptr Ds:[CASH_RESOLUTION_X],161
		Mov Dword Ptr Ds:[CASH_RESOLUTION_Y],-55
		Jmp EXIT
		NEXT7:
		Cmp Dword Ptr Ss:[Ebp-0x338],0x07
		Jnz NEXT8
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_X],1680
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_Y],1050
		Mov Dword Ptr Ds:[CASH_RESOLUTION_X],200
		Mov Dword Ptr Ds:[CASH_RESOLUTION_Y],20
		Jmp EXIT
		NEXT8:
		Cmp Dword Ptr Ss:[Ebp-0x338],0x08
		Jnz EXIT
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_X],1920
		Mov Dword Ptr Ds:[MAIN_RESOLUTION_Y],1080
		Mov Dword Ptr Ds:[CASH_RESOLUTION_X],320
		Mov Dword Ptr Ds:[CASH_RESOLUTION_Y],36
		EXIT:
		Jmp [ResolutionSwitchAddress1]
	}
}

__declspec(naked) void ResolutionSwitchFont() // OK
{
	static DWORD ResolutionSwitchFontAddress1 = 0x004DD205;

	_asm
	{
		Mov Eax,Dword Ptr Ds:[MAIN_RESOLUTION_X]
		Mov Dword Ptr Ss:[Ebp-0xEE8],Eax
		Cmp Dword Ptr Ss:[Ebp-0xEE8],640
		Jnz NEXT1
		Mov Dword Ptr Ds:[MAIN_FONT_SIZE],0x0C
		Jmp EXIT
		NEXT1:
		Cmp Dword Ptr Ss:[Ebp-0xEE8],800
		Jnz NEXT2
		Mov Dword Ptr Ds:[MAIN_FONT_SIZE],0x0D
		Jmp EXIT
		NEXT2:
		Cmp Dword Ptr Ss:[Ebp-0xEE8],1024
		Jnz NEXT3
		Mov Dword Ptr Ds:[MAIN_FONT_SIZE],0x0E
		Jmp EXIT
		NEXT3:
		Cmp Dword Ptr Ss:[Ebp-0xEE8],1280
		Jnz NEXT4
		Mov Dword Ptr Ds:[MAIN_FONT_SIZE],0x0F
		Jmp EXIT
		NEXT4:
		Mov Dword Ptr Ds:[MAIN_FONT_SIZE],0x0F
		Lea Ecx,Resolution_ClipX1
		Mov Edx,Dword Ptr Ds:[Ecx+0x00]
		Mov Eax,Dword Ptr Ds:[Ecx+0x04]
		Push Eax
		Push Edx
		Push 0x007B382C
		Call [SetDouble]
		Add Esp,0x08
		Lea Ecx,Resolution_ClipX2
		Mov Edx,Dword Ptr Ds:[Ecx+0x00]
		Mov Eax,Dword Ptr Ds:[Ecx+0x04]
		Push Eax
		Push Edx
		Push 0x007B383C
		Call [SetDouble]
		Add Esp,0x08
		EXIT:
		Jmp [ResolutionSwitchFontAddress1]
	}
}

__declspec(naked) void ResolutionMoveList() // OK
{
	static DWORD ResolutionMoveListAddress1 = 0x00468410;
	static DWORD ResolutionMoveListAddress2 = 0x00468373;

	_asm
	{
		Mov Ecx,Dword Ptr Ds:[MAIN_RESOLUTION_X]
		Mov Dword Ptr Ss:[Ebp-0x1B4],Ecx
		Cmp Dword Ptr Ss:[Ebp-0x1B4],0x500
		Jbe EXIT
		Jmp [ResolutionMoveListAddress1]
		EXIT:
		Jmp [ResolutionMoveListAddress2]
	}
}

__declspec(naked) void ResolutionMoveList2() // OK
{
	static DWORD ResolutionMoveListAddress1 = 0x00467DCB;
	static DWORD ResolutionMoveListAddress2 = 0x00467D2E;

	_asm
	{
		Mov Ecx,Dword Ptr Ds:[MAIN_RESOLUTION_X]
		Mov Dword Ptr Ss:[Ebp-0x1B4],Ecx
		Cmp Dword Ptr Ss:[Ebp-0x1B4],0x500
		Jbe EXIT
		Jmp [ResolutionMoveListAddress1]
		EXIT:
		Jmp [ResolutionMoveListAddress2]
	}
}