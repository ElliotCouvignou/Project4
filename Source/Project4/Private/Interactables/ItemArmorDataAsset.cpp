// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/ItemArmorDataAsset.h"

UItemArmorDataAsset::UItemArmorDataAsset()
	: Super()
{
	// Set base info thats consistent for all armor items
	ItemInfo.bIsStackable = false;
	ItemInfo.ItemType = EItemType::Armor;
}

void UItemArmorDataAsset::GetAttributeNamesAndValuesFromGameplayEffect(TArray<FAttributeDataUIStruct>& OutDataArray)
{
	if (!EquippedGameplayEffect)
		return;

	float val;
	UGameplayEffect* GE = EquippedGameplayEffect->GetDefaultObject<UGameplayEffect>();

	if (GE)
	{
		for (FGameplayModifierInfo Mod : GE->Modifiers)
		{
			if (Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(1.f, val))
				OutDataArray.Add(FAttributeDataUIStruct(Mod.Attribute.GetName(), val));
		}
	}
}
