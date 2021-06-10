// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/P4AbilitySystemComponent.h"
#include "AbilitySystem/P4AbilityModifier.h"
#include "Project4.h"
#include "P4PlayerAbilitySystemComponent.generated.h"





DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageDealt, const FGameplayTagContainer&, DamageTags);


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
		TMap<FGameplayTag, UP4AbilityModifierInfo*> AbilityModifiers;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Abilities | Pools")
		int MaxAbilityPools = 1;

	UFUNCTION(BlueprintCallable, Category = "Abilities | Pools")
		void GetLearnedPoolAbilities(TArray<TSubclassOf<UP4GameplayAbility>>& Abilities);

	//UFUNCTION(BlueprintCallable, Category = "BP Exposed")
	//	void BP_GetAbilityMagnitudesByHandle(TSubclassOf<UP4GameplayAbility> AbilityClass, TArray<float>& Magnitudes, bool& FoundResult);


	UFUNCTION(BlueprintCallable, Category = "Delegates")
		void QueryDamageTagsForDelegates(FGameplayTagContainer& DamageTags);

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
		FOnDamageDealt DamageDealtDelegate;

	/*******************************/
	/* AbilityPool/Class selection */
	/*******************************/

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Abilities | Pools")
		void Server_OnAbilityPoolPicked(EClassAbilityPoolType newPoolType);
	void Server_OnAbilityPoolPicked_Implementation(EClassAbilityPoolType newPoolType);
	bool Server_OnAbilityPoolPicked_Validate(EClassAbilityPoolType newPoolType) { return true; }

	/*******************************/
	/*      Ability selection      */
	/*******************************/

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Abilities")
		void Server_OnPlayerSkillDropInteracted();
	void Server_OnPlayerSkillDropInteracted_Implementation();
	bool Server_OnPlayerSkillDropInteracted_Validate() { return true; }

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Abilities")
		void Server_OnGeneralSkillPoolSelected(EAbilityCategory SelectedPool);
	void Server_OnGeneralSkillPoolSelected_Implementation(EAbilityCategory SelectedPool);
	bool Server_OnGeneralSkillPoolSelected_Validate(EAbilityCategory SelectedPool) { return true; }

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Abilities")
		void Server_OnAbilityChoiceSelected(TSubclassOf<UP4GameplayAbility> SelectedAbility);
	void Server_OnAbilityChoiceSelected_Implementation(TSubclassOf<UP4GameplayAbility> SelectedAbility);
	bool Server_OnAbilityChoiceSelected_Validate(TSubclassOf<UP4GameplayAbility> SelectedAbility) { return true; }

	/****************************************/
	/*      Ability Modifier selection      */
	/****************************************/

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Abilities")
		void Server_OnPlayerSkillModifierDropInteracted();
	void Server_OnPlayerSkillModifierDropInteracted_Implementation();
	bool Server_OnPlayerSkillModifierDropInteracted_Validate() { return true; }

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Abilities")
		void Server_OnAbilityModifierAbilityChoiceSelected(TSubclassOf<UP4GameplayAbility> SelectedAbility);
	void Server_OnAbilityModifierAbilityChoiceSelected_Implementation(TSubclassOf<UP4GameplayAbility> SelectedAbility);
	bool Server_OnAbilityModifierAbilityChoiceSelected_Validate(TSubclassOf<UP4GameplayAbility> SelectedAbility) { return true; }

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Abilities")
		void Server_OnPlayerAbilityModifierSelected(UP4AbilityModifierInfo* ModifierInfo);
	void Server_OnPlayerAbilityModifierSelected_Implementation(UP4AbilityModifierInfo* ModifierInfo);
	bool Server_OnPlayerAbilityModifierSelected_Validate(UP4AbilityModifierInfo* ModifierInfo) { return true; }
};
