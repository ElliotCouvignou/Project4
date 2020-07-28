// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"

UENUM(BlueprintType)
enum class EDistanceMeterType : uint8
{
	None				 UMETA(DisplayName = "None"),
	DistanceToTarget     UMETA(DisplayName = "DistanceToTarget"),
	DistanceFromCrosshair UMETA(DisplayName = "DistanceFromCrosshair")
};

/**
 * 
 */
UCLASS()
class PROJECT4_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:

	/* BlueprintImplementableEvents */

	/* calls private bp fucntion */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void TryFindCrosshairTarget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetDistanceText(float Distance);


	/*   Public Varaibles */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target")
		bool bFoundTarget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target")
		class AProject4Character* LastHoveredTarget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essential")
		class AProject4Character* PlayerRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essential")
		FSlateColor BaseTint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essential")
		FSlateColor EnemyHoveredTint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Essential")
		FSlateColor AllyHoveredTint;

	UPROPERTY(BLueprintReadWrite, EditAnywhere, Category = "DistanceMeter")
		EDistanceMeterType DistanceMeterType = EDistanceMeterType::None;


private:

	

	

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
