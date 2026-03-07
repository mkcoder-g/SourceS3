#pragma once

#include "User.h"
#include <vector>

#define MAX_WORLD_BOSS_EVENTS 32

enum eCustomWorldBossState
{
	CUSTOM_WORLD_BOSS_STATE_BLANK = 0,
	CUSTOM_WORLD_BOSS_STATE_EMPTY = 1,
	CUSTOM_WORLD_BOSS_STATE_START = 2,
};

struct CUSTOM_WORLD_BOSS_START_TIME
{
	int EventIndex;
	int Year;
	int Month;
	int Day;
	int DayOfWeek;
	int Hour;
	int Minute;
	int Second;
};

struct CUSTOM_WORLD_BOSS_RULE_INFO
{
	int Index;
	char Name[64];
	int Map;
	int X;
	int Y;
	int Range;
	int AlarmTime;
	int EventTime;
	int BossClass;
	int BossDir;
	int KillerBagSpecial;
	int Top1BagSpecial;
	int Top2BagSpecial;
	int Top3BagSpecial;
	int ParticipationBagSpecial;
	int ParticipationMinDamage;
	int Fireworks;
};

struct CUSTOM_WORLD_BOSS_RANK_INFO
{
	int Index;
	int Damage;
};

struct CUSTOM_WORLD_BOSS_EVENT_INFO
{
	CUSTOM_WORLD_BOSS_RULE_INFO RuleInfo;
	std::vector<CUSTOM_WORLD_BOSS_START_TIME> StartTime;
	int State;
	int RemainTime;
	int TargetTime;
	DWORD TickCount;
	int AlarmMinSave;
	int AlarmMinLeft;
	int BossIndex;
	bool RewardSent[5];
};

class CCustomWorldBoss
{
public:
	CCustomWorldBoss();
	virtual ~CCustomWorldBoss();
	void Init();
	void ReadCustomWorldBossInfo(char* section,char* path);
	void Load(char* path);
	void MainProc();
	void MonsterDieProc(LPOBJ lpTarget,LPOBJ lpObj);
	bool IsWorldBoss(int index);
	void CommandStart(LPOBJ lpObj,int eventIndex);
	void CommandInfo(LPOBJ lpObj,int eventIndex);
	void CommandEnd(LPOBJ lpObj,int eventIndex);
private:
	void Clear();
	void ResetEvent(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo);
	void ProcState_BLANK(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo);
	void ProcState_EMPTY(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo);
	void ProcState_START(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo);
	void SetState(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo,int state);
	void SetState_BLANK(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo);
	void SetState_EMPTY(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo);
	void SetState_START(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo);
	void CheckSync(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo);
	void ClearBoss(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo);
	void GetBossRanking(LPOBJ lpBoss,std::vector<CUSTOM_WORLD_BOSS_RANK_INFO>& ranking);
	void SendRankingNotice(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo,LPOBJ lpBoss,LPOBJ lpKiller,std::vector<CUSTOM_WORLD_BOSS_RANK_INFO>& ranking);
	void GiveRewardBySpecialValue(int aIndex,int special,const char* rewardName,const char* bossName);
	void RewardParticipants(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo,LPOBJ lpBoss,std::vector<CUSTOM_WORLD_BOSS_RANK_INFO>& ranking);
	const char* GetMapNameSafe(int map);
	CUSTOM_WORLD_BOSS_EVENT_INFO* GetEventByBossIndex(int index);
	CUSTOM_WORLD_BOSS_EVENT_INFO* GetEventByRuleIndex(int index);
public:
	int m_CustomWorldBossSwitch;
	std::vector<CUSTOM_WORLD_BOSS_EVENT_INFO> m_EventInfo;
};

extern CCustomWorldBoss gCustomWorldBoss;
