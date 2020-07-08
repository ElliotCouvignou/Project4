// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Project4Controller.generated.h"


// location of this struct might need to be moved if necessary
// ONLY CREATE A NEW ENUM IF: you want to bind an enum to a NEW inputAction event DisplayName
UENUM(BlueprintType)
enum class EAbilityErrorText : uint8
{
	OutOfRange			UMETA(DisplayName = "OutOfRange"),
	OutOfMana			UMETA(DisplayName = "OutOfMana"),
	NoTarget			UMETA(DisplayName = "NoTarget"),
	InvalidTarget       UMETA(DisplayName = "InvalidTarget"),
	OnCooldown			UMETA(DisplayName = "OnCooldown")
};

/**
 * 
 */
UCLASS()
class PROJECT4_API AProject4Controller : public APlayerController
{
	GENERATED_BODY()

	/***************************/
	/*       Components        */
	/***************************/


public:
	AProject4Controller(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// creates a widget based on gameplayHUDwidgetclass
		void CreateMainHUDWidget();

	UFUNCTION(BlueprintCallable, Category = "Utility")
		class UGameplayHudWidget* GetMainHUDWidget();

	/*******************************/
	/* Client RPC/Functions for UI */
	/*******************************/

	// Server calls this fucntion to display damage numbers to a player
	UFUNCTION(Client, Reliable, WithValidation)
		void DisplayDamageNumber(float DamageValue, AProject4Character* TargetCharacter);
	void DisplayDamageNumber_Implementation(float DamageValue, AProject4Character* TargetCharacter);
	bool DisplayDamageNumber_Validate(float DamageValue, AProject4Character* TargetCharacter) { return true; }

	// Server calls this fucntion to display damage numbers to a player
	UFUNCTION(Client, Reliable, WithValidation)
		void DisplayHealNumber(float HealValue, AProject4Character* TargetCharacter);
	void DisplayHealNumber_Implementation(float HealValue, AProject4Character* TargetCharacter);
	bool DisplayHealNumber_Validate(float HealValue, AProject4Character* TargetCharacter) { return true; }

	UFUNCTION(Client, Reliable, WithValidation)
		void UpdateUICurrentXP(float NewXP);
	void UpdateUICurrentXP_Implementation(float NewXP);
	bool UpdateUICurrentXP_Validate(float NewXP) { return true; }

	UFUNCTION(Client, Reliable, WithValidation)
		void UpdateUIMaxXP(float NewMaxXP);
	void UpdateUIMaxXP_Implementation(float NewMaxXP);
	bool UpdateUIMaxXP_Validate(float NewMaxXP) { return true; }

	// Called on Ding! Gratz
	UFUNCTION(Client, Reliable, WithValidation)
		void UpdateUILevel(float NewLevel);
	void UpdateUILevel_Implementation(float NewLevel);
	bool UpdateUILevel_Validate(float NewLevel) { return true; }


	// Called on Ability Error (e.g. Out of range, Out of mana, etc.)
	//UFUNCTION(Client, Reliable, WithValidation)
	UFUNCTION(BlueprintCallable)
		void SendUIAbilityError(EAbilityErrorText ErrorType);




protected:

	// Floating Text Component Class to create for dmg text
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UFloatingTextWidgetComponent> FloatingTextClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
		class UGameplayHudWidget* GameplayHUDWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
		TSubclassOf<class UGameplayHudWidget> GameplayHUDWidgetClass;

	// Server only
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;
};


