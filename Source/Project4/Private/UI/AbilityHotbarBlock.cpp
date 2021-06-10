// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AbilityHotbarBlock.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)


UAbilityHotbarBlock::UAbilityHotbarBlock(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
	
}

void UAbilityHotbarBlock::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeInputComponent();
	InitializeInputCallback();
}

void UAbilityHotbarBlock::SetAbilitySpecFromAbility()
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(Ability);
	if (Spec)
	{
		print(FString("SetAbilitySpec"));
		AbilitySpec = Spec->Handle;
	}
		
}

void UAbilityHotbarBlock::InitializeInputCallback()
{
	const FString FullStr = InputBindActionName; // EnumBinds->GetNameStringByIndex(static_cast<int32>(InputBind));

	FInputActionBinding AB(FName(*FullStr), IE_Pressed);
	//print(FString("Creating pressed bind for: " + FullStr));
	//UE_LOG(LogTemp, Warning, TEXT("Creating action binding for block  Action: %s"), *FullStr);
	AB.ActionDelegate.GetDelegateForManualSet().BindUObject(this, &UAbilityHotbarBlock::OnInputPressed);
	InputComponent->AddActionBinding(AB);

	AB = FInputActionBinding(FName(*FullStr), IE_Released);
	//print(FString("Creating released bind for: " + FullStr));
	//UE_LOG(LogTemp, Warning, TEXT("Creating action binding for block  Action: %s"), *FullStr);
	AB.ActionDelegate.GetDelegateForManualSet().BindUObject(this, &UAbilityHotbarBlock::OnInputReleased);
	InputComponent->AddActionBinding(AB);
}

void UAbilityHotbarBlock::OnInputPressed()
{
	/* Code taken from source AbilityLocalInputPressed(uint32 InputID) */
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(Ability);
	if (Spec)
	{
		if (Spec->Ability)
		{
			Spec->InputPressed = true;
			if (Spec->IsActive())
			{
				if (Spec->Ability->bReplicateInputDirectly && ASC->IsOwnerActorAuthoritative() == false)
				{
					ASC->ServerSetInputPressed(Spec->Handle);
				}

				ASC->AbilitySpecInputPressed(*Spec);

				// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
				ASC->InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec->Handle, Spec->ActivationInfo.GetActivationPredictionKey());
			}
			else
			{
				// Ability is not active, so try to activate it
				ASC->TryActivateAbility(Spec->Handle);

			}
		}
	}


	//ASC->AbilityLocalInputPressed(static_cast<int32>(InputBind));

	//ASC->TryActivateAbility(AbilitySpec);
	//ASC->ServerSetInputPressed(AbilitySpec);
}

void UAbilityHotbarBlock::OnInputReleased()
{
	/* Code taken from source AbilityLocalInputReleased(uint32 InputID) */
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(Ability);
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->Ability && Spec->IsActive())
		{
			if (Spec->Ability->bReplicateInputDirectly && ASC->IsOwnerActorAuthoritative() == false)
			{
				ASC->ServerSetInputReleased(Spec->Handle);
			}

			ASC->AbilitySpecInputReleased(*Spec);

			ASC->InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec->Handle, Spec->ActivationInfo.GetActivationPredictionKey());
		}
	}
	
	
	//ASC->AbilityLocalInputReleased(static_cast<int32>(InputBind));

	//ASC->ServerSetInputReleased(AbilitySpec);
}

