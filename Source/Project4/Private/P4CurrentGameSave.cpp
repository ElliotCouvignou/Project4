// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "P4CurrentGameSave.h"
#include "AbilitySystem/AttributeSets/PlayerAttributeSet.h"
#include "Project4PlayerState.h"
#include "Interactables/P4InventoryBagComponent.h"


UP4CurrentGameSave::UP4CurrentGameSave()
{
	//PlayerHotbarSave.AbilityHotbars.AddUninitialized(5);
	//PlayerHotbarSave.NumberHotbars.AddUninitialized(10);
}

void UP4CurrentGameSave::SaveCharacterInfoFromPlayer(AProject4Controller* PC, AP4PlayerCharacterBase* PlayerChar)
{
	//UP4PlayerAbilitySystemComponent* ASC = Cast<UP4PlayerAbilitySystemComponent>(PlayerChar->GetAbilitySystemComponent());
	UP4PlayerAbilitySystemComponent* ASC = Cast<UP4PlayerAbilitySystemComponent>(PC->GetPlayerState<AProject4PlayerState>()->GetAbilitySystemComponent());
	UPlayerAttributeSet* AS = Cast<UPlayerAttributeSet>(PlayerChar->GetAttributeSet());

	if (PC && PlayerChar && ASC)
	{
		FP4CharacterSaveStruct* CharSave = PlayerCharacterSave.Find(PC);
		if (!CharSave)
		{
			PlayerCharacterSave.Add(TTuple<AProject4Controller*, FP4CharacterSaveStruct>(PC, FP4CharacterSaveStruct()));
			CharSave = PlayerCharacterSave.Find(PC);
		}


		// 0. Save Player Character Choice
		CharSave->PlayerChar = PlayerChar;
		CharSave->PlayerClass = ASC->AbilityPools[0];  // first choice is main

		// 1. Save Essential attributes, Base attributes carried over from playerclass info being filled in
		CharSave->AttributeSaves.Add(TTuple<FName, float>("Level", AS->GetLevel()));			//  Level
		CharSave->AttributeSaves.Add(TTuple<FName, float>("Experience", AS->GetExperience()));		//  Current Exp

		// 2. Inventory Items and Equipped items
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

		// 3. Save abilities, modifiers
		FP4ASCSaveStruct& ASCSave = CharSave->ASCSaves;
		TArray<TSubclassOf<UP4GameplayAbility>> PoolAbilities;
		ASC->GetLearnedPoolAbilities(PoolAbilities);
		for (auto e : PoolAbilities)
		{
			FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(e);
			if(Spec)
				ASCSave.LearnedAbilities.Add(*Spec);
		}

		ASCSave.AbilityModifiers = ASC->AbilityModifiers;
		

		// 
		// 4. Relevant GE's (enchants and others)
		// TODO this idk
	}
}

void UP4CurrentGameSave::InitHotbarBlockSave(const EHotbarType HotbarType, const UAbilityHotbarBlock* BlockWidget, FP4HotbarBlockSaveStruct& Result)
{
	// Finds corresponding block's save, if none then make one
	
	print(FString("UP4CurrentGameSave::InitHotbarBlockSave"));
	if (BlockWidget)
	{
		// default to this but find corresponding
		TArray<FP4HotbarBlockSaveStruct>& Blocks = (HotbarType == EHotbarType::AbilityHotbar) ? PlayerHotbarSave.AbilityHotbars : PlayerHotbarSave.NumberHotbars;

		// get block array ref
		//switch (HotbarType)
		//{
		//case(EHotbarType::AbilityHotbar):
		//	print(FString("UP4CurrentGameSave::InitHotbarBlockSave for AbilityHotbar, Idx: " + FString::FromInt(BlockWidget->BlockIdx)));
		//	UE_LOG(LogTemp, Warning, TEXT("UP4CurrentGameSave::InitHotbarBlockSave for AbilityHotbar, Idx: %s"), *FString::FromInt(BlockWidget->BlockIdx));
		//	break;
		//
		//case(EHotbarType::Hotbar):
		//	//Blocks = PlayerHotbarSave.NumberHotbars;
		//	print(FString("UP4CurrentGameSave::InitHotbarBlockSave for NumberHotbar, Idx: " + FString::FromInt(BlockWidget->BlockIdx)));
		//	UE_LOG(LogTemp, Warning, TEXT("UP4CurrentGameSave::InitHotbarBlockSave for NumberHotbar, Idx: %s"), *FString::FromInt(BlockWidget->BlockIdx));
		//	break;
		//}

		if (Blocks.IsValidIndex(BlockWidget->BlockIdx))
		{
			Result = Blocks[BlockWidget->BlockIdx];
			Result.BlockIndex = BlockWidget->BlockIdx;
			Result.GAClass = BlockWidget->Ability;
			UE_LOG(LogTemp, Warning, TEXT("UP4CurrentGameSave::InitHotbarBlockSave Return existing"));
		}
		else
		{
			//FP4HotbarBlockSaveStruct* Save = new FP4HotbarBlockSaveStruct(BlockWidget->BlockIdx, BlockWidget->Ability);
			Blocks.Add(FP4HotbarBlockSaveStruct(BlockWidget->BlockIdx, BlockWidget->Ability));
			UE_LOG(LogTemp, Warning, TEXT("UP4CurrentGameSave::InitHotbarBlockSave Blocks size: %s"), *FString::FromInt(Blocks.Num()));

			Result = Blocks[BlockWidget->BlockIdx];	
			
			//Blocks.Insert(Save, BlockWidget->BlockIdx);
			

			UE_LOG(LogTemp, Warning, TEXT("UP4CurrentGameSave::InitHotbarBlockSave make new"));
		}
	}
}

