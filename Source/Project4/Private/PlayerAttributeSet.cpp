// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "Project4Character.h"



UPlayerAttributeSet::UPlayerAttributeSet(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{

}

/*  is called before... well, an attribute's base value (so without any temporary modifiers) is changed. 
    It would be unwise to use this for game logic, and is mostly there to allow you to describe stat clamping.*/
void UPlayerAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == HealthAttribute()) {
		NewValue = FMath::Clamp(NewValue, -0.1f, HealthMax.GetCurrentValue());
	}
}


/* same as above , but here you can define clamping with temporary modifiers instead.
Either way, NewValue describes the new value of a changed stat, and FGameplayAttribute
Attribute describes some info about the stat we're talking about */
void UPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
}


/* is a function that takes the data a GameplayEffectExecutionCalculation spits out
(including which stats it wishes to modify, and by how much), and can then decide if the
GameplayEffectExecutionCalculation is allowed to influence the AttributeSet in any way,
by returning an appropriate bool. */
void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{

}

void UPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, HealthMax, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, MagicResistance, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, MagicPower, COND_None, REPNOTIFY_Always);
}



/*--------------------------------------------------------------------------*/
/* Attribute Grabbers and Replication Functions*/

FGameplayAttribute UPlayerAttributeSet::HealthAttribute()
{
	static FProperty* Property = FindFieldChecked<FProperty>(UPlayerAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UPlayerAttributeSet, Health));
	return FGameplayAttribute(Property);
}

FGameplayAttribute UPlayerAttributeSet::HealthMaxAttribute()
{
	static FProperty* Property = FindFieldChecked<FProperty>(UPlayerAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UPlayerAttributeSet, HealthMax));
	return FGameplayAttribute(Property);
}

FGameplayAttribute UPlayerAttributeSet::HealthRegenAttribute()
{
	static FProperty* Property = FindFieldChecked<FProperty>(UPlayerAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UPlayerAttributeSet, HealthRegen));
	return FGameplayAttribute(Property);
}

FGameplayAttribute UPlayerAttributeSet::ArmrorAttribute()
{
	static FProperty* Property = FindFieldChecked<FProperty>(UPlayerAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UPlayerAttributeSet, Armor));
	return FGameplayAttribute(Property);

}

FGameplayAttribute UPlayerAttributeSet::MagicResistanceAttribute()
{
	static FProperty* Property = FindFieldChecked<FProperty>(UPlayerAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UPlayerAttributeSet, MagicResistance));
	return FGameplayAttribute(Property);
}

FGameplayAttribute UPlayerAttributeSet::AttackPowerAttribute()
{
	static FProperty* Property = FindFieldChecked<FProperty>(UPlayerAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UPlayerAttributeSet, AttackPower));
	return FGameplayAttribute(Property);
}

FGameplayAttribute UPlayerAttributeSet::MagicPowerAttribute()
{
	static FProperty* Property = FindFieldChecked<FProperty>(UPlayerAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UPlayerAttributeSet, MagicPower));
	return FGameplayAttribute(Property);
}

