// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "P4OpenWorldGameMode.h"



void AP4OpenWorldGameMode::StartPlay()
{
	Super::StartPlay();

	LoadCurrentGameInfo();

	AProject4Controller* P4C = Cast<AProject4Controller>(GetWorld()->GetFirstLocalPlayerFromController());
	if (P4C)
	{
		P4C->CreateMainHUDWidget();
	}
}

void AP4OpenWorldGameMode::PostLogin(APlayerController* NewPlayer)
{

	Super::PostLogin(NewPlayer);

	AProject4Controller* P4C = Cast<AProject4Controller>(NewPlayer);
	if (P4C)
	{
		P4C->CreateMainHUDWidget();
	}
}