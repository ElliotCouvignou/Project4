// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Project4PlayerState.h"

#include "Characters/Project4Character.h"
#include "Characters/P4PlayerCharacterBase.h"
#include "Project4Controller.h"

#include "AbilitySystem/AttributeSets/PlayerAttributeSet.h"
#include "AbilitySystem/AttributeSets/P4BaseAttributeSet.h"
#include "AbilitySystem/P4PlayerAbilitySystemComponent.h"
#include "AbilitySystem/P4GameplayAbility.h"
#include "AbilitySystemComponent.h"


#include "UI/GameplayHudWidget.h"
#include "UI/FloatingStatusBarWidget.h"
#include "UI/PreGameLobbyWidget.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)


AProject4PlayerState::AProject4PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UP4PlayerAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	bActorSeamlessTraveled = true;

    AttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 10.f;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	BuffDebuffTag = FGameplayTag::RequestGameplayTag(FName("Buffs"));
}

UAbilitySystemComponent* AProject4PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UP4BaseAttributeSet* AProject4PlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

bool AProject4PlayerState::IsAlive() const
{
	return AttributeSet->GetHealth() > 0.f;
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
		
		AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &AProject4PlayerState::OnActiveGameplayEffectApplied);
		//AbilitySystemComponent->RegisterGameplayTagEvent(BuffDebuffTag).AddUObject(this, &AProject4PlayerState::OnBuffTagChanged);

		/*   Resource Stat Bindings   */
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AProject4PlayerState::HealthChanged);
		HealthMaxChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthMaxAttribute()).AddUObject(this, &AProject4PlayerState::HealthMaxChanged);
		HealthRegenChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthRegenAttribute()).AddUObject(this, &AProject4PlayerState::HealthRegenChanged);

		ManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute()).AddUObject(this, &AProject4PlayerState::ManaChanged);
		ManaMaxChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaMaxAttribute()).AddUObject(this, &AProject4PlayerState::ManaMaxChanged);
		ManaRegenChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaRegenAttribute()).AddUObject(this, &AProject4PlayerState::ManaRegenChanged);
		
		EnduranceChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetEnduranceAttribute()).AddUObject(this, &AProject4PlayerState::EnduranceChanged);
		EnduranceMaxChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetEnduranceMaxAttribute()).AddUObject(this, &AProject4PlayerState::EnduranceMaxChanged);
		EnduranceRegenChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetEnduranceRegenAttribute()).AddUObject(this, &AProject4PlayerState::EnduranceRegenChanged);

		/* following Delegates are UI ONLY, dont waste server's time */
		/*   Progression Stat Bindings   */
		LevelChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetLevelAttribute()).AddUObject(this, &AProject4PlayerState::LevelChanged);
		//CurrentExperienceDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetExperienceAttribute()).AddUObject(this, &AProject4PlayerState::CurrentExperienceChanged);
		//MaxExperienceDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetExperienceMaxAttribute()).AddUObject(this, &AProject4PlayerState::MaxExperienceChanged);
		//CarryWeightChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetCarryWeightAttribute()).AddUObject(this, &AProject4PlayerState::CarryWeightChanged);
		//MaxCarryWeightChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxCarryWeightAttribute()).AddUObject(this, &AProject4PlayerState::MaxCarryWeightChanged);

		/*   Base Stat Bindings   */
		StrengthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStrengthAttribute()).AddUObject(this, &AProject4PlayerState::StrengthChanged);
		DexterityChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetDexterityAttribute()).AddUObject(this, &AProject4PlayerState::DexterityChanged);
		IntelligenceChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetIntelligenceAttribute()).AddUObject(this, &AProject4PlayerState::IntelligenceChanged);
		SpiritChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetSpiritAttribute()).AddUObject(this, &AProject4PlayerState::SpiritChanged);

		/*   Defensive Stat Bindings   */
		ArmorChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetArmorAttribute()).AddUObject(this, &AProject4PlayerState::ArmorChanged);
		MagicResistanceChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMagicResistanceAttribute()).AddUObject(this, &AProject4PlayerState::MagicResistanceChanged);
		MovementSpeedChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMovementSpeedAttribute()).AddUObject(this, &AProject4PlayerState::MovementSpeedChanged);

		/*   Offsensive Stat Bindings   */
		AttackPowerDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetAttackPowerAttribute()).AddUObject(this, &AProject4PlayerState::AttackPowerChanged);
		MagicPowerChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMagicPowerAttribute()).AddUObject(this, &AProject4PlayerState::MagicPowerChanged);
		AttackSpeedChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetAttackSpeedAttribute()).AddUObject(this, &AProject4PlayerState::AttackSpeedChanged);
		CritChanceChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetCritChanceAttribute()).AddUObject(this, &AProject4PlayerState::CritChanceChanged);
		CritDamageChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetCritDamageAttribute()).AddUObject(this, &AProject4PlayerState::CritDamageChanged);
		

		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("Buffs.Negative.Stunned")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AProject4PlayerState::OnStunTagChanged);
	}
}



