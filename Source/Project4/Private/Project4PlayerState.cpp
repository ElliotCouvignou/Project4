// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Project4PlayerState.h"

#include "Characters/Project4Character.h"
#include "Characters/P4PlayerCharacterBase.h"
#include "Project4Controller.h"

#include "AbilitySystem/PlayerAttributeSet.h"
#include "AbilitySystem/P4AbilitySystemComponent.h"

#include "UI/GameplayHudWidget.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)


AProject4PlayerState::AProject4PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
    
    AttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 10.f;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("PlayerState.Dead"));
}

UAbilitySystemComponent* AProject4PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UPlayerAttributeSet* AProject4PlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

void AProject4PlayerState::BeginPlay()
{
	Super::BeginPlay();

	//BindAbilityDelegates();
}

//void AProject4PlayerState::ShowAbilityConfirmCancelText(bool ShowText)
//{
//}


void AProject4PlayerState::BindAbilityDelegates()
{
	if (AbilitySystemComponent && AttributeSet)
	{
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AProject4PlayerState::HealthChanged);
		HealthMaxChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthMaxAttribute()).AddUObject(this, &AProject4PlayerState::HealthMaxChanged);
		HealthRegenChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthRegenAttribute()).AddUObject(this, &AProject4PlayerState::HealthRegenChanged);

		ManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute()).AddUObject(this, &AProject4PlayerState::ManaChanged);
		ManaMaxChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaMaxAttribute()).AddUObject(this, &AProject4PlayerState::ManaMaxChanged);
		ManaRegenChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaRegenAttribute()).AddUObject(this, &AProject4PlayerState::ManaRegenChanged);
		
		EnduranceChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetEnduranceAttribute()).AddUObject(this, &AProject4PlayerState::EnduranceChanged);
		EnduranceMaxChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetEnduranceMaxAttribute()).AddUObject(this, &AProject4PlayerState::EnduranceMaxChanged);
		EnduranceRegenChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetEnduranceRegenAttribute()).AddUObject(this, &AProject4PlayerState::EnduranceRegenChanged);

	}
}


bool AProject4PlayerState::IsAlive() const
{
	return AttributeSet->GetHealth() > 0.f;
}



/*********************/
/* Delegate Handlers */
/*********************/


void AProject4PlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;
	
	AP4PlayerCharacterBase* PChar = Cast<AP4PlayerCharacterBase>(GetPawn());

	// update floating status bar

	// update Player HUD (ref in controller)
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateHealth(Health);
		}
	}

	// if dead and no deathtag on player (true = ondeath transient)
	if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	{
		if (PChar)
		{
			// do die stuff (call ragdoll rn)
			print(FString("Died"));
		}
	}
}

void AProject4PlayerState::HealthMaxChanged(const FOnAttributeChangeData& Data)
{
	float HealthMax = Data.NewValue;

	// update floating bar
	AProject4Character* PChar = Cast<AProject4Character>(GetPawn());
	if (PChar)
	{

	}

	// update Player HUD (ref in controller)
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateHealthMax(HealthMax);
		}
	}

}

void AProject4PlayerState::HealthRegenChanged(const FOnAttributeChangeData& Data)
{
	float HealthRegen = Data.NewValue;

	// update Player HUD (ref in controller)
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateHealthRegen(HealthRegen);
		}
	}
}

void AProject4PlayerState::ManaChanged(const FOnAttributeChangeData& Data)
{
	float Mana = Data.NewValue;

	AP4PlayerCharacterBase* PChar = Cast<AP4PlayerCharacterBase>(GetPawn());

	// update floating status bar

	// update Player HUD (ref in controller)
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateMana(Mana);
		}
	}
}

void AProject4PlayerState::ManaMaxChanged(const FOnAttributeChangeData& Data)
{
	float ManaMax = Data.NewValue;

	AP4PlayerCharacterBase* PChar = Cast<AP4PlayerCharacterBase>(GetPawn());

	// update floating status bar 

	// update Player HUD (ref in controller)
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateManaMax(ManaMax);
		}
	}
}

void AProject4PlayerState::ManaRegenChanged(const FOnAttributeChangeData& Data)
{
	float ManaRegen = Data.NewValue;

	// update Player HUD (ref in controller)
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateManaRegen(ManaRegen);
		}
	}
}

void AProject4PlayerState::EnduranceChanged(const FOnAttributeChangeData& Data)
{
	float Endurance = Data.NewValue;

	AP4PlayerCharacterBase* PChar = Cast<AP4PlayerCharacterBase>(GetPawn());

	// update floating status bar

	// update Player HUD (ref in controller)
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateEndurance(Endurance);
		}
	}
}

void AProject4PlayerState::EnduranceMaxChanged(const FOnAttributeChangeData& Data)
{
	float EnduranceMax = Data.NewValue;

	AP4PlayerCharacterBase* PChar = Cast<AP4PlayerCharacterBase>(GetPawn());

	// update floating status bar

	// update Player HUD (ref in controller)
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateEnduranceMax(EnduranceMax);
		}
	}
}

void AProject4PlayerState::EnduranceRegenChanged(const FOnAttributeChangeData& Data)
{
	float EnduranceRegen = Data.NewValue;

	// update Player HUD (ref in controller)
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateEnduranceRegen(EnduranceRegen);
		}
	}
}


