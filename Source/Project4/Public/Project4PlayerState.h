// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "Project4PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API AProject4PlayerState : public APlayerState  //, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AProject4PlayerState();

	// Implement IAbilitySystemInterface
	//class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//
	//UFUNCTION(BlueprintCallable, Category = "PlayerState")
	//	bool IsAlive() const;
	//
	//UFUNCTION(BlueprintCallable, Category = "PlayerState|UI")
	//	void ShowAbilityConfirmCancelText(bool ShowText);
	//
	//UFUNCTION(BlueprintCallable, Category = "PlayerState|Utility")
	//	void SetAbilitySystemComponent(UP4AbilitySystemComponent* NewASC);

protected:

	// Weak Reference to character's ASC
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	//	TWeakObjectPtr<class UP4AbilitySystemComponent> AbilitySystemComponent;
	//
	//UPROPERTY()
	//FGameplayTag DeadTag;
	//
	//// Called when the game starts or when spawned
	//virtual void BeginPlay() override;
};
