// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/PlayerAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"




UPlayerAttributeSet::UPlayerAttributeSet(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{

}
//  
//  /*  is called before... well, an attribute's base value (so without any temporary modifiers) is changed. 
//      It would be unwise to use this for game logic, and is mostly there to allow you to describe stat clamping.*/
//  void UPlayerAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
//  {
//  	Super::PreAttributeBaseChange(Attribute, NewValue);
//  }
//  
//  
//  /* same as above , but here you can define clamping with temporary modifiers instead.
//  Either way, NewValue describes the new value of a changed stat, and FGameplayAttribute
//  Attribute describes some info about the stat we're talking about */
//  void UPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
//  {
//  	Super::PreAttributeChange(Attribute, NewValue);
//  
//  }
//  
//  
//  /* is a function that takes the data a GameplayEffectExecutionCalculation spits out
//  (including which stats it wishes to modify, and by how much), and can then decide if the
//  GameplayEffectExecutionCalculation is allowed to influence the AttributeSet in any way,
//  by returning an appropriate bool. */
//  void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
//  {
//  	Super::PostGameplayEffectExecute(Data);
//  
//  }

void UPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, BlockChance, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, EvadeChance, COND_OwnerOnly, REPNOTIFY_Always);

	////////////////////////////////////
	/*         Progression Stats      */
	////////////////////////////////////

	// rep these to owner as other players can't see these values so shouldnt be repped
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Experience, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, ExperienceMax, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, CarryWeight, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, MaxCarryWeight, COND_OwnerOnly, REPNOTIFY_Always);

	
}



