// CustomTreasureHunt.cpp: implementation of the CCustomTreasureHunt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomTreasureHunt.h"
#include "User.h"
#include "ObjectManager.h"
#include "Monster.h"
#include "Notice.h"
#include "ItemBagManager.h"
#include "Util.h"

#include "CustomEventDrop.h"
#include "DSProtocol.h"
#include "MapManager.h"
#include "MemScript.h"
#include "ScheduleManager.h"

CCustomTreasureHunt gCustomTreasureHunt;

CCustomTreasureHunt::CCustomTreasureHunt() // OK
{
	this->m_CustomTreasureHuntSwitch = 0;
	this->m_State = CUSTOM_TREASURE_HUNT_STATE_BLANK;
	this->m_RemainTime = 0;
	this->m_TargetTime = 0;
	this->m_TickCount = GetTickCount();
	this->m_AlarmMinSave = -1;
	this->m_AlarmMinLeft = -1;
	memset(&this->m_RuleInfo,0,sizeof(this->m_RuleInfo));
	strcpy_s(this->m_RuleInfo.Name,"Treasure Hunt");
	this->m_RuleInfo.Map = 0;
	this->m_RuleInfo.X = 0;
	this->m_RuleInfo.Y = 0;
	this->m_RuleInfo.Range = 5;
	this->m_RuleInfo.AlarmTime = 5;
	this->m_RuleInfo.EventTime = 3;
	this->m_RuleInfo.ChestClass = 0;
	this->m_RuleInfo.ChestCount = 10;
	this->m_RuleInfo.RealBagSpecial = 0;
	this->m_RuleInfo.FakeBagSpecial = 0;
	this->m_RuleInfo.Fireworks = 1;
	this->m_RealChestIndex = -1;
}

CCustomTreasureHunt::~CCustomTreasureHunt() // OK
{
}

void CCustomTreasureHunt::Init() // OK
{
	this->ClearChests();

	if(this->m_CustomTreasureHuntSwitch == 0)
	{
		this->SetState(CUSTOM_TREASURE_HUNT_STATE_BLANK);
	}
	else
	{
		this->SetState(CUSTOM_TREASURE_HUNT_STATE_EMPTY);
	}
}

void CCustomTreasureHunt::ReadCustomTreasureHuntInfo(char* section,char* path) // OK
{
	this->m_CustomTreasureHuntSwitch = GetPrivateProfileInt(section,"CustomTreasureHuntSwitch",0,path);
}

void CCustomTreasureHunt::Load(char* path) // OK
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

	this->m_StartTime.clear();
	this->ClearChests();
	this->m_RealChestIndex = -1;

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
				if(section == 0)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					CUSTOM_TREASURE_HUNT_START_TIME info;
					info.Year = lpMemScript->GetNumber();
					info.Month = lpMemScript->GetAsNumber();
					info.Day = lpMemScript->GetAsNumber();
					info.DayOfWeek = lpMemScript->GetAsNumber();
					info.Hour = lpMemScript->GetAsNumber();
					info.Minute = lpMemScript->GetAsNumber();
					info.Second = lpMemScript->GetAsNumber();
					this->m_StartTime.push_back(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					strcpy_s(this->m_RuleInfo.Name,lpMemScript->GetAsString());
					this->m_RuleInfo.Map = lpMemScript->GetAsNumber();
					this->m_RuleInfo.X = lpMemScript->GetAsNumber();
					this->m_RuleInfo.Y = lpMemScript->GetAsNumber();
					this->m_RuleInfo.Range = lpMemScript->GetAsNumber();
					this->m_RuleInfo.AlarmTime = lpMemScript->GetAsNumber();
					this->m_RuleInfo.EventTime = lpMemScript->GetAsNumber();
					this->m_RuleInfo.ChestClass = lpMemScript->GetAsNumber();
					this->m_RuleInfo.ChestCount = lpMemScript->GetAsNumber();
					this->m_RuleInfo.RealBagSpecial = lpMemScript->GetAsNumber();
					this->m_RuleInfo.FakeBagSpecial = lpMemScript->GetAsNumber();
					this->m_RuleInfo.Fireworks = lpMemScript->GetAsNumber();
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
}

