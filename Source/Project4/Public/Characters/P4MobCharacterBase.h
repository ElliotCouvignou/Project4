// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Characters/Project4Character.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Containers/Map.h"
#include "Interactables/RolleableItemsDataAsset.h"
#include "P4MobCharacterBase.generated.h"

class UItemBaseDataAsset;

/* struct represents a dependent pool item to roll against other of this same struct */
USTRUCT(BlueprintType)
struct FDependentItemDrop
{
	GENERATED_USTRUCT_BODY()

	/* Item Data asset of item to drop */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drop | Item")
		UItemBaseDataAsset* Item;

	/* # points this item should have for the dependent rolls, higher value the more likely this item will roll */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drop | PointCount")
		int PointCount;

	/* For non-stackable items keep this at 1, else roll stackcount in range */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drop | StackCount")
		int StackCountMin;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drop | StackCount")
		int StackCountMax;

	FDependentItemDrop()
	{
		Item = nullptr;
		PointCount = 0;
		StackCountMin = 1;
		StackCountMax = 1;
	}
};

/* struct represents an entire dependent pool
	NOTE: UE4 can't handle nexted TArrays but can do Tarray of struct with Tarray as a workaround */
USTRUCT(BlueprintType)
struct FDependentItemDropPool
{
	GENERATED_USTRUCT_BODY()

	/* Item Data asset of item to drop */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drop | Item")
		TArray<FDependentItemDrop> DependentDropPool;

	// helper function to roll item in this pool
	TTuple<UItemBaseDataAsset*, int> RollDependentDropPool();

	FDependentItemDropPool()
	{

	}
};


/* Indepentent item to drop, chances this item drops is only related to chance on not on result of other drops */
USTRUCT(BlueprintType)
struct FIndependentItemDrop
{
	GENERATED_USTRUCT_BODY()

	/* Item Data asset of item to drop */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drop | Item")
		UItemBaseDataAsset* Item;

	/* Keep in range 0-1 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drop | Chance")
		float Chance;

	/* For non-stackable items keep this at 1, else roll stackcount in range */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drop | StackCount")
		int StackCountMin;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drop | StackCount")
		int StackCountMax;

	TTuple<UItemBaseDataAsset*, int> RollIndependentItem()
	{
		return (FMath::RandRange(0.f, 1.f) < Chance) ? TTuple<UItemBaseDataAsset*, int>(Item, FMath::RandRange(0.f, 1.f) * (StackCountMax - StackCountMin) + StackCountMin) : TTuple<UItemBaseDataAsset*, int>(nullptr, 0);
	}

	FIndependentItemDrop()
	{
		Item = nullptr;
		Chance = 0.f;
		StackCountMin = 1;
		StackCountMax = 1;
	}
};

USTRUCT(BlueprintType)
struct FDropTableStruct
{
	GENERATED_USTRUCT_BODY()

	/* Each item in array is given own chance to roll and are independent from each other.
		This means each item in this array has no influence on how the other drops. Therefore
		Ordering doesn't matter for this array */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drops | Independent")
		TArray<FIndependentItemDrop> IndependentDrops;

	/*  Array for multiple pools of dependent drops, Each pool of drops will compete on one roll to be dropped.
		This means no two items from the same dependent pool can drop. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Drops | Dependent")
		TArray<FDependentItemDropPool> DependentDrops;

	// Rolls item drops and returns all dropped item data assets in an array
	TArray<TTuple<UItemBaseDataAsset*,int>> RollItemDrops();

	FDropTableStruct()
	{

	}
};
 


class UP4GameplayAbility;

/**
 * 
 */
UCLASS()
class PROJECT4_API AP4MobCharacterBase : public AProject4Character
{
	GENERATED_BODY()
	
public:
	AP4MobCharacterBase(const class FObjectInitializer& ObjectInitializer);
	
	

	/* Helper and general utility function to determine if actor is enemy/friendly with this caracter */
	virtual bool IsEnemiesWith(AProject4Character* Other) override;

	virtual bool IsAlliesWith(AProject4Character* Other) override;

	/* Use patrol point actor if you want to visually set markers, you can still fill this out
	   Yourself if needed*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI | Patrol ")
		class APatrolPointsActor* PatrolPointsActor;
	/* filled out if there is a PatrolPointActor */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI | Patrol ", Meta = (MakeEditWidget = true))
		TArray<FVector> PatrolPoints;

	/* Used in AI behavior tree to tell if we are in range to attack, this represents range for both melee and ranged */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI | Stats ")
		float AttackRange;

	/* Used in AI behavior tree for abilities that this mob can cast */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI | Abilities ")
		TArray<TSubclassOf<UP4GameplayAbility>> Abilities;

	/* Map of injection tags to behavior tree (i.e what combat behavior tree or out of combat behavior tree to use ....) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI | Behavior ")
		TMap<FGameplayTag, class UBehaviorTree*> BehaviorTreeMap;

	UFUNCTION(BlueprintCallable)
		FGameplayTagContainer& GetCooldownContainer() { return CooldownContainer;  }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI | Abilities ")
		void GetNextAbility(TSubclassOf<UP4GameplayAbility>& Result);

	virtual void GetNextAbility_Implementation(TSubclassOf<UP4GameplayAbility>& Result);

	/* override to roll droptable and drop */
	virtual void Die() override;

protected:

	/* container to hold all the spell cooldowns that this mob casts. */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AI | Cooldowns ")
		FGameplayTagContainer CooldownContainer;

	// Hard refs of ASC and AS, unique names to not conflict with base class
	UPROPERTY()
	class UAbilitySystemComponent* AbilitySystemComponentHardRef;

	UPROPERTY()
	class UMobBaseAttributeSet* AttributeSetHardRef;

	// Init playerAttributes with .csv
	virtual void InitializeAttributeSet() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Drops | Drop Table")
		FDropTableStruct DropTable;

	/* In seconds, the delay from death to drop items (why not have the choice to set a delay) */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Drops | Drop Delay")
		float DropDelay = 0.f;

	/* if true then procedurally generate item drops */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Drops | Drop Delay")
		bool bGenerateItemDrops = true;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Drops | Procedural Generation")
		URolleableAttributesDataAsset* RollableAttributesDataAsset;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Drops | Procedural Generation")
		URolleableItemsDataAsset* RollableItemsDataAsset;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Drops | Procedural Generation")
		TSubclassOf<class UEquipItemGameplayEffect> EquipItemGETemplate;

	/* Delegates */
	UFUNCTION(BlueprintCallable)
		void BindDelegates();

	/* Maps injection tags to this mob's class default BehaviorTreeMap variable */
	UFUNCTION()
		void SetDynamicBehaviorSubtrees();

	/* procedurally generate item drops, this can be moved elsewhere
		ONLY CALL THIS ON SERVER */
	void GenerateEquipItemDrop(UP4ItemBaseObject** GeneratedItem);

	/* helper function of GenerateEquipItemDrop */
	void GenerateArmorDrop(EArmorType ArmorType, class UP4ItemArmorObject** GeneratedItem, float Budget);

	/* helper function of GenerateEquipItemDrop */
	void GenerateWeaponDrop(UP4ItemWeaponObject** GeneratedItem, float Budget);

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle ManaChangedDelegateHandle;

	/* Delegate Handlers */
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void ManaChanged(const FOnAttributeChangeData& Data);

	virtual void OnStunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	/*  Virtual Overrides  */
	virtual void FinishDying() override;

	virtual void BeginPlay() override;

};
