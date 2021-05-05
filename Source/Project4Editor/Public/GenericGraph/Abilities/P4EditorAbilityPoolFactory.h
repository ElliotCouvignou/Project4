// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "P4EditorAbilityPoolFactory.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4EDITOR_API UP4EditorAbilityPoolFactory : public UFactory
{
	GENERATED_BODY()
	

public:
	UP4EditorAbilityPoolFactory();

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual FText GetDisplayName() const override;
	virtual FString GetDefaultNewAssetName() const override;

};
