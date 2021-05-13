// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "P4CurrentGameSave.h"
#include "AbilitySystem/AttributeSets/PlayerAttributeSet.h"
#include "Interactables/P4InventoryBagComponent.h"



void UP4CurrentGameSave::SaveCharacterInfoFromPlayer(AProject4Controller* PC, AP4PlayerCharacterBase* PlayerChar)
{
	UP4PlayerAbilitySystemComponent* ASC = Cast<UP4PlayerAbilitySystemComponent>(PlayerChar->GetAbilitySystemComponent());
	UPlayerAttributeSet* AS = Cast<UPlayerAttributeSet>(PlayerChar->GetAttributeSet());

	if (PC && PlayerChar && ASC)
	{
		FP4CharacterSaveStruct* CharSave = PlayerCharacterSave.Find(PC);
		if (!CharSave)
		{
			PlayerCharacterSave.Add(TTuple<AProject4Controller*, FP4CharacterSaveStruct>(PC, FP4CharacterSaveStruct()));
			CharSave = PlayerCharacterSave.Find(PC);
		}


		// 1. Save Player Character Choice
		CharSave->PlayerChar = PlayerChar;
		CharSave->PlayerClass = ASC->AbilityPools[0];  // first choice is main

		// 2. Save Essential attributes, Base attributes carried over from playerclass info being filled in
		CharSave->AttributeSaves.Add(TTuple<FName, float>("Level", AS->GetLevel()));			//  Level
		CharSave->AttributeSaves.Add(TTuple<FName, float>("Experience", AS->GetExperience()));		//  Current Exp

		// 3. Inventory Items and Equipped items
		UP4InventoryBagComponent* IBC = PlayerChar->GetInventoryBagComponent();
		if (IBC)
		{
			// Save Inventory
			for (int i=0; i<IBC->InventoryArray.Num(); i++)
			{
				if (IBC->InventoryArray[i] && IBC->InventoryArray[i]->bIsEmpty)
					CharSave->InventorySaves.Add(FP4InventorySaveStruct(i, IBC->InventoryArray[i]));
			}

			// Save Equips
			CharSave->PlayerEquips = IBC->EquippmentSlots;
		}

		// 4. Relevant GE's (enchants and others)
		// TODO this idk
	}
}
