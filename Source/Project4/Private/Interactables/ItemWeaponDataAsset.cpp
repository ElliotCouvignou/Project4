// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/ItemWeaponDataAsset.h"


UItemWeaponDataAsset::UItemWeaponDataAsset()
	: Super()
{
	// Set base info thats consistent for all weapon items
	ItemInfo.bIsStackable = false;
	ItemInfo.ItemType = EItemType::Weapon;
}


void UItemWeaponDataAsset::GetAttributeNamesAndValuesFromGameplayEffect(TArray<FAttributeDataUIStruct>& OutDataArray)
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
