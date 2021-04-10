// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/RolleableArmorItemsDataAsset.h"
#include "Characters/P4MobCharacterBase.h"

URolleableArmorItemsDataAsset::URolleableArmorItemsDataAsset()
	: Super()
{
	SumTotalWeights();
}

// Might have to look into hopscotch selection if this is too slow (requires sorted by weights)
void URolleableArmorItemsDataAsset::GetRandomArmorItem(EArmorType ArmorType, TSubclassOf<class AP4MobCharacterBase> MobClass, FRolleableArmorItemStruct& RandomArmorItem)
{
	if (ArmorType == EArmorType::None)
	{
		return;
	}

	// Roll random item from array, then check if this is can be dropped for this class
	// If not then remember this rolled index and never roll it again
	bool bSearchingForItem = true;
	TArray<int> InvalidIndexes;
	TArray<FRolleableArmorItemStruct>& ArmorItems = ArmorTypeToItemAssets.Find(ArmorType)->ArmorItems;

	float randWeight = FMath::RandRange(0.f, *TotalWeights.Find(ArmorType));
	for (int i = 0; i < ArmorItems.Num(); i++)
	{
		FRolleableArmorItemStruct& Item = ArmorItems[i];

		randWeight -= Item.Weight;
		if (randWeight <= 0.f)
		{
			// drop this item, if this isn't valid then get most previous latest element
			if ((Item.MobsToDropOn.Num() == 0 || (Item.MobsToDropOn.Contains(MobClass))) \
				&& (Item.MobsToNotDropOn.Num() == 0 || !Item.MobsToNotDropOn.Contains(MobClass)))
			{
				RandomArmorItem = Item;
				return;
			}
			else
			{
				// go forward and find most recent valid if not then go back
				// this can be cleaned up a little if necessary
				int ii = i;
				while (ii < ArmorItems.Num())
				{
					Item = ArmorItems[i];
					if ((Item.MobsToDropOn.Num() == 0 || (Item.MobsToDropOn.Contains(MobClass))) \
						&& (Item.MobsToNotDropOn.Num() == 0 || !Item.MobsToNotDropOn.Contains(MobClass)))
					{
						RandomArmorItem = Item;
						return;
					}
					i++;
				}
				
				ii = i;
				while (ii >= 0)
				{
					Item = ArmorItems[i];
					if ((Item.MobsToDropOn.Num() == 0 || (Item.MobsToDropOn.Contains(MobClass))) \
						&& (Item.MobsToNotDropOn.Num() == 0 || !Item.MobsToNotDropOn.Contains(MobClass)))
					{
						RandomArmorItem = Item;
						return;
					}
					ii--;
				}
			}
		}
	}
}


void URolleableArmorItemsDataAsset::SumTotalWeights()
{
	TotalWeights.Empty();

	for (auto Elem : ArmorTypeToItemAssets)
	{
		if (!TotalWeights.Contains(Elem.Key))
		{
			TotalWeights.Add(TTuple<EArmorType, float>(Elem.Key, 0.f));
		}
		for (auto Att : Elem.Value.ArmorItems)
		{
			*TotalWeights.Find(Elem.Key) += Att.Weight;
		}
	}
}