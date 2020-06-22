// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "P4AbilitySystemComponent.h"

void UP4AbilitySystemComponent::RecieveDamage(UP4AbilitySystemComponent* SourceASC, float RawDamage, float TotalDamage)
{
	RecievedDamage.Broadcast(SourceASC, RawDamage, TotalDamage);
}
