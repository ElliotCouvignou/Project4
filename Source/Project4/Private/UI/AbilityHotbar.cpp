// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/AbilityHotbar.h"
#include "Characters/P4PlayerCharacterBase.h"
#include "Project4.h"

//#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

UAbilityHotbar::UAbilityHotbar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{
	AbilityBlockAmount = ABILITY_BLOCK_AMOUNT;
}


void UAbilityHotbar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UAbilityHotbar::GivePlayerAbility(int32 BlockIndex, TSubclassOf<class UP4GameplayAbility> Ability)
{
	if (PlayerRef) {
		PlayerRef->GivePlayerAbilityToBlock(PlayerRef, BlockIndex, Ability);
	}
}


