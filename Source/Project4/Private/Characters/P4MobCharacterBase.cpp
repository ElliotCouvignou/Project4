// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Characters/P4MobCharacterBase.h"
#include "Characters/PatrolPointsActor.h"
#include "Interactables/P4InventoryBagComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/MobBaseAttributeSet.h"
#include "AbilitySystem/P4AbilitySystemComponent.h"
#include "AbilitySystem/GameplayEffects/EquipItemGameplayEffect.h"
#include "AbilitySystem/AttributeSets/PlayerAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "UI/FloatingStatusBarWidget.h"
#include "Interactables/P4ItemBaseActor.h"
#include "AI/P4AIControllerBase.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h" 

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

	auto temp = ConstructorHelpers::FObjectFinder<URolleableAttributesDataAsset>(TEXT("/Game/Project4/Interactables/Items/Equips/RolleableAttributesDataAsset"));
	if (temp.Object)
	{
		RollableArmorDataAsset = temp.Object;
		RollableArmorDataAsset->SumTotalWeights();
	}
	auto temp2 = ConstructorHelpers::FObjectFinder<URolleableArmorItemsDataAsset>(TEXT("/Game/Project4/Interactables/Items/Equips/Armor/RolleableArmorItemsDataAsset"));
	if (temp2.Object)
	{
		RollableArmorItemsDataAsset = temp2.Object;
		RollableArmorItemsDataAsset->SumTotalWeights();
	}
}


void AP4MobCharacterBase::Die()
{
	if (HasAuthority())
	{
		// Server Roll Drop table and spawn rolled items nearby
		if (bGenerateItemDrops)
		{
			TArray<FInventoryItemStruct> ItemsToDrop;
			int NumItems = (int)FMath::RandRange(0.f, 2.f);
			
			// Roll for Equip drops
			FInventoryItemStruct GeneratedItem;
			for(int i=0; i< NumItems; i++)
			{
				GenerateEquipItemDrop(GeneratedItem);

				// create FInventoryItemStruct and ItemActor, then spawn
				AP4ItemBaseActor* ItemActor = Cast<AP4ItemBaseActor>(GetWorld()->SpawnActorDeferred<AActor>(AP4ItemBaseActor::StaticClass(), GetActorTransform(), this, (APawn*)this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
				if (ItemActor)
				{
					ItemActor->SetInventoryItemStruct(GeneratedItem);
					FTransform Transform = GetActorTransform();
					Transform.AddToTranslation(FVector(FMath::RandRange(-100.f, 100.f), FMath::RandRange(-100.f, 100.f), 0.f));
					ItemActor->FinishSpawning(Transform);
				}

				//ItemsToDrop.Add(GeneratedItem);
			}

			// TODO: roll for other drops (materials, currency, etc.)
			
			//for (FInventoryItemStruct Item : ItemsToDrop)
			//{
			//	
			//}			
		}
		else
		{
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
	}
	
	Super::Die();
}

void AP4MobCharacterBase::GenerateEquipItemDrop(FInventoryItemStruct& GeneratedItem)
{
	// this is very WIP and randominess can be controlled more later

	float Budget = 100.f; // TODO: figure out current game difficulty to this value 
	GeneratedItem.bIsEmpty = false;
	GeneratedItem.StackCount = 1.f;

	// Randomly select item gear type TODO: create structure for weights to allow  wep to drop more often
	// 11 armor types, add 4 for 3x weight of weapons
	int ItemType = (int)FMath::RandRange(0.f, float(11 + 4));
	if (ItemType <= 10)
	{
		EArmorType ArmorType = (EArmorType)ItemType;
		GenerateArmorDrop(ArmorType, GeneratedItem, Budget); // TODO: if we drop multiple, split overall budget with each call to this func
	}
	else
	{
		// Generate random Weapon
		GenerateWeaponDrop(GeneratedItem);
	}

}

void AP4MobCharacterBase::GenerateArmorDrop(EArmorType ArmorType, FInventoryItemStruct& GeneratedItem, float Budget)
{
	if (ArmorType != EArmorType::None && RollableArmorDataAsset && RollableArmorItemsDataAsset)
	{
		// TODO: write function to determine item SM, name, etc. (choose random item for ItemClass, name, mesh, weight?, level req?, itemID, ItemIcon.)
		// Other words: roll for item visuals but not it's stats 
		FRolleableArmorItemStruct ArmorItem;
		RollableArmorItemsDataAsset->GetRandomArmorItem(ArmorType, this->StaticClass(), ArmorItem);

		FItemArmorInfoStruct temp = ArmorItem.ArmorDataAsset->ItemInfo;
		temp.ArmorType = ArmorType;
		temp.bIsStackable = false;
		temp.ItemRank = EItemRank::Common;// TODO: roll from weighted tables

		// Generate random stats for gameplayeffect for this item
		FGameplayEffectSpecHandle GEHandle = AbilitySystemComponentHardRef->MakeOutgoingSpec(UEquipItemGameplayEffect::StaticClass(), 1.f, AbilitySystemComponentHardRef->MakeEffectContext());
		TSharedPtr<FGameplayEffectSpec> GE = GEHandle.Data;
		if (GE.IsValid())
		{
			int numAtt = (int)FMath::RandRange(1.f, 4.999f);
			for (int i = 0; i < numAtt; i++)
			{
				// split budget for this attr roll
				float AttBudgetPercent = FMath::RandRange(FMath::Max(.2f, (i * 1.f) / numAtt), FMath::Max(.5f, (i*1.f)/numAtt)); // TODO: these ranges, can make variables for .2 nd .5
				float AttBudget = Budget * AttBudgetPercent;
				Budget -= AttBudget; 

				// Get Random Attribute and info about it
				FRolleableAttributeStruct AttInfoStruct;
				RollableArmorDataAsset->GetRandomAttribute(ArmorType, AttInfoStruct);

				//TODO: mutually exclusive Attributes here (don't see a reason ATM)

				FGameplayEffectModifiedAttribute* AttGE = GE->AddModifiedAttribute(AttInfoStruct.Attribute);
				if (AttGE)
				{
					AttGE->TotalMagnitude = AttBudget * AttInfoStruct.Value;
				}
				 
			}
			temp.EquippedGameplayEffect = GEHandle;
			
		}
		
		GeneratedItem.ItemInfoStruct = temp;
	}
}

void AP4MobCharacterBase::GenerateWeaponDrop(FInventoryItemStruct& GeneratedItem)
{
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