void UP4CurrentGameSave::LoadHotbarBlockSave(const EHotbarType HotbarType, const UAbilityHotbarBlock* BlockWidget, FP4HotbarBlockSaveStruct& Result)
{

	print(FString("UP4CurrentGameSave::InitHotbarBlockSave"));
	if (BlockWidget)
	{
		// default to this but find corresponding
		TArray<FP4HotbarBlockSaveStruct>& Blocks = (HotbarType == EHotbarType::AbilityHotbar) ? PlayerHotbarSave.AbilityHotbars : PlayerHotbarSave.NumberHotbars;

		if (Blocks.IsValidIndex(BlockWidget->BlockIdx))
		{
			Result = Blocks[BlockWidget->BlockIdx];
			UE_LOG(LogTemp, Warning, TEXT("UP4CurrentGameSave::LoadHotbarBlockSave Return existing"));
		} 
		else
		{
			InitHotbarBlockSave(HotbarType, BlockWidget, Result);
		}
	}
}

void UP4CurrentGameSave::SaveHotbarBlockInfo(const EHotbarType HotbarType, const UAbilityHotbarBlock* BlockWidget)
{
	print(FString("UP4CurrentGameSave::SaveHotbarBlockInfo"));
	if (BlockWidget)
	{
		// default to this but find corresponding
		TArray<FP4HotbarBlockSaveStruct>& Blocks = (HotbarType == EHotbarType::AbilityHotbar) ? PlayerHotbarSave.AbilityHotbars : PlayerHotbarSave.NumberHotbars;


		// get block array ref
		//switch (HotbarType)
		//{
		//case(EHotbarType::AbilityHotbar):
		//	print(FString("UP4CurrentGameSave::InitHotbarBlockSave for AbilityHotbar, Idx: " + FString::FromInt(BlockWidget->BlockIdx)));
		//	UE_LOG(LogTemp, Warning, TEXT("UP4CurrentGameSave::InitHotbarBlockSave for AbilityHotbar, Idx: %s"), *FString::FromInt(BlockWidget->BlockIdx));
		//	break;
		//
		//case(EHotbarType::Hotbar):
		//	print(FString("UP4CurrentGameSave::InitHotbarBlockSave for NumberHotbar, Idx: " + FString::FromInt(BlockWidget->BlockIdx)));
		//	UE_LOG(LogTemp, Warning, TEXT("UP4CurrentGameSave::InitHotbarBlockSave for NumberHotbar, Idx: %s"), *FString::FromInt(BlockWidget->BlockIdx));
		//	break;
		//}

		if (Blocks.IsValidIndex(BlockWidget->BlockIdx))
		{
			FP4HotbarBlockSaveStruct& Save = Blocks[BlockWidget->BlockIdx];
			Save.GAClass = BlockWidget->Ability;
			Save.BlockIndex = BlockWidget->BlockIdx;

			UE_LOG(LogTemp, Warning, TEXT("UP4CurrentGameSave::InitHotbarBlockSave Return existing"));
		}
	}
}
