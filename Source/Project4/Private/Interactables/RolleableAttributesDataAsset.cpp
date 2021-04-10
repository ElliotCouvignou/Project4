// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/RolleableAttributesDataAsset.h"



URolleableAttributesDataAsset::URolleableAttributesDataAsset()
	: Super()
{
	SumTotalWeights();
}



void URolleableAttributesDataAsset::GetRandomAttribute(EArmorType ArmorType, FRolleableAttributeStruct& RolleableAttStruct)
{
	float weight = FMath::RandRange(0.f, *TotalWeight.Find(ArmorType));
	
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



void URolleableAttributesDataAsset::SumTotalWeights()
{
	TotalWeight.Empty();

	for (auto Elem : ArmorTypeToRolleableAttributes)
	{
		if (!TotalWeight.Contains(Elem.Key))
		{
			TotalWeight.Add(TTuple<EArmorType, float>(Elem.Key, 0.f));
		}
		for (auto Att : Elem.Value.Attributes)
		{
			*TotalWeight.Find(Elem.Key) += Att.Weight;
		}
	}
}