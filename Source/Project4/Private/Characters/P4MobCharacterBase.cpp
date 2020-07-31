// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Characters/P4MobCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/PlayerAttributeSet.h"
#include "AbilitySystem/P4AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/FloatingStatusBarWidget.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)



AP4MobCharacterBase::AP4MobCharacterBase(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create ASC with minimal replication since theres no need for prediction on mobs
	AbilitySystemComponentHardRef = CreateDefaultSubobject<UP4AbilitySystemComponent>(TEXT("AbilitySystemComponentHardRef"));
	AbilitySystemComponentHardRef->SetIsReplicated(true);
	AbilitySystemComponentHardRef->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AbilitySystemComponent = AbilitySystemComponentHardRef;

	AttributeSetHardRef = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("AttributeSetHardRef"));

	AttributeSet = AttributeSetHardRef;

}

void AP4MobCharacterBase::FinishDying()
{
	Super::FinishDying();

	Destroy();
}

void AP4MobCharacterBase::BeginPlay()
{
	Super::BeginPlay();


	if (AbilitySystemComponent.IsValid())
	{

		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		InitializeAttributeSet();
		AddAllStartupEffects();
		GiveEssentialAbilities();


		// TODO: Create floating status bars to clients here
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC && PC->IsLocalPlayerController())
		{
			// HERE
		}

		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AP4MobCharacterBase::HealthChanged);

		BindDelegates();

		InitFloatingStatusBarWidget();
	}
}


/******************/
/* Delegate Stuff */
/******************/

void AP4MobCharacterBase::BindDelegates()
{
	if (AbilitySystemComponent.IsValid())
	{
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AP4MobCharacterBase::HealthChanged);
		ManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AP4MobCharacterBase::ManaChanged);
	}
}
void AP4MobCharacterBase::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;

	// Update floating bar
	if (UIFloatingStatusBar)
	{
		UIFloatingStatusBar->SetHealthPercentage(AttributeSet->GetHealth() / AttributeSet->GetHealthMax());
	}

	// check ded
	if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	{
		this->Die();
	}
}

void AP4MobCharacterBase::ManaChanged(const FOnAttributeChangeData& Data)
{
	float Mana = Data.NewValue;

	// Update floating bar
	if (UIFloatingStatusBar)
	{
		UIFloatingStatusBar->SetManaPercentage(AttributeSet->GetMana() / AttributeSet->GetManaMax());
	}
}
