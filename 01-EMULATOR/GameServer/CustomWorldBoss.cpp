#include "stdafx.h"
#include "CustomWorldBoss.h"
#include "MemScript.h"
#include "ScheduleManager.h"
#include "Monster.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "ItemBagManager.h"
#include "CustomEventDrop.h"
#include "MapManager.h"
#include "Log.h"
#include "Util.h"

CCustomWorldBoss gCustomWorldBoss;

CCustomWorldBoss::CCustomWorldBoss()
{
	this->m_CustomWorldBossSwitch = 1;
	this->m_EventInfo.clear();
}

CCustomWorldBoss::~CCustomWorldBoss()
{
}

void CCustomWorldBoss::Clear()
{
	for(std::vector<CUSTOM_WORLD_BOSS_EVENT_INFO>::iterator it=this->m_EventInfo.begin();it != this->m_EventInfo.end();it++)
	{
		this->ClearBoss(&(*it));
	}

	this->m_EventInfo.clear();
}

void CCustomWorldBoss::ResetEvent(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo)
{
	lpInfo->StartTime.clear();
	lpInfo->RuleInfo.Index = -1;
	strcpy_s(lpInfo->RuleInfo.Name,"World Boss");
	lpInfo->RuleInfo.Map = 0;
	lpInfo->RuleInfo.X = 125;
	lpInfo->RuleInfo.Y = 125;
	lpInfo->RuleInfo.Range = 5;
	lpInfo->RuleInfo.AlarmTime = 5;
	lpInfo->RuleInfo.EventTime = 10;
	lpInfo->RuleInfo.BossClass = -1;
	lpInfo->RuleInfo.BossDir = -1;
	lpInfo->RuleInfo.KillerBagSpecial = 0;
	lpInfo->RuleInfo.Top1BagSpecial = 0;
	lpInfo->RuleInfo.Top2BagSpecial = 0;
	lpInfo->RuleInfo.Top3BagSpecial = 0;
	lpInfo->RuleInfo.ParticipationBagSpecial = 0;
	lpInfo->RuleInfo.ParticipationMinDamage = 1;
	lpInfo->RuleInfo.Fireworks = 1;
	lpInfo->State = CUSTOM_WORLD_BOSS_STATE_BLANK;
	lpInfo->RemainTime = 0;
	lpInfo->TargetTime = 0;
	lpInfo->TickCount = GetTickCount();
	lpInfo->AlarmMinSave = -1;
	lpInfo->AlarmMinLeft = -1;
	lpInfo->BossIndex = -1;
	for(int n=0;n < 5;n++)
	{
		lpInfo->RewardSent[n] = false;
	}
}

void CCustomWorldBoss::Init()
{
	for(std::vector<CUSTOM_WORLD_BOSS_EVENT_INFO>::iterator it=this->m_EventInfo.begin();it != this->m_EventInfo.end();it++)
	{
		this->ClearBoss(&(*it));

		if(this->m_CustomWorldBossSwitch == 0 || it->RuleInfo.BossClass < 0)
		{
			this->SetState(&(*it),CUSTOM_WORLD_BOSS_STATE_BLANK);
		}
		else
		{
			this->SetState(&(*it),CUSTOM_WORLD_BOSS_STATE_EMPTY);
		}
	}
}

void CCustomWorldBoss::ReadCustomWorldBossInfo(char* section,char* path)
{
	this->m_CustomWorldBossSwitch = GetPrivateProfileInt(section,"CustomWorldBossSwitch",1,path);
}


