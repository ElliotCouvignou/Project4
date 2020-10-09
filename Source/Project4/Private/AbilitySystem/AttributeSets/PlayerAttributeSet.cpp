// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/PlayerAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Project4Controller.h"
#include "Project4.h"
#include "Characters/Project4Character.h"
#include "Characters/P4PlayerCharacterBase.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)


UPlayerAttributeSet::UPlayerAttributeSet(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	CritTag = FGameplayTag::RequestGameplayTag(FName("Effect.Damage.Crit"));
	PhysicalDamageTag = FGameplayTag::RequestGameplayTag(FName("Effect.Damage.Physical"));
	MagicDamageTag = FGameplayTag::RequestGameplayTag(FName("Effect.Damage.Magic"));

	DamageNumberContainerFilter = FGameplayTagContainer(CritTag);
	DamageNumberContainerFilter.AddTagFast(PhysicalDamageTag);
	DamageNumberContainerFilter.AddTagFast(MagicDamageTag);
}

/*  is called before... well, an attribute's base value (so without any temporary modifiers) is changed. 
    It would be unwise to use this for game logic, and is mostly there to allow you to describe stat clamping.*/
void UPlayerAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, HealthMax.GetCurrentValue());
	}
	else if (Attribute == GetManaAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, ManaMax.GetCurrentValue());
	}
	else if (Attribute == GetEnduranceAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, EnduranceMax.GetCurrentValue());
	}
	else if (Attribute == GetAttackSpeedAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, 100.f);
	}

}


/* same as above , but here you can define clamping with temporary modifiers instead.
Either way, NewValue describes the new value of a changed stat, and FGameplayAttribute
Attribute describes some info about the stat we're talking about */
void UPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// Dynamic Attribute value clamps
	if (Attribute == GetHealthAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, HealthMax.GetCurrentValue());
	}
	else if (Attribute == GetManaAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, ManaMax.GetCurrentValue());
	}
	else if (Attribute == GetEnduranceAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, EnduranceMax.GetCurrentValue());
	}
	else if (Attribute == GetAttackSpeedAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, 100.f);
	}
	else if (Attribute == GetExperienceAttribute())
	{
		AProject4Controller* PC = Cast<AProject4Controller>(GetActorInfo()->PlayerController);
		PC->UpdateUICurrentXP(NewValue);
	}
	else if (Attribute == GetExperienceMaxAttribute())
	{
		AProject4Controller* PC = Cast<AProject4Controller>(GetActorInfo()->PlayerController);
		PC->UpdateUIMaxXP(NewValue);
	}
	else if (Attribute == GetLevelAttribute())
	{
		AProject4Controller* PC = Cast<AProject4Controller>(GetActorInfo()->PlayerController);
		PC->UpdateUILevel(NewValue);
	}

	// max value clampers, makes sure health/maxhealth % stays same 
	else if (Attribute == GetHealthMaxAttribute())
	{
		AdjustAttributeForMaxChange(Health, HealthMax, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetManaMaxAttribute())
	{
		AdjustAttributeForMaxChange(Mana, ManaMax, NewValue, GetManaAttribute());
	}
	else if (Attribute == GetEnduranceMaxAttribute())
	{
		AdjustAttributeForMaxChange(Endurance, EnduranceMax, NewValue, GetEnduranceAttribute());
	}
	else if (Attribute == GetMovementSpeedAttribute()) {
		UCharacterMovementComponent* CMC = Cast<UCharacterMovementComponent>(GetActorInfo()->MovementComponent);
		if (CMC)
		{
			CMC->MaxWalkSpeed = NewValue;
		}
	}
}


