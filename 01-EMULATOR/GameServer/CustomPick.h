// CustomPick.h: interface for the CCustomPick class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"
#include "User.h"

#define MAX_CUSTOM_PICK_LIST 20

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_CUSTOM_PICK_RECV
{
	PSWMSG_HEAD header; // C1:13:00
	WORD index;
	char account[11];
	char name[11];
	BYTE CustomPickList[MAX_CUSTOM_PICK_LIST][20];
	int CustomPickZen;
	int CustomPickJewel;
	int CustomPickExc;
	int CustomPickSet;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_CUSTOM_PICK_SEND
{
	PSBMSG_HEAD header; // C1:13:00
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_CUSTOM_PICK_SAVE_SEND
{
	PSWMSG_HEAD header; // C1:13:30
	WORD index;
	char account[11];
	char name[11];
	BYTE CustomPickList[MAX_CUSTOM_PICK_LIST][20];
	int CustomPickZen;
	int CustomPickJewel;
	int CustomPickExc;
	int CustomPickSet;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct CUSTOM_PICK_LIST
{
	DWORD Index;
	char Syntax[32];
};

struct CUSTOM_PICK_INFO
{
	DWORD Index;
};

class CCustomPick
{
public:
	CCustomPick();
	virtual ~CCustomPick();
	void ReadCustomPickInfo(char* section,char* path);
	void Load(char* path);
	void MainProc(LPOBJ lpObj);
	void CommandCustomPick(LPOBJ lpObj);
	void CommandCustomPickSet(LPOBJ lpObj,char* arg);
	void CommandCustomPickClear(LPOBJ lpObj);
	void OnPickClose(LPOBJ lpObj);
	bool GenerateListPick(LPOBJ lpObj);
	bool CheckCustomPickByItem(int aIndex,int ItemIndex,int ItemLevel);
	void DGCustomPickRecv(SDHP_CUSTOM_PICK_RECV* lpMsg);
	void GDCustomPickSend(int aIndex);
	void GDCustomPickSaveSend(int aIndex);
private:
	int m_CustomPickSwitch;
	int m_CustomPickMaxItemList;
	int m_CustomPickMaxTime[MAX_ACCOUNT_LEVEL];
	std::map<int,CUSTOM_PICK_INFO> m_CustomPickInfo;
};

extern CCustomPick gCustomPick;