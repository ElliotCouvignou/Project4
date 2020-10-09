// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillTreeDataAsset.generated.h"

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI Widget")
		TSubclassOf<class USkillTreeWidget> SkillTreeWidget;
};