void CCustomWorldBoss::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->Clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while(true)
			{
				if(strcmp("end",lpMemScript->GetAsString()) == 0)
				{
					break;
				}

				if(section == 0)
				{
					CUSTOM_WORLD_BOSS_START_TIME info;
					info.EventIndex = lpMemScript->GetNumber();
					info.Year = lpMemScript->GetAsNumber();
					info.Month = lpMemScript->GetAsNumber();
					info.Day = lpMemScript->GetAsNumber();
					info.DayOfWeek = lpMemScript->GetAsNumber();
					info.Hour = lpMemScript->GetAsNumber();
					info.Minute = lpMemScript->GetAsNumber();
					info.Second = lpMemScript->GetAsNumber();

					CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo = this->GetEventByRuleIndex(info.EventIndex);

					if(lpInfo == 0)
					{
						CUSTOM_WORLD_BOSS_EVENT_INFO eventInfo;
						this->ResetEvent(&eventInfo);
						eventInfo.RuleInfo.Index = info.EventIndex;
						eventInfo.StartTime.push_back(info);
						this->m_EventInfo.push_back(eventInfo);
					}
					else
					{
						lpInfo->StartTime.push_back(info);
					}
				}
				else if(section == 1)
				{
					CUSTOM_WORLD_BOSS_RULE_INFO info;
					memset(&info,0,sizeof(info));
					info.Index = lpMemScript->GetNumber();
					strcpy_s(info.Name,lpMemScript->GetAsString());
					info.Map = lpMemScript->GetAsNumber();
					info.X = lpMemScript->GetAsNumber();
					info.Y = lpMemScript->GetAsNumber();
					info.Range = lpMemScript->GetAsNumber();
					info.AlarmTime = lpMemScript->GetAsNumber();
					info.EventTime = lpMemScript->GetAsNumber();
					info.BossClass = lpMemScript->GetAsNumber();
					info.BossDir = lpMemScript->GetAsNumber();
					info.KillerBagSpecial = lpMemScript->GetAsNumber();
					info.Top1BagSpecial = lpMemScript->GetAsNumber();
					info.Top2BagSpecial = lpMemScript->GetAsNumber();
					info.Top3BagSpecial = lpMemScript->GetAsNumber();
					info.ParticipationBagSpecial = lpMemScript->GetAsNumber();
					info.ParticipationMinDamage = lpMemScript->GetAsNumber();
					info.Fireworks = lpMemScript->GetAsNumber();

					CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo = this->GetEventByRuleIndex(info.Index);

					if(lpInfo == 0)
					{
						CUSTOM_WORLD_BOSS_EVENT_INFO eventInfo;
						this->ResetEvent(&eventInfo);
						eventInfo.RuleInfo = info;
						this->m_EventInfo.push_back(eventInfo);
					}
					else
					{
						lpInfo->RuleInfo = info;
					}
				}
				else
				{
					break;
				}
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;

	this->Init();

	gLog.Output(LOG_GENERAL,"[CustomWorldBoss] Loaded %d event(s)",(int)this->m_EventInfo.size());
}

void CCustomWorldBoss::MainProc()
{
	if(this->m_CustomWorldBossSwitch == 0)
	{
		return;
	}

	for(std::vector<CUSTOM_WORLD_BOSS_EVENT_INFO>::iterator it=this->m_EventInfo.begin();it != this->m_EventInfo.end();it++)
	{
		if((GetTickCount()-it->TickCount) < 1000)
		{
			continue;
		}

		it->TickCount = GetTickCount();
		it->RemainTime = (int)difftime(it->TargetTime,time(0));

		switch(it->State)
		{
			case CUSTOM_WORLD_BOSS_STATE_BLANK:
				this->ProcState_BLANK(&(*it));
				break;
			case CUSTOM_WORLD_BOSS_STATE_EMPTY:
				this->ProcState_EMPTY(&(*it));
				break;
			case CUSTOM_WORLD_BOSS_STATE_START:
				this->ProcState_START(&(*it));
				break;
		}
	}
}

void CCustomWorldBoss::ProcState_BLANK(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo)
{
}

