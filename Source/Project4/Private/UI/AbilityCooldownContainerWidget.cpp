// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "UI/AbilityCooldownContainerWidget.h"
#include "Characters/P4PlayerCharacterBase.h"
#include "AbilitySystem/P4GameplayAbility.h"

void UAbilityCooldownContainerWidget::NativeConstruct()
{
	// look for player ASC and bind to on cooldown delegate
	AP4PlayerCharacterBase* PChar = Cast<AP4PlayerCharacterBase>(GetOwningPlayerPawn());
	if (PChar)
	{
		UAbilitySystemComponent* ASC = PChar->GetAbilitySystemComponent();
		if (ASC)
		{
			ASC->AbilityCommittedCallbacks.AddUObject(this, &UAbilityCooldownContainerWidget::OnAbilityCooldownCommited);
		}
	}

}

void UAbilityCooldownContainerWidget::OnAbilityCooldownCommited(UGameplayAbility* Ability)
{
	// Find Cooldown GE effect for this ability to get the granted tag as the cooldown tag.
	// It would be nice if this was a p4gameplayability to use the class var but that means new delegate

	if (Ability)
	{
		UP4GameplayAbility* P4Ability = Cast<UP4GameplayAbility>(Ability);
		if (P4Ability)
		{
			OnNewAbilityCooldown(P4Ability->GetClass());
		}
	}
}
