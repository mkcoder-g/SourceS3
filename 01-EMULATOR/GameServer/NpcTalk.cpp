// NpcTalk.cpp: implementation of the CNpcTalk class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NpcTalk.h"
#include "BloodCastle.h"
#include "CastleSiege.h"
#include "CastleSiegeSync.h"
#include "CastleSiegeWeapon.h"
#include "ChaosBox.h"
#include "CustomLukeHelper.h"
#include "DevilSquare.h"
#include "DSProtocol.h"
#include "Duel.h"
#include "EffectManager.h"
#include "GameMain.h"
#include "GoldenArcher.h"
#include "IllusionTemple.h"
#include "ItemBagManager.h"
#include "KanturuEntranceNPC.h"
#include "Map.h"
#include "MemScript.h"
#include "Message.h"
#include "MonsterSchedule.h"
#include "Notice.h"
#include "Quest.h"
#include "ServerInfo.h"
#include "ShopManager.h"
#include "Trade.h"
#include "Util.h"
#include "Warehouse.h"

CNpcTalk gNpcTalk;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNpcTalk::CNpcTalk() // OK
{
	this->m_NpcTalkInfo.clear();
}

CNpcTalk::~CNpcTalk() // OK
{

}

void CNpcTalk::Load(char* path) // OK
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

	this->m_NpcTalkInfo.clear();

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

			NPC_TALK_INFO info;

			memset(&info,0,sizeof(info));

			info.MonsterClass = lpMemScript->GetNumber();

			info.Month = lpMemScript->GetAsNumber();

			info.Day = lpMemScript->GetAsNumber();

			info.DoW = lpMemScript->GetAsNumber();

			strcpy_s(info.Message,lpMemScript->GetAsString());

			this->m_NpcTalkInfo.push_back(info);

			if(IsValidMonster(info.MonsterClass) == 0){Console(LOG_RED,"[%d] Invalid MonsterIndex. [%s]",info.MonsterClass,path);}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CNpcTalk::NpcTalk(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(lpObj->Interface.use != 0)
	{
		return 1;
	}

	if(gItemManager.CheckItemInChaosBox(lpObj) != 0 || gItemManager.CheckItemInTrade(lpObj) != 0)
	{
		return 1;
	};

	if(gQuest.NpcTalk(lpNpc,lpObj) != 0)
	{
		return 1;
	}

	for(std::vector<NPC_TALK_INFO>::iterator it = this->m_NpcTalkInfo.begin(); it != this->m_NpcTalkInfo.end(); it++)
	{
		SYSTEMTIME SystemTime;

		GetLocalTime(&SystemTime);

		if(it->MonsterClass != lpNpc->Class)
		{
			continue;
		}

		if(it->Day != -1 && it->Day != SystemTime.wDay)
		{
			continue;
		}

		if(it->Month != -1 && it->Month != SystemTime.wMonth)
		{
			continue;
		}

		if(it->DoW != -1 && it->DoW != (SystemTime.wDayOfWeek+1))
		{
			continue;
		}

		GCChatTargetSend(lpObj,lpNpc->Index,it->Message);
		
		break;
	}

	switch(lpNpc->Class)
	{
		case 205:
			this->NpcCrywolfAltar(lpNpc,lpObj);
			break;
		case 206:
			this->NpcCrywolfAltar(lpNpc,lpObj);
			break;
		case 207: 
			this->NpcCrywolfAltar(lpNpc,lpObj);
			break;
		case 208:
			this->NpcCrywolfAltar(lpNpc,lpObj);
			break;
		case 209: 
			this->NpcCrywolfAltar(lpNpc,lpObj);
			break;
		case 216:
			this->NpcCastleSiegeCrown(lpNpc,lpObj);
			break;
		case 217:
			this->NpcCastleSiegeCrownSwitch(lpNpc,lpObj);
			break;
		case 218:
			this->NpcCastleSiegeCrownSwitch(lpNpc,lpObj);
			break;
		case 219:
			this->NpcCastleSiegeGateSwitch(lpNpc,lpObj);
			break;
		case 220:
			this->NpcCastleSiegeGuard(lpNpc,lpObj);
			break;
		case 221:
			this->NpcCastleSiegeWeaponOffense(lpNpc,lpObj);
			break;
		case 222:
			this->NpcCastleSiegeWeaponDefense(lpNpc,lpObj);
			break;
		case 223:
			this->NpcCastleSiegeSenior(lpNpc,lpObj);
			break;
		case 224:
			this->NpcCastleSiegeGuardsman(lpNpc,lpObj);
			break;
		case 226:
			this->NpcTrainer(lpNpc,lpObj);
			break;
		case 232:
			this->NpcAngelKing(lpNpc,lpObj);
			break;
		case 233:
			this->NpcAngelMessenger(lpNpc,lpObj);
			break;
		case 234:
			this->NpcServerTransfer(lpNpc,lpObj);
			break;
		case 236:
			this->NpcGoldenArcher(lpNpc,lpObj);
			break;
		case 237:
			this->NpcCharon(lpNpc,lpObj);
			break;
		case 238:
			this->NpcChaosGoblin(lpNpc,lpObj);
			break;
		case 240:
			this->NpcWarehouse(lpNpc,lpObj);
			break;
		case 241:
			this->NpcGuildMaster(lpNpc,lpObj);
			break;
		case 256:
			this->NpcLehap(lpNpc,lpObj);
			break;
		case 257:
			this->NpcElfBuffer(lpNpc,lpObj);
			break;
		case 367:
			this->NpcKanturuGate(lpNpc,lpObj);
			break;
		case 368:
			this->NpcElpis(lpNpc,lpObj);
			break;
		case 369:
			this->NpcOsbourne(lpNpc,lpObj);
			break;
		case 370:
			this->NpcJerridon(lpNpc,lpObj);
			break;
		case 371:
			this->NpcLeoTheHelper(lpNpc,lpObj);
			break;
		case 375:
			this->NpcChaosCardMaster(lpNpc,lpObj);
			break;
		case 380:
			this->NpcStoneStatue(lpNpc,lpObj);
			break;
		case 383:
			this->NpcYellowStorage(lpNpc,lpObj);
			break;
		case 384:
			this->NpcBlueStorage(lpNpc,lpObj);
			break;
		case 385:
			this->NpcMirageTheMummy(lpNpc,lpObj);
			break;
		case 450:
			this->NpcGovernorBlossom(lpNpc,lpObj);
			break;
		default:
			return 0;
	}

	return 1;
}