/* is a function that takes the data a GameplayEffectExecutionCalculation spits out
(including which stats it wishes to modify, and by how much), and can then decide if the
GameplayEffectExecutionCalculation is allowed to influence the AttributeSet in any way,
by returning an appropriate bool. */
void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	FGameplayTagContainer SpecAssetTags;
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	AProject4Character* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<AProject4Character>(TargetActor);
	}

	// Get the Source actor
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	AProject4Character* SourceCharacter = nullptr;
	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		if (SourceController == nullptr && SourceActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}

		// Use the controller to find the source pawn
		if (SourceController)
		{
			SourceCharacter = Cast<AProject4Character>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast<AProject4Character>(SourceActor);
		}

		// Set the causer actor based on context if it's set
		if (Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	}

	// Source and Target Info collected, onto the 'meat'
	if (Data.EvaluatedData.Attribute == GetDamageAttribute()) {

		// Try to extract a hit result
		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		const float DamageTaken = GetDamage();
		SetDamage(0.f); // clear for next instance of damage

		if (DamageTaken > 0.f) {
			bool WasAlive = true;

			if (TargetCharacter) {
				// TODO: Function in character for isAlive bool into wasAlive
				// WasAlive = TargetCharacter->IsAlive();
			}
			// Calculate new Health here for on-death transients
			const float NewHealth = GetHealth() - DamageTaken;

			if (TargetCharacter && WasAlive) {

				const FHitResult* Hit = Data.EffectSpec.GetContext().GetHitResult();
				if (Hit) {
					// Trigger Animation on-hit Reactions here e.g flinch
				}

				// Get source PC
				AProject4Controller* SourcePC = Cast<AProject4Controller>(SourceController);

				// Show Damage numbers to source actor (ignore self-damage)
				if (SourceActor != TargetActor) {
					if (SourcePC) 
					{
						FGameplayTagContainer DamageNumberTags;
						Data.EffectSpec.GetAllAssetTags(DamageNumberTags);
						DamageNumberTags = DamageNumberTags.Filter(DamageNumberContainerFilter);

						/* Look at dynamic asset tags for info about damage type */
						if (Data.EffectSpec.DynamicAssetTags.HasTag(CritTag))
						{
							DamageNumberTags.AddTagFast(CritTag);
						}					

						/* Send collected damage data tags to client, let them do parse */
						SourcePC->DisplayDamageNumber(FP4DamageNumber(DamageTaken, DamageNumberTags), TargetCharacter);
					}
				}
				
				if (NewHealth < 0.f) {
					// target died, give xp and generate loots
					// check if source PC exists in case an npc killed something
					if (SourceCharacter && SourcePC && (SourceController != TargetController))
					{
						AP4PlayerCharacterBase* PlayerChar = Cast<AP4PlayerCharacterBase>(SourceCharacter);
						PlayerChar->GainExperience(GetExperienceBounty());

						//UPlayerAttributeSet* SourceAS = SourceCharacter->GetAttributeSet();
						//float NewSourceXp = SourceAS->GetExperience() + GetExperienceBounty();
						//
						//// Check Character Levelup
						//if (NewSourceXp > SourceAS->GetExperienceMax()) {
						//	// Call level up VFX Handler
						//	NewSourceXp -= SourceAS->GetExperienceMax();
						//	
						//	// +1 Level, Send to UI							
						//	const float NewLevel = SourceAS->GetLevel() + 1.f;
						//	SourceAS->SetLevel(NewLevel);
						//	SourcePC->UpdateUILevel(NewLevel);
						//
						//	// ^ MaxXP, Send to UI
						//	const float NewMaxXP = SourceAS->GetExperienceMax() * 2.5f;
						//	SourceAS->SetExperienceMax(NewMaxXP);
						//	SourcePC->UpdateUIMaxXP(NewMaxXP);
						//}
						//// Add XP
						//SourceAS->SetExperience(NewSourceXp);
						//SourcePC->UpdateUICurrentXP(NewSourceXp);
						
					}
				}
				SetHealth(FMath::Clamp(NewHealth, 0.f, GetHealthMax()));
			}
		}
	}

	if (Data.EvaluatedData.Attribute == GetHealAttribute()) {
		// Try to extract a hit result
		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		const float HealingDone = GetHeal();
		SetHeal(0.f);

		if (HealingDone > 0.f && GetHealth() != GetHealthMax()) {
			// no testing for death transients here, Spell shouln't go off anyway if target is dead
			// i.e check death before casting spell 

			const float NewHealth = GetHealth() + HealingDone;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetHealthMax()));

			if (TargetCharacter) {
				const FHitResult* Hit = Data.EffectSpec.GetContext().GetHitResult();
				if (Hit) {
					// Trigger Animation on-hit Reactions here e.g flinch
					// I don't think we need this part, VFX are on abilities not triggered here
					// and getting healed animations are mostly cringe
				}

				// show healing numbers to source actor (DONT IGNORE SELF-HEAL)
				AProject4Controller* PController = Cast<AProject4Controller>(SourceController);
				if (PController) {
					PController->DisplayHealNumber(HealingDone, TargetCharacter);
				}
			}
		}
	}
}

void UPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	////////////////////////////////////
	/*         Progression Stats      */
	////////////////////////////////////

	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Level, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Experience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, ExperienceMax, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, ExperienceBounty, COND_None, REPNOTIFY_Always);

	////////////////////////////////////
	/*         Resource Stats        */
	////////////////////////////////////

	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, HealthMax, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, ManaMax, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, ManaRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Endurance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, EnduranceMax, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, EnduranceRegen, COND_None, REPNOTIFY_Always);

	////////////////////////////////////
	/*         Defensive Stats        */
	////////////////////////////////////

	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, MagicResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);

	////////////////////////////////////
	/*         Offensive Stats        */
	////////////////////////////////////

	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, MagicPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, CritChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, CritDamage, COND_None, REPNOTIFY_Always);
}

void UPlayerAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}


