#include "stdafx.h"
#include "Item.h"
#include "CustomEffect.h"
#include "CustomItem.h"
#include "CustomJewel.h"
#include "CustomTooltip.h"
#include "CustomWing.h"
#include "Offset.h"
#include "Util.h"

void InitItem() // OK
{
	SetCompleteHook(0xE8,0x006B2055,&ItemModelLoad);

	SetCompleteHook(0xE8,0x006B206A,&ItemTextureLoad);

	SetCompleteHook(0xFF,0x0053B485,&GetItemColor);

	SetCompleteHook(0xFF,0x00691E9E,&GetItemColor);

	SetCompleteHook(0xE8,0x00530988,&GetItemEffect);

	SetCompleteHook(0xE8,0x006976F6,&GetItemEffect);

	SetCompleteHook(0xE9,0x00617762,&GetItemToolTip);

	SetCompleteHook(0xE8,0x0062DC8B,&MoveItem);
}

void InitJewel() // OK
{
	SetCompleteHook(0xE9,0x006318F5,&JewelCheckApplyItem1);

	SetCompleteHook(0xE9,0x00644F8F,&JewelCheckApplyItem2);

	SetCompleteHook(0xE9,0x004F37D7,&JewelCheckIndex1);

	SetCompleteHook(0xE9,0x00645E04,&JewelCheckIndex2);

	SetCompleteHook(0xE9,0x0068944B,&JewelCheckIndex3);
}

void InitWing() // OK
{
	SetCompleteHook(0xE9,0x0053FA5A,&WingMakePreviewCharSet);

	SetCompleteHook(0xE9,0x006924C1,&WingDisableLevelGlow);

	SetCompleteHook(0xE9,0x00617BA1,&WingSetIncDamage);

	SetCompleteHook(0xE9,0x00617BE0,&WingSetDecDamage);

	SetCompleteHook(0xE9,0x005966B8,&WingSetDefense);

	SetCompleteHook(0xE9,0x005967B4,&WingSetExtraDefense);

	SetCompleteHook(0xE9,0x00597732,&WingSetOption);

	SetCompleteHook(0xE9,0x00596DBF,&WingSetNewOption);

	SetCompleteHook(0xE9,0x00539000,&WingSetModelType);

	SetCompleteHook(0xE9,0x00595FAC,&WingCheckIndex1);

	SetCompleteHook(0xE9,0x005969A6,&WingCheckIndex2);

	SetCompleteHook(0xE9,0x00597318,&WingCheckIndex3);

	SetCompleteHook(0xE9,0x0059943E,&WingCheckIndex4);

	SetCompleteHook(0xE9,0x00599594,&WingCheckIndex5);

	SetCompleteHook(0xE9,0x005996D5,&WingCheckIndex6);

	SetCompleteHook(0xE9,0x0059A4D6,&WingCheckIndex7);

	SetCompleteHook(0xE9,0x0059A5DB,&WingCheckIndex8);

	SetCompleteHook(0xE9,0x0059A6D4,&WingCheckIndex9);

	SetCompleteHook(0xE9,0x0059A9B7,&WingCheckIndex10);

	SetCompleteHook(0xE9,0x005F04BB,&WingCheckIndex11);

	SetCompleteHook(0xE9,0x00612938,&WingCheckIndex12);

	SetCompleteHook(0xE9,0x00612FE9,&WingCheckIndex13);

	SetCompleteHook(0xE9,0x00613BCF,&WingCheckIndex14);

	SetCompleteHook(0xE9,0x00615266,&WingCheckIndex15);

	SetCompleteHook(0xE9,0x006184F2,&WingCheckIndex16);

	SetCompleteHook(0xE9,0x0061A171,&WingCheckIndex17);

	SetCompleteHook(0xE9,0x0061A7E5,&WingCheckIndex18);

	SetCompleteHook(0xE9,0x0061EE19,&WingCheckIndex19);

	SetCompleteHook(0xE9,0x00626152,&WingCheckIndex20);

	SetCompleteHook(0xE9,0x0062619E,&WingCheckIndex21);

	SetCompleteHook(0xE9,0x00644FCC,&WingCheckIndex22);

	SetCompleteHook(0xE9,0x00645D51,&WingCheckIndex23);

	SetCompleteHook(0xE9,0x005969ED,&WingCheckIndex24);

	SetCompleteHook(0xE9,0x00596A34,&WingCheckIndex25);

	SetCompleteHook(0xE9,0x0059853A,&WingCheckIndex26);

	SetCompleteHook(0xE9,0x00468795,&WingCheckIndex27);

	SetCompleteHook(0xE9,0x00468097,&WingCheckIndex28);

	SetCompleteHook(0xE9,0x0061DC0C,&WingCheckModelIndex1);

	SetCompleteHook(0xE9,0x006967BD,&WingCheckModelIndex2);

	SetCompleteHook(0xE9,0x00692587,&WingCheckModelIndex3);
}