void CNpcTalk::NpcCrywolfAltar(LPOBJ lpNpc,LPOBJ lpObj) // OK
{

}

void CNpcTalk::NpcCastleSiegeCrown(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if( gObjIsConnected(lpObj->Index) == FALSE )
	{
		return;
	}

	if(gCastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE)
	{
		return;
	}
	
	if(lpObj->CsJoinSide < 2 || lpObj->CsGuildInvolved == FALSE )
	{
		return;
	}

	if(gCastleSiege.CheckUnionGuildMaster(lpObj->Index) == FALSE )
	{
		return;
	}

	if( (abs(lpObj->Y - lpNpc->Y)) > 3 || (abs(lpObj->X - lpNpc->X)) > 3)
	{
		return;
	}

	if(gCastleSiege.GetRegCrownAvailable() == FALSE )
	{
		return;
	}

	int iUserIndex = gCastleSiege.GetCrownUserIndex();

	if(gObjIsConnected(iUserIndex) == FALSE )
	{
		int iSwitchIndex1 = gCastleSiege.GetCrownSwitchUserIndex(217);
		int iSwitchIndex2 = gCastleSiege.GetCrownSwitchUserIndex(218);

		if(gObjIsConnected(iSwitchIndex1) == FALSE || gObjIsConnected(iSwitchIndex2) == FALSE )
		{
			GCAnsCsAccessCrownState(lpObj->Index,4);
			LogAdd(LOG_BLACK,"[CastleSiege] [%s][%s] Failed to Register Castle Crown (GUILD:%s)",lpObj->Account,lpObj->Name,lpObj->GuildName);
			return;
		}

		if(lpObj->CsJoinSide != gObj[iSwitchIndex1].CsJoinSide || lpObj->CsJoinSide != gObj[iSwitchIndex2].CsJoinSide)
		{
			GCAnsCsAccessCrownState(lpObj->Index,4);
			LogAdd(LOG_BLACK,"[CastleSiege] [%s][%s] Failed to Register Castle Crown (GUILD:%s) (S1:%s/%s)(S2:%s/%s)",lpObj->Account,lpObj->Name,lpObj->GuildName,gObj[iSwitchIndex1].Name,gObj[iSwitchIndex1].GuildName,gObj[iSwitchIndex2].Name,gObj[iSwitchIndex2].GuildName);
			return;
		}
		else
		{
			GCAnsCsAccessCrownState(lpObj->Index,0);
			gCastleSiege.SetCrownUserIndex(lpObj->Index);
			gCastleSiege.SetCrownAccessUserX((BYTE)lpObj->X);
			gCastleSiege.SetCrownAccessUserY((BYTE)lpObj->Y);
			gCastleSiege.SetCrownAccessTickCount();
			gCastleSiege.NotifyAllUserCsProgState(0,lpObj->GuildName);
			LogAdd(LOG_BLACK,"[CastleSiege] [%s][%s] Start to Register Castle Crown (GUILD:%s)",lpObj->Account,lpObj->Name,lpObj->GuildName);
		}
	}
	else if(lpObj->Index != iUserIndex)
	{
		GCAnsCsAccessCrownState(lpObj->Index,3);
	}
}

