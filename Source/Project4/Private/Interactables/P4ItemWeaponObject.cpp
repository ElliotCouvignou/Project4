// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/P4ItemWeaponObject.h"
#include "..\..\Public\Interactables\P4ItemWeaponObject.h"

UP4ItemWeaponObject::UP4ItemWeaponObject()
	: Super()
{
	ItemType = EItemType::Weapon;
	WeaponType = EWeaponType::None;
}

void UP4ItemWeaponObject::InitializeFromDataAsset(UItemBaseDataAsset* Data)
{
	Super::InitializeFromDataAsset(Data);
	UItemWeaponDataAsset* WepData = Cast<UItemWeaponDataAsset>(Data);
	if (WepData)
	{
		WeaponType = WepData->WeaponType;
		WeaponHandType = WepData->WeaponHandType;
		ItemRank = WepData->ItemInfo.ItemRank;
		WeaponPower = WepData->WeaponPower;
		AttackInterval = WepData->AttackInterval;
		WeaponSkeletalMesh = WepData->WeaponSkeletalMesh;
		MainHandAttatchTransform = WepData->MainHandAttatchTransform;
		OffHandAttatchTransform = WepData->OffHandAttatchTransform;
		PremadeGameplayEffectClass = WepData->EquippedGameplayEffect;
	}
}

void UP4ItemWeaponObject::SetEquippedGameplayEffectByClass(TSubclassOf<UGameplayEffect> GEClass, UAbilitySystemComponent* ASC)
{
	if (ASC && GEClass)
	{
		EquippedGameplayEffect = ASC->MakeOutgoingSpec(GEClass, 1.f, ASC->MakeEffectContext());
	}
}
