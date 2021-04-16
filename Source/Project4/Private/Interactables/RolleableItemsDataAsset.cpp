// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Interactables/RolleableItemsDataAsset.h"
#include "Characters/P4MobCharacterBase.h"

URolleableItemsDataAsset::URolleableItemsDataAsset()
	: Super()
{
	SumTotalWeights();
}

// Might have to look into hopscotch selection if this is too slow (requires sorted by weights)
void URolleableItemsDataAsset::GetRandomArmorItem(EArmorType ArmorType, TSubclassOf<class AP4MobCharacterBase> MobClass, FRolleableArmorItemStruct& RandomArmorItem)
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

	float randWeight = FMath::RandRange(0.f, *TotalArmorWeights.Find(ArmorType));
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

void URolleableItemsDataAsset::GetRandomWeaponItem(EWeaponType WeaponType, TSubclassOf<class AP4MobCharacterBase> MobClass, FRolleableWeaponItemStruct& RandomWeaponItem)
{
	if (WeaponType == EWeaponType::None)
	{
		return;
	}

	// Roll random item from array, then check if this is can be dropped for this class
	// If not then remember this rolled index and never roll it again
	bool bSearchingForItem = true;
	TArray<int> InvalidIndexes;
	TArray<FRolleableWeaponItemStruct>& WeaponItems = WeaponTypeToItemAssets.Find(WeaponType)->WeaponItems;

	float randWeight = FMath::RandRange(0.f, *TotalWeaponWeights.Find(WeaponType));
	for (int i = 0; i < WeaponItems.Num(); i++)
	{
		FRolleableWeaponItemStruct& Item = WeaponItems[i];

		randWeight -= Item.Weight;
		if (randWeight <= 0.f)
		{
			// drop this item, if this isn't valid then get most previous latest element
			if ((Item.MobsToDropOn.Num() == 0 || (Item.MobsToDropOn.Contains(MobClass))) \
				&& (Item.MobsToNotDropOn.Num() == 0 || !Item.MobsToNotDropOn.Contains(MobClass)))
			{
				RandomWeaponItem = Item;
				return;
			}
			else
			{
				// go forward and find most recent valid if not then go back
				// this can be cleaned up a little if necessary
				int ii = i;
				while (ii < WeaponItems.Num())
				{
					Item = WeaponItems[i];
					if ((Item.MobsToDropOn.Num() == 0 || (Item.MobsToDropOn.Contains(MobClass))) \
						&& (Item.MobsToNotDropOn.Num() == 0 || !Item.MobsToNotDropOn.Contains(MobClass)))
					{
						RandomWeaponItem = Item;
						return;
					}
					i++;
				}

				ii = i;
				while (ii >= 0)
				{
					Item = WeaponItems[i];
					if ((Item.MobsToDropOn.Num() == 0 || (Item.MobsToDropOn.Contains(MobClass))) \
						&& (Item.MobsToNotDropOn.Num() == 0 || !Item.MobsToNotDropOn.Contains(MobClass)))
					{
						RandomWeaponItem = Item;
						return;
					}
					ii--;
				}
			}
		}
	}
}


void URolleableItemsDataAsset::SumTotalWeights()
{
	TotalArmorWeights.Empty();
	TotalWeaponWeights.Empty();

	for (auto Elem : ArmorTypeToItemAssets)
	{
		if (!TotalArmorWeights.Contains(Elem.Key))
		{
			TotalArmorWeights.Add(TTuple<EArmorType, float>(Elem.Key, 0.f));
		}
		for (auto Att : Elem.Value.ArmorItems)
		{
			*TotalArmorWeights.Find(Elem.Key) += Att.Weight;
		}
	}

	for (auto Elem : WeaponTypeToItemAssets)
	{
		if (!TotalWeaponWeights.Contains(Elem.Key))
		{
			TotalWeaponWeights.Add(TTuple<EWeaponType, float>(Elem.Key, 0.f));
		}
		for (auto Att : Elem.Value.WeaponItems)
		{
			*TotalWeaponWeights.Find(Elem.Key) += Att.Weight;
		}
	}
}