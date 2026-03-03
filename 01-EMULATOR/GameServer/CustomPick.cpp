// CustomPick.cpp: implementation of the CCustomPick class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomPick.h"
#include "CommandManager.h"
#include "GameMain.h"
#include "ItemManager.h"
#include "Log.h"
#include "Map.h"
#include "MemScript.h"
#include "Message.h"
#include "Notice.h"
#include "Util.h"
#include "Viewport.h"

CCustomPick gCustomPick;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomPick::CCustomPick() // OK
{
	this->m_CustomPickInfo.clear();
}

CCustomPick::~CCustomPick() // OK
{

}

void CCustomPick::ReadCustomPickInfo(char* section,char* path) // OK
{
	this->m_CustomPickSwitch = GetPrivateProfileInt(section,"CustomPickSwitch",0,path);

	this->m_CustomPickMaxItemList = GetPrivateProfileInt(section,"CustomPickMaxItemList",0,path);

	this->m_CustomPickMaxTime[0] = GetPrivateProfileInt(section,"CustomPickMaxTime_AL0",0,path);

	this->m_CustomPickMaxTime[1] = GetPrivateProfileInt(section,"CustomPickMaxTime_AL1",0,path);

	this->m_CustomPickMaxTime[2] = GetPrivateProfileInt(section,"CustomPickMaxTime_AL2",0,path);

	this->m_CustomPickMaxTime[3] = GetPrivateProfileInt(section,"CustomPickMaxTime_AL3",0,path);
}

void CCustomPick::Load(char* path) // OK
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

	this->m_CustomPickInfo.clear();

	try
	{
		while (true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			CUSTOM_PICK_INFO info;

			info.Index = SafeGetItem(GET_ITEM(lpMemScript->GetNumber(),lpMemScript->GetAsNumber()));

			std::map<int,CUSTOM_PICK_INFO>::iterator it = this->m_CustomPickInfo.find(info.Index);

			if(it == this->m_CustomPickInfo.end())
			{
				this->m_CustomPickInfo.insert(std::pair<int, CUSTOM_PICK_INFO>(info.Index,info));
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCustomPick::MainProc(LPOBJ lpObj) // OK
{
	if(lpObj->CustomPickSwitch == 0)
	{
		return;
	}

	if(this->m_CustomPickMaxTime[lpObj->AccountLevel] != 0 && (GetTickCount()-((DWORD)lpObj->CustomPickMaxTime)) >= ((DWORD)this->m_CustomPickMaxTime[lpObj->AccountLevel]*1000))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(458));
		this->OnPickClose(lpObj);
		return;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
	{
		this->OnPickClose(lpObj);
		return;
	}

	for(int n=0;n<MAX_MAP_ITEM;n++)
	{
		if(gMap[lpObj->Map].m_Item[n].IsItem() == 0)
		{
			continue;
		}

		if(gViewport.CheckViewportObjectPosition(lpObj->Index,lpObj->Map,gMap[lpObj->Map].m_Item[n].m_X,gMap[lpObj->Map].m_Item[n].m_Y,4) != 0)
		{
			std::map<int, CUSTOM_PICK_INFO>::iterator it = this->m_CustomPickInfo.find(gMap[lpObj->Map].m_Item[n].m_Index);

			if(it != this->m_CustomPickInfo.end())
			{
				continue;
			}

			PMSG_ITEM_GET_RECV pMsg;

			pMsg.header.set(0x22,sizeof(pMsg));

			pMsg.index[0] = SET_NUMBERHB(n);
			pMsg.index[1] = SET_NUMBERLB(n);

			if(this->CheckCustomPickByItem(lpObj->Index,gMap[lpObj->Map].m_Item[n].m_Index,gMap[lpObj->Map].m_Item[n].m_Level) != 0)
			{
				gItemManager.CGItemGetRecv(&pMsg,lpObj->Index);
				continue;
			}

			if(lpObj->CustomPickZen != 0 && gMap[lpObj->Map].m_Item[n].m_Index == GET_ITEM(14,15))
			{
				gItemManager.CGItemGetRecv(&pMsg,lpObj->Index);
				continue;
			}

			if(lpObj->CustomPickJewel != 0 && gItemManager.IsJewelItem(gMap[lpObj->Map].m_Item[n].m_Index) != 0)
			{
				gItemManager.CGItemGetRecv(&pMsg,lpObj->Index);
				continue;
			}

			if(lpObj->CustomPickExc != 0 && gMap[lpObj->Map].m_Item[n].IsExcItem() != 0)
			{
				gItemManager.CGItemGetRecv(&pMsg,lpObj->Index);
				continue;
			}

			if(lpObj->CustomPickSet != 0 && gMap[lpObj->Map].m_Item[n].IsSetItem() != 0)
			{
				gItemManager.CGItemGetRecv(&pMsg,lpObj->Index);
				continue;
			}
		}
	};
}

void CCustomPick::CommandCustomPick(LPOBJ lpObj) // OK
{
	if(this->m_CustomPickSwitch == 0)
	{
		return;
	}

	if(lpObj->CustomPickSwitch != 0)
	{
		this->OnPickClose(lpObj);
		return;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(450));
		return;
	}

	if(this->GenerateListPick(lpObj) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(451));
		return;
	}

	lpObj->CustomPickSwitch = 1;

	lpObj->CustomPickMaxTime = GetTickCount();

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(452),lpObj->CustomPickItemListCount,lpObj->CustomPickZen,lpObj->CustomPickJewel,lpObj->CustomPickExc,lpObj->CustomPickSet);

	gLog.Output(LOG_COMMAND,"[CommandCustomPick][%s][%s] - Active (ItemCount: %d, Zen: %d, Jewel: %d, Exc: %d, Set: %d)",lpObj->Account,lpObj->Name,lpObj->CustomPickItemListCount,lpObj->CustomPickZen,lpObj->CustomPickJewel,lpObj->CustomPickExc,lpObj->CustomPickSet);

	gCommandManager.DiscountRequirement(lpObj, COMMAND_CUSTOM_PICK);
}

