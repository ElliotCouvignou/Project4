// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Characters/P4MobCharacterBase.h"
#include "Characters/PatrolPointsActor.h"
#include "Interactables/P4InventoryBagComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/MobBaseAttributeSet.h"
#include "AbilitySystem/P4AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/FloatingStatusBarWidget.h"
#include "Interactables/P4ItemBaseActor.h"
#include "Characters/AI/P4AIControllerBase.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)
#define ISHOST (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer) || (GEngine->GetNetMode(GetWorld()) == NM_ListenServer)

AP4MobCharacterBase::AP4MobCharacterBase(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create ASC with minimal replication since theres no need for prediction on mobs
	AbilitySystemComponentHardRef = CreateDefaultSubobject<UP4AbilitySystemComponent>(TEXT("AbilitySystemComponentHardRef"));
	AbilitySystemComponentHardRef->SetIsReplicated(true);
	AbilitySystemComponentHardRef->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AbilitySystemComponent = AbilitySystemComponentHardRef;

	AttributeSetHardRef = CreateDefaultSubobject<UMobBaseAttributeSet>(TEXT("AttributeSetHardRef"));

	AttributeSet = AttributeSetHardRef;
}


void AP4MobCharacterBase::Die()
{
	if (HasAuthority())
	{
		// Server Roll Drop table and spawn rolled items nearby
		TArray<TTuple<UItemBaseDataAsset*, int>> ItemsToDrop = DropTable.RollItemDrops();

		for (TTuple<UItemBaseDataAsset*, int> ItemDrop : ItemsToDrop)
		{
			if (ItemDrop.Key && ItemDrop.Value > 0)
			{
				// create FInventoryItemStruct and ItemActor, then spawn
				AP4ItemBaseActor* ItemActor = Cast<AP4ItemBaseActor>(GetWorld()->SpawnActorDeferred<AActor>(AP4ItemBaseActor::StaticClass(), GetActorTransform(), this, (APawn*)this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
				if (ItemActor)
				{
					ItemActor->SetInventoryItemStruct(FInventoryItemStruct(ItemDrop.Key, ItemDrop.Value));
					FTransform Transform = GetActorTransform();
					Transform.AddToTranslation(FVector(FMath::RandRange(-100.f, 100.f), FMath::RandRange(-100.f, 100.f), 0.f));
					ItemActor->FinishSpawning(Transform);
				}
			}
		}
	}
	
	Super::Die();
}


void AP4MobCharacterBase::FinishDying()
{
	Super::FinishDying();

	Destroy();
}

void AP4MobCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// Try get patrol points
	if (PatrolPointsActor)
	{
		PatrolPointsActor->GetPatrolPointsInWorldSpace(PatrolPoints);
	}

	if (AbilitySystemComponent.IsValid())
	{
		
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		InitializeAttributeSet();

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

		AddAllStartupEffects();
	}
}


/******************/
/* Delegate Stuff */
/******************/



void AP4MobCharacterBase::InitializeAttributeSet()
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->InitStats(UMobBaseAttributeSet::StaticClass(), AttrDataTable);
	}
}

void AP4MobCharacterBase::BindDelegates()
{
	if (AbilitySystemComponent.IsValid())
	{
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AP4MobCharacterBase::HealthChanged);
		ManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AP4MobCharacterBase::ManaChanged);
		
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("Buffs.Negative.Stunned")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AP4MobCharacterBase::OnStunTagChanged);
	}
}
void AP4MobCharacterBase::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;

	// Update floating bar
	if (UIFloatingStatusBar && AttributeSet.IsValid())
	{
		UIFloatingStatusBar->SetHealthPercentage(AttributeSet->GetHealth() / AttributeSet->GetHealthMax());
	}

	//  AP4AIControllerBase* AIC = Cast<AP4AIControllerBase>(GetController());
	//  if (AIC && HasAuthority() && Data.GEModData)
	//  {
	//  	AActor* Source = Data.GEModData->EffectSpec.GetContext().GetInstigator();
	//  	float DamageTaken = Data.OldValue - Data.NewValue;
	//  	if (Source && DamageTaken > 0.f)
	//  	{
	//  		print(FString("MobTamageTaken"));
	//  		AIC->MobDamageTaken(Source, DamageTaken);
	//  	}
	//  	if (!Source)
	//  	{
	//  		print(FString("Invalid Source"));
	//  	}
	//  }

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
	if (UIFloatingStatusBar && AttributeSet.IsValid())
	{
		UIFloatingStatusBar->SetManaPercentage(AttributeSet->GetMana() / AttributeSet->GetManaMax());
	}
}

void AP4MobCharacterBase::OnStunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		// TODO: Create Tag Layer for unsstunnable abilities (Dont think they will exist)
		FGameplayTagContainer AbilityTagsToIgnore;
		//AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);

		PlayStunnedAnimationMontage();

	}
}


/* ************************* */
/* ************************* */   
/* Item Drop Table Functions */
/* ************************* */
/* ************************* */


TTuple<UItemBaseDataAsset*, int> FDependentItemDropPool::RollDependentDropPool()
{
	int TotalPoints = 0;
	for (FDependentItemDrop& DropStruct : DependentDropPool)
	{
		TotalPoints += DropStruct.PointCount;
	}
	int Points = FMath::RandRange(0, TotalPoints);
	for (FDependentItemDrop& DropStruct : DependentDropPool)
	{
		Points -= DropStruct.PointCount;
		if (Points <= 0)
		{
			int StackCount = FMath::RandRange(0.f, 1.f) * (DropStruct.StackCountMax - DropStruct.StackCountMin) + DropStruct.StackCountMin;
			return TTuple<UItemBaseDataAsset*, int>(DropStruct.Item, StackCount);
		}
	}
	return TTuple<UItemBaseDataAsset*, int>(nullptr, 0);
}


TArray<TTuple<UItemBaseDataAsset*, int>> FDropTableStruct::RollItemDrops()
{
	TArray<TTuple<UItemBaseDataAsset*, int>> Ret;

	for (FIndependentItemDrop& DropStruct : IndependentDrops)
	{
		TTuple<UItemBaseDataAsset*, int> Item = DropStruct.RollIndependentItem();
		if (Item.Key)
		{
			Ret.Add(Item);
		}
	}

	for (FDependentItemDropPool& DropPool : DependentDrops)
	{
		TTuple<UItemBaseDataAsset*, int> PoolItem = DropPool.RollDependentDropPool();
		if (PoolItem.Key)
		{
			Ret.Add(PoolItem);
		}
	}

	return Ret;
}


