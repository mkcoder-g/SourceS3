// OnlineReward.cpp: implementation of the COnlineReward class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OnlineReward.h"
#include "BonusManager.h"
#include "MemScript.h"
#include "Util.h"

COnlineReward gOnlineReward;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COnlineReward::COnlineReward() // OK
{
	this->m_OnlineRewardInfo.clear();
}

COnlineReward::~COnlineReward() // OK
{

}

void COnlineReward::Load(char* path) // OK
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

	this->m_OnlineRewardInfo.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			ONLINE_REWARD_INFO info;

			info.MinLevel = lpMemScript->GetNumber();

			info.MaxLevel = lpMemScript->GetAsNumber();

			info.MinReset = lpMemScript->GetAsNumber();

			info.MaxReset = lpMemScript->GetAsNumber();

			info.Delay = lpMemScript->GetAsNumber();

			info.Coin1[0] = lpMemScript->GetAsNumber();

			info.Coin1[1] = lpMemScript->GetAsNumber();

			info.Coin1[2] = lpMemScript->GetAsNumber();

			info.Coin1[3] = lpMemScript->GetAsNumber();

			this->m_OnlineRewardInfo.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void COnlineReward::MainProc() // OK
{
	for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if(gObjIsConnectedGP(n) == 0)
		{
			continue;
		}

		this->GetOnlineReward(n);
	}
}

void COnlineReward::GetOnlineReward(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	for(std::vector<ONLINE_REWARD_INFO>::iterator it = this->m_OnlineRewardInfo.begin(); it != this->m_OnlineRewardInfo.end(); it++)
	{
		if(it->MinLevel != -1 && it->MinLevel > lpObj->Level)
		{
			continue;
		}

		if(it->MaxLevel != -1 && it->MaxLevel < lpObj->Level)
		{
			continue;
		}

		if(it->MinReset != -1 && it->MinReset > lpObj->Reset)
		{
			continue;
		}

		if(it->MaxReset != -1 && it->MaxReset < lpObj->Reset)
		{
			continue;
		}

		if((GetTickCount()-lpObj->OnlineRewardTime) < (DWORD)it->Delay*1000)
		{
			continue;
		}

		lpObj->OnlineRewardTime = GetTickCount();

		if(it->Coin1[lpObj->AccountLevel] > 0)
		{
			gObjCoinAdd(lpObj->Index,gBonusManager.GetBonusValue(lpObj,BONUS_INDEX_COIN1,it->Coin1[lpObj->AccountLevel]));
		}
	}
}