void CNpcTalk::NpcCastleSiegeCrownSwitch(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(gObjIsConnected(lpObj->Index) == FALSE )
	{
		return;
	}

	if(gCastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE)
	{
		return;
	}
	
	if(lpObj->CsJoinSide < 2)
	{
		return;
	}

	if( (abs(lpObj->Y - lpNpc->Y)) > 3 || (abs(lpObj->X - lpNpc->X)) > 3)
	{
		return;
	}

	if(gCastleSiege.CheckGuardianStatueExist() == TRUE)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(121));
		return;
	}

	int iUserIndex = gCastleSiege.GetCrownSwitchUserIndex(lpNpc->Class);

	if(!gObjIsConnected(iUserIndex))
	{
		GCAnsCsAccessSwitchState(lpObj->Index,lpNpc->Index,-1,1);
		gCastleSiege.SetCrownSwitchUserIndex(lpNpc->Class,lpObj->Index);
		LogAdd(LOG_BLACK,"[CastleSiege] [%s][%s] Start to Push Castle Crown Switch (GUILD:%s) - CS X:%d/Y:%d",lpObj->Account,lpObj->Name,lpObj->GuildName,lpNpc->X,lpNpc->Y);
	}
	else if(lpObj->Index != iUserIndex)
	{
		GCAnsCsAccessSwitchState(lpObj->Index,lpNpc->Index,iUserIndex,2);
	}
}

