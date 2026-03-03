#include "stdafx.h"
#include "Shop.h"
#include "Item.h"
#include "ItemValue.h"
#include "Offset.h"
#include "Util.h"

SHOP_INFO gShopPrice[MAX_ITEM_SHOP_PRICE];

DWORD ShopValueType = 0;

void InitShop() // OK
{
	SetCompleteHook(0xE8,0x00613C70,&GetBuyPrice);

	SetCompleteHook(0xE8,0x00613D3F,&GetBuyPrice);

	SetCompleteHook(0xE8,0x00613D85,&GetSellPrice);

	SetCompleteHook(0xFF,0x00613D75,&ShowBuyPrice);
}

int GetBuyPrice(DWORD address) // OK
{
	if(ShopValueType != 0)
	{
		for(int n=0;n<MAX_ITEM_SHOP_PRICE;n++)
		{
			if(gShopPrice[n].ItemIndex != *(WORD*)(address+0x00))
			{
				continue;
			}

			if(gShopPrice[n].ItemLevel != GET_ITEM_OPT_LEVEL(*(DWORD*)(address+0x04)))
			{
				continue;
			}

			if(gShopPrice[n].ItemDur != *(BYTE*)(address+0x1A))
			{
				continue;
			}

			if(gShopPrice[n].ItemNewOption != GET_ITEM_OPT_EXC(*(BYTE*)(address+0x1B)))
			{
				continue;
			}

			return gShopPrice[n].ItemPrice;
		}

		return 0;
	}

	int value = 0;

	if(gItemValue.GetItemValue(address,&value) != 0)
	{
		value = ((value>=100)?((value/10)*10):value);

		value = ((value>=1000)?((value/100)*100):value);

		return value;
	}

	return ((int(*)(DWORD,int))0x00598717)(address,0);
}

int GetSellPrice(DWORD address) // OK
{
	int value = 0;

	if(gItemValue.GetItemValue(address,&value) != 0)
	{
		value = (value/3);

		value = ((value>=100)?((value/10)*10):value);

		value = ((value>=1000)?((value/100)*100):value);

		return value;
	}

	return ((int(*)(DWORD,int))0x00598717)(address,1);
}

void ShowBuyPrice(char* a,char* b,char* c) // OK
{
	if(ShopValueType==0)
	{
		wsprintf(a,b,c);
	}
	else
	{
		wsprintf(a,pGetTextLine(1988),c);
	}
}

void ClearShopPriceList() // OK
{
	for(int n=0;n<MAX_ITEM_SHOP_PRICE;n++)
	{
		gShopPrice[n].ItemIndex = -1;
		gShopPrice[n].ItemLevel = -1;
		gShopPrice[n].ItemDur = -1;
		gShopPrice[n].ItemNewOption = -1;
		gShopPrice[n].ItemPrice = -1;
	}
}

void InsertShopPrice(DWORD index,DWORD level,DWORD dur,DWORD exc,DWORD price) // OK
{
	for(int n=0;n<MAX_ITEM_SHOP_PRICE;n++)
	{
		if(gShopPrice[n].ItemIndex == -1)
		{
			gShopPrice[n].ItemIndex = index;
			gShopPrice[n].ItemLevel = level;
			gShopPrice[n].ItemDur = dur;
			gShopPrice[n].ItemNewOption = exc;
			gShopPrice[n].ItemPrice = price;
			return;
		}
	}
}