void AProject4PlayerState::ClientPlayerReadyStateChanged_Implementation(int index, bool NewState)
{
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC && PC->GetPreGameLobbyWidget())
	{
		PC->GetPreGameLobbyWidget()->PlayerReadyStatusChanged(index, NewState);
	}
}

void AProject4PlayerState::ClientInitPlayerReadyStates_Implementation(const TArray<bool>& States)
{
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC && PC->GetPreGameLobbyWidget())
	{
		PC->GetPreGameLobbyWidget()->InitPlayerReadyStatus(States);
	}	
}


/*********************/
/* Delegate Handlers */
/*********************/


void AProject4PlayerState::OnActiveGameplayEffectApplied(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	// get tags
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());

	FGameplayTagContainer TagContainer = FGameplayTagContainer();
	SpecApplied.GetAllGrantedTags(TagContainer);

	if (TagContainer.HasTag(BuffDebuffTag) && HasAuthority())
	{
		// Buff GE, find some stats for UI update
		// Need: IconImage (in ability class default), Duration. TooltipText(with iconimage), Bufftag, bool buffresettimer 
		
		// Find buff tag in this GE
		TArray<FGameplayTag> TagArray;
		TagContainer.GetGameplayTagArray(TagArray);

		TArray<FGameplayTag> BuffTags;
		for (const FGameplayTag& tag : TagArray)
		{
			if (tag.ToString().Contains(BuffDebuffTag.ToString()) || tag.RequestDirectParent().ToString().Contains(BuffDebuffTag.ToString()))
			{
				BuffTags.Add(tag);
			}
		}
		
		// find reset policy
		bool BuffResetTimer = (SpecApplied.Def->StackDurationRefreshPolicy == EGameplayEffectStackingDurationPolicy::NeverRefresh) ? false : true;
		bool Stackable = (SpecApplied.Def->StackLimitCount == 0) ? false : true;

		const UP4GameplayAbility* Ability = Cast<UP4GameplayAbility>(SpecApplied.GetEffectContext().GetAbility());
		
		for (const FGameplayTag BuffTag : BuffTags)
		{
			int OwnerStackCount = AbilitySystemComponent->GetCurrentStackCount(ActiveHandle);
			if (OwnerStackCount == 1)
			{
				//PC->SendBuffIconToUI(Ability->AbilityIcon, SpecApplied.GetDuration(), Ability->BuffToolTipText, BuffTag, BuffResetTimer, Stackable);
				PC->SendBuffIconToUI(BuffTag, SpecApplied, ActiveHandle);
			}
			else
			{
				// Possible case of new GE sharing buff tag with another GE
				PC->UpdateBuffIconStacksUI(BuffTag, OwnerStackCount);
			}

			if (Stackable)
			{
				// create binding on GE tag for stack count changes 
				AbilitySystemComponent->OnGameplayEffectStackChangeDelegate(ActiveHandle)->AddUObject(this, &AProject4PlayerState::OnBuffGameplayEffectStackChanged);
			}
			else if (SpecApplied.Def->DurationPolicy == EGameplayEffectDurationType::Infinite)
			{	
				if (!DelegatedBuffTags.Contains(BuffTag.ToString())) 
				{
					AbilitySystemComponent->RegisterGameplayTagEvent(BuffTag).AddUObject(this, &AProject4PlayerState::OnBuffTagRemoved);
					DelegatedBuffTags.Add(BuffTag.ToString());
				}
			}
		}
	}
}

