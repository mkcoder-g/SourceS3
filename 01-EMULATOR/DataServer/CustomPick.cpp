// CustomPick.cpp: implementation of the CCustomPick class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomPick.h"
#include "QueryManager.h"
#include "SocketManager.h"

CCustomPick gCustomPick;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomPick::CCustomPick() // OK
{

}

CCustomPick::~CCustomPick() // OK
{

}

void CCustomPick::GDCustomPickRecv(SDHP_CUSTOM_PICK_RECV* lpMsg,int index) // OK
{
	SDHP_CUSTOM_PICK_SEND pMsg;

	pMsg.header.set(0x13,0x00,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	if(gQueryManager.ExecQuery("SELECT * FROM CustomPick WHERE Name='%s'",lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		memset(pMsg.CustomPickList,0xFF,sizeof(pMsg.CustomPickList));

		pMsg.CustomPickZen = 0;

		pMsg.CustomPickJewel = 0;

		pMsg.CustomPickExc = 0;

		pMsg.CustomPickSet = 0;
	}
	else
	{
		gQueryManager.GetAsBinary("CustomPickList",pMsg.CustomPickList[0],sizeof(pMsg.CustomPickList));

		pMsg.CustomPickZen = gQueryManager.GetAsInteger("Money");

		pMsg.CustomPickJewel = gQueryManager.GetAsInteger("Jewel");

		pMsg.CustomPickExc = gQueryManager.GetAsInteger("ItemExc");

		pMsg.CustomPickSet = gQueryManager.GetAsInteger("ItemSet");

		gQueryManager.Close();
	}

	gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
}

void CCustomPick::GDCustomPickSaveRecv(SDHP_CUSTOM_PICK_SAVE_RECV* lpMsg) // OK
{
	if(gQueryManager.ExecQuery("SELECT Name FROM CustomPick WHERE Name='%s'",lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		gQueryManager.BindParameterAsBinary(1,lpMsg->CustomPickList[0],sizeof(lpMsg->CustomPickList));
		gQueryManager.ExecQuery("INSERT INTO CustomPick (Name,CustomPickList,Money,Jewel,ItemExc,ItemSet) VALUES ('%s',?,%d,%d,%d,%d)",lpMsg->name,lpMsg->CustomPickZen,lpMsg->CustomPickJewel,lpMsg->CustomPickExc,lpMsg->CustomPickSet);
		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();
		gQueryManager.BindParameterAsBinary(1,lpMsg->CustomPickList[0],sizeof(lpMsg->CustomPickList));
		gQueryManager.ExecQuery("UPDATE CustomPick SET CustomPickList=?, Money=%d, Jewel=%d, ItemExc=%d, ItemSet=%d  WHERE Name='%s'",lpMsg->CustomPickZen,lpMsg->CustomPickJewel,lpMsg->CustomPickExc,lpMsg->CustomPickSet,lpMsg->name);
		gQueryManager.Close();
	}
}
