// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "P4PlayerGroupObject.generated.h"


class AProject4Character;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewCharacterJoined, AProject4Character*, NewMember);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewCharacterLeft, AProject4Character*, Member);

/**
 * 
 */
UCLASS()
class PROJECT4_API UP4PlayerGroupObject : public UObject
{
	GENERATED_BODY()



public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<AProject4Character*> Players;

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
		void AddCharacterToGroup(AProject4Character* NewMember);
	void AddCharacterToGroup_Implementation(AProject4Character* NewMember);
	bool AddCharacterToGroup_Validate(AProject4Character* NewMember) { return true; }


	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
		void RemoveCharacterFromGroup(AProject4Character* Member);
	void RemoveCharacterFromGroup_Implementation(AProject4Character* NewMember);
	bool RemoveCharacterFromGroup_Validate(AProject4Character* NewMember) { return true; }

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
		void RemoveCharacterIndexFromGroup(int Index);
	void RemoveCharacterIndexFromGroup_Implementation(int Index);
	bool RemoveCharacterIndexFromGroup_Validate(int Index) { return true; }



	UPROPERTY(BlueprintAssignable)
		FOnNewCharacterJoined PlayerJoinedDelegate;

	UPROPERTY(BlueprintAssignable)
		FOnNewCharacterLeft PlayerLeftDelegate;

};
