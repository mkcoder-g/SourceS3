#include "stdafx.h"
#include "EventEntryLevel.h"
#include "Offset.h"
#include "Protect.h"
#include "Util.h"

void InitEventEntryLevel() // OK
{
	MemoryCpy(0x007B3688,gProtect.m_MainInfo.m_BloodCastleEntryLevelCommon,sizeof(gProtect.m_MainInfo.m_BloodCastleEntryLevelCommon));

	MemoryCpy(0x007B36C0,gProtect.m_MainInfo.m_BloodCastleEntryLevelSpecial,sizeof(gProtect.m_MainInfo.m_BloodCastleEntryLevelSpecial));

	MemoryCpy(0x007B3628,gProtect.m_MainInfo.m_DevilSquareEntryLevelCommon,sizeof(gProtect.m_MainInfo.m_DevilSquareEntryLevelCommon));

	MemoryCpy(0x007B3658,gProtect.m_MainInfo.m_DevilSquareEntryLevelSpecial,sizeof(gProtect.m_MainInfo.m_DevilSquareEntryLevelSpecial));

	MemoryCpy(0x007B36F8,gProtect.m_MainInfo.m_ChaosCastleEntryLevelCommon,sizeof(gProtect.m_MainInfo.m_ChaosCastleEntryLevelCommon));

	MemoryCpy(0x007B3728,gProtect.m_MainInfo.m_ChaosCastleEntryLevelSpecial,sizeof(gProtect.m_MainInfo.m_ChaosCastleEntryLevelSpecial));

	MemoryCpy(0x007B1BF4,gProtect.m_MainInfo.m_KalimaEntryLevelCommon,sizeof(gProtect.m_MainInfo.m_KalimaEntryLevelCommon));

	MemoryCpy(0x007B1C2C,gProtect.m_MainInfo.m_KalimaEntryLevelSpecial,sizeof(gProtect.m_MainInfo.m_KalimaEntryLevelSpecial));

	SetDword(0x0061837A,0x270F); // Chaos Castle MaxLevel

	SetDword(0x00451B83,0x270F); // Kalima MaxLevel

	SetDword(0x0074F7E9+3,gProtect.m_MainInfo.m_IllusionTempleEntryLevelMin[0]);

	SetDword(0x0074F7F0+3,gProtect.m_MainInfo.m_IllusionTempleEntryLevelMin[1]);

	SetDword(0x0074F7F7+3,gProtect.m_MainInfo.m_IllusionTempleEntryLevelMin[2]);

	SetDword(0x0074F7FE+3,gProtect.m_MainInfo.m_IllusionTempleEntryLevelMin[3]);

	SetDword(0x0074F805+3,gProtect.m_MainInfo.m_IllusionTempleEntryLevelMin[4]);

	SetDword(0x0074F80C+6,gProtect.m_MainInfo.m_IllusionTempleEntryLevelMax[0]);

	SetDword(0x0074F816+6,gProtect.m_MainInfo.m_IllusionTempleEntryLevelMax[1]);

	SetDword(0x0074F820+6,gProtect.m_MainInfo.m_IllusionTempleEntryLevelMax[2]);

	SetDword(0x0074F82A+6,gProtect.m_MainInfo.m_IllusionTempleEntryLevelMax[3]);

	SetDword(0x0074F834+3,gProtect.m_MainInfo.m_IllusionTempleEntryLevelMax[4]);

	SetCompleteHook(0xE8,0x006706F1,&CheckIllusionTemple);

	SetCompleteHook(0xE8,0x0074F1F7,&CheckIllusionTemple);

	SetCompleteHook(0xE8,0x0074F224,&CheckIllusionTemple);

	SetCompleteHook(0xE8,0x0074F487,&CheckIllusionTemple);

	SetCompleteHook(0xE8,0x0074F76A,&CheckIllusionTemple);
}

bool CheckIllusionTemple(int* value)
{
	STRUCT_DECRYPT

	WORD Level = *(WORD*)(*(DWORD*)(MAIN_CHARACTER_STRUCT)+0x0E);

	STRUCT_ENCRYPT

	for(int n=0;n < 5;n++)
	{
		if(Level >= gProtect.m_MainInfo.m_IllusionTempleEntryLevelMin[n] && Level <= gProtect.m_MainInfo.m_IllusionTempleEntryLevelMax[n])
		{
			(*value) = n+1;
			return 1;
		}
	}

	return 0;
}