void CCustomWorldBoss::ProcState_EMPTY(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo)
{
	if(lpInfo->RemainTime > 0 && lpInfo->RemainTime <= (lpInfo->RuleInfo.AlarmTime*60))
	{
		if((lpInfo->AlarmMinSave=(((lpInfo->RemainTime%60)==0)?((lpInfo->RemainTime/60)-1):(lpInfo->RemainTime/60))) != lpInfo->AlarmMinLeft)
		{
			lpInfo->AlarmMinLeft = lpInfo->AlarmMinSave;
			gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[WORLD BOSS]\n%s começa em %d minuto(s)\nMapa: %s \nCoordenadas: %d, %d",lpInfo->RuleInfo.Name,(lpInfo->AlarmMinLeft+1),this->GetMapNameSafe(lpInfo->RuleInfo.Map),lpInfo->RuleInfo.X,lpInfo->RuleInfo.Y);
		}
	}

	if(lpInfo->RemainTime <= 0)
	{
		this->SetState(lpInfo,CUSTOM_WORLD_BOSS_STATE_START);
	}
}

void CCustomWorldBoss::ProcState_START(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo)
{
	if(lpInfo->RemainTime <= 0)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[WORLD BOSS]\n%s terminou por tempo esgotado!",lpInfo->RuleInfo.Name);
		this->SetState(lpInfo,CUSTOM_WORLD_BOSS_STATE_EMPTY);
		return;
	}

	if(OBJECT_RANGE(lpInfo->BossIndex) == 0)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[WORLD BOSS]\n%s terminou! Boss removido.",lpInfo->RuleInfo.Name);
		this->SetState(lpInfo,CUSTOM_WORLD_BOSS_STATE_EMPTY);
		return;
	}

	if(gObj[lpInfo->BossIndex].Connected != OBJECT_ONLINE || gObj[lpInfo->BossIndex].Live == 0)
	{
		return;
	}
}

void CCustomWorldBoss::SetState(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo,int state)
{
	lpInfo->State = state;

	switch(lpInfo->State)
	{
		case CUSTOM_WORLD_BOSS_STATE_BLANK:
			this->SetState_BLANK(lpInfo);
			break;
		case CUSTOM_WORLD_BOSS_STATE_EMPTY:
			this->SetState_EMPTY(lpInfo);
			break;
		case CUSTOM_WORLD_BOSS_STATE_START:
			this->SetState_START(lpInfo);
			break;
	}
}

void CCustomWorldBoss::SetState_BLANK(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo)
{
	lpInfo->RemainTime = 0;
	lpInfo->TargetTime = 0;
	lpInfo->AlarmMinSave = -1;
	lpInfo->AlarmMinLeft = -1;
	this->ClearBoss(lpInfo);
}

void CCustomWorldBoss::SetState_EMPTY(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo)
{
	lpInfo->AlarmMinSave = -1;
	lpInfo->AlarmMinLeft = -1;
	this->ClearBoss(lpInfo);
	this->CheckSync(lpInfo);
}