void ItemModelLoad() // OK
{
	((void(*)())0x0069D437)();

	for(int n=0;n<MAX_CUSTOM_JEWEL;n++)
	{
		if(gCustomJewel.m_CustomJewelInfo[n].Index != -1)
		{
			LoadItemModel((gCustomJewel.m_CustomJewelInfo[n].ItemIndex+ITEM_BASE_MODEL),"Item\\",gCustomJewel.m_CustomJewelInfo[n].ModelName);
		}
	}

	for(int n=0;n<MAX_CUSTOM_WING;n++)
	{
		if(gCustomWing.m_CustomWingInfo[n].Index != -1)
		{
			LoadItemModel((gCustomWing.m_CustomWingInfo[n].ItemIndex+ITEM_BASE_MODEL),"Item\\",gCustomWing.m_CustomWingInfo[n].ModelName);
		}
	}

	for(int n=0;n<MAX_CUSTOM_ITEM;n++)
	{
		if(gCustomItem.m_CustomItemInfo[n].Index != -1)
		{
			LoadItemModel((gCustomItem.m_CustomItemInfo[n].ItemIndex+ITEM_BASE_MODEL),((gCustomItem.m_CustomItemInfo[n].ItemIndex >= GET_ITEM(7,0) && gCustomItem.m_CustomItemInfo[n].ItemIndex<GET_ITEM(12,0)) ? "Player\\" : "Item\\"),gCustomItem.m_CustomItemInfo[n].ModelName);
		}
	}
}

void ItemTextureLoad() // OK
{
	for(int n=0;n<MAX_CUSTOM_JEWEL;n++)
	{
		if(gCustomJewel.m_CustomJewelInfo[n].Index != -1)
		{
			LoadItemTexture((gCustomJewel.m_CustomJewelInfo[n].ItemIndex+ITEM_BASE_MODEL),"Item\\");
		}
	}

	for(int n=0;n<MAX_CUSTOM_WING;n++)
	{
		if(gCustomWing.m_CustomWingInfo[n].Index != -1)
		{
			LoadItemTexture((gCustomWing.m_CustomWingInfo[n].ItemIndex+ITEM_BASE_MODEL),"Item\\");
		}
	}

	for(int n=0;n<MAX_CUSTOM_ITEM;n++)
	{
		if(gCustomItem.m_CustomItemInfo[n].Index != -1)
		{
			LoadItemTexture((gCustomItem.m_CustomItemInfo[n].ItemIndex+ITEM_BASE_MODEL),((gCustomItem.m_CustomItemInfo[n].ItemIndex >= GET_ITEM(7,0) && gCustomItem.m_CustomItemInfo[n].ItemIndex<GET_ITEM(12,0)) ? "Player\\" : "Item\\"));
		}
	}

	((void(*)())0x0069F856)();
}

void LoadItemModel(int index,char* folder,char* name) // OK
{
	if(name[0] == 0)
	{
		return;
	}

	char path[MAX_PATH]={ 0 };

	wsprintf(path,"Data\\%s",folder);

	pLoadItemModel(index,path,name,-1);
}

void LoadItemTexture(int index,char* folder) // OK
{
	pLoadItemTexture(index,folder,GL_REPEAT,GL_NEAREST,GL_TRUE);
}

void GetItemColor(DWORD a,DWORD b,DWORD c,DWORD d,DWORD e) // OK
{
	if(gCustomItem.GetCustomItemColor((a-ITEM_BASE_MODEL),(float*)d) == 0)
	{
		((void(*)(DWORD,DWORD,DWORD,DWORD,DWORD))0x0068A345)(a,b,c,d,e);
	}
}

void GetItemEffect(DWORD a,int b,float* c,float d,int e,int f,int g,int h,int i) // OK
{
	for(int n=0;n < MAX_CUSTOM_EFFECT;n++)
	{
		if(gCustomEffect.m_CustomEffectInfo[n].Index != -1)
		{
			if(gCustomEffect.m_CustomEffectInfo[n].ItemIndex != (b-ITEM_BASE_MODEL))
			{
				continue;
			}

			if(gCustomEffect.m_CustomEffectInfo[n].MinItemLevel != -1 && gCustomEffect.m_CustomEffectInfo[n].MinItemLevel > GET_ITEM_OPT_LEVEL(e))
			{
				continue;
			}

			if(gCustomEffect.m_CustomEffectInfo[n].MaxItemLevel != -1 && gCustomEffect.m_CustomEffectInfo[n].MaxItemLevel < GET_ITEM_OPT_LEVEL(e))
			{
				continue;
			}

			if(gCustomEffect.m_CustomEffectInfo[n].MinNewOption != -1 && gCustomEffect.m_CustomEffectInfo[n].MinNewOption > GET_ITEM_OPT_EXC(f))
			{
				continue;
			}

			if(gCustomEffect.m_CustomEffectInfo[n].MaxNewOption != -1 && gCustomEffect.m_CustomEffectInfo[n].MaxNewOption < GET_ITEM_OPT_EXC(f))
			{
				continue;
			}

			DWORD o = 240 * b + *(DWORD*)0x0587C428;

			float ItemColor[3];
			
			float Position[3] = {0.0f,0.0f,0.0f};

			float WorldPosition[3] = {0.0f,0.0f,0.0f};

			ItemColor[0] = (float)(gCustomEffect.m_CustomEffectInfo[n].ColorR/255.0f);
			
			ItemColor[1] = (float)(gCustomEffect.m_CustomEffectInfo[n].ColorG/255.0f);
			
			ItemColor[2] = (float)(gCustomEffect.m_CustomEffectInfo[n].ColorB/255.0f);

			pTransformPosition(o,0x069C416C+(48*gCustomEffect.m_CustomEffectInfo[n].EffectValue),Position,WorldPosition,true);

			switch(gCustomEffect.m_CustomEffectInfo[n].EffectType)
			{
				case 0:
					pCreateSprite(gCustomEffect.m_CustomEffectInfo[n].EffectIndex,WorldPosition,gCustomEffect.m_CustomEffectInfo[n].Scale,ItemColor,1,0,gCustomEffect.m_CustomEffectInfo[n].EffectLevel);
					break;
				case 1:
					pCreateParticle(gCustomEffect.m_CustomEffectInfo[n].EffectIndex,WorldPosition,a+28,ItemColor,gCustomEffect.m_CustomEffectInfo[n].EffectLevel,gCustomEffect.m_CustomEffectInfo[n].Scale,1);
					break;
				case 2:
					pCreateEffect(gCustomEffect.m_CustomEffectInfo[n].EffectIndex,WorldPosition,a+28,ItemColor,gCustomEffect.m_CustomEffectInfo[n].EffectLevel,a,0,0,0,0,0);
					break;
			}
		}
	}

	pRenderPartObjectEffect(a,b,c,d,e,f,g,h,i);
}

