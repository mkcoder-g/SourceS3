#include "stdafx.h"
#include "PrintPlayer.h"
#include "Offset.h"
#include "Protect.h"
#include "Reconnect.h"
#include "Util.h"

DWORD ViewIndex = 0;
DWORD ViewLevel = 0;
DWORD ViewReset = 0;
DWORD ViewPoint = 0;
DWORD ViewCurHP = 0;
DWORD ViewMaxHP = 0;
DWORD ViewCurMP = 0;
DWORD ViewMaxMP = 0;
DWORD ViewCurBP = 0;
DWORD ViewMaxBP = 0;
DWORD ViewCurSD = 0;
DWORD ViewMaxSD = 0;
DWORD ViewDamageHP = 0;
DWORD ViewDamageSD = 0;
DWORD ViewExperience = 0;
DWORD ViewNextExperience = 0;
DWORD ViewStrength = 0;
DWORD ViewDexterity = 0;
DWORD ViewVitality = 0;
DWORD ViewEnergy = 0;
DWORD ViewLeadership = 0;
DWORD ViewAddStrength = 0;
DWORD ViewAddDexterity = 0;
DWORD ViewAddVitality = 0;
DWORD ViewAddEnergy = 0;
DWORD ViewAddLeadership = 0;
DWORD ViewPhysiSpeed = 0;
DWORD ViewMagicSpeed = 0;

void InitPrintPlayer() // OK
{
	SetCompleteHook(0xE8,0x006026DD,&PrintDrawCircleHPMP);

	SetCompleteHook(0xE8,0x006027A6,&PrintDrawCircleHPMP);

	SetCompleteHook(0xE8,0x0060191C,&PrintDrawCircleSDBP);

	SetCompleteHook(0xE8,0x00601CAC,&PrintDrawCircleSDBP);

	SetCompleteHook(0xE8,0x006051F9,&PrintBarExperience1);

	SetCompleteHook(0xE8,0x00605237,&PrintBarExperience2);

	SetCompleteHook(0xE8,0x00416045,&PrintPlayerViewHP1);

	SetCompleteHook(0xE8,0x00602737,&PrintPlayerViewHP1);

	SetCompleteHook(0xE8,0x0065FC9A,&PrintPlayerViewHP1);

	SetCompleteHook(0xE8,0x006035EB,&PrintPlayerViewHP2);

	SetCompleteHook(0xE8,0x006027FE,&PrintPlayerViewMP1);

	SetCompleteHook(0xE8,0x0065FF6E,&PrintPlayerViewMP1);

	SetCompleteHook(0xE8,0x00603612,&PrintPlayerViewMP2);

	SetCompleteHook(0xE8,0x0060197F,&PrintPlayerViewBP1);

	SetCompleteHook(0xE8,0x00601936,&PrintPlayerViewBP2);

	SetCompleteHook(0xE8,0x00601D0F,&PrintPlayerViewSD1);

	SetCompleteHook(0xE8,0x00601CC6,&PrintPlayerViewSD2);

	SetCompleteHook(0xE8,0x00660BDB,&PrintPlayerViewLevelUp);

	SetCompleteHook(0xE8,0x0065E274,&PrintPlayerViewStrength);

	SetCompleteHook(0xE8,0x0065F3C6,&PrintPlayerViewDexterity);

	SetCompleteHook(0xE8,0x0065FC0A,&PrintPlayerViewVitality);

	SetCompleteHook(0xE8,0x0065FEED,&PrintPlayerViewEnergy);

	SetCompleteHook(0xE8,0x0066115B,&PrintPlayerViewLeadership);

	SetCompleteHook(0xE8,0x0065F8CA,&PrintPlayerViewAttackSpeed);

	SetCompleteHook(0xE9,0x0059CE62,&PrintPlayerSetAttackSpeed);

	SetCompleteHook(0xE8,0x004EC2E7,&PrintDamageOnScreenHP);

	SetCompleteHook(0xE8,0x004EC6AC,&PrintDamageOnScreenHP);

	SetCompleteHook(0xE8,0x004EC7BA,&PrintDamageOnScreenHP);

	SetCompleteHook(0xE8,0x004EC811,&PrintDamageOnScreenSD);

	SetCompleteHook(0xE8,0x004ED3C5,&PrintDamageOnScreenHP);

	SetCompleteHook(0xE8,0x004ED420,&PrintDamageOnScreenHP);

	SetCompleteHook(0xE8,0x004ED4B7,&PrintDamageOnScreenHP);

	SetCompleteHook(0xE8,0x004ED512,&PrintDamageOnScreenHP);

	SetCompleteHook(0xE8,0x004ED52D,&PrintDamageOnScreenHP);

	SetCompleteHook(0xE8,0x004ED587,&PrintDamageOnScreenSD);

	SetCompleteHook(0xE8,0x004F1769,&PrintDamageOnScreenHP);

	SetCompleteHook(0xE8,0x004F20EC,&PrintDamageOnScreenHP);

	SetCompleteHook(0xE8,0x004F2122,&PrintDamageOnScreenHP);

	SetCompleteHook(0xE8,0x004F2C0E,&PrintDamageOnScreenHP);

	SetCompleteHook(0xE8,0x004F2C44,&PrintDamageOnScreenHP);

	SetCompleteHook(0xE8,0x00512EE1,&PrintDamageOnScreenHP);
}