void CCustomPick::CommandCustomPickSet(LPOBJ lpObj, char* arg) // OK
{
	if(this->m_CustomPickSwitch == 0)
	{
		return;
	}

	int slot = gCommandManager.GetNumber(arg,0);

	if(slot == -1)
	{
		lpObj->CustomPickZen = ((gCommandManager.GetNumber(arg,1) != 0)?1:0);

		lpObj->CustomPickJewel = ((gCommandManager.GetNumber(arg,2) != 0)?1:0);

		lpObj->CustomPickExc = ((gCommandManager.GetNumber(arg,3) != 0)?1:0);

		lpObj->CustomPickSet = ((gCommandManager.GetNumber(arg,4) != 0)?1:0);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(457),lpObj->CustomPickZen,lpObj->CustomPickJewel,lpObj->CustomPickExc,lpObj->CustomPickSet);

		gLog.Output(LOG_COMMAND,"[CommandCustomPickSet][%s][%s] - Set (Zen: %d, Jewel: %d, Exc: %d, Set: %d)",lpObj->Account,lpObj->Name,lpObj->CustomPickZen,lpObj->CustomPickJewel,lpObj->CustomPickExc,lpObj->CustomPickSet);

		gCommandManager.DiscountRequirement(lpObj,COMMAND_CUSTOM_PICK_SET);

		return;
	}

	if(slot < 0 || slot >= MAX_CUSTOM_PICK_LIST)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(454));
		return;
	}

	this->OnPickClose(lpObj);

	char buff[32] = {0};

	gCommandManager.GetString(arg,buff,sizeof(buff),1);

	lpObj->CustomPickList[slot].Index = slot;

	strcpy_s(lpObj->CustomPickList[slot].Syntax,buff);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(455),slot,buff);

	gLog.Output(LOG_COMMAND,"[CommandCustomPickSet][%s][%s] - Set (Slot: %d Text: %d)",lpObj->Account,lpObj->Name,slot,buff);

	gCommandManager.DiscountRequirement(lpObj,COMMAND_CUSTOM_PICK_SET);
}

void CCustomPick::CommandCustomPickClear(LPOBJ lpObj) // OK
{
	if(this->m_CustomPickSwitch == 0)
	{
		return;
	}

	this->OnPickClose(lpObj);

	for(int n=0;n < MAX_CUSTOM_PICK_LIST;n++)
	{
		memset(&lpObj->CustomPickList[n],0xFF,sizeof(lpObj->CustomPickList[n]));
	}

	lpObj->CustomPickZen = 0;
	
	lpObj->CustomPickJewel = 0;

	lpObj->CustomPickExc = 0;
	
	lpObj->CustomPickSet = 0;

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(456));

	gLog.Output(LOG_COMMAND,"[CommandCustomPickClear][%s][%s] - Reset config",lpObj->Account,lpObj->Name);

	gCommandManager.DiscountRequirement(lpObj,COMMAND_CUSTOM_PICK_CLEAR);
}