void DrawItemToolTip(DWORD address) // OK
{
	for(int i=0;i<MAX_CUSTOM_TOOLTIP;i++)
	{
		if(gCustomTooltip.m_CustomTooltipInfo[i].Index != -1 && gCustomTooltip.m_CustomTooltipInfo[i].ItemIndex == *(WORD*)(address+0x00))
		{
			if (gCustomTooltip.m_CustomTooltipInfo[i].ItemLevel == -1 || gCustomTooltip.m_CustomTooltipInfo[i].ItemLevel == GET_ITEM_OPT_LEVEL(*(DWORD*)(address+0x04)))
			{
				*(&*(DWORD*)0x079F3F04 + *(DWORD*)0x07A25A60) = gCustomTooltip.m_CustomTooltipInfo[i].FontValue;
				*(&*(DWORD*)0x07A22F00 + *(DWORD*)0x07A25A60) = gCustomTooltip.m_CustomTooltipInfo[i].FontColor;

				wsprintf((char*)(0x64 * *(DWORD*)0x07A25A60 +0x079F2D68),gCustomTooltip.m_CustomTooltipInfo[i].Text);

				*(DWORD*)0x07A25A60 += 1;
			}
		}
	}
}

bool MoveItem(DWORD a,DWORD b,int c,int d,int e) // OK
{
	if (pCheckWindow(WINDOWS_TRADE) != 0)
	{
		return ((bool(*)(DWORD,DWORD,int,int,int))0x006308F0)(0x113,0x10F,0x07A22F78,d,0x04);
	}
	else if (pCheckWindow(WINDOWS_CHAOS_MIX) != 0 || pCheckWindow(WINDOWS_SENIOR_MIX) != 0 || pCheckWindow(WINDOWS_REFINERY) != 0 || pCheckWindow(WINDOWS_CHAOS_CARD) != 0)
	{
		return ((bool(*)(DWORD,DWORD,int,int,int))0x006308F0)(0x113,0x6E,0x07EA7370,d,0x04);
	}
	else if (pCheckWindow(WINDOWS_WAREHOUSE) != 0)
	{
		return ((bool(*)(DWORD,DWORD,int,int,int))0x006308F0)(a,b,c,d,e);
	}
	
	return 0;
}

__declspec(naked) void GetItemToolTip() // OK
{
	static DWORD ItemApplyToolTipAddress1 = 0x00617768;

	_asm
	{
		PushAd
		Push Ebp
		Call [DrawItemToolTip]
		Add Esp,0x04
		PopAd
		Cmp Word Ptr Ss:[Ebp],0x1C0D
		Jmp [ItemApplyToolTipAddress1]
	}
}

__declspec(naked) void JewelCheckApplyItem1() // OK
{
	static DWORD JewelCheckApplyItemAddress1 = 0x006318FC;
	static DWORD JewelCheckApplyItemAddress2 = 0x00631913;

	_asm
	{
		PushAd
		Cmp Si,0x1C2C
		Jz Exit
		Push Esi
		Lea Ecx,gCustomJewel
		Call [CCustomJewel::CheckCustomJewelByItem]
		Test Eax,Eax
		Jnz Exit
		PopAd
		Jmp [JewelCheckApplyItemAddress1]
		Exit:
		PopAd
		Jmp [JewelCheckApplyItemAddress2]
	}
}

__declspec(naked) void JewelCheckApplyItem2() // OK
{
	static DWORD JewelCheckApplyItemAddress1 = 0x00645C00;
	static DWORD JewelCheckApplyItemAddress2 = 0x00644F9A;

	_asm
	{
		Cmp Di,0x1C2C
		Jz Exit
		Push Edi
		Lea Ecx,gCustomJewel
		Call [CCustomJewel::CheckCustomJewelByItem]
		Test Eax,Eax
		Jnz Exit
		Jmp [JewelCheckApplyItemAddress1]
		Exit:
		Jmp [JewelCheckApplyItemAddress2]
	}
}

__declspec(naked) void JewelCheckIndex1() // OK
{
	static DWORD JewelCheckIndexAddress1 = 0x004F3829;
	static DWORD JewelCheckIndexAddress2 = 0x004F37DC;

	_asm
	{
		PushAd
		Push Eax
		Lea Ecx,gCustomJewel
		Call [CCustomJewel::CheckCustomJewelByItem]
		Test Eax,Eax
		Je EXIT
		PopAd
		Jmp [JewelCheckIndexAddress1]
		EXIT:
		PopAd
		Cmp Eax,0x1C0E
		Jmp [JewelCheckIndexAddress2]
	}
}

__declspec(naked) void JewelCheckIndex2() // OK
{
	static DWORD JewelCheckIndexAddress1 = 0x00645E0A;
	static DWORD JewelCheckIndexAddress2 = 0x00645E0E;

	_asm
	{
		Cmp Ax,0x1A35
		Jz Exit
		Push Eax
		Lea Ecx,gCustomJewel
		Call [CCustomJewel::CheckCustomJewelByItem]
		Test Eax,Eax
		Jnz Exit
		Jmp [JewelCheckIndexAddress1]
		Exit:
		Jmp [JewelCheckIndexAddress2]
	}
}

