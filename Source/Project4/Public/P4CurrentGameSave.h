// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Characters/P4PlayerCharacterBase.h"
#include "AbilitySystem/P4PlayerAbilitySystemComponent.h"
#include "Interactables/P4InventoryBagComponent.h"
#include "Project4.h"
#include "UI/AbilityHotbar.h"
#include "UI/AbilityHotbarBlock.h"

#include "P4CurrentGameSave.generated.h"


// Info saved per player controller (per character)
// Need to save all progress gained

/* Saves info about one slot in player's inventory */
USTRUCT(BlueprintType)
struct FP4InventorySaveStruct
{
	GENERATED_USTRUCT_BODY()
	
	/* Index into player's inventory */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Essential")
		int InventoryIndex;

	// TODO: test to see if just saving object ptr saves data to prevent garbage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Essential")
		UP4ItemBaseObject* ItemObject;

	// else use below to reconstruct
	/* ItemClass */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Essential")
		TSubclassOf<class UP4ItemBaseObject> ObjectClass;

	// TODO: if i have to reconstuct implement seeded inv
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Essential")
		float ItemSeed;

	FP4InventorySaveStruct()
	{

	}

	FP4InventorySaveStruct(int Index, UP4ItemBaseObject* Obj)
	{
		InventoryIndex = Index;
		ItemObject = Obj;
	}

};

USTRUCT(BlueprintType)
struct FP4ASCSaveStruct
{
	GENERATED_USTRUCT_BODY()

	/* Index into player's inventory */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Essential")
		TArray<FGameplayAbilitySpec>  LearnedAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Essential")
		TMap<FGameplayTag, UP4AbilityModifierInfo*> AbilityModifiers;

	FP4ASCSaveStruct()
	{

	}

};


/*
		List Of Things that need to be saved
	0. Player Class Choice
	1. Attribute values (save essential ones like Level and have GE's do their work for the rest)
	2. Inventory Items, Save Generated Seed and class type (ensure deterministic outcomes)
	3. Learned Abilities and ability modifiers
	4. Any relevlant gameplay effects (temp buff)
	5. Enchants (WIP)

*/
USTRUCT(BlueprintType)
struct FP4CharacterSaveStruct
{
	GENERATED_USTRUCT_BODY()

	/* Character for controller to repossess after traveling */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Essential")
		AP4PlayerCharacterBase* PlayerChar;

	// TODO : replace this with playerclass if wedecide to not swap
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Essential")
		EClassAbilityPoolType PlayerClass;

	/* Only save essential attributes, not ones we eventually derive from GE's as they will stack */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
		TMap<FName, float> AttributeSaves;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory Items")
		TArray<FP4InventorySaveStruct> InventorySaves;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Equips")
		FEquippedItemsStruct PlayerEquips;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
		FP4ASCSaveStruct ASCSaves;

	// TODO: figure this out when necessary
	/* snapshot of the player's gameplayeffects */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameplayEffects")
		TArray<FGameplayEffectSpec> GameplayEffects;


	FP4CharacterSaveStruct()
	{

	}

};

USTRUCT(BlueprintType)
struct FP4HotbarBlockSaveStruct
{
	GENERATED_USTRUCT_BODY()

	/* probably unecessary as we can use array idx */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Essential")
		int BlockIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Essential")
		TSubclassOf<UP4GameplayAbility> GAClass;


	FP4HotbarBlockSaveStruct()
	{

	}

	FP4HotbarBlockSaveStruct(int i, TSubclassOf<UP4GameplayAbility> c)
	{
		BlockIndex = i;
		GAClass = c;
	}

};

USTRUCT(BlueprintType)
struct FP4HotbarSaveStruct
{
	GENERATED_USTRUCT_BODY()

		/* Character for controller to repossess after traveling */
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Essential")
		TArray<FP4HotbarBlockSaveStruct> AbilityHotbars;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Essential")
		TArray<FP4HotbarBlockSaveStruct> NumberHotbars;

	FP4HotbarSaveStruct()
	{

	}

};

/**
 * Holds all info about current game session to be used to save/load player character info between levels
 */
UCLASS()
class PROJECT4_API UP4CurrentGameSave : public USaveGame
{
	GENERATED_BODY()
	

public:

	UP4CurrentGameSave();

	// TODO include save for current game difficulty (easy, med, hard) and for current overall progression (e.g # levels cleared)

	/* Info of all players */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essential")
		TMap<AProject4Controller*, FP4CharacterSaveStruct> PlayerCharacterSave;

	// Data locally saved and loaded on clients
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essential")
		FP4HotbarSaveStruct PlayerHotbarSave;

	// TODO: save data about hub or something

	UFUNCTION(BlueprintCallable)
		void SaveCharacterInfoFromPlayer(AProject4Controller* PC, AP4PlayerCharacterBase* PlayerChar);

	// returns reference to save struct for this hotbar block
	UFUNCTION(BlueprintCallable)
		void InitHotbarBlockSave(const EHotbarType HotbarType, const UAbilityHotbarBlock* BlockWidget, FP4HotbarBlockSaveStruct& Result);

	UFUNCTION(BlueprintCallable)
		void LoadHotbarBlockSave(const EHotbarType HotbarType, const UAbilityHotbarBlock* BlockWidget, FP4HotbarBlockSaveStruct& Result);

	UFUNCTION(BlueprintCallable)
		void SaveHotbarBlockInfo(const EHotbarType HotbarType, const UAbilityHotbarBlock* BlockWidget);
};
