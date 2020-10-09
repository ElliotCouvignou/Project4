// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/SkillTree/SkillTreeDataAsset.h"
#include "GameplayAbilitySpec.h"
#include "SkillTreeComponent.generated.h"


USTRUCT(BlueprintType)
struct FSkillTreeNodeStruct
{
	GENERATED_USTRUCT_BODY()

	/* Index to data asset skill tree node array */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data Asset Node Index")
		int DataAssetNodeIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Rank")
		int CurrentRank;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability Spec Handle")
		FGameplayAbilitySpecHandle SpecHandle;

	FSkillTreeNodeStruct()
	{
		DataAssetNodeIndex = -1;
		CurrentRank = 0;
	}

	FSkillTreeNodeStruct(FSkillTreeNodeDataAssetStruct& DataAssetStruct, int Index)
	{
		DataAssetNodeIndex = Index;
		CurrentRank = 0;
	}
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillTreeNodeUpdate, const TArray<int>&, ChangedArrayIndexes);

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillTreePointsUpdate, int, NewPoints);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT4_API USkillTreeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillTreeComponent();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "RPC | Server")
		void ServerTryRankUpSkill(int SkillTreeIndex, bool IsMainTree);
	void ServerTryRankUpSkill_Implementation(int SkillTreeIndex, bool IsMainTree);
	bool ServerTryRankUpSkill_Validate(int SkillTreeIndex, bool IsMainTree) { return true; }

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "RPC | Server")
		void ServerTryRankDownSkill(int SkillTreeIndex, bool IsMainTree);
	void ServerTryRankDownSkill_Implementation(int SkillTreeIndex, bool IsMainTree);
	bool ServerTryRankDownSkill_Validate(int SkillTreeIndex, bool IsMainTree) { return true; }


	UFUNCTION(BlueprintCallable, Client, Reliable, WithValidation, Category = "RPC | Client")
		void ClientSkillTreeNodeUpdateDelegate(int Index, bool IsMainTree);
	void ClientSkillTreeNodeUpdateDelegate_Implementation(int Index, bool IsMainTree);
	bool ClientSkillTreeNodeUpdateDelegate_Validate(int Index, bool IsMainTree) { return true; }
	

	/* Delegates, currently bound for UI updates */
	UPROPERTY(BlueprintAssignable, Category = "Delegates | MainTree")
		FOnSkillTreeNodeUpdate OnMainTreeNodeUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Delegates | Secondary Tree")
		FOnSkillTreeNodeUpdate OnSecondaryTreeNodeUpdated;

	//UPROPERTY(BlueprintAssignable, Category = "Delegates | MainTree")
	//	FOnSkillTreePointsUpdate OnMainTreePointsUpdated;
	//
	//UPROPERTY(BlueprintAssignable, Category = "Delegates | Secondary Tree")
	//	FOnSkillTreePointsUpdate OnSecondaryTreePointsUpdated;

	UFUNCTION(BlueprintCallable, Category = "Utility")
		void GetAssetNodeStruct(const int NodeIndex, const bool IsMainTree, FSkillTreeNodeDataAssetStruct& DataAssetNodeStruct);

	UFUNCTION(BlueprintCallable, Category = "Utility")
		TArray<class USkillTreeDataAsset*>& GetUseableSkillTrees() { return UseableSkillTrees; }

protected:

	/* Inits skill tree array from data asset, all ranks will be 0 
		TODO: add read to SQL server or local saves */
	UFUNCTION()
		void InitSkillTreeFromDataAsset();

	/* Skill Points to spend in tree */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Skill Tree | Points")
		int	MainSkillTreePoints;

	/* Skill Points to spend in tree */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Skill Tree | Points")
		int	SecondarySkillTreePoints;

	/* Used by client and populated by server to store queued changes into dynamic skill tree node array */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Skill Tree | Main Tree")
		TArray<int> ChangedMainSkillTreeNodes;

	/* Used by client and populated by server to store queued changes into dynamic skill tree node array */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Skill Tree | Secondary Tree")
		TArray<int> ChangedSecondarySkillTreeNodes;

	// copy of data asset array but with new struct to hold current rank
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, ReplicatedUsing=OnRep_MainSkillTree, Category = "Skill Tree | Main Tree")
		TArray<FSkillTreeNodeStruct> MainSkillTree;

	// copy of data asset array but with new struct to hold current rank
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, ReplicatedUsing=OnRep_SecondarySkillTree, Category = "Skill Tree | Secondary Tree")
		TArray<FSkillTreeNodeStruct> SecondarySkillTree;

	/* Skill Tree based off data asset but adds on values relevant for this player only e.g Current rank */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill Tree | Data Asset")
		class USkillTreeDataAsset* MainSkillTreeDataAsset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill Tree | Data Asset")
		class USkillTreeDataAsset* SecondarySkillTreeDataAsset;

	/* Current selected main Tree index into useable skill trees */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Skill Tree | Active Tree Index")
		int ActiveMainSkillTreeIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Skill Tree | Active Tree Index")
		int ActiveSecondarySkillTreeIndex;

	/* All useable skill trees to interchange from (Read by UI) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill Tree Node | Data Asset", Meta = (ExposeOnSpawn = true))
		TArray<class USkillTreeDataAsset*> UseableSkillTrees;

	UFUNCTION(Category = "Utility")
		void ClearChangedSkillTreeNodes();

	UFUNCTION()
		void OnRep_MainSkillTree();

	UFUNCTION()
		void OnRep_SecondarySkillTree();

	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
