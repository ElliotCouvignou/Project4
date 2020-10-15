// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillTreeDataAsset.generated.h"



/* each enum represents type of branch (straight, turn, etc.) */
UENUM(BlueprintType)
enum class ESkillTreeBranchType : uint8
{
	Vertical			UMETA(DisplayName = "Vertical"),
	Horizontal		UMETA(DisplayName = "Horizontal"),
	LTurnVtoR		UMETA(DisplayName = "LTurnVtoR"),
	LTurnRtoV		UMETA(DisplayName = "LTurnRtoV"),
	LTurnLtoV		UMETA(DisplayName = "LTurnLtoV"),
	LTurnVtoL		UMETA(DisplayName = "LTurnVtoL")
};

USTRUCT(BlueprintType)
struct FSkillTreeBranchDataAssetStruct
{

	GENERATED_USTRUCT_BODY()

	///* Index into SkillTreeNodes array of parent to this branch, node branch comes from */
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Branch | UI | Parents")
	//	int ParentNodeIndex;
	//
	///* index of SkillTreeBranches array conencting branch towards parent */
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Branch | UI | Parents")
	//	int ParentBranchIndex;
	//
	///* Index into SkillTreeNodes array of parent to this branch, node branch goes to*/
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Branch | UI | Children")
	//	int ChildNodeIndex;
	//
	///* index of SkillTreeBranches array  connecting branch towards child */
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Branch | UI | Children")
	//	int ChildBranchIndex;

	/* visual representation of branch (straight, horizontal, curved to direction) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Branch | UI | Type")
		ESkillTreeBranchType BranchType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Branch | UI | Grid Position")
		int GridRow;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Branch | UI | Grid Position")
		int GridCol;

	FSkillTreeBranchDataAssetStruct()
	{
		//ParentNodeIndex = -1;
		//ParentBranchIndex = -1;
		//ChildNodeIndex = -1;
		//ChildBranchIndex = -1;

		BranchType = ESkillTreeBranchType::Vertical;
		GridRow = 0;
		GridCol = 0;
	}

};

USTRUCT(BlueprintType)
struct FSkillTreeNodeDataAssetStruct
{

	GENERATED_USTRUCT_BODY()

		/* Empty array is interpreted as parent */
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parents")
		TArray<int>  Parents;

	/* Index of skilltree data asset array of children
		Empty array is interpreted as leaf */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Children")
		TArray<int> Children;

	// TODO: Change this to talent tree ability class 
	/* Ability Class this node will grant and rank up when interacted with */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node | Ability")
		TSubclassOf<class UP4GameplayAbility> AbilityClass;

	/* If True, then all parent's nodes must be learned for this to be learned,
		else only one parent is needed to be filled*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node | Parents")
		bool RequiresAllParents;

	/* Branches from this node to connecting children */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node | Branches")
		TArray<FSkillTreeBranchDataAssetStruct> SkillTreeBranches;

	/* Max Rank */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node | Rank")
		int MaxRank;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node | UI | Grid Position")
		int GridRow;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node | UI | Grid Position")
		int GridCol;

	FSkillTreeNodeDataAssetStruct()
	{
		AbilityClass = nullptr;
		RequiresAllParents = true;
		MaxRank = 1;
	}

	FSkillTreeNodeDataAssetStruct(const FSkillTreeNodeDataAssetStruct& other)
	{
		Parents = other.Parents;
		Children = other.Children;
		AbilityClass = other.AbilityClass;
		RequiresAllParents = other.RequiresAllParents;
		MaxRank = other.MaxRank;
	}
};

/**
 * Data Asset to handle representation of skill trees for server verificaiton 
 */
UCLASS(BlueprintType)
class PROJECT4_API USkillTreeDataAsset : public UDataAsset
{
	GENERATED_BODY()
	

public:
	USkillTreeDataAsset();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill Tree Nodes")
		TArray<FSkillTreeNodeDataAssetStruct> SkillTreeNodes;

	/* Name to be displayed in UI to Players */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Name")
		FString SkillTreeName;

};