__declspec(naked) void JewelCheckIndex3() // OK
{
	static DWORD JewelCheckIndexAddress1 = 0x00689481;
	static DWORD JewelCheckIndexAddress2 = 0x00689452;

	_asm
	{
		Mov Eax,Dword Ptr Ss:[Ebp-0x04]
		Push Eax
		Lea Ecx,gCustomJewel
		Call [CCustomJewel::CheckCustomJewelByItem]
		Test Eax,Eax
		Je EXIT
		Jmp [JewelCheckIndexAddress1]
		EXIT:
		Cmp Dword Ptr Ss:[Ebp-0x04],0x1C0D
		Jmp [JewelCheckIndexAddress2]
	}
}

__declspec(naked) void WingMakePreviewCharSet() // OK
{
	static DWORD WingMakePreviewCharSetAddress1 = 0x0053FA9C;
	static DWORD WingMakePreviewCharSetAddress2 = 0x0053FA63;

	_asm
	{
		Mov Ecx,Dword Ptr Ss:[Ebp+0x0C]
		Movzx Edx,Byte Ptr Ds:[Ecx+0x10]
		Sar Edx,0x01
		And Edx,0x0F
		Test Edx,Edx
		Je EXIT
		Mov Eax,Dword Ptr Ss:[Ebp+0x0C]
		Movzx Ecx,Byte Ptr Ds:[Eax+0x10]
		Sar Ecx,0x01
		And Ecx,0x0F
		Sub Ecx,0x01
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWing]
		Test Eax,Eax
		Je EXIT
		Mov Edx,Dword Ptr Ss:[Ebp+0x0C]
		Movzx Eax,Byte Ptr Ds:[Edx+0x10]
		Sar Eax,0x01
		And Eax,0x0F
		Sub Eax,0x01
		Push Eax
		Lea Ecx,gCustomWing
		Call [CCustomWing::GetCustomWingItem]
		Add Eax,ITEM_BASE_MODEL
		Mov Ecx,Dword Ptr Ss:[Ebp-0x08]
		Mov Word Ptr Ds:[Ecx+0x338],Ax
		Jmp [WingMakePreviewCharSetAddress1]
		EXIT:
		Mov Ecx,Dword Ptr Ss:[Ebp-0x04]
		And Ecx,0xFF
		Jmp [WingMakePreviewCharSetAddress2]
	}
}

__declspec(naked) void WingDisableLevelGlow() // OK
{
	static DWORD WingDisableLevelGlowAddress1 = 0x00692A2A;
	static DWORD WingDisableLevelGlowAddress2 = 0x006924CA;

	_asm
	{
		Mov Ecx,Dword Ptr Ss:[Ebp+0x0C]
		Mov Dword Ptr Ss:[Ebp-0x454],Ecx
		Pushad
		Sub Ecx,ITEM_BASE_MODEL
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Mov Dword Ptr Ss:[Ebp-0x08],0x00
		Jmp [WingDisableLevelGlowAddress1]
		EXIT:
		Popad
		Jmp [WingDisableLevelGlowAddress2]
	}
}

__declspec(naked) void WingSetIncDamage() // OK
{
	static DWORD WingSetIncDamageAddress1 = 0x00617BAF;
	static DWORD WingSetIncDamageAddress2 = 0x00617BA6;
	static DWORD Damage;

	_asm
	{
		Pushad
		Movsx Eax,Cx
		Push Eax
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Movsx Eax,Word Ptr Ds:[Ebp+0]
		Mov Ecx,Dword Ptr Ds:[Ebp+0x04]
		Sar Ecx,0x03
		And Ecx,0x0F
		Push Ecx
		Push Eax
		Lea Ecx,gCustomWing
		Call [CCustomWing::GetCustomWingIncDamage]
		Mov Damage,Eax
		Popad
		Lea Eax,[Eax+Eax*4]
		Mov Ecx,Damage
		Jmp [WingSetIncDamageAddress1]
		EXIT:
		Popad
		Cmp Cx,0x1806
		Jmp [WingSetIncDamageAddress2]
	}
}

__declspec(naked) void WingSetDecDamage() // OK
{
	static DWORD WingSetDecDamageAddress1 = 0x00617C5A;
	static DWORD Damage;

	_asm
	{
		Pushad
		Movsx Ecx,Word Ptr Ds:[Ebp+0]
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Movsx Eax,Word Ptr Ds:[Ebp+0]
		Mov Ecx,Dword Ptr Ds:[Ebp+0x04]
		Sar Ecx,0x03
		And Ecx,0x0F
		Push Ecx
		Push Eax
		Lea Ecx,gCustomWing
		Call [CCustomWing::GetCustomWingDecDamage]
		Mov Damage,Eax
		Popad
		Mov Ecx,Damage
		Push Ecx
		Mov Dword Ptr Ds:[0x7A25A60],Eax
		Jmp [WingSetDecDamageAddress1]
		EXIT:
		Popad
		Push Ecx
		Mov Dword Ptr Ds:[0x7A25A60],Eax
		Jmp [WingSetDecDamageAddress1]
	}
}

