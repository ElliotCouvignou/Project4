// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Project4Character.h"
#include "../UI/GameplayHudWidget.h"
#include "Project4Controller.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API AProject4Controller : public APlayerController
{
	GENERATED_BODY()
	
public:
	


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

	// Allows BP's to set themselves as main widget
	UFUNCTION(BlueprintCallable)
		void SetMainHUDWidget(UGameplayHudWidget* NewWidget);

protected:
	// Floating Text Component Class to create
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UFloatingTextWidgetComponent> FloatingTextClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
		class UGameplayHudWidget* GameplayHUDWidget;
};
