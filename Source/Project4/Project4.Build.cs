// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project4 : ModuleRules
{
	public Project4(ReadOnlyTargetRules Target) : base(Target)
	{
		bEnableExceptions = true;

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "GameLiftServerSDK", "GameLiftClientSDK" });
	}
}
