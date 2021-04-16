// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/RolleableAttributesDataAsset.h"



URolleableAttributesDataAsset::URolleableAttributesDataAsset()
	: Super()
{
	SumTotalWeights();
}



void URolleableAttributesDataAsset::GetRandomArmorAttribute(EArmorType ArmorType, FRolleableAttributeStruct& RolleableAttStruct)
{
	float weight = FMath::RandRange(0.f, *TotalArmorWeight.Find(ArmorType));
	
	TArray<FRolleableAttributeStruct> AttributeArray = ArmorTypeToRolleableAttributes.Find(ArmorType)->Attributes;
	for (auto att : AttributeArray)
	{
		weight -= att.Weight;
		if (weight <= 0.f)
		{
			RolleableAttStruct = att;
			return;
		}
	}
}

void URolleableAttributesDataAsset::GetRandomWeaponAttribute(EWeaponType WeaponType, FRolleableAttributeStruct& RolleableAttStruct)
{
	float weight = FMath::RandRange(0.f, *TotalWeaponWeight.Find(WeaponType));

	TArray<FRolleableAttributeStruct> AttributeArray = WeaponTypeToRolleableAttributes.Find(WeaponType)->Attributes;
	for (auto att : AttributeArray)
	{
		weight -= att.Weight;
		if (weight <= 0.f)
		{
			RolleableAttStruct = att;
			return;
		}
	}
}



void URolleableAttributesDataAsset::SumTotalWeights()
{
	TotalArmorWeight.Empty();

	for (auto Elem : ArmorTypeToRolleableAttributes)
	{
		if (!TotalArmorWeight.Contains(Elem.Key))
		{
			TotalArmorWeight.Add(TTuple<EArmorType, float>(Elem.Key, 0.f));
		}
		for (auto Att : Elem.Value.Attributes)
		{
			*TotalArmorWeight.Find(Elem.Key) += Att.Weight;
		}
	}

	TotalWeaponWeight.Empty();

	for (auto Elem : WeaponTypeToRolleableAttributes)
	{
		if (!TotalWeaponWeight.Contains(Elem.Key))
		{
			TotalWeaponWeight.Add(TTuple<EWeaponType, float>(Elem.Key, 0.f));
		}
		for (auto Att : Elem.Value.Attributes)
		{
			*TotalWeaponWeight.Find(Elem.Key) += Att.Weight;
		}
	}
}