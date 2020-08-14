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
		void UpdatePlayerLevel(float NewLevel);


	/******************/
	/*    Resource    */
	/******************/

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateHealthMax(float NewValue);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateHealthRegen(float NewValue);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateManaMax(float NewValue);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateManaRegen(float NewValue);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateEnduranceMax(float NewValue);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateEnduranceRegen(float NewValue);

	/********************/
	/*     Defensive    */
	/********************/

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateArmor(float NewValue);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateMagicResistance(float NewValue);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateMovementSpeed(float NewValue);

	/********************/
	/*     Offensive    */
	/********************/

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateAttackPower(float NewValue);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateMagicPower(float NewValue);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateAttackSpeed(float NewValue);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateCritChance(float NewValue);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateCritDamage(float NewValue);
};
