// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Project4.h"

#include "P4CharacterInfoDataAsset.generated.h"




USTRUCT(BlueprintType)
struct FCharacterInfoStruct
{
	GENERATED_USTRUCT_BODY()

	/* Character Skeletal Mesh */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essential")
		USkeletalMesh* CharacterSkeletalMesh;

	/* Animation Blueprint */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essential")
		TSubclassOf<UAnimInstance> AnimationBlueprint;

	/* Base attribute set of this character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GAS)
		class UDataTable* ChracterBaseAttributes;


	// TODO: temp color Remove this when we have diff meshes 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essential")
		FLinearColor TEMPColor;

	FCharacterInfoStruct()
	{

	}
};
/**
 * 
 */
UCLASS()
class PROJECT4_API UP4CharacterInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essential")
		TMap<EClassAbilityPoolType, FCharacterInfoStruct> PoolTypeToInfoMap;
};