void CNpcTalk::NpcCastleSiegeGateSwitch(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if( (lpObj->Interface.use) > 0 )
	{
		return;
	}

	BOOL bControlEnable = FALSE;
	BYTE btResult = 0;
	int iGateIndex = -1;

	if(gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		if(lpObj->CsJoinSide != TRUE)
		{
			btResult = 4;
			bControlEnable = FALSE;
		}
		else
		{
			bControlEnable = TRUE;
		}
	}
	else
	{
		if(gCastleSiege.CheckCastleOwnerMember(lpObj->Index) == FALSE && gCastleSiege.CheckCastleOwnerUnionMember(lpObj->Index) == FALSE)
		{
			btResult = 4;
			bControlEnable = FALSE;
		}
		else
		{
			bControlEnable = TRUE;
		}
	}

	if(bControlEnable != FALSE)
	{
		if(gCastleSiege.CheckLeverAlive(lpNpc->Index) == TRUE)
		{
			if(gCastleSiege.CheckCsGateAlive(lpNpc->CsGateLeverLinkIndex) == TRUE)
			{
				btResult = 1;
				iGateIndex = lpNpc->CsGateLeverLinkIndex;
			}
			else
			{
				btResult = 2;
			}
		}
		else
		{
			btResult = 3;
		}

	}

	GCAnsCsGateState(lpObj->Index,btResult,iGateIndex);

	if(btResult != 1)
	{
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = 12;
}

void CNpcTalk::NpcCastleSiegeGuard(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(gObjIsConnected(lpObj->Index) == FALSE )
	{
		return;
	}

	if( (lpObj->Interface.use ) > 0 )
	{
		return;
	}

	PMSG_ANS_GUARD_IN_CASTLE_HUNTZONE pResult = {0};

	pResult.h.set(0xB9,0x03,sizeof(PMSG_ANS_GUARD_IN_CASTLE_HUNTZONE));
	pResult.btResult = 1;
	pResult.iMaxPrice = 300000;
	pResult.iUnitOfPrice = 10000;
	pResult.btUsable = gCastleSiege.GetHuntZoneEnter();
	pResult.iCurrentPrice = gCastleSiegeSync.GetTaxHuntZone(lpObj->Index,FALSE);

	if(lpObj->Guild != NULL)
	{
		if(gCastleSiege.CheckCastleOwnerMember(lpObj->Index))
		{
			if(lpObj->GuildStatus == 0x80)
			{
				pResult.btResult = 3;
			}
			else
			{
				pResult.btResult = 2;
				pResult.btUsable = 0;
			}
		}
		else if(gCastleSiege.CheckCastleOwnerUnionMember(lpObj->Index))
		{
			pResult.btResult = 2;
			pResult.btUsable = 0;
		}
	}

	DataSend(lpObj->Index,(LPBYTE)&pResult,pResult.h.size);
}

void CNpcTalk::NpcCastleSiegeWeaponOffense(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	gCastleSiegeWeapon.NpcCastleSiegeWeaponOffense(lpNpc,lpObj);
}

void CNpcTalk::NpcCastleSiegeWeaponDefense(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	gCastleSiegeWeapon.NpcCastleSiegeWeaponDefense(lpNpc,lpObj);
}

void CNpcTalk::NpcCastleSiegeSenior(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(lpObj->PShopOpen != 0)
	{
		return;
	}

	if(gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(241));
		return;
	}

	if(gCastleSiege.CheckGuildOwnCastle(lpObj->GuildName) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(242));
		return;
	}

	if(lpObj->GuildStatus != 0x40 && lpObj->GuildStatus != 0x80)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(243));
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_CHAOS_BOX;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30,sizeof(pMsg));

	pMsg.result = 12;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	GCTaxInfoSend(lpObj->Index,1,gCastleSiegeSync.GetTaxRateChaos(lpObj->Index));

	lpObj->IsChaosMixCompleted = 0;

	lpObj->CsGuildInvolved = 0;

	lpObj->CsSeniorMix = 1;

	gObjInventoryTransaction(lpObj->Index);
}

void CNpcTalk::NpcCastleSiegeGuardsman(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		GCChatTargetSend(lpObj,lpNpc->Index,gMessage.GetMessage(244));
		return;
	}
	
	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_COMMON;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30,sizeof(pMsg));

	pMsg.result = 13;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
}