void CCustomWorldBoss::SetState_START(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo)
{
	this->ClearBoss(lpInfo);

	if(lpInfo->RuleInfo.BossClass < 0)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[WORLD BOSS]\nERRO: BossClass invalido em %s",lpInfo->RuleInfo.Name);
		this->SetState(lpInfo,CUSTOM_WORLD_BOSS_STATE_EMPTY);
		return;
	}

	int index = gObjAddMonster(lpInfo->RuleInfo.Map);

	if(OBJECT_RANGE(index) == 0)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[WORLD BOSS]\nERRO: nao foi possivel criar o boss %s",lpInfo->RuleInfo.Name);
		this->SetState(lpInfo,CUSTOM_WORLD_BOSS_STATE_EMPTY);
		return;
	}

	LPOBJ lpBoss = &gObj[index];

	int px = lpInfo->RuleInfo.X;
	int py = lpInfo->RuleInfo.Y;

	if(gObjGetRandomMonsterLocation(lpInfo->RuleInfo.Map,&px,&py,lpInfo->RuleInfo.Range,lpInfo->RuleInfo.Range,50) == 0)
	{
		px = lpInfo->RuleInfo.X;
		py = lpInfo->RuleInfo.Y;
	}

	lpBoss->PosNum = -1;
	lpBoss->X = px;
	lpBoss->Y = py;
	lpBoss->TX = px;
	lpBoss->TY = py;
	lpBoss->OldX = px;
	lpBoss->OldY = py;
	lpBoss->StartX = px;
	lpBoss->StartY = py;
	lpBoss->Dir = ((lpInfo->RuleInfo.BossDir==-1)?(GetLargeRand()%8):lpInfo->RuleInfo.BossDir);
	lpBoss->Map = lpInfo->RuleInfo.Map;
	lpBoss->MoveRange = 0;

	if(gObjSetMonster(index,lpInfo->RuleInfo.BossClass) == 0)
	{
		gObjDel(index);
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[WORLD BOSS]\nERRO: nao foi possivel setar o boss %s (Class:%d)",lpInfo->RuleInfo.Name,lpInfo->RuleInfo.BossClass);
		this->SetState(lpInfo,CUSTOM_WORLD_BOSS_STATE_EMPTY);
		return;
	}

	lpBoss->DieRegen = 0;
	lpBoss->RegenTime = 1;
	lpBoss->MoveRange = 0;
	lpBoss->PathCount = 0;
	lpBoss->Rest = 0;

	gObjMonsterInitHitDamage(lpBoss);

	lpInfo->BossIndex = index;
	for(int n=0;n < 5;n++)
	{
		lpInfo->RewardSent[n] = false;
	}

	lpInfo->RemainTime = lpInfo->RuleInfo.EventTime*60;
	lpInfo->TargetTime = (int)(time(0)+lpInfo->RemainTime);

	gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[WORLD BOSS]\n%s apareceu!\nMapa: %s \nCoordenadas: %d, %d",lpInfo->RuleInfo.Name,this->GetMapNameSafe(lpInfo->RuleInfo.Map),px,py);
	gLog.Output(LOG_GENERAL,"[CustomWorldBoss] Spawned [%s] Class:%d Map:%s(%d) (%d,%d) Index:%d",lpInfo->RuleInfo.Name,lpInfo->RuleInfo.BossClass,this->GetMapNameSafe(lpInfo->RuleInfo.Map),lpInfo->RuleInfo.Map,px,py,index);
}

void CCustomWorldBoss::CheckSync(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo)
{
	if(lpInfo->StartTime.empty() != 0)
	{
		this->SetState(lpInfo,CUSTOM_WORLD_BOSS_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;
	CScheduleManager ScheduleManager;

	for(std::vector<CUSTOM_WORLD_BOSS_START_TIME>::iterator it=lpInfo->StartTime.begin();it != lpInfo->StartTime.end();it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(lpInfo,CUSTOM_WORLD_BOSS_STATE_BLANK);
		return;
	}

	lpInfo->RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));
	lpInfo->TargetTime = (int)ScheduleTime.GetTime();

	gLog.Output(LOG_GENERAL,"[CustomWorldBoss] Sync [%s] remain %d minute(s)",lpInfo->RuleInfo.Name,(lpInfo->RemainTime/60));
}

void CCustomWorldBoss::ClearBoss(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo)
{
	if(OBJECT_RANGE(lpInfo->BossIndex) != 0)
	{
		if(gObj[lpInfo->BossIndex].Connected == OBJECT_ONLINE && gObj[lpInfo->BossIndex].Type == OBJECT_MONSTER)
		{
			gObjDel(lpInfo->BossIndex);
		}
	}

	lpInfo->BossIndex = -1;
}


const char* CCustomWorldBoss::GetMapNameSafe(int map)
{
	char* name = gMapManager.GetMapName(map);

	if(name != 0 && name[0] != 0)
	{
		return name;
	}

	static char text[32];
	wsprintf(text,"Mapa %d",map);
	return text;
}

bool CCustomWorldBoss::IsWorldBoss(int index)
{
	return (this->GetEventByBossIndex(index) != 0);
}

CUSTOM_WORLD_BOSS_EVENT_INFO* CCustomWorldBoss::GetEventByBossIndex(int index)
{
	for(std::vector<CUSTOM_WORLD_BOSS_EVENT_INFO>::iterator it=this->m_EventInfo.begin();it != this->m_EventInfo.end();it++)
	{
		if(it->BossIndex == index)
		{
			return &(*it);
		}
	}

	return 0;
}