// Server only function (Client tag counts are bugged... IDK man.......)
void AProject4PlayerState::OnBuffTagRemoved(const FGameplayTag Tag, int32 Count)
{
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());

	// delete buff icon in local player UI
	if (Count == 0)
	{
		PC->RemoveBuffIconFromUI(Tag);
	}
}

void AProject4PlayerState::OnBuffGameplayEffectStackChanged(FActiveGameplayEffectHandle ActiveHandle, int32 NewStackCount, int32 OldStackCount)
{
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());

	const FGameplayTagContainer* TagContainer = AbilitySystemComponent->GetGameplayEffectTargetTagsFromHandle(ActiveHandle);

	if (TagContainer->HasTag(BuffDebuffTag))
	{
		TArray<FGameplayTag> TagArray;
		TagContainer->GetGameplayTagArray(TagArray);

		TArray<FGameplayTag> BuffTags;
		for (const FGameplayTag& tag : TagArray)
		{
			if (tag.ToString().Contains(BuffDebuffTag.ToString()) || tag.RequestDirectParent().ToString().Contains(BuffDebuffTag.ToString()))
			{
				PC->UpdateBuffIconStacksUI(tag, NewStackCount);
			}
		}
	}
}

void AProject4PlayerState::CarryWeightChanged(const FOnAttributeChangeData& Data)
{
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{
		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{	
			HUD->UpdateCurrentCarryWeight(Data.NewValue);
		}
	}
}

void AProject4PlayerState::MaxCarryWeightChanged(const FOnAttributeChangeData& Data)
{
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{
		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateMaxCarryWeight(Data.NewValue);
		}
	}
}

void AProject4PlayerState::LevelChanged(const FOnAttributeChangeData& Data)
{
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{
		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdatePlayerLevel(Data.NewValue);
		}
	}
}

void AProject4PlayerState::CurrentExperienceChanged(const FOnAttributeChangeData& Data)
{
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{
		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			UXPBarWidget* XPWidget = HUD->GetXPBarWidget();
			if (XPWidget)
			{
				XPWidget->UpdateCurrentXP(Data.NewValue);
			}
		}		
	}
}

void AProject4PlayerState::MaxExperienceChanged(const FOnAttributeChangeData& Data)
{
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{
		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			UXPBarWidget* XPWidget = HUD->GetXPBarWidget();
			if (XPWidget)
			{
				XPWidget->UpdateMaxXP(Data.NewValue);
			}
		}
	}
}

void AProject4PlayerState::StrengthChanged(const FOnAttributeChangeData& Data)
{
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{
		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateStrength(Data.NewValue);
		}
	}
}

void AProject4PlayerState::DexterityChanged(const FOnAttributeChangeData& Data)
{
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{
		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateDexterity(Data.NewValue);
		}
	}
}

void AProject4PlayerState::IntelligenceChanged(const FOnAttributeChangeData& Data)
{
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{
		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateIntelligence(Data.NewValue);
		}
	}
}

void AProject4PlayerState::SpiritChanged(const FOnAttributeChangeData& Data)
{
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{
		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateSpirit(Data.NewValue);
		}
	}
}

