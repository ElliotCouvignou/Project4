// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "MainMenuHUD.h"
#include "Project4GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h" 



AMainMenuHUD::AMainMenuHUD()
{
	if (!IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuObj(TEXT("/Game/Project4/UI/MainMenu/BP_MainMenuWidget"));
		MainMenuWidgetClass = MainMenuObj.Class;
	}
}


void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Create MainMenuWidget from HUD"));

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController != nullptr) {
		PlayerController->bShowMouseCursor = true;
	}
	
	
	UProject4GameInstance* GI = Cast<UProject4GameInstance>(GetGameInstance());
	GI->LoadMainMenu();
	//
	//if (MainMenuWidgetClass != nullptr) {
	//	UUserWidget* MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
	//	if (MainMenuWidget != nullptr) {
	//		MainMenuWidget->AddToViewport();
	//		MainMenuWidget->SetFocus();
	//	}
	//}

}