void CCustomPick::OnPickClose(LPOBJ lpObj) // OK
{
	if(lpObj->CustomPickSwitch != 0)
	{
		lpObj->CustomPickSwitch = 0;
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(453));
	}
}

bool CCustomPick::GenerateListPick(LPOBJ lpObj) // OK
{
	if(gObjIsConnectedGP(lpObj->Index) == 0)
	{
		return 0;
	}

	for(int n=0;n<this->m_CustomPickMaxItemList;n++)
	{
		lpObj->CustomPickItemList[n] = -1;

		lpObj->CustomPickItemLevel[n] = -1;
	}

	lpObj->CustomPickItemListCount = 0;

	for(int n=0;n<MAX_CUSTOM_PICK_LIST;n++)
	{
		if(lpObj->CustomPickList[n].Index == -1)
		{
			continue;
		}

		for(int ItemIndex=0;ItemIndex<MAX_ITEM;ItemIndex++)
		{
			if(lpObj->CustomPickItemListCount >= (DWORD)this->m_CustomPickMaxItemList)
			{
				break;
			}

			if(IsValidItem(ItemIndex) == 0)
			{
				continue;
			}

			for(int ItemLevel=0;ItemLevel<MAX_ITEM_LEVEL;ItemLevel++)
			{
				if(strstr(gItemManager.GetItemName(ItemIndex,ItemLevel),lpObj->CustomPickList[n].Syntax) == 0)
				{
					continue;
				}

				if(this->CheckCustomPickByItem(lpObj->Index,ItemIndex,ItemLevel) != 0)
				{
					continue;
				}

				lpObj->CustomPickItemList[lpObj->CustomPickItemListCount++] = ItemIndex;

				lpObj->CustomPickItemLevel[lpObj->CustomPickItemListCount] = ItemLevel;
			}
		}
	}

	if(lpObj->CustomPickItemListCount == 0 && lpObj->CustomPickZen == 0 && lpObj->CustomPickJewel == 0 && lpObj->CustomPickExc == 0 && lpObj->CustomPickSet == 0)
	{
		return 0;
	}

	return 1;
}

bool CCustomPick::CheckCustomPickByItem(int aIndex, int ItemIndex,int ItemLevel) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return 0;
	}

	for(int n=0;n<this->m_CustomPickMaxItemList;n++)
	{
		if(lpObj->CustomPickItemList[n] == -1)
		{
			continue;
		}

		if(lpObj->CustomPickItemList[n] == ItemIndex && lpObj->CustomPickItemLevel[n] == ItemLevel)
		{
			return 1;
		}
	}

	return 0;
}

void CCustomPick::DGCustomPickRecv(SDHP_CUSTOM_PICK_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCustomPickRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	memcpy(lpObj->CustomPickList,lpMsg->CustomPickList,sizeof(lpMsg->CustomPickList));

	lpObj->CustomPickZen = lpMsg->CustomPickZen;

	lpObj->CustomPickJewel = lpMsg->CustomPickJewel;

	lpObj->CustomPickExc = lpMsg->CustomPickExc;

	lpObj->CustomPickSet = lpMsg->CustomPickSet;
}

void CCustomPick::GDCustomPickSend(int aIndex) // OK
{
	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_CUSTOM_PICK_SEND pMsg;

	pMsg.header.set(0x13,0x00,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void CCustomPick::GDCustomPickSaveSend(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_CUSTOM_PICK_SAVE_SEND pMsg;

	pMsg.header.set(0x13,0x30,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	memcpy(pMsg.CustomPickList,lpObj->CustomPickList,sizeof(pMsg.CustomPickList));

	pMsg.CustomPickZen = lpObj->CustomPickZen;

	pMsg.CustomPickJewel = lpObj->CustomPickJewel;

	pMsg.CustomPickExc = lpObj->CustomPickExc;

	pMsg.CustomPickSet = lpObj->CustomPickSet;

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg),aIndex);
}