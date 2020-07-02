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

	

	// Allows BP's to set themselves as main widget
	// No constructors in controllers so we need to relay the hud back to controller once that is made
	UFUNCTION(BlueprintCallable)
		void SetMainHUDWidget(class UGameplayHudWidget* NewWidget);

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
		void UpdateCurrentXPUI(float NewXP);
	void UpdateCurrentXPUI_Implementation(float NewXP);
	bool UpdateCurrentXPUI_Validate(float NewXP) { return true; }

	UFUNCTION(Client, Reliable, WithValidation)
		void UpdateMaxXPUI(float NewMaxXP);
	void UpdateMaxXPUI_Implementation(float NewMaxXP);
	bool UpdateMaxXPUI_Validate(float NewMaxXP) { return true; }

	// Called on Ding! Gratz
	UFUNCTION(Client, Reliable, WithValidation)
		void UpdateLevelUI(float NewLevel);
	void UpdateLevelUI_Implementation(float NewLevel);
	bool UpdateLevelUI_Validate(float NewLevel) { return true; }

	// Called on Ability Error (e.g. Out of range, Out of mana, etc.)
	//UFUNCTION(Client, Reliable, WithValidation)
	UFUNCTION(BlueprintCallable)
		void SendAbilityErrorUI(EAbilityErrorText ErrorType);




protected:

	// Floating Text Component Class to create
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UFloatingTextWidgetComponent> FloatingTextClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
		class UGameplayHudWidget* GameplayHUDWidget;
};


