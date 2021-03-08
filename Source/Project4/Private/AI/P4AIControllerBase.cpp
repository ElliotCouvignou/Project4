// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AI/P4AIControllerBase.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)

AProject4Character* AP4AIControllerBase::GetHighestThreatTarget()
{
	return (ThreatArray.Num() > 0) ? ThreatArray[0].Target : nullptr;
}

void AP4AIControllerBase::AddUniqueThreatActor(AProject4Character* Actor, float Threat)
{
	// add sorted, if Threat == other threat then set this one at lower priority/higher inde

	// if we made it here then this is lowest or == lowerst so place at end
	ThreatArray.AddUnique(FThreatTarget(Actor, Threat));
	ThreatArray.Sort([](const FThreatTarget& A, const FThreatTarget& B) { return A.Threat < B.Threat; });
}

void AP4AIControllerBase::AddThreatToThreatArray(AProject4Character* Actor, float ThreatToAdd)
{
	for (int i = 0; i < ThreatArray.Num(); i++)
	{
		if (ThreatArray[i].Target == Actor)
		{
			ThreatArray[i].Threat += ThreatToAdd;
			ThreatArray.Sort([](const FThreatTarget& A, const FThreatTarget& B) { return A.Threat < B.Threat; });
			return;
		}
	}
	
}

void AP4AIControllerBase::RemoveActorFromThreatArray(AProject4Character* Actor)
{
	int foundindex = -1;
	for (int i = 0; i < ThreatArray.Num(); i++)
	{
		if (ThreatArray[i].Target == Actor)
		{
			ThreatArray.RemoveAt(i);
			foundindex = i;
			break;
		}
	}
	if (foundindex >= 0)
	{
		for (int i = foundindex; i < ThreatArray.Num(); i++)
		{
			ThreatArray[foundindex] = ThreatArray[foundindex + 1];
		}
	}
}

void AP4AIControllerBase::PrintThreatArray()
{
	print(FString("Printing Threat Array..."));
	for (int i = 0; i < ThreatArray.Num(); i++)
	{
		print(FString("		ThreatArray[" + FString::FromInt(i) + "] = (Threat: " + \
			FString::SanitizeFloat(ThreatArray[i].Threat) + ",  Actor: " + ThreatArray[i].Target->GetName() + ") "));
	}
}

void AP4AIControllerBase::StopFightingTarget_Implementation(AProject4Character* Actor)
{
	RemoveActorFromThreatArray(Actor);
}

AP4AIControllerBase::AP4AIControllerBase(const class FObjectInitializer& ObjectInitializer)
{


	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
}