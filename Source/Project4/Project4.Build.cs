// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project4 : ModuleRules
{
	public Project4(ReadOnlyTargetRules Target) : base(Target)
	{
		bEnableExceptions = true;

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		/* Engine Essentials */
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"GameplayTags",
			"GameplayAbilities",
			"HeadMountedDisplay", // remove eventually (VR headsets)
			"GameLiftServerSDK", 
			"GameLiftClientSDK" 
			}
		);

		/* Gameplay Ability System w/related plugins */
		PrivateDependencyModuleNames.AddRange(new string[] { 
			"GameplayAbilities",
			"GameplayAbilitiesEditor",
			"GameplayTagsEditor",
			"GameplayTasks"
			}
		);

		/* UI related modules */
		PrivateDependencyModuleNames.AddRange(new string[] {
			"Slate",
			"SlateCore",
			"UMG"
			}
		);

	}
}