void CCustomTreasureHunt::MainProc() // OK
{
	if((GetTickCount()-this->m_TickCount) < 1000)
	{
		return;
	}

	this->m_TickCount = GetTickCount();
	this->m_RemainTime = (int)difftime(this->m_TargetTime,time(0));

	switch(this->m_State)
	{
		case CUSTOM_TREASURE_HUNT_STATE_BLANK:
			this->ProcState_BLANK();
			break;
		case CUSTOM_TREASURE_HUNT_STATE_EMPTY:
			this->ProcState_EMPTY();
			break;
		case CUSTOM_TREASURE_HUNT_STATE_START:
			this->ProcState_START();
			break;
	}
}

void CCustomTreasureHunt::ProcState_BLANK() // OK
{
	// nothing
}

void CCustomTreasureHunt::ProcState_EMPTY() // OK
{
	if(this->m_RemainTime > 0 && this->m_RemainTime <= (this->m_RuleInfo.AlarmTime*60))
	{
		if((this->m_AlarmMinSave=(((this->m_RemainTime%60)==0)?((this->m_RemainTime/60)-1):(this->m_RemainTime/60))) != this->m_AlarmMinLeft)
		{
			this->m_AlarmMinLeft = this->m_AlarmMinSave;
			gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[EVENTO] \nCaça ao Tesouro em %s em %d minuto(s)!",this->m_RuleInfo.Name,(this->m_AlarmMinLeft+1));
		}
	}

	if(this->m_RemainTime <= 0)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[EVENTO] \nCaça ao Tesouro em %s comeou!\nProcure o baú verdadeiro!",this->m_RuleInfo.Name);
		this->SetState(CUSTOM_TREASURE_HUNT_STATE_START);
	}
}

void CCustomTreasureHunt::ProcState_START() // OK
{
	if(this->m_RemainTime <= 0)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[EVENTO] \nCaça ao tesouro em %s terminou!",this->m_RuleInfo.Name);
		this->SetState(CUSTOM_TREASURE_HUNT_STATE_EMPTY);
		return;
	}

	// If all chests are gone, end event
	if(this->GetChestAliveCount() <= 0)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[EVENTO] \nCaça ao tesouro em %s terminou (não restaram baús).",this->m_RuleInfo.Name);
		this->SetState(CUSTOM_TREASURE_HUNT_STATE_EMPTY);
		return;
	}
}

void CCustomTreasureHunt::SetState(int state) // OK
{
	this->m_State = state;

	switch(this->m_State)
	{
		case CUSTOM_TREASURE_HUNT_STATE_BLANK:
			this->SetState_BLANK();
			break;
		case CUSTOM_TREASURE_HUNT_STATE_EMPTY:
			this->SetState_EMPTY();
			break;
		case CUSTOM_TREASURE_HUNT_STATE_START:
			this->SetState_START();
			break;
	}
}

void CCustomTreasureHunt::SetState_BLANK() // OK
{
	this->m_RemainTime = 0;
	this->m_TargetTime = 0;
	this->m_AlarmMinSave = -1;
	this->m_AlarmMinLeft = -1;
	this->ClearChests();
}

void CCustomTreasureHunt::SetState_EMPTY() // OK
{
	this->m_AlarmMinSave = -1;
	this->m_AlarmMinLeft = -1;
	this->ClearChests();
	this->CheckSync();
}