CUSTOM_WORLD_BOSS_EVENT_INFO* CCustomWorldBoss::GetEventByRuleIndex(int index)
{
	for(std::vector<CUSTOM_WORLD_BOSS_EVENT_INFO>::iterator it=this->m_EventInfo.begin();it != this->m_EventInfo.end();it++)
	{
		if(it->RuleInfo.Index == index)
		{
			return &(*it);
		}
	}

	return 0;
}

void CCustomWorldBoss::GetBossRanking(LPOBJ lpBoss,std::vector<CUSTOM_WORLD_BOSS_RANK_INFO>& ranking)
{
	ranking.clear();

	for(int n=0;n < MAX_HIT_DAMAGE;n++)
	{
		if(OBJECT_RANGE(lpBoss->HitDamage[n].index) == 0)
		{
			continue;
		}

		if(gObjIsConnected(lpBoss->HitDamage[n].index) == 0)
		{
			continue;
		}

		if(gObj[lpBoss->HitDamage[n].index].Type != OBJECT_USER)
		{
			continue;
		}

		if(lpBoss->HitDamage[n].damage <= 0)
		{
			continue;
		}

		CUSTOM_WORLD_BOSS_RANK_INFO info;
		info.Index = lpBoss->HitDamage[n].index;
		info.Damage = lpBoss->HitDamage[n].damage;
		ranking.push_back(info);
	}

	std::sort(ranking.begin(),ranking.end(),[](const CUSTOM_WORLD_BOSS_RANK_INFO& a,const CUSTOM_WORLD_BOSS_RANK_INFO& b){return (a.Damage > b.Damage);});
}

void CCustomWorldBoss::GiveRewardBySpecialValue(int aIndex,int special,const char* rewardName,const char* bossName)
{
	if(OBJECT_RANGE(aIndex) == 0 || special <= 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	gItemBagManager.DropItemBySpecialValue(special,0,0,lpObj,lpObj->Map,lpObj->X,lpObj->Y);
	gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,"[WORLD BOSS]\nReward [%s] recebido no evento %s!",rewardName,bossName);
}

void CCustomWorldBoss::RewardParticipants(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo,LPOBJ lpBoss,std::vector<CUSTOM_WORLD_BOSS_RANK_INFO>& ranking)
{
	if(lpInfo->RuleInfo.ParticipationBagSpecial <= 0)
	{
		return;
	}

	for(std::vector<CUSTOM_WORLD_BOSS_RANK_INFO>::iterator it=ranking.begin();it != ranking.end();it++)
	{
		if(it->Damage < lpInfo->RuleInfo.ParticipationMinDamage)
		{
			continue;
		}

		this->GiveRewardBySpecialValue(it->Index,lpInfo->RuleInfo.ParticipationBagSpecial,"Participation",lpInfo->RuleInfo.Name);
	}
}

void CCustomWorldBoss::SendRankingNotice(CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo,LPOBJ lpBoss,LPOBJ lpKiller,std::vector<CUSTOM_WORLD_BOSS_RANK_INFO>& ranking)
{
	char rank1[128] = "";
	char rank2[128] = "";
	char rank3[128] = "";

	if(ranking.size() > 0)
	{
		sprintf_s(rank1,"1º %s (%d)",gObj[ranking[0].Index].Name,ranking[0].Damage);
	}

	if(ranking.size() > 1)
	{
		sprintf_s(rank2,"2º %s (%d)",gObj[ranking[1].Index].Name,ranking[1].Damage);
	}

	if(ranking.size() > 2)
	{
		sprintf_s(rank3,"3º %s (%d)",gObj[ranking[2].Index].Name,ranking[2].Damage);
	}

	gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[WORLD BOSS]\n%s foi derrotado por %s!\nTop Damage: %s %s %s",lpInfo->RuleInfo.Name,((lpKiller!=0)?lpKiller->Name:"Unknown"),rank1,rank2,rank3);
}



