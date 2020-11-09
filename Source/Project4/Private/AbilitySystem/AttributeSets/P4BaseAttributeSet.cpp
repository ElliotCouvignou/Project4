// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AbilitySystem/AttributeSets/P4BaseAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Project4Controller.h"
#include "Project4.h"
#include "Characters/Project4Character.h"
#include "Characters/P4PlayerCharacterBase.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)


UP4BaseAttributeSet::UP4BaseAttributeSet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CritTag = FGameplayTag::RequestGameplayTag(FName("Effect.Damage.Crit"));
	PhysicalDamageTag = FGameplayTag::RequestGameplayTag(FName("Effect.Damage.Physical"));
	MagicDamageTag = FGameplayTag::RequestGameplayTag(FName("Effect.Damage.Magic"));

	DamageNumberContainerFilter = FGameplayTagContainer(CritTag);
	DamageNumberContainerFilter.AddTagFast(PhysicalDamageTag);
	DamageNumberContainerFilter.AddTagFast(MagicDamageTag);
}

void UP4BaseAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute.AttributeName == GetHealthAttribute().AttributeName) {
		NewValue = FMath::Clamp(NewValue, 0.0f, HealthMax.GetCurrentValue());
	}
	else if (Attribute.AttributeName == GetManaAttribute().AttributeName) {
		NewValue = FMath::Clamp(NewValue, 0.0f, ManaMax.GetCurrentValue());
	}
	else if (Attribute.AttributeName == GetEnduranceAttribute().AttributeName) {
		NewValue = FMath::Clamp(NewValue, 0.0f, EnduranceMax.GetCurrentValue());
	}
	else if (Attribute.AttributeName == GetAttackSpeedAttribute().AttributeName) {
		NewValue = FMath::Clamp(NewValue, 0.0f, 100.f);
	}

}

void UP4BaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// Dynamic Attribute value clamps
	if (Attribute.AttributeName == GetHealthAttribute().AttributeName) {
		NewValue = FMath::Clamp(NewValue, 0.0f, HealthMax.GetCurrentValue());
	}
	else if (Attribute.AttributeName == GetManaAttribute().AttributeName) {
		NewValue = FMath::Clamp(NewValue, 0.0f, ManaMax.GetCurrentValue());
	}
	else if (Attribute.AttributeName == GetEnduranceAttribute().AttributeName) {
		NewValue = FMath::Clamp(NewValue, 0.0f, EnduranceMax.GetCurrentValue());
	}
	else if (Attribute.AttributeName == GetAttackSpeedAttribute().AttributeName) {
		NewValue = FMath::Clamp(NewValue, 0.0f, 100.f);
	}

	// max value clampers, makes sure health/maxhealth % stays same 
	else if (Attribute.AttributeName == GetHealthMaxAttribute().AttributeName)
	{
		AdjustAttributeForMaxChange(Health, HealthMax, NewValue, GetHealthAttribute());
	}
	else if (Attribute.AttributeName == GetManaMaxAttribute().AttributeName)
	{
		AdjustAttributeForMaxChange(Mana, ManaMax, NewValue, GetManaAttribute());
	}
	else if (Attribute.AttributeName == GetEnduranceMaxAttribute().AttributeName)
	{
		AdjustAttributeForMaxChange(Endurance, EnduranceMax, NewValue, GetEnduranceAttribute());
	}
	else if (Attribute.AttributeName == GetMovementSpeedAttribute().AttributeName) {
		UCharacterMovementComponent* CMC = Cast<UCharacterMovementComponent>(GetActorInfo()->MovementComponent);
		if (CMC)
		{
			CMC->MaxWalkSpeed = NewValue;
		}
	}

}

void UP4BaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
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
				// show healing numbers to source actor (DONT IGNORE SELF-HEAL)
				AProject4Controller* PController = Cast<AProject4Controller>(SourceController);
				if (PController) {
					PController->DisplayHealNumber(HealingDone, TargetCharacter);
				}
			}
		}
	}
}

void UP4BaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, Level, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, ExperienceBounty, COND_None, REPNOTIFY_Always);

	////////////////////////////////////
	/*         Resource Stats        */
	////////////////////////////////////

	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, HealthMax, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, ManaMax, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, ManaRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, Endurance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, EnduranceMax, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, EnduranceRegen, COND_None, REPNOTIFY_Always);

	////////////////////////////////////
	/*			Base Stats			  */
	////////////////////////////////////

	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, Spirit, COND_None, REPNOTIFY_Always);

	////////////////////////////////////
	/*         Defensive Stats        */
	////////////////////////////////////

	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, MagicResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);

	////////////////////////////////////
	/*         Offensive Stats        */
	////////////////////////////////////

	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, MagicPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, MainHandWeaponPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, MainHandAttackInterval, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, OffHandWeaponPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, OffHandAttackInterval, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, CritChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UP4BaseAttributeSet, CritDamage, COND_None, REPNOTIFY_Always);

}

void UP4BaseAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
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
