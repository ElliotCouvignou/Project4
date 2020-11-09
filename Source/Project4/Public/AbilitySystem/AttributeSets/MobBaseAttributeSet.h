// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AttributeSets/P4BaseAttributeSet.h"
#include "MobBaseAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UMobBaseAttributeSet : public UP4BaseAttributeSet
{
	GENERATED_BODY()


public:

	/* Constructor */
	UMobBaseAttributeSet(const FObjectInitializer& ObjectInitializer);

	/*  is called before... well, an attribute's base value (so without any temporary modifiers) is changed.
	It would be unwise to use this for game logic, and is mostly there to allow you to describe stat clamping.*/
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	/* same as above , but here you can define clamping with temporary modifiers instead.
	Either way, NewValue describes the new value of a changed stat, and FGameplayAttribute
	Attribute describes some info about the stat we're talking about */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/* is a function that takes the data a GameplayEffectExecutionCalculation spits out
	(including which stats it wishes to modify, and by how much), and can then decide if the
	GameplayEffectExecutionCalculation is allowed to influence the AttributeSet in any way,
	by returning an appropriate bool. */
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	// For Replicaiton 
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

};
