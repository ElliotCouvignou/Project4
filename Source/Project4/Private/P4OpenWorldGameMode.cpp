// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "P4OpenWorldGameMode.h"






void AP4OpenWorldGameMode::GenericPlayerInitialization(AController* Controller)
{
	Super::GenericPlayerInitialization(Controller);

	AProject4Controller* P4C = Cast<AProject4Controller>(Controller);
	if (P4C)
	{
		UE_LOG(LogTemp, Warning, TEXT("\n\n[AP4OpenWorldGameMode::GenericPlayerInitialization]   %s "), *Controller->GetName());
		LoadCharacterForClient(P4C);
		P4C->CreateMainHUDWidget();
	}
}

void AP4OpenWorldGameMode::StartPlay()
{
	Super::StartPlay();

	LoadCharacterForClient(GetWorld()->GetFirstPlayerController());

	AProject4Controller* P4C = Cast<AProject4Controller>(GetWorld()->GetFirstLocalPlayerFromController());
	if (P4C)
	{
		P4C->CreateMainHUDWidget();
	}
}

void AP4OpenWorldGameMode::PostLogin(APlayerController* NewPlayer)
{

	Super::PostLogin(NewPlayer);

	LoadCharacterForClient(NewPlayer);

	AProject4Controller* P4C = Cast<AProject4Controller>(NewPlayer);
	if (P4C)
	{
		P4C->CreateMainHUDWidget();
	}
}