void CCustomTreasureHunt::SetState_START() // OK
{
	this->ClearChests();
	this->m_RealChestIndex = -1;

	if(this->m_RuleInfo.ChestClass <= 0)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[Caça ao Tesouro] ERRO: ChestClass invlido no config!");
		this->SetState(CUSTOM_TREASURE_HUNT_STATE_EMPTY);
		return;
	}

	int chestCount = this->m_RuleInfo.ChestCount;
	if(chestCount < 1) chestCount = 1;
	if(chestCount > MAX_CUSTOM_TREASURE_HUNT_CHESTS) chestCount = MAX_CUSTOM_TREASURE_HUNT_CHESTS;

	for(int n=0; n<chestCount; n++)
	{
		int index = gObjAddMonster(this->m_RuleInfo.Map);
		if(OBJECT_RANGE(index) == 0)
		{
			continue;
		}

		LPOBJ lpChest = &gObj[index];

		int px = this->m_RuleInfo.X;
		int py = this->m_RuleInfo.Y;

		if(gObjGetRandomMonsterLocation(this->m_RuleInfo.Map,&px,&py,this->m_RuleInfo.Range,this->m_RuleInfo.Range,50) == 0)
		{
			px = this->m_RuleInfo.X;
			py = this->m_RuleInfo.Y;
		}

		lpChest->PosNum = -1;
		lpChest->X = px;
		lpChest->Y = py;
		lpChest->TX = px;
		lpChest->TY = py;
		lpChest->OldX = px;
		lpChest->OldY = py;
		lpChest->StartX = px;
		lpChest->StartY = py;
		lpChest->Dir = GetLargeRand() % 8;
		lpChest->Map = this->m_RuleInfo.Map;
		lpChest->MoveRange = 0;

		if(gObjSetMonster(index,this->m_RuleInfo.ChestClass) == 0)
		{
			gObjDel(index);
			continue;
		}

		lpChest->DieRegen = 0;
		lpChest->RegenTime = 1;

		this->m_ChestIndex.push_back(index);
	}

	if(this->m_ChestIndex.empty() != 0)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[Caça ao Tesouro] ERRO: no foi possvel spawnar bas.");
		this->SetState(CUSTOM_TREASURE_HUNT_STATE_EMPTY);
		return;
	}

	// Choose the real chest
	this->m_RealChestIndex = this->m_ChestIndex[GetLargeRand()%this->m_ChestIndex.size()];

	LogAdd(LOG_BLACK,"[TreasureHunt] Spawned %d chest(s) on map %d. RealChestIndex=%d",(int)this->m_ChestIndex.size(),this->m_RuleInfo.Map,this->m_RealChestIndex);

	this->m_RemainTime = this->m_RuleInfo.EventTime*60;
	this->m_TargetTime = (int)(time(0)+this->m_RemainTime);
}

void CCustomTreasureHunt::CheckSync() // OK
{
	if(this->m_StartTime.empty() != 0)
	{
		this->SetState(CUSTOM_TREASURE_HUNT_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;
	CScheduleManager ScheduleManager;

	for(std::vector<CUSTOM_TREASURE_HUNT_START_TIME>::iterator it=this->m_StartTime.begin(); it != this->m_StartTime.end(); it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(CUSTOM_TREASURE_HUNT_STATE_BLANK);
		return;
	}

	this->m_RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));
	this->m_TargetTime = (int)ScheduleTime.GetTime();

	LogAdd(LOG_BLACK,"[TreasureHunt] Sync Start Time. [%d] min remain",(this->m_RemainTime/60));
}

void CCustomTreasureHunt::ClearChests() // OK
{
	for(std::vector<int>::iterator it=this->m_ChestIndex.begin(); it != this->m_ChestIndex.end(); it++)
	{
		int index = *it;
		if(OBJECT_RANGE(index) == 0)
		{
			continue;
		}

		if(gObj[index].Connected == OBJECT_ONLINE && gObj[index].Type == OBJECT_MONSTER)
		{
			gObjDel(index);
		}
	}

	this->m_ChestIndex.clear();
	this->m_RealChestIndex = -1;
}

bool CCustomTreasureHunt::IsEventChestIndex(int index)
{
	for(std::vector<int>::iterator it=this->m_ChestIndex.begin(); it != this->m_ChestIndex.end(); it++)
	{
		if((*it) == index)
		{
			return true;
		}
	}

	return false;
}

int CCustomTreasureHunt::GetChestAliveCount()
{
	int count = 0;
	for(std::vector<int>::iterator it=this->m_ChestIndex.begin(); it != this->m_ChestIndex.end(); it++)
	{
		int index = *it;
		if(OBJECT_RANGE(index) == 0)
		{
			continue;
		}

		if(gObj[index].Connected == OBJECT_ONLINE && gObj[index].Live != 0)
		{
			count++;
		}
	}
	return count;
}

