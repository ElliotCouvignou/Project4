// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UCharacterInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdatePlayerName(const FString& NewName);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdatePlayerLevel();


	/******************/
	/*    Resource    */
	/******************/

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateHealthMax();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateHealthRegen();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateManaMax();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateManaRegen();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateEnduranceMax();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateEnduranceRegen();

	/******************/
	/*    Base att    */
	/******************/

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateStrength();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateDexterity();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateIntelligence();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateSpirit();

	/********************/
	/*     Defensive    */
	/********************/

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateArmor();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateMagicResistance();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateMovementSpeed();

	/********************/
	/*     Offensive    */
	/********************/

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateAttackPower();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateMagicPower();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateAttackSpeed();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateCritChance();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateCritDamage();
};