void AProject4PlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;
	
	AP4PlayerCharacterBase* PChar = Cast<AP4PlayerCharacterBase>(GetPawn());

	// update floating status bar
	if (PChar)
	{
		UFloatingStatusBarWidget* FSBWidget = PChar->GetFloatingStatusBarWidget();
		if (FSBWidget && AttributeSet)
		{
			FSBWidget->SetHealthPercentage(Health / AttributeSet->GetHealthMax());
		}
	}

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
			PChar->Die();
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
		UFloatingStatusBarWidget* FSBWidget = PChar->GetFloatingStatusBarWidget();
		if (FSBWidget && AttributeSet)
		{
			FSBWidget->SetHealthPercentage(AttributeSet->GetHealth() / HealthMax);
		}
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

	//print(FString("New mana: " + FString::SanitizeFloat(Mana, 2)));

	AP4PlayerCharacterBase* PChar = Cast<AP4PlayerCharacterBase>(GetPawn());

	//  update floating status bar
	if (PChar)
	{
		UFloatingStatusBarWidget* FSBWidget = PChar->GetFloatingStatusBarWidget();
		if (FSBWidget && AttributeSet)
		{
			FSBWidget->SetManaPercentage(Mana / AttributeSet->GetManaMax());
		}
	}

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
	if (PChar)
	{
		UFloatingStatusBarWidget* FSBWidget = PChar->GetFloatingStatusBarWidget();
		if (FSBWidget && AttributeSet)
		{
			FSBWidget->SetManaPercentage(AttributeSet->GetMana() / ManaMax);
		}
	}

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

	// TODO: update floating status bar

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

	// TODO: update floating status bar

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

void AProject4PlayerState::ArmorChanged(const FOnAttributeChangeData& Data)
{
	float NewValue  = Data.NewValue;

	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateArmor(NewValue);
		}
	}
}

void AProject4PlayerState::MagicResistanceChanged(const FOnAttributeChangeData& Data)
{
	float NewValue = Data.NewValue;

	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateMagicResistance(NewValue);
		}
	}
}

void AProject4PlayerState::MovementSpeedChanged(const FOnAttributeChangeData& Data)
{
	float NewValue = Data.NewValue;

	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateMovementSpeed(NewValue);
		}
	}
}

void AProject4PlayerState::AttackPowerChanged(const FOnAttributeChangeData& Data)
{
	float NewValue = Data.NewValue;

	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateAttackPower(NewValue);
		}
	}
}

void AProject4PlayerState::MagicPowerChanged(const FOnAttributeChangeData& Data)
{
	float NewValue = Data.NewValue;

	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateMagicPower(NewValue);
		}
	}
}

void AProject4PlayerState::AttackSpeedChanged(const FOnAttributeChangeData& Data)
{
	float NewValue = Data.NewValue;
	
	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateAttackSpeed(NewValue);
		}
	}
}

void AProject4PlayerState::CritChanceChanged(const FOnAttributeChangeData& Data)
{
	float NewValue = Data.NewValue;

	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateCritChance(NewValue);
		}
	}
}

void AProject4PlayerState::CritDamageChanged(const FOnAttributeChangeData& Data)
{
	float NewValue = Data.NewValue;

	AProject4Controller* PC = Cast<AProject4Controller>(GetOwner());
	if (PC)
	{

		UGameplayHudWidget* HUD = PC->GetMainHUDWidget();
		if (HUD)
		{
			HUD->UpdateCritDamage(NewValue);
		}
	}
}

void AProject4PlayerState::OnStunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		

		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		// TODO: Create Tag Layer for unsstunnable abilities (Dont think they will exist)
		FGameplayTagContainer AbilityTagsToIgnore;
		//AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);

		AProject4Character* P4Char = Cast<AProject4Character>(GetPawn());
		if (P4Char)
		{
			P4Char->PlayStunnedAnimationMontage();
		}

	}
}