void CNpcTalk::NpcTrainer(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(lpObj->PShopOpen != 0)
	{
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_TRAINER;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30,sizeof(pMsg));

	pMsg.result = 7;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	GCTaxInfoSend(lpObj->Index,1,gCastleSiegeSync.GetTaxRateChaos(lpObj->Index));

	lpObj->IsChaosMixCompleted = 0;

	gObjInventoryTransaction(lpObj->Index);
}

void CNpcTalk::NpcAngelKing(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	gBloodCastle.NpcAngelKing(lpNpc,lpObj);
}

void CNpcTalk::NpcAngelMessenger(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	gBloodCastle.NpcAngelMessenger(lpNpc,lpObj);
}

void CNpcTalk::NpcServerTransfer(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(lpObj->Guild != 0)
	{
		GCServerCommandSend(lpObj->Index,6,0,0,0);
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_TRANSFER;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30,sizeof(pMsg));

	pMsg.result = 5;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
}

void CNpcTalk::NpcGoldenArcher(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_GOLDEN_ARCHER;
	lpObj->Interface.state = 0;

	gGoldenArcher.CGGoldenArcherCountRecv(lpObj->Index);
}

void CNpcTalk::NpcCharon(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	gDevilSquare.NpcCharon(lpNpc,lpObj);
}

void CNpcTalk::NpcChaosGoblin(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(lpObj->PShopOpen != 0)
	{
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_CHAOS_BOX;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30,sizeof(pMsg));

	pMsg.result = 3;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	GCTaxInfoSend(lpObj->Index,1,gCastleSiegeSync.GetTaxRateChaos(lpObj->Index));

	lpObj->IsChaosMixCompleted = 0;

	gObjInventoryTransaction(lpObj->Index);
}

void CNpcTalk::NpcWarehouse(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(lpObj->Interface.type == INTERFACE_TRADE || lpObj->Interface.type == INTERFACE_CHAOS_BOX || lpObj->Interface.type == INTERFACE_PERSONAL_SHOP)
	{
		return;
	}

	if(gItemManager.CheckItemInChaosBox(lpObj) != 0 || gItemManager.CheckItemInTrade(lpObj) != 0)
	{
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_WAREHOUSE;
	lpObj->Interface.state = 0;

	gWarehouse.GDWarehouseItemSend(lpObj->Index,lpObj->Account);
}

void CNpcTalk::NpcGuildMaster(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(lpObj->GuildNumber > 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(245));
		return;
	}

	if(lpObj->Level < gServerInfo.m_GuildCreateMinLevel[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(246));
		return;
	}

	if(lpObj->Reset < gServerInfo.m_GuildCreateMinReset[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(247));
		return;
	}

	GCGuildMasterQuestionSend(lpObj->Index);
}

void CNpcTalk::NpcLehap(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_COMMON;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30,sizeof(pMsg));

	pMsg.result = 9;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
}

void CNpcTalk::NpcElfBuffer(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(lpObj->Level > gServerInfo.m_ElfBufferMaxLevel[lpObj->AccountLevel])
	{
		GCServerCommandSend(lpObj->Index,13,0,0,0);
		return;
	}

	if(lpObj->Reset > gServerInfo.m_ElfBufferMaxReset[lpObj->AccountLevel])
	{
		GCServerCommandSend(lpObj->Index,13,0,0,0);
		return;
	}

	gEffectManager.AddEffect(lpObj,0,EFFECT_ELF_BUFFER,1800,(gServerInfo.m_ElfBufferDamageConstA+(lpObj->Level/gServerInfo.m_ElfBufferDamageConstB)),(gServerInfo.m_ElfBufferDefenseConstA+(lpObj->Level/gServerInfo.m_ElfBufferDefenseConstB)),0,0);
}

void CNpcTalk::NpcKanturuGate(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	gKanturuEntranceNPC.CGKanturuEnterInfoRecv(lpObj->Index);
}

