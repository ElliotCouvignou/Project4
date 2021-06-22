// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "AI/P4AIControllerBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/P4MobCharacterBase.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)


AP4AIControllerBase::AP4AIControllerBase(const class FObjectInitializer& ObjectInitializer)
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

	SetGenericTeamId(FGenericTeamId(1));
}

void AP4AIControllerBase::OnCombatStarted(AProject4Character * Actor)
{
	// assuming This actor is already included in threat array,  this is just a transient helper function for when we start combat

	if (Actor)
	{
		NotifyNearbyFirendsOfNewThreat(Actor);
		
		// deactivate 
		AIPerceptionComponent->Deactivate();
	}
}

void AP4AIControllerBase::OnCombatEnded(AProject4Character* Actor)
{
	// assuming This actor is already included in threat array,  this is just a transient helper function for when we start combat

	if (Actor)
	{
		// deactivate 
		AIPerceptionComponent->Activate();
	}
}


void AP4AIControllerBase::NotifyNearbyFirendsOfNewThreat(AProject4Character * Actor)
{
	// Friends in this context is enemies to player duh
	TArray<TEnumAsByte<EObjectTypeQuery>> Objects;   	Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> Ignores;   							Ignores.Add(GetPawn());

	TArray<AActor*>Out;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetPawn()->GetActorLocation(), CombatNotifyRadius, Objects, AP4MobCharacterBase::StaticClass(), Ignores, Out);

	for (auto e : Out)
	{
		AP4MobCharacterBase* FriendMob = Cast<AP4MobCharacterBase>(e);
		if (FriendMob)
		{
			AP4AIControllerBase* AIC = Cast<AP4AIControllerBase>(FriendMob->GetController());
			if (AIC)
			{
				AIC->MobDamageTaken(Actor, 0.f);
			}
		}
	}
}


ETeamAttitude::Type AP4AIControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other)) {

		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			return Super::GetTeamAttitudeTowards(*OtherPawn->GetController());
		}
	}

	return ETeamAttitude::Neutral;
}

AProject4Character* AP4AIControllerBase::GetHighestThreatTarget()
{
	return (ThreatArray.Num() > 0) ? ThreatArray[0].Target : nullptr;
}

void AP4AIControllerBase::AddUniqueThreatActor(AProject4Character* Actor, float Threat)
{
	// add sorted, if Threat == other threat then set this one at lower priority/higher inde

	// if we made it here then this is lowest or == lowerst so place at end
	for (auto e : ThreatArray)
	{
		if (e.Target == Actor)
			return; // already exists
	}

	ThreatArray.AddUnique(FThreatTarget(Actor, Threat));
	ThreatArray.Sort([](const FThreatTarget& A, const FThreatTarget& B) { return A.Threat < B.Threat; });

	if (ThreatArray.Num() == 1)
		OnCombatStarted(Actor);
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

	// if made it here doesnt exist so add unique
	AddUniqueThreatActor(Actor, ThreatToAdd);
	
}

void AP4AIControllerBase::RemoveActorFromThreatArray(AProject4Character* Actor)
{
	if (!Actor)
		return; 

	for (int i = 0; i < ThreatArray.Num(); i++)
	{
		if (ThreatArray[i].Target == Actor)
		{
			ThreatArray.RemoveAt(i);
			break;
		}
	}

	if (ThreatArray.Num() == 0)
		OnCombatEnded(Actor);
	
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



