// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "Characters/P4MobCharacterBase.h"
#include "Characters/PatrolPointsActor.h"
#include "Interactables/P4InventoryBagComponent.h"
#include "Interactables/P4ItemBaseActor.h"
#include "Interactables/P4ItemArmorObject.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/MobBaseAttributeSet.h"
#include "AbilitySystem/P4AbilitySystemComponent.h"
#include "AbilitySystem/GameplayEffects/EquipItemGameplayEffect.h"
#include "AbilitySystem/AttributeSets/PlayerAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "UI/FloatingStatusBarWidget.h"

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
		RollableAttributesDataAsset = temp.Object;
		RollableAttributesDataAsset->SumTotalWeights();
	}
	auto temp2 = ConstructorHelpers::FObjectFinder<URolleableItemsDataAsset>(TEXT("/Game/Project4/Interactables/Items/Equips/RolleableItemsDataAsset"));
	if (temp2.Object)
	{
		RollableItemsDataAsset = temp2.Object;
		RollableItemsDataAsset->SumTotalWeights();
	}
	//auto temp3 = ConstructorHelpers::FObjectFinder<UEquipItemGameplayEffect>(TEXT("/Game/Project4/Interactables/Items/Equips/GE_GeneratedEquip"));
	//if (temp3.Object)
	//{
	//	EquipItemGETemplate = temp3.Object->GetClass();
	//}
}


