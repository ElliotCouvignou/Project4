// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "UI/P4ItemToolTip.h"


void UP4ItemToolTip::GetAttributeNamesAndValuesFromGameplayEffectClass(TArray<FAttributeDataUIStruct>& OutDataArray, TSubclassOf<UGameplayEffect> GEClass)
{
	if (!GEClass)
		return;

	float val;
	UGameplayEffect* GE = GEClass->GetDefaultObject<UGameplayEffect>();

	if (GE)
	{
		for (FGameplayModifierInfo Mod : GE->Modifiers)
		{
			if (Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(1.f, val))
				OutDataArray.Add(FAttributeDataUIStruct(Mod.Attribute.GetName(), val));
		}
	}
}

void UP4ItemToolTip::GetAttributeNamesAndValuesFromItemInfo(const FItemBaseInfoStruct& ItemInfo, TArray<FAttributeDataUIStruct>& OutDataArray)
{
	//switch (ItemInfo.ItemType)
	//{
	//case EItemType::Armor:
	//	if (const FItemArmorInfoStruct* ArmorStruct = Cast<FItemArmorInfoStruct>(&ItemInfo))
	//	{
	//		GetAttributeNamesAndValuesFromGameplayEffectSpec(*ArmorStruct->EquippedGameplayEffect.Data.Get(), OutDataArray);
	//	}
	//	break;
	//}


}


void UP4ItemToolTip::GetAttributeNamesAndValuesFromGameplayEffectSpec(const FGameplayEffectSpec& GESpec, TArray<FAttributeDataUIStruct>& OutDataArray)
{
	
}