void CNpcTalk::NpcElpis(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(lpObj->PShopOpen != 0)
	{
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_CHAOS_BOX;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30,sizeof(pMsg));

	pMsg.result = 17;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	GCTaxInfoSend(lpObj->Index,1,gCastleSiegeSync.GetTaxRateChaos(lpObj->Index));

	lpObj->IsChaosMixCompleted = 0;

	gObjInventoryTransaction(lpObj->Index);
}

void CNpcTalk::NpcOsbourne(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(lpObj->PShopOpen != 0)
	{
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_CHAOS_BOX;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30,sizeof(pMsg));

	pMsg.result = 18;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	GCTaxInfoSend(lpObj->Index,1,gCastleSiegeSync.GetTaxRateChaos(lpObj->Index));

	lpObj->IsChaosMixCompleted = 0;

	gObjInventoryTransaction(lpObj->Index);
}

void CNpcTalk::NpcJerridon(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(lpObj->PShopOpen != 0)
	{
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_CHAOS_BOX;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30,sizeof(pMsg));

	pMsg.result = 19;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	GCTaxInfoSend(lpObj->Index,1,gCastleSiegeSync.GetTaxRateChaos(lpObj->Index));

	lpObj->IsChaosMixCompleted = 0;

	gObjInventoryTransaction(lpObj->Index);
}

void CNpcTalk::NpcLeoTheHelper(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	GCServerCommandSend(lpObj->Index,15,1,0,0);
}

void CNpcTalk::NpcChaosCardMaster(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(lpObj->PShopOpen != 0)
	{
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_CHAOS_BOX;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30,sizeof(pMsg));

	pMsg.result = 21;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	GCTaxInfoSend(lpObj->Index,1,gCastleSiegeSync.GetTaxRateChaos(lpObj->Index));

	lpObj->IsChaosMixCompleted = 0;

	gObjInventoryTransaction(lpObj->Index);
}

void CNpcTalk::NpcStoneStatue(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	gIllusionTemple.NpcStoneStatue(lpNpc,lpObj);
}

void CNpcTalk::NpcYellowStorage(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	gIllusionTemple.NpcYellowStorage(lpNpc,lpObj);
}

void CNpcTalk::NpcBlueStorage(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	gIllusionTemple.NpcBlueStorage(lpNpc,lpObj);
}

void CNpcTalk::NpcMirageTheMummy(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	gIllusionTemple.NpcMirageTheMummy(lpNpc,lpObj);
}

void CNpcTalk::NpcGovernorBlossom(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(lpObj->PShopOpen != 0)
	{
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_CHAOS_BOX;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30,sizeof(pMsg));

	pMsg.result = 22;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	GCTaxInfoSend(lpObj->Index,1,gCastleSiegeSync.GetTaxRateChaos(lpObj->Index));

	lpObj->IsChaosMixCompleted = 0;

	gObjInventoryTransaction(lpObj->Index);
}

