// CustomTreasureHunt.h: interface for the CCustomTreasureHunt class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <map>

// Forward declarations to avoid heavy includes
struct OBJECTSTRUCT;
typedef OBJECTSTRUCT* LPOBJ;

#define MAX_CUSTOM_TREASURE_HUNT_CHESTS 64

enum eCustomTreasureHuntState
{
	CUSTOM_TREASURE_HUNT_STATE_BLANK = 0,
	CUSTOM_TREASURE_HUNT_STATE_EMPTY = 1,
	CUSTOM_TREASURE_HUNT_STATE_START = 2,
};

struct CUSTOM_TREASURE_HUNT_START_TIME
{
	int Year;
	int Month;
	int Day;
	int DayOfWeek;
	int Hour;
	int Minute;
	int Second;
};

struct CUSTOM_TREASURE_HUNT_RULE_INFO
{
	char Name[32];
	int Map;
	int X;
	int Y;
	int Range;
	int AlarmTime;   // minutes before start
	int EventTime;   // minutes running
	int ChestClass;  // monster class for the chest NPC
	int ChestCount;  // total chests spawned
	int RealBagSpecial; // ItemBagManager SpecialValue for real chest
	int FakeBagSpecial; // ItemBagManager SpecialValue for fake chest
	int Fireworks;   // 1 = send fireworks near chest on real found
};

class CCustomTreasureHunt
{
public:
	CCustomTreasureHunt();
	virtual ~CCustomTreasureHunt();

	void Init();
	void Load(char* path);
	void ReadCustomTreasureHuntInfo(char* section,char* path);

	void MainProc();
	void MonsterDieProc(LPOBJ lpTarget,LPOBJ lpObj);

	// Commands
	void CommandStart(LPOBJ lpObj); // GM
	void CommandInfo(LPOBJ lpObj);
	void CommandEnd(LPOBJ lpObj);

	int GetState() const { return this->m_State; }
	int GetRemainTime() const { return this->m_RemainTime; }
	int GetChestAliveCount();

private:
	void SetState(int state);
	void SetState_BLANK();
	void SetState_EMPTY();
	void SetState_START();

	void ProcState_BLANK();
	void ProcState_EMPTY();
	void ProcState_START();

	void CheckSync();
	void ClearChests();
	bool IsEventChestIndex(int index);

private:
	int m_CustomTreasureHuntSwitch;
	int m_State;
	int m_RemainTime;
	time_t m_TargetTime;
	DWORD m_TickCount;
	int m_AlarmMinSave;
	int m_AlarmMinLeft;

	CUSTOM_TREASURE_HUNT_RULE_INFO m_RuleInfo;
	std::vector<CUSTOM_TREASURE_HUNT_START_TIME> m_StartTime;

	std::vector<int> m_ChestIndex;
	int m_RealChestIndex;
};

extern CCustomTreasureHunt gCustomTreasureHunt;