__declspec(naked) void WingSetDefense() // OK
{
	static DWORD WingSetDefenseAddress1 = 0x005966FF;
	static DWORD WingSetDefenseAddress2 = 0x005966BE;

	_asm
	{
		Mov Ecx,Dword Ptr[Ebp+0x08]
		Movsx Edx,Word Ptr[Ecx]
		Pushad
		Push Edx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Mov Ecx,Dword Ptr[Ebp+0x08]
		Mov Ebx,Dword Ptr[Ecx+0x04]
		Sar Ebx,0x03
		And Ebx,0x0F
		Push Ebx
		Movsx Edx,Word Ptr[Ecx]
		Push Edx
		Lea Ecx,gCustomWing
		Call [CCustomWing::GetCustomWingDefense]
		Mov Ecx,Dword Ptr[Ebp+0x08]
		Movsx Edx,Word Ptr[Ecx+0x12]
		Add Edx,Eax
		Mov Word Ptr[Ecx+0x12],Dx
		Popad
		Jmp [WingSetDefenseAddress1]
		EXIT:
		Popad
		Jmp [WingSetDefenseAddress2]
	}
}

__declspec(naked) void WingSetExtraDefense() // OK
{
	static DWORD WingSetExtraDefenseAddress1 = 0x0059683A;
	static DWORD WingSetExtraDefenseAddress2 = 0x005967BA;

	_asm
	{
		Mov Ecx,Dword Ptr Ss:[Ebp+0x08]
		Movsx Edx,Word Ptr Ds:[Ecx]
		Pushad
		Push Edx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Mov Edx,Dword Ptr Ss:[Ebp-0x28]
		Cmp Edx,0x0A
		Jnz NEXT1
		Mov Eax,Dword Ptr Ss:[Ebp+0x08]
		Movzx Ecx,Word Ptr Ds:[Eax+0x12]
		Add Ecx,0x01
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Mov Word Ptr Ds:[Edx+0x12],Cx
		Popad
		Jmp [WingSetExtraDefenseAddress1]
		NEXT1:
		Mov Edx,Dword Ptr Ss:[Ebp-0x28]
		Cmp Edx,0x0B
		Jnz NEXT2
		Mov Eax,Dword Ptr Ss:[Ebp+0x08]
		Movzx Ecx,Word Ptr Ds:[Eax+0x12]
		Add Ecx,0x03
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Mov Word Ptr Ds:[Edx+0x12],Cx
		Popad
		Jmp [WingSetExtraDefenseAddress1]
		NEXT2:
		Mov Edx,Dword Ptr Ss:[Ebp-0x28]
		Cmp Edx,0x0C
		Jnz NEXT3
		Mov Eax,Dword Ptr Ss:[Ebp+0x08]
		Movzx Ecx,Word Ptr Ds:[Eax+0x12]
		Add Ecx,0x06
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Mov Word Ptr Ds:[Edx+0x12],Cx
		Popad
		Jmp [WingSetExtraDefenseAddress1]
		NEXT3:
		Mov Edx,Dword Ptr Ss:[Ebp-0x28]
		Cmp Edx,0x0D
		Jnz NEXT4
		Mov Eax,Dword Ptr Ss:[Ebp+0x08]
		Movzx Ecx,Word Ptr Ds:[Eax+0x12]
		Add Ecx,0x0A
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Mov Word Ptr Ds:[Edx+0x12],Cx
		Popad
		Jmp [WingSetExtraDefenseAddress1]
		NEXT4:
		Popad
		Jmp [WingSetExtraDefenseAddress1]
		EXIT:
		Popad
		Jmp [WingSetExtraDefenseAddress2]
	}
}