void CNpcTalk::CGNpcTalkRecv(PMSG_NPC_TALK_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Interface.use != 0)
	{
		return;
	}

	if(lpObj->Live == 0)
	{
		return;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);

	if(OBJECT_RANGE(bIndex) == 0)
	{
		return;
	}

	LPOBJ lpNpc = &gObj[bIndex];

	if(lpNpc->Type != OBJECT_NPC || lpObj->Map != lpNpc->Map)
	{
		return;
	}

	if(gObjCalcDistance(lpObj,lpObj) > 5)
	{
		return;
	}

	if(gMonsterSchedule.CheckEnableNpc(lpObj,lpNpc) != 0)
	{
		return;
	}

	if(gCustomLukeHelper.NpcLukeHelper(lpNpc,lpObj) != 0)
	{
		return;
	}

	if(this->NpcTalk(lpNpc,lpObj) != 0)
	{
		return;
	}

	if(SHOP_RANGE(lpNpc->ShopNumber) == 0)
	{
		return;
	}

	if(gServerInfo.m_PKLimitFree == 0 && lpObj->PKLevel > 4 && gServerInfo.m_PKLimitShop != 0)
	{
		GCChatTargetSend(lpObj,lpNpc->Index,gMessage.GetMessage(184));
		return;
	}

	if(gShopManager.GetItemCountByIndex(lpNpc->ShopNumber) == 0)
	{
		GCChatTargetSend(lpObj,lpNpc->Index,gMessage.GetMessage(180));
		return;
	}

	if(gShopManager.CheckShopGameMasterLevel(lpNpc,lpObj) == 0)
	{
		GCChatTargetSend(lpObj,lpNpc->Index,gMessage.GetMessage(181));
		return;
	}

	if(gShopManager.CheckShopAccountLevel(lpNpc,lpObj) == 0)
	{
		GCChatTargetSend(lpObj,lpNpc->Index,gMessage.GetMessage(182));
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_SHOP;
	lpObj->Interface.state = 0;

	PMSG_NPC_TALK_SEND pMsg;

	pMsg.header.setE(0x30,sizeof(pMsg));

	pMsg.result = 0;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	lpObj->TargetShopNumber = lpNpc->ShopNumber;

	gShopManager.GCShopItemListSendByIndex(lpNpc->ShopNumber,aIndex);

	gShopManager.GCShopItemPriceSendByIndex(lpNpc->ShopNumber,aIndex);

	GCTaxInfoSend(aIndex,2,gCastleSiegeSync.GetTaxRateStore(aIndex));
}

void CNpcTalk::CGNpcTalkCloseRecv(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Interface.use == 0 || lpObj->Interface.type == INTERFACE_NONE || (lpObj->Interface.type == INTERFACE_CHAOS_BOX && lpObj->CsSeniorMix == 0))
	{
		return;
	}

	switch(lpObj->Interface.type)
	{
		case INTERFACE_TRADE:
			gTrade.CGTradeCancelButtonRecv(aIndex);
			break;
		case INTERFACE_SHOP:
			if(((lpObj->Interface.state==1)?(lpObj->Interface.state=0):1)==0){return;}
			lpObj->TargetShopNumber = -1;
			break;
		case INTERFACE_WAREHOUSE:
			gWarehouse.CGWarehouseClose(aIndex);
			break;
		case INTERFACE_TRAINER:
			gChaosBox.ChaosBoxInit(lpObj);
			gObjInventoryCommit(aIndex);
			break;
		case INTERFACE_CHAOS_BOX:
			lpObj->CsSeniorMix = 0;
			break;
	}

	lpObj->Interface.use = 0;
	lpObj->Interface.type = INTERFACE_NONE;
	lpObj->Interface.state = 0;
}

void CNpcTalk::CGNpcLeoTheHelperRecv(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	this->GDNpcLeoTheHelperSend(aIndex);
}

void CNpcTalk::DGNpcLeoTheHelperRecv(SDHP_NPC_LEO_THE_HELPER_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGNpcLeoTheHelperRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	if(lpMsg->status != 0)
	{
		GCServerCommandSend(lpObj->Index,15,0,0,0);
		return;
	}

	gItemBagManager.DropItemBySpecialValue(ITEM_BAG_LEO_THE_HELPER,-1,-1,lpObj,lpObj->Map,lpObj->X,lpObj->Y);

	this->GDNpcLeoTheHelperSaveSend(lpObj->Index,1);
}

void CNpcTalk::GDNpcLeoTheHelperSend(int aIndex) // OK
{
	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_NPC_LEO_THE_HELPER_SEND pMsg;

	pMsg.header.set(0x0E,0x00,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void CNpcTalk::GDNpcLeoTheHelperSaveSend(int aIndex,BYTE status) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_NPC_LEO_THE_HELPER_SAVE_SEND pMsg;

	pMsg.header.set(0x0E,0x30,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	pMsg.status = status;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}