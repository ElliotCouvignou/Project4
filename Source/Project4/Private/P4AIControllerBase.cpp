// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "P4AIControllerBase.h"

AP4AIControllerBase::AP4AIControllerBase(const class FObjectInitializer& ObjectInitializer)
{


	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
}