void CCustomWorldBoss::CommandStart(LPOBJ lpObj,int eventIndex)
{
	if(lpObj == 0)
	{
		return;
	}

	if(this->m_CustomWorldBossSwitch == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[WORLD BOSS]\nSistema desativado no GameServerInfo - Custom.dat");
		return;
	}

	CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo = this->GetEventByRuleIndex(eventIndex);

	if(lpInfo == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[WORLD BOSS]\nEvento %d nao encontrado no script.",eventIndex);
		return;
	}

	if(lpInfo->State == CUSTOM_WORLD_BOSS_STATE_START)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[WORLD BOSS]\n%s ja esta em andamento.",lpInfo->RuleInfo.Name);
		return;
	}

	gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[WORLD BOSS]\n%s foi iniciado pelo GM %s.",lpInfo->RuleInfo.Name,lpObj->Name);
	gLog.Output(LOG_COMMAND,"[CustomWorldBoss][%s][%s] - Start EventIndex:%d Name:%s",lpObj->Account,lpObj->Name,eventIndex,lpInfo->RuleInfo.Name);
	this->SetState(lpInfo,CUSTOM_WORLD_BOSS_STATE_START);
}

void CCustomWorldBoss::CommandInfo(LPOBJ lpObj,int eventIndex)
{
	if(lpObj == 0)
	{
		return;
	}

	if(this->m_CustomWorldBossSwitch == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[WORLD BOSS]\nSistema desativado no GameServerInfo - Custom.dat");
		return;
	}

	if(eventIndex >= 0)
	{
		CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo = this->GetEventByRuleIndex(eventIndex);

		if(lpInfo == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[WORLD BOSS]\nEvento %d nao encontrado no script.",eventIndex);
			return;
		}

		char stateName[16] = {0};
		if(lpInfo->State == CUSTOM_WORLD_BOSS_STATE_BLANK) strcpy_s(stateName,"BLANK");
		else if(lpInfo->State == CUSTOM_WORLD_BOSS_STATE_EMPTY) strcpy_s(stateName,"EMPTY");
		else if(lpInfo->State == CUSTOM_WORLD_BOSS_STATE_START) strcpy_s(stateName,"START");
		else strcpy_s(stateName,"?");

		if(lpInfo->State == CUSTOM_WORLD_BOSS_STATE_START)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[WORLD BOSS]\n[%d] %s | Estado: %s | Restante: %d s \n| Mapa: %s | Coord: %d, %d | BossClass: %d",lpInfo->RuleInfo.Index,lpInfo->RuleInfo.Name,stateName,lpInfo->RemainTime,this->GetMapNameSafe(lpInfo->RuleInfo.Map),lpInfo->RuleInfo.X,lpInfo->RuleInfo.Y,lpInfo->RuleInfo.BossClass);
		}
		else
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[WORLD BOSS]\n[%d] %s | Estado:%s | Proximo em:%d s \n| Mapa: %s | Coord: %d, %d | BossClass: %d",lpInfo->RuleInfo.Index,lpInfo->RuleInfo.Name,stateName,lpInfo->RemainTime,this->GetMapNameSafe(lpInfo->RuleInfo.Map),lpInfo->RuleInfo.X,lpInfo->RuleInfo.Y,lpInfo->RuleInfo.BossClass);
		}
		return;
	}

	if(this->m_EventInfo.empty() != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[WORLD BOSS]\nNenhum evento carregado no script.");
		return;
	}

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[WORLD BOSS]\nTotal de eventos carregados: %d",(int)this->m_EventInfo.size());

	for(std::vector<CUSTOM_WORLD_BOSS_EVENT_INFO>::iterator it=this->m_EventInfo.begin();it != this->m_EventInfo.end();it++)
	{
		char stateName[16] = {0};
		if(it->State == CUSTOM_WORLD_BOSS_STATE_BLANK) strcpy_s(stateName,"BLANK");
		else if(it->State == CUSTOM_WORLD_BOSS_STATE_EMPTY) strcpy_s(stateName,"EMPTY");
		else if(it->State == CUSTOM_WORLD_BOSS_STATE_START) strcpy_s(stateName,"START");
		else strcpy_s(stateName,"?");

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[WORLD BOSS]\n[%d] %s | Estado:%s | Mapa:%s | Coord:%d,%d | Restante:%d s",it->RuleInfo.Index,it->RuleInfo.Name,stateName,this->GetMapNameSafe(it->RuleInfo.Map),it->RuleInfo.X,it->RuleInfo.Y,it->RemainTime);
	}
}

