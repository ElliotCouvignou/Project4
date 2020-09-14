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
			"Niagara",
			"GameplayTags",
			"GameplayAbilities",
			"GameplayTasks",
			"GameLiftServerSDK", 
			"UMG",
			"SlateCore",
			"Http",
			"Json",
			"JsonUtilities",
			"WebBrowserWidget",
			"GameLiftClientSDK" // removing this plugin soon
			}
		);

		/* Gameplay Ability System w/related plugins */
		//PrivateDependencyModuleNames.AddRange(new string[] { 
		//	"GameplayAbilities",
		//	"GameplayTasks"
		//	}
		//);

		//			"GameplayAbilitiesEditor",
		//			"GameplayTagsEditor",

		/* UI related modules */
		PrivateDependencyModuleNames.AddRange(new string[] {
			"Slate",
			"SlateCore",
			"UMG",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks"
			}
		);

	}
}