void PrintDamageOnScreenHP(DWORD a,DWORD b,DWORD c,DWORD d,DWORD e) // OK
{
	if(((int)b) > 0)
	{
		b = ViewDamageHP;
	}

	((void(*)(DWORD,DWORD,DWORD,DWORD,DWORD))0x005926D0)(a,b,c,d,e);
}

void PrintDamageOnScreenSD(DWORD a,DWORD b,DWORD c,DWORD d,DWORD e) // OK
{
	if(((int)b) > 0)
	{
		b = ViewDamageSD;
	}

	((void(*)(DWORD,DWORD,DWORD,DWORD,DWORD))0x005926D0)(a,b,c,d,e);
}

void PrintDrawCircleHPMP(DWORD a,float b,float c,float d,float e,float f,float g,float h,float i,int j,int k,GLfloat l) // OK
{
	float HP = (float)(ViewMaxHP-ViewCurHP)/(float)ViewMaxHP;
	float MP = (float)(ViewMaxMP-ViewCurMP)/(float)ViewMaxMP;

	c = ((a==0xEB)?MP:HP)*48.0f+432.0f;
    e = 48.0f-((a==0xEB)?MP:HP)*48.0f;
    g = ((a==0xEB)?MP:HP)*48.0f/64.0f;
    i = (1.0f-((a==0xEB)?MP:HP))*48.0f/64.0f;

	return pDrawImage(a,b,c,d,e,f,g,h,i,j,k,l);
}

void PrintDrawCircleSDBP(DWORD a,float b,float c,float d,float e,float f,float g,float h,float i,int j,int k,GLfloat l) // OK
{
	float SD = (float)(ViewMaxSD-ViewCurSD)/(float)ViewMaxSD;
	float BP = (float)(ViewMaxBP-ViewCurBP)/(float)ViewMaxBP;

	c = ((a==0x101)?BP:SD)*36.0f+438.0f;
    e = 36.0f-((a==0x101)?BP:SD)*36.0f;
	g = ((a==0x101)?BP:SD)*36.0f/64.0f;
    i = (1.0f-((a==0x101)?BP:SD))*36.0f/64.0f;

	return pDrawImage(a,b,c,d,e,f,g,h,i,j,k,l);
}

void PrintBarExperience1(float a,float b,float c,float d,float e,int f) // OK
{
	float TotalBarValue = 1782.0f*((float)(gLevelExperience[ViewLevel]-ViewExperience)/(float)(gLevelExperience[ViewLevel]-gLevelExperience[ViewLevel-1]));
	float BarSplit = TotalBarValue/198.0f;
	float Modf[2];

	Modf[0] = modf(BarSplit,&Modf[1]);

	pDrawBarForm(a,b,(Modf[0]==0)?0:(198.0f-(Modf[0]*198.0f)),d,e,f);
}

void PrintBarExperience2(float a,float b,DWORD c,float d,float e) // OK
{
	float TotalBarValue = 1782.0f*((float)(gLevelExperience[ViewLevel]-ViewExperience)/(float)(gLevelExperience[ViewLevel]-gLevelExperience[ViewLevel-1]));
	float BarSplit = TotalBarValue/198.0f;
	float Modf[2];

	Modf[0] = modf(BarSplit,&Modf[1]);

	pDrawBigText(a,b,((DWORD)(9-Modf[1])>9)?9:(DWORD)(9-Modf[1]),d,e);
}

void PrintPlayerViewHP1(char* a, char* b) // OK
{
	wsprintf(a,b,ViewCurHP,ViewMaxHP);
}

void PrintPlayerViewHP2(float a,float b,DWORD c,float d,float e) // OK
{
	PrintFixStatPoint();

	pDrawBigText(a+6,b,ViewCurHP,d,e);
}

void PrintPlayerViewMP1(char* a, char* b) // OK
{
	wsprintf(a,b,ViewCurMP,ViewMaxMP);
}

