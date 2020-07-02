// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Project4PlayerState.h"

#include "AbilitySystem/P4AbilitySystemComponent.h"
#include "Project4Character.h"



AProject4PlayerState::AProject4PlayerState()
{
	//AbilitySystemComponent = CreateDefaultSubobject<UP4AbilitySystemComponent>(TEXT("AbilitySystem"));
	//AbilitySystemComponent->SetIsReplicated(true);
	//AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

//void AProject4PlayerState::BeginPlay()
//{
//	Super::BeginPlay();
//
//	// Try to get pawn ASC
//	//if (!AbilitySystemComponent.IsValid())
//	//{
//	//	AProject4Character* PChar = Cast<AProject4Character>(GetPawn());
//	//	if (PChar)
//	//	{
//	//		UP4AbilitySystemComponent* ASC = Cast<UP4AbilitySystemComponent>(PChar->GetAbilitySystemComponent());
//	//		if (ASC)
//	//		{
//	//			SetAbilitySystemComponent(ASC);
//	//		}
//	//	}
//	//}
//}
//
//UAbilitySystemComponent* AProject4PlayerState::GetAbilitySystemComponent() const
//{
//	return AbilitySystemComponent.Get();
//}
//
//void AProject4PlayerState::SetAbilitySystemComponent(UP4AbilitySystemComponent* NewASC)
//{
//	AbilitySystemComponent = NewASC;
//}
//
//bool AProject4PlayerState::IsAlive() const
//{
//	return true;
//}
//
//void AProject4PlayerState::ShowAbilityConfirmCancelText(bool ShowText)
//{
//}
