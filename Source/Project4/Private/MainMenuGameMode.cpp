// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "MainMenuGameMode.h"
#include "UI/MainMenuWidget.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)

AMainMenuGameMode::AMainMenuGameMode()
{
	HUDClass = UMainMenuWidget::StaticClass();
}

void AMainMenuGameMode::BeginPlay()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController != nullptr) {
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
		print(FString("Show Mouse Cursor"));
	}
}