void AP4MobCharacterBase::Die()
{
	if (HasAuthority())
	{
		// Server Roll Drop table and spawn rolled items nearby
		if (bGenerateItemDrops)
		{
			TArray<UP4ItemBaseObject*> ItemsToDrop;
			int NumItems = (int)FMath::RandRange(0.f, 2.f);
			
			// Roll for Equip drops
			
			for(int i=0; i< NumItems; i++)
			{
				UP4ItemBaseObject* NewGeneratedItem = nullptr; 
				GenerateEquipItemDrop(&NewGeneratedItem);

				// create FInventoryItemStruct and ItemActor, then spawn
				if (NewGeneratedItem)
				{
					AP4ItemBaseActor* ItemActor = Cast<AP4ItemBaseActor>(GetWorld()->SpawnActorDeferred<AActor>(AP4ItemBaseActor::StaticClass(), GetActorTransform(), this, (APawn*)this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
					if (ItemActor)
					{
						ItemActor->SetInventoryItemObject(&NewGeneratedItem);
						FTransform Transform = GetActorTransform();
						Transform.AddToTranslation(FVector(FMath::RandRange(-100.f, 100.f), FMath::RandRange(-100.f, 100.f), 0.f));
						ItemActor->FinishSpawning(Transform);
					}
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
			// TODO: Update this type of item drops with new UObjects
			//TArray<TTuple<UItemBaseDataAsset*, int>> ItemsToDrop = DropTable.RollItemDrops();
			//
			//for (TTuple<UItemBaseDataAsset*, int> ItemDrop : ItemsToDrop)
			//{
			//	if (ItemDrop.Key && ItemDrop.Value > 0)
			//	{
			//		// create FInventoryItemStruct and ItemActor, then spawn
			//		AP4ItemBaseActor* ItemActor = Cast<AP4ItemBaseActor>(GetWorld()->SpawnActorDeferred<AActor>(AP4ItemBaseActor::StaticClass(), GetActorTransform(), this, (APawn*)this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
			//		if (ItemActor)
			//		{
			//			ItemActor->SetInventoryItemObject(FInventoryItemStruct(ItemDrop.Key, ItemDrop.Value));
			//			FTransform Transform = GetActorTransform();
			//			Transform.AddToTranslation(FVector(FMath::RandRange(-100.f, 100.f), FMath::RandRange(-100.f, 100.f), 0.f));
			//			ItemActor->FinishSpawning(Transform);
			//		}
			//	}
			//}
		}
	}
	
	Super::Die();
}

void AP4MobCharacterBase::GenerateEquipItemDrop(UP4ItemBaseObject** GeneratedItem)
{
	// this is very WIP and randominess can be controlled more later

	float Budget = 100.f; // TODO: figure out current game difficulty to this value 

	// Randomly select item gear type TODO: create structure for weights to allow  wep to drop more often
	// 11 armor types, add 4 for 3x weight of weapons
	
	//                                  ( 1.f,  14.99f)
	int ItemType = (int)FMath::RandRange(1.f, float(10));
	if (ItemType <= 10)
	{
		EArmorType ArmorType = (EArmorType)ItemType;
		UP4ItemArmorObject* temp = NewObject<UP4ItemArmorObject>();
		GenerateArmorDrop(ArmorType, &temp, Budget); // TODO: if we drop multiple, split overall budget with each call to this func
		*GeneratedItem = Cast<UP4ItemBaseObject>(temp);
	}
	else
	{
		// Generate random Weapon
		UP4ItemWeaponObject* temp = NewObject<UP4ItemWeaponObject>();
		GenerateWeaponDrop(&temp, Budget);
		*GeneratedItem = Cast<UP4ItemBaseObject>(temp);
	}

}

void AP4MobCharacterBase::GenerateArmorDrop(EArmorType ArmorType, UP4ItemArmorObject** GeneratedItem, float Budget)
{
	if (*GeneratedItem && ArmorType != EArmorType::None && RollableAttributesDataAsset && RollableItemsDataAsset)
	{
		// TODO: write function to determine item SM, name, etc. (choose random item for ItemClass, name, mesh, weight?, level req?, itemID, ItemIcon.)
		// Other words: roll for item visuals but not it's stats 
		FRolleableArmorItemStruct ArmorItem;
		RollableItemsDataAsset->GetRandomArmorItem(ArmorType, this->StaticClass(), ArmorItem);
		(*GeneratedItem)->ArmorType = ArmorType;
		(*GeneratedItem)->bIsEmpty = false;
		(*GeneratedItem)->bIsStackable = false;
		(*GeneratedItem)->ItemRank = EItemRank::Common;// TODO: roll from weighted tables
		(*GeneratedItem)->ItemName = ArmorItem.ArmorDataAsset->ItemInfo.ItemName;
		(*GeneratedItem)->ItemIcon = ArmorItem.ArmorDataAsset->ItemInfo.ItemIcon;

		// Generate random stats for gameplayeffect for this item
		FGameplayEffectSpecHandle GEHandle = AbilitySystemComponentHardRef->MakeOutgoingSpec(EquipItemGETemplate, 1.f, AbilitySystemComponentHardRef->MakeEffectContext());
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
				RollableAttributesDataAsset->GetRandomArmorAttribute(ArmorType, AttInfoStruct);

				//TODO: mutually exclusive Attributes here (don't see a reason ATM)

				FGameplayEffectModifiedAttribute* AttGE = (GE->GetModifiedAttribute(AttInfoStruct.Attribute)) ? (GE->GetModifiedAttribute(AttInfoStruct.Attribute)) : GE->AddModifiedAttribute(AttInfoStruct.Attribute);
				
				if (AttGE)
				{
					FGameplayTag AttrTag = FGameplayTag::RequestGameplayTag(FName(FString("Data.Attribute." + AttInfoStruct.Attribute.GetName())));
					GE->SetSetByCallerMagnitude(AttrTag, AttBudget * AttInfoStruct.Value);
				}
				 
			}
			(*GeneratedItem)->EquippedGameplayEffect = GEHandle;		
		}
	}
}

void AP4MobCharacterBase::GenerateWeaponDrop(UP4ItemWeaponObject** GeneratedItem, float Budget)
{
	if (*GeneratedItem && RollableAttributesDataAsset && RollableItemsDataAsset)
	{
		// TODO: write function to determine item SM, name, etc. (choose random item for ItemClass, name, mesh, weight?, level req?, itemID, ItemIcon.)
		// Other words: roll for item visuals but not it's stats 

		EWeaponType WeaponType = (EWeaponType)FMath::RandRange(1.f, 11.f);
		FRolleableWeaponItemStruct WeaponItem;
		RollableItemsDataAsset->GetRandomWeaponItem(WeaponType, this->StaticClass(), WeaponItem);
		(*GeneratedItem)->WeaponType = WeaponType;
		(*GeneratedItem)->WeaponHandType = WeaponItem.WeaponDataAsset->WeaponHandType;
		(*GeneratedItem)->WeaponSkeletalMesh = WeaponItem.WeaponDataAsset->WeaponSkeletalMesh;
		(*GeneratedItem)->bIsEmpty = false;
		(*GeneratedItem)->bIsStackable = false;
		(*GeneratedItem)->ItemRank = EItemRank::Common;// TODO: roll from weighted tables
		(*GeneratedItem)->ItemName = WeaponItem.WeaponDataAsset->ItemInfo.ItemName;
		(*GeneratedItem)->ItemIcon = WeaponItem.WeaponDataAsset->ItemInfo.ItemIcon;
		(*GeneratedItem)->AttackInterval = WeaponItem.WeaponDataAsset->AttackInterval * FMath::RandRange(0.9f, 1.1f);
		
		// generate random weapon power and attack interval
		FRolleableWeaponAttributeStructArray fwats = *RollableAttributesDataAsset->WeaponTypeToRolleableAttributes.Find(WeaponType);
		float WeaponPowerValue = *fwats.HandTypeToWeaponPower.Find((*GeneratedItem)->WeaponHandType);
		(*GeneratedItem)->WeaponPower = WeaponPowerValue * Budget / 10.f * (*GeneratedItem)->AttackInterval;// TODO: fix this formula

		// Generate random stats for gameplayeffect for this item
		FGameplayEffectSpecHandle GEHandle = AbilitySystemComponentHardRef->MakeOutgoingSpec(EquipItemGETemplate, 1.f, AbilitySystemComponentHardRef->MakeEffectContext());
		TSharedPtr<FGameplayEffectSpec> GE = GEHandle.Data;
		if (GE.IsValid())
		{
			int numAtt = (int)FMath::RandRange(1.f, 4.999f);
			for (int i = 0; i < numAtt; i++)
			{
				// split budget for this attr roll
				float AttBudgetPercent = FMath::RandRange(FMath::Max(.2f, (i * 1.f) / numAtt), FMath::Max(.5f, (i * 1.f) / numAtt)); // TODO: these ranges, can make variables for .2 nd .5
				float AttBudget = Budget * AttBudgetPercent;
				Budget -= AttBudget;

				// Get Random Attribute and info about it
				FRolleableAttributeStruct AttInfoStruct;
				RollableAttributesDataAsset->GetRandomWeaponAttribute(WeaponType, AttInfoStruct);

				//TODO: mutually exclusive Attributes here (don't see a reason ATM)

				FGameplayEffectModifiedAttribute* AttGE = GE->AddModifiedAttribute(AttInfoStruct.Attribute);
				if (AttGE)
				{
					FGameplayTag AttrTag = FGameplayTag::RequestGameplayTag(FName(FString("Data.Attribute." + AttInfoStruct.Attribute.GetName())));
					GE->SetSetByCallerMagnitude(AttrTag, AttBudget * AttInfoStruct.Value);
				}

			}
			(*GeneratedItem)->EquippedGameplayEffect = GEHandle;
		}
	}
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
		InitFloatingTextWidgetComponent();

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