void PrintPlayerViewMP2(float a,float b,DWORD c,float d,float e) // OK
{
	pDrawBigText(a-20,b,ViewCurMP,d,e);
}

void PrintPlayerViewBP1(char* a, char* b) // OK
{
	wsprintf(a,b,ViewCurBP,ViewMaxBP);
}

void PrintPlayerViewBP2(float a,float b,DWORD c,float d,float e) // OK
{
	pDrawBigText(a,b,ViewCurBP,d,e);
}

void PrintPlayerViewSD1(char* a, char* b) // OK
{
	wsprintf(a,b,ViewCurSD,ViewMaxSD);
}

void PrintPlayerViewSD2(float a,float b,DWORD c,float d,float e) // OK
{
	pDrawBigText(a,b,ViewCurSD,d,e);
}

void PrintPlayerViewLevelUp(char* a, char* b) //OK
{
	wsprintf(a,b,ViewPoint);
}

void PrintPlayerViewStrength(char* a, char* b) // OK
{
	wsprintf(a,b,ViewStrength+ViewAddStrength);
}

void PrintPlayerViewDexterity(char* a, char* b) // OK
{
	wsprintf(a,b,ViewDexterity+ViewAddDexterity);
}

void PrintPlayerViewVitality(char* a, char* b) // OK
{
	wsprintf(a,b,ViewVitality+ViewAddVitality);
}

void PrintPlayerViewEnergy(char* a, char* b) // OK
{
	wsprintf(a,b,ViewEnergy+ViewAddEnergy);
}

void PrintPlayerViewLeadership(char* a, char* b) // OK
{
	wsprintf(a,b,ViewLeadership+ViewAddLeadership);
}

void PrintPlayerViewAttackSpeed(char* a, char* b) // OK
{
	if(((*(BYTE*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x0B)) & 7) == 0)
	{
		wsprintf(a,b,ViewMagicSpeed);
	}
	else
	{
		wsprintf(a,b,ViewPhysiSpeed);
	}
}

void PrintFixStatPoint() // OK
{
	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x18) = GET_MAX_WORD_VALUE(ViewStrength);

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x32) = GET_MAX_WORD_VALUE(ViewAddStrength);

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x1A) = GET_MAX_WORD_VALUE(ViewDexterity);

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x34) = GET_MAX_WORD_VALUE(ViewAddDexterity);

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x1C) = GET_MAX_WORD_VALUE(ViewVitality);

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x36) = GET_MAX_WORD_VALUE(ViewAddVitality);

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x1E) = GET_MAX_WORD_VALUE(ViewEnergy);

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x38) = GET_MAX_WORD_VALUE(ViewAddEnergy);

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x20) = GET_MAX_WORD_VALUE(ViewLeadership);

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x3E) = GET_MAX_WORD_VALUE(ViewAddLeadership);

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x0E) = GET_MAX_WORD_VALUE(ViewLevel);

	*(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x70) = GET_MAX_WORD_VALUE(ViewPoint);

	*(DWORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x10) = (ViewExperience > ViewNextExperience) ? ViewNextExperience : ViewExperience;

	*(DWORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x14) = ViewNextExperience;
}

__declspec(naked) void PrintPlayerSetAttackSpeed() // OK
{
	static DWORD PrintPlayerSetAttackSpeedAddress1 = 0x0059CF3B;

	_asm
	{
		Mov Eax,ViewPhysiSpeed
		Mov Ecx,Dword Ptr Ss:[Ebp-0x20]
		Mov Edx,Dword Ptr Ds:[gProtect.m_MainInfo.MaxAttackSpeed+Ecx*4]
		And Edx,0xFFFF
		Cmp Eax,Edx
		Jle NEXT1
		Mov Eax,Edx
		NEXT1:
		Mov Ecx,Dword Ptr Ss:[Ebp-0x44]
		Mov Word Ptr Ds:[Ecx+0x54],Ax
		Mov Edx,ViewMagicSpeed
		Mov Eax,Dword Ptr Ss:[Ebp-0x20]
		Mov Ecx,Dword Ptr Ds:[gProtect.m_MainInfo.MaxAttackSpeed+Eax*4]
		And Ecx,0xFFFF
		Cmp Edx,Ecx
		Jle NEXT2
		Mov Edx,Ecx
		NEXT2:
		Mov Eax,Dword Ptr Ss:[Ebp-0x44]
		Mov Word Ptr Ds:[Eax+0x60],Dx
		Jmp [PrintPlayerSetAttackSpeedAddress1]
	}
}