void CCustomWorldBoss::CommandEnd(LPOBJ lpObj,int eventIndex)
{
	if(lpObj == 0)
	{
		return;
	}

	if(this->m_CustomWorldBossSwitch == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[WORLD BOSS]\nSistema desativado no GameServerInfo - Custom.dat");
		return;
	}

	CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo = this->GetEventByRuleIndex(eventIndex);

	if(lpInfo == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[WORLD BOSS]\nEvento %d nao encontrado no script.",eventIndex);
		return;
	}

	if(lpInfo->State != CUSTOM_WORLD_BOSS_STATE_START)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[WORLD BOSS]\n%s nao esta em andamento.",lpInfo->RuleInfo.Name);
		return;
	}

	gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[WORLD BOSS]\n%s foi finalizado pelo GM %s.",lpInfo->RuleInfo.Name,lpObj->Name);
	gLog.Output(LOG_COMMAND,"[CustomWorldBoss][%s][%s] - End EventIndex:%d Name:%s",lpObj->Account,lpObj->Name,eventIndex,lpInfo->RuleInfo.Name);
	this->SetState(lpInfo,CUSTOM_WORLD_BOSS_STATE_EMPTY);
}

void CCustomWorldBoss::MonsterDieProc(LPOBJ lpTarget,LPOBJ lpObj)
{
	CUSTOM_WORLD_BOSS_EVENT_INFO* lpInfo = this->GetEventByBossIndex(lpTarget->Index);

	if(lpInfo == 0)
	{
		return;
	}

	if(lpInfo->State != CUSTOM_WORLD_BOSS_STATE_START)
	{
		return;
	}

	std::vector<CUSTOM_WORLD_BOSS_RANK_INFO> ranking;
	this->GetBossRanking(lpTarget,ranking);

	if(lpObj != 0 && lpInfo->RuleInfo.KillerBagSpecial > 0)
	{
		this->GiveRewardBySpecialValue(lpObj->Index,lpInfo->RuleInfo.KillerBagSpecial,"Killer",lpInfo->RuleInfo.Name);
	}

	if(ranking.size() > 0 && lpInfo->RuleInfo.Top1BagSpecial > 0)
	{
		this->GiveRewardBySpecialValue(ranking[0].Index,lpInfo->RuleInfo.Top1BagSpecial,"Top 1",lpInfo->RuleInfo.Name);
	}

	if(ranking.size() > 1 && lpInfo->RuleInfo.Top2BagSpecial > 0)
	{
		this->GiveRewardBySpecialValue(ranking[1].Index,lpInfo->RuleInfo.Top2BagSpecial,"Top 2",lpInfo->RuleInfo.Name);
	}

	if(ranking.size() > 2 && lpInfo->RuleInfo.Top3BagSpecial > 0)
	{
		this->GiveRewardBySpecialValue(ranking[2].Index,lpInfo->RuleInfo.Top3BagSpecial,"Top 3",lpInfo->RuleInfo.Name);
	}

	this->RewardParticipants(lpInfo,lpTarget,ranking);

	if(lpInfo->RuleInfo.Fireworks != 0)
	{
		gCustomEventDrop.GCFireworksSendToNearUser(lpTarget->Map,lpTarget->X,lpTarget->Y);
	}

	this->SendRankingNotice(lpInfo,lpTarget,lpObj,ranking);

	gLog.Output(LOG_GENERAL,"[CustomWorldBoss] [%s] Dead Killer:[%s] RankingCount:%d",lpInfo->RuleInfo.Name,((lpObj!=0)?lpObj->Name:"Unknown"),(int)ranking.size());

	this->SetState(lpInfo,CUSTOM_WORLD_BOSS_STATE_EMPTY);
}
