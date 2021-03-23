// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/P4AbilitySystemComponent.h"
#include "P4PlayerAbilitySystemComponent.generated.h"
UENUM(BlueprintType)
enum class EClassAbilityPoolType : uint8
{
	// 0 None
	None					UMETA(DisplayName = "None"), // this is default for fresh character
	Hero					UMETA(DisplayName = "Hero"),    // has access to everything (mostly for testing maybe dont give to players)
	Berserker				UMETA(DisplayName = "Berserker"),  // "None"
	Ranger					UMETA(DisplayName = "Ranger")
};
/**
 * 
 */
UCLASS()
class PROJECT4_API UP4PlayerAbilitySystemComponent : public UP4AbilitySystemComponent
{
	GENERATED_BODY()
	


public:
	UP4PlayerAbilitySystemComponent();

	// Array of player selected ability pools, the size of this array can be increased but always 2 for now
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Abilities | Pools")
		TArray<EClassAbilityPoolType> AbilityPools;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Abilities | Pools")
		int MaxAbilityPools = 1;

	UFUNCTION(BlueprintCallable, Category = "Abilities | Pools")
		void GetLearnedPoolAbilities(TArray<TSubclassOf<UP4GameplayAbility>>& Abilities);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Abilities | Pools")
		void Server_OnAbilityPoolPicked(EClassAbilityPoolType newPoolType);
	void Server_OnAbilityPoolPicked_Implementation(EClassAbilityPoolType newPoolType);
	bool Server_OnAbilityPoolPicked_Validate(EClassAbilityPoolType newPoolType) { return true; }

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Abilities")
		void Server_OnPlayerSkillDropInteracted();
	void Server_OnPlayerSkillDropInteracted_Implementation();
	bool Server_OnPlayerSkillDropInteracted_Validate() { return true; }

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Abilities")
		void Server_OnGeneralSkillPoolSelected(EAbilityCategory SelectedPool);
	void Server_OnGeneralSkillPoolSelected_Implementation(EAbilityCategory SelectedPool);
	bool Server_OnGeneralSkillPoolSelected_Validate(EAbilityCategory SelectedPool) { return true; }
};
