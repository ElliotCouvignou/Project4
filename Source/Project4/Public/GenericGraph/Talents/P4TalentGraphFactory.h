// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "P4TalentGraphFactory.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT4_API UP4TalentGraphFactory : public UFactory
{
	GENERATED_BODY()

public:
	UP4TalentGraphFactory();

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual FText GetDisplayName() const override;
	virtual FString GetDefaultNewAssetName() const override;
};