__declspec(naked) void WingSetOption() // OK
{
	static DWORD WingSetOptionAddress1 = 0x00597E5B;
	static DWORD WingSetOptionAddress2 = 0x00597738;

	_asm
	{
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Movsx Eax,Word Ptr Ds:[Edx]
		Pushad
		Push Eax
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Mov Eax,Dword Ptr Ss:[Ebp-0x14]
		Sar Eax,0x05
		And Eax,0x01
		Je NEXT1
		Mov Ecx,Dword Ptr Ss:[Ebp+0x08]
		Movsx Edx,Word Ptr Ds:[Ecx]
		Push 0x02
		Push Edx
		Lea Ecx,gCustomWing
		Call [CCustomWing::GetCustomWingOptionValue]
		Mov Ecx,Dword Ptr Ss:[Ebp-0x10]
		Imul Eax,Ecx
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Movsx Ecx,Byte Ptr Ds:[Edx+0x2A]
		Mov Byte Ptr Ds:[Edx+Ecx+0x33],Al
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Movsx Eax,Word Ptr Ds:[Edx]
		Push 0x02
		Push Eax
		Lea Ecx,gCustomWing
		Call [CCustomWing::GetCustomWingOptionIndex]
		Mov Ecx,Dword Ptr Ss:[Ebp+0x08]
		Movzx Edx,Byte Ptr Ds:[Ecx+0x2A]
		Mov Byte Ptr Ds:[Ecx+Edx+0x2B],Al
		Mov Eax,Dword Ptr Ss:[Ebp+0x08]
		Mov Cl,Byte Ptr Ds:[Eax+0x2A]
		Add Cl,1
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Mov Byte Ptr Ds:[Edx+0x2A],Cl
		Popad
		Jmp [WingSetOptionAddress1]
		NEXT1:
		Mov Eax,Dword Ptr Ss:[Ebp-0x14]
		Sar Eax,0x04
		And Eax,0x01
		Je NEXT2
		Mov Ecx,Dword Ptr Ss:[Ebp+0x08]
		Movsx Edx,Word Ptr Ds:[Ecx]
		Push 0x01
		Push Edx
		Lea Ecx,gCustomWing
		Call [CCustomWing::GetCustomWingOptionValue]
		Mov Ecx,Dword Ptr Ss:[Ebp-0x10]
		Imul Eax,Ecx
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Movsx Ecx,Byte Ptr Ds:[Edx+0x2A]
		Mov Byte Ptr Ds:[Edx+Ecx+0x33],Al
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Movsx Eax,Word Ptr Ds:[Edx]
		Push 0x01
		Push Eax
		Lea Ecx,gCustomWing
		Call [CCustomWing::GetCustomWingOptionIndex]
		Mov Ecx,Dword Ptr Ss:[Ebp+0x08]
		Movzx Edx,Byte Ptr Ds:[Ecx+0x2A]
		Mov Byte Ptr Ds:[Ecx+Edx+0x2B],Al
		Mov Eax,Dword Ptr Ss:[Ebp+0x08]
		Mov Cl,Byte Ptr Ds:[Eax+0x2A]
		Add Cl,1
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Mov Byte Ptr Ds:[Edx+0x2A],Cl
		Popad
		Jmp [WingSetOptionAddress1]
		NEXT2:
		Mov Ecx,Dword Ptr Ss:[Ebp+0x08]
		Movsx Edx,Word Ptr Ds:[Ecx]
		Push 0x00
		Push Edx
		Lea Ecx,gCustomWing
		Call [CCustomWing::GetCustomWingOptionValue]
		Mov Ecx,Dword Ptr Ss:[Ebp-0x10]
		Imul Eax,Ecx
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Movsx Ecx,Byte Ptr Ds:[Edx+0x2A]
		Mov Byte Ptr Ds:[Edx+Ecx+0x33],Al
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Movsx Eax,Word Ptr Ds:[Edx]
		Push 0x00
		Push Eax
		Lea Ecx,gCustomWing
		Call [CCustomWing::GetCustomWingOptionIndex]
		Mov Ecx,Dword Ptr Ss:[Ebp+0x08]
		Movzx Edx,Byte Ptr Ds:[Ecx+0x2A]
		Mov Byte Ptr Ds:[Ecx+Edx+0x2B],Al
		Mov Eax,Dword Ptr Ss:[Ebp+0x08]
		Mov Cl,Byte Ptr Ds:[Eax+0x2A]
		Add Cl,1
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Mov Byte Ptr Ds:[Edx+0x2A],Cl
		Popad
		Jmp [WingSetOptionAddress1]
		EXIT:
		Popad
		Jmp [WingSetOptionAddress2]
	}
}

__declspec(naked) void WingSetNewOption() // OK
{
	static DWORD WingSetNewOptionAddress1 = 0x00597259;
	static DWORD WingSetNewOptionAddress2 = 0x00596DC5;

	_asm
	{
		Pushad
		Push Edx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Mov Edx,Dword Ptr Ss:[Ebp-0x14]
		And Edx,1
		Je NEXT1
		Mov Eax,Dword Ptr Ss:[Ebp+0x08]
		Movsx Ecx,Word Ptr Ds:[Eax]
		Push 0x00
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::GetCustomWingNewOptionValue]
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Movzx Ecx,Byte Ptr Ds:[Edx+0x2A]
		Mov Byte Ptr Ds:[Edx+Ecx+0x33],Al
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Movsx Eax,Word Ptr Ds:[Edx]
		Push 0x00
		Push Eax
		Lea Ecx,gCustomWing
		Call [CCustomWing::GetCustomWingNewOptionIndex]
		Mov Ecx,Dword Ptr Ss:[Ebp+0x08]
		Movzx Edx,Byte Ptr Ds:[Ecx+0x2A]
		Mov Byte Ptr Ds:[Ecx+Edx+0x2B],Al
		Mov Eax,Dword Ptr Ss:[Ebp+0x08]
		Mov Cl,Byte Ptr Ds:[Eax+0x2A]
		Add Cl,1
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Mov Byte Ptr Ds:[Edx+0x2A],Cl
		NEXT1:
		Mov Edx,Dword Ptr Ss:[Ebp-0x14]
		Sar Edx,1
		And Edx,1
		Je NEXT2
		Mov Eax,Dword Ptr Ss:[Ebp+0x08]
		Movsx Ecx,Word Ptr Ds:[Eax]
		Push 0x01
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::GetCustomWingNewOptionValue]
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Movzx Ecx,Byte Ptr Ds:[Edx+0x2A]
		Mov Byte Ptr Ds:[Edx+Ecx+0x33],Al
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Movsx Eax,Word Ptr Ds:[Edx]
		Push 0x01
		Push Eax
		Lea Ecx,gCustomWing
		Call [CCustomWing::GetCustomWingNewOptionIndex]
		Mov Ecx,Dword Ptr Ss:[Ebp+0x08]
		Movzx Edx,Byte Ptr Ds:[Ecx+0x2A]
		Mov Byte Ptr Ds:[Ecx+Edx+0x2B],Al
		Mov Eax,Dword Ptr Ss:[Ebp+0x08]
		Mov Cl,Byte Ptr Ds:[Eax+0x2A]
		Add Cl,1
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Mov Byte Ptr Ds:[Edx+0x2A],Cl
		NEXT2:
		Mov Edx,Dword Ptr Ss:[Ebp-0x14]
		Sar Edx,2
		And Edx,1
		Je NEXT3
		Mov Eax,Dword Ptr Ss:[Ebp+0x08]
		Movsx Ecx,Word Ptr Ds:[Eax]
		Push 0x02
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::GetCustomWingNewOptionValue]
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Movzx Ecx,Byte Ptr Ds:[Edx+0x2A]
		Mov Byte Ptr Ds:[Edx+Ecx+0x33],Al
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Movsx Eax,Word Ptr Ds:[Edx]
		Push 0x02
		Push Eax
		Lea Ecx,gCustomWing
		Call [CCustomWing::GetCustomWingNewOptionIndex]
		Mov Ecx,Dword Ptr Ss:[Ebp+0x08]
		Movzx Edx,Byte Ptr Ds:[Ecx+0x2A]
		Mov Byte Ptr Ds:[Ecx+Edx+0x2B],Al
		Mov Eax,Dword Ptr Ss:[Ebp+0x08]
		Mov Cl,Byte Ptr Ds:[Eax+0x2A]
		Add Cl,1
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Mov Byte Ptr Ds:[Edx+0x2A],Cl
		NEXT3:
		Mov Edx,Dword Ptr Ss:[Ebp-0x14]
		Sar Edx,3
		And Edx,1
		Je NEXT4
		Mov Eax,Dword Ptr Ss:[Ebp+0x08]
		Movsx Ecx,Word Ptr Ds:[Eax]
		Push 0x03
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::GetCustomWingNewOptionValue]
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Movzx Ecx,Byte Ptr Ds:[Edx+0x2A]
		Mov Byte Ptr Ds:[Edx+Ecx+0x33],Al
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Movsx Eax,Word Ptr Ds:[Edx]
		Push 0x03
		Push Eax
		Lea Ecx,gCustomWing
		Call [CCustomWing::GetCustomWingNewOptionIndex]
		Mov Ecx,Dword Ptr Ss:[Ebp+0x08]
		Movzx Edx,Byte Ptr Ds:[Ecx+0x2A]
		Mov Byte Ptr Ds:[Ecx+Edx+0x2B],Al
		Mov Eax,Dword Ptr Ss:[Ebp+0x08]
		Mov Cl,Byte Ptr Ds:[Eax+0x2A]
		Add Cl,1
		Mov Edx,Dword Ptr Ss:[Ebp+0x08]
		Mov Byte Ptr Ds:[Edx+0x2A],Cl
		NEXT4:
		Popad
		Jmp [WingSetNewOptionAddress1]
		EXIT:
		Popad
		Cmp Edx,0x1806
		Jmp [WingSetNewOptionAddress2]
	}
}

