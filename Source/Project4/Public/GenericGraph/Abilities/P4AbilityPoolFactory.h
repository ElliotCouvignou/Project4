// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "P4AbilityPoolFactory.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UP4AbilityPoolFactory : public UFactory
{
	GENERATED_BODY()
	

public:
	UP4AbilityPoolFactory();

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual FText GetDisplayName() const override;
	virtual FString GetDefaultNewAssetName() const override;

};