void CCustomTreasureHunt::MonsterDieProc(LPOBJ lpTarget,LPOBJ lpObj) // OK
{
	if(this->m_State != CUSTOM_TREASURE_HUNT_STATE_START)
	{
		return;
	}

	if(lpTarget == 0 || lpObj == 0)
	{
		return;
	}

	if(this->IsEventChestIndex(lpTarget->Index) == false)
	{
		return;
	}

	int map = lpTarget->Map;
	int x = lpTarget->X;
	int y = lpTarget->Y;

	// Real chest
	if(lpTarget->Index == this->m_RealChestIndex)
	{
		if(this->m_RuleInfo.RealBagSpecial > 0)
		{
			gItemBagManager.DropItemBySpecialValue(this->m_RuleInfo.RealBagSpecial,0,0,lpObj,map,x,y);
		}

		if(this->m_RuleInfo.Fireworks != 0)
		{
			gCustomEventDrop.GCFireworksSendToNearUser(map,x,y);
		}

		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[EVENTO] \nCaça ao Tesouro %s encontrou o baú verdadeiro em %s",lpObj->Name,this->m_RuleInfo.Name);

		// End the event after real chest is found
		this->SetState(CUSTOM_TREASURE_HUNT_STATE_EMPTY);
		return;
	}

	// Fake chest
	if(this->m_RuleInfo.FakeBagSpecial > 0)
	{
		gItemBagManager.DropItemBySpecialValue(this->m_RuleInfo.FakeBagSpecial,0,0,lpObj,map,x,y);
	}
}

//============================================================
// Commands
//============================================================

void CCustomTreasureHunt::CommandStart(LPOBJ lpObj)
{
	if(lpObj == 0)
	{
		return;
	}

	if(this->m_CustomTreasureHuntSwitch == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[EVENTO] \nCaça ao Tesouro desativado no GameServerInfo - Custom.dat");
		return;
	}

	if(this->m_State == CUSTOM_TREASURE_HUNT_STATE_START)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[EVENTO] \nCaça ao Tesouro\nJá está em andamento. Baús vivos: %d",this->GetChestAliveCount());
		return;
	}

	gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[EVENTO] \nCaça ao tesouro começou!\nProcure o baú verdadeiro em %s!",this->m_RuleInfo.Name);
	this->SetState(CUSTOM_TREASURE_HUNT_STATE_START);
}

void CCustomTreasureHunt::CommandInfo(LPOBJ lpObj)
{
	if(lpObj == 0)
	{
		return;
	}

	char stateName[16] = {0};
	if(this->m_State == CUSTOM_TREASURE_HUNT_STATE_BLANK) strcpy_s(stateName,"BLANK");
	else if(this->m_State == CUSTOM_TREASURE_HUNT_STATE_EMPTY) strcpy_s(stateName,"EMPTY");
	else if(this->m_State == CUSTOM_TREASURE_HUNT_STATE_START) strcpy_s(stateName,"START");
	else strcpy_s(stateName,"?");

	if(this->m_State == CUSTOM_TREASURE_HUNT_STATE_START)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[EVENTO] \nCaça ao Tesouro Estado: %s | Restante: %d s \n Mapa: %d | Centro: %d,%d | Baús vivos: %d",stateName,this->m_RemainTime,this->m_RuleInfo.Map,this->m_RuleInfo.X,this->m_RuleInfo.Y,this->GetChestAliveCount());
	}
	else
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[EVENTO] \nCaça ao Tesouro Estado: %s | Próximo em: %d s | Config: %s",stateName,this->m_RemainTime,this->m_RuleInfo.Name);
	}
}


void CCustomTreasureHunt::CommandEnd(LPOBJ lpObj)
{
	if(lpObj == 0)
	{
		return;
	}

	if(this->m_CustomTreasureHuntSwitch == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[EVENTO] \nCaça ao Tesouro desativado no GameServerInfo - Custom.dat");
		return;
	}

	if(this->m_State != CUSTOM_TREASURE_HUNT_STATE_START)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"[EVENTO] \nCaça ao Tesouro não esta em andamento.");
		return;
	}

	gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[EVENTO] \nCaça ao Tesouro em %s foi finalizado pelo GM (%s).",this->m_RuleInfo.Name,lpObj->Name);

	this->SetState(CUSTOM_TREASURE_HUNT_STATE_EMPTY);
}