__declspec(naked) void WingSetModelType() // OK
{
	static DWORD WingSetModelTypeAddress1 = 0x0053900E;
	static DWORD WingSetModelTypeAddress2 = 0x0053900A;

	_asm
	{
		Mov Edx,Dword Ptr Ss:[Ebp-0x308]
		Sub Edx,ITEM_BASE_MODEL
		Push 1
		Push Edx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByModelType]
		Test Eax,Eax
		Je EXIT
		Jmp [WingSetModelTypeAddress1]
		EXIT:
		Cmp Dword Ptr Ss:[Ebp-0x308],0x1AA2
		Jmp [WingSetModelTypeAddress2]
	}
}

__declspec(naked) void WingCheckIndex1() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x00596011;
	static DWORD WingCheckIndexAddress2 = 0x00595FB4;

	__asm
	{
		Cmp Edx,0x1806
		Jle NEXT
		Pushad
		Push Edx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex2() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x005969AE;
	static DWORD WingCheckIndexAddress2 = 0x005969B5;

	__asm
	{
		Cmp Ecx,0x1806
		Jle NEXT
		Pushad
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex3() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x0059731F;
	static DWORD WingCheckIndexAddress2 = 0x0059733E;

	__asm
	{
		Cmp Eax,0x1806
		Jle NEXT
		Pushad
		Push Eax
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex4() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x00599461;
	static DWORD WingCheckIndexAddress2 = 0x00599446;

	__asm
	{
		Cmp Ecx,0x1806
		Jle NEXT
		Pushad
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex5() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x005995C5;
	static DWORD WingCheckIndexAddress2 = 0x0059959C;

	__asm
	{
		Cmp Edx,0x1806
		Jle NEXT
		Pushad
		Push Edx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex6() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x00599700;
	static DWORD WingCheckIndexAddress2 = 0x005996DD;

	__asm
	{
		Cmp Edx,0x1806
		Jle NEXT
		Pushad
		Push Edx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex7() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x0059A4DE;
	static DWORD WingCheckIndexAddress2 = 0x0059A4EA;

	__asm
	{
		Cmp Ecx,0x1806
		Jle NEXT
		Pushad
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex8() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x0059A5E3;
	static DWORD WingCheckIndexAddress2 = 0x0059A5FE;

	__asm
	{
		Cmp Edx,0x1806
		Jle NEXT
		Pushad
		Push Edx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex9() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x0059A6DF;
	static DWORD WingCheckIndexAddress2 = 0x0059A79C;

	__asm
	{
		Cmp Eax,0x1806
		Jle NEXT
		Pushad
		Push Eax
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex10() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x0059AA8B;
	static DWORD WingCheckIndexAddress2 = 0x0059A9C3;

	__asm
	{
		Cmp Ecx,0x1806
		Jle NEXT
		Pushad
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex11() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x005F054A;
	static DWORD WingCheckIndexAddress2 = 0x005F04C5;

	__asm
	{
		Cmp Ax,0x1806
		Jle NEXT
		Pushad
		Movsx Ecx,Ax
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex12() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x00612977;
	static DWORD WingCheckIndexAddress2 = 0x0061293F;

	__asm
	{
		Cmp Dx,0x1806
		Jle NEXT
		Pushad
		Movsx Ecx,Dx
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex13() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x00612FF0;
	static DWORD WingCheckIndexAddress2 = 0x00613021;

	__asm
	{
		Cmp Eax,0x1806
		Jle NEXT
		Pushad
		Push Eax
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex14() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x00613BE7;
	static DWORD WingCheckIndexAddress2 = 0x00613BD5;

	__asm
	{
		Cmp Ax,0x1806
		Jle NEXT
		Pushad
		Movsx Ecx,Ax
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex15() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x0061526D;
	static DWORD WingCheckIndexAddress2 = 0x00615294;

	__asm
	{
		Cmp Cx,0x1806
		Jle NEXT
		Pushad
		Movsx Eax,Cx
		Push Eax
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex16() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x00618507;
	static DWORD WingCheckIndexAddress2 = 0x006184F9;

	__asm
	{
		Cmp Cx,0x1806
		Jle NEXT
		Pushad
		Movsx Eax,Cx
		Push Eax
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex17() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x0061A189;
	static DWORD WingCheckIndexAddress2 = 0x0061A177;

	__asm
	{
		Cmp Ax,0x1806
		Jle NEXT
		Pushad
		Movsx Ecx,Ax
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex18() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x0061A892;
	static DWORD WingCheckIndexAddress2 = 0x0061A7F0;

	__asm
	{
		Cmp Cx,0x1806
		Jle NEXT
		Pushad
		Movsx Eax,Cx
		Push Eax
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex19() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x0061EE52;
	static DWORD WingCheckIndexAddress2 = 0x0061EE1F;

	__asm
	{
		Cmp Ax,0x1806
		Jle NEXT
		Pushad
		Movsx Ecx,Ax
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex20() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x0062616A;
	static DWORD WingCheckIndexAddress2 = 0x00626158;

	__asm
	{
		Cmp Ax,0x1806
		Jle NEXT
		Pushad
		Movsx Ecx,Ax
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex21() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x006271C4;
	static DWORD WingCheckIndexAddress2 = 0x006261A8;

	__asm
	{
		Cmp Ax,0x1806
		Jle NEXT
		Pushad
		Movsx Ecx,Ax
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex22() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x00644FF2;
	static DWORD WingCheckIndexAddress2 = 0x00644FD4;

	__asm
	{
		Cmp Ecx,0x1806
		Jle NEXT
		Pushad
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex23() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x00645E0E;
	static DWORD WingCheckIndexAddress2 = 0x00645D5B;

	__asm
	{
		Cmp Ax,0x1806
		Jle NEXT
		Pushad
		Movsx Ecx,Ax
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex24() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x00596A13;
	static DWORD WingCheckIndexAddress2 = 0x005969F5;

	__asm
	{
		Cmp Edx,0x1806
		Jle NEXT
		Pushad
		Push Edx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex25() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x00596A49;
	static DWORD WingCheckIndexAddress2 = 0x00596A3C;

	__asm
	{
		Cmp Ecx,0x1807
		Jle NEXT
		Pushad
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex26() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x00598542;
	static DWORD WingCheckIndexAddress2 = 0x0059854E;

	__asm
	{
		Cmp Ecx,0x1806
		Jle NEXT
		Pushad
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex27() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x004687CD;
	static DWORD WingCheckIndexAddress2 = 0x0046879C;

	__asm
	{
		Cmp Eax,0x1806
		Jle NEXT
		Pushad
		Push Eax
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckIndex28() // OK
{
	static DWORD WingCheckIndexAddress1 = 0x004680CF;
	static DWORD WingCheckIndexAddress2 = 0x0046809F;

	__asm
	{
		Cmp Ecx,0x1806
		Jle NEXT
		Pushad
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckIndexAddress2]
		NEXT:
		Jmp [WingCheckIndexAddress1]
	}
}

__declspec(naked) void WingCheckModelIndex1() // OK
{
	static DWORD WingCheckModelIndexAddress1 = 0x0061DC38;
	static DWORD WingCheckModelIndexAddress2 = 0x0061DC12;

	__asm
	{
		Cmp Ax,0x1A80
		Jle NEXT
		Pushad
		Movsx Ecx,Ax
		Sub Ecx,ITEM_BASE_MODEL
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckModelIndexAddress2]
		NEXT:
		Jmp [WingCheckModelIndexAddress1]
	}
}

__declspec(naked) void WingCheckModelIndex2() // OK
{
	static DWORD WingCheckModelIndexAddress1 = 0x0069689D;
	static DWORD WingCheckModelIndexAddress2 = 0x006967C9;

	__asm
	{
		Cmp Ecx,0x1A80
		Jle NEXT
		Pushad
		Sub Ecx,ITEM_BASE_MODEL
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckModelIndexAddress2]
		NEXT:
		Jmp [WingCheckModelIndexAddress1]
	}
}

__declspec(naked) void WingCheckModelIndex3() // OK
{
	static DWORD WingCheckModelIndexAddress1 = 0x00692841;
	static DWORD WingCheckModelIndexAddress2 = 0x00692A2A;

	__asm
	{
		Cmp Dword Ptr[Ebp-0x454],0x1A80
		Jle NEXT
		Pushad
		Mov Ecx,Dword Ptr[Ebp-0x454]
		Sub Ecx,ITEM_BASE_MODEL
		Push Ecx
		Lea Ecx,gCustomWing
		Call [CCustomWing::CheckCustomWingByItem]
		Test Eax,Eax
		Je EXIT
		Popad
		Jmp NEXT
		EXIT:
		Popad
		Jmp [WingCheckModelIndexAddress2]
		NEXT:
		Jmp [WingCheckModelIndexAddress1]
	}
}