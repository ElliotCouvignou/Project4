// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/P4ItemArmorObject.h"
#include "..\..\Public\Interactables\P4ItemArmorObject.h"

UP4ItemArmorObject::UP4ItemArmorObject()
	: Super()
{
	ItemType = EItemType::Armor;
	ArmorType = EArmorType::None;
}

void UP4ItemArmorObject::InitializeFromDataAsset(UItemBaseDataAsset* Data) 
{
	Super::InitializeFromDataAsset(Data);
	UItemArmorDataAsset* ArmorData = Cast<UItemArmorDataAsset>(Data);
	if (ArmorData)
	{
		ArmorType = ArmorData->ArmorType;
		ItemRank = ArmorData->ItemInfo.ItemRank;
		PremadeGameplayEffectClass = ArmorData->EquippedGameplayEffect;
	}
}

void UP4ItemArmorObject::SetEquippedGameplayEffectByClass(TSubclassOf<UGameplayEffect> GEClass, UAbilitySystemComponent* ASC)
{
	if (ASC && GEClass)
	{
		EquippedGameplayEffect = ASC->MakeOutgoingSpec(GEClass, 1.f, ASC->MakeEffectContext());
	}
}
