// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "UI/P4ItemToolTip.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)

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
	for (auto mod : GESpec.ModifiedAttributes)
	{
		FGameplayTag AttrTag = FGameplayTag::RequestGameplayTag(FName(FString("Data.Attribute." + mod.Attribute.GetName())));
		print(FString("findsetbycall: " + AttrTag.GetTagName().ToString()));
		float mag = GESpec.GetSetByCallerMagnitude(AttrTag);
		if (FMath::IsNearlyEqual(0.f, mag))
		{
			mag = mod.TotalMagnitude;
		}	
		
		OutDataArray.Add(FAttributeDataUIStruct(mod.Attribute.GetName(), mag));
	}		
}

void UP4ItemToolTip::GetAttributeNamesAndValuesFromGameplayEffectSpecHandle(const FGameplayEffectSpecHandle& GESpecHandle, TArray<FAttributeDataUIStruct>& OutDataArray)
{
	if (GESpecHandle.Data.IsValid())
	{
		GetAttributeNamesAndValuesFromGameplayEffectSpec(*GESpecHandle.Data.Get(), OutDataArray);
	}
}
