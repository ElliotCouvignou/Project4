// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityHotbar.h"
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

UAbilityHotbar::UAbilityHotbar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{

}


void UAbilityHotbar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UAbilityHotbar::GivePlayerAbility(int32 BlockIndex, TSubclassOf<class UP4GameplayAbility> Ability)
{
	if (PlayerRef) {
		print(FString("AbilityGranted"));
		PlayerRef->GivePlayerAbility(PlayerRef, BlockIndex, Ability);
	}
}


