#include "Project4EditorModule.h"


IMPLEMENT_GAME_MODULE(FProject4EditorModule, Project4Editor);

#define LOCTEXT_NAMESPACE "FProject4EditorModule"

void FProject4EditorModule::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("StartupModule() called"));
}

void FProject4EditorModule::ShutdownModule()
{
	UE_LOG(LogTemp, Warning, TEXT("ShutdownModule() called"));
}