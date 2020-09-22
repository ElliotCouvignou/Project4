// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "TextReaderComponent.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/HAL/PlatformFilemanager.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"


UTextReaderComponent::UTextReaderComponent()
{
	
}


FString UTextReaderComponent::ReadFile(FString FilePath) {
	FString DirectoryPath = FPaths::ProjectContentDir();
	FString FullPath = DirectoryPath + "/" + FilePath;
	FString Result;
	IPlatformFile& File = FPlatformFileManager::Get().GetPlatformFile();

	if (File.FileExists(*FullPath)) {
		FFileHelper::LoadFileToString(Result, *FullPath);
	}

	return Result;
}

