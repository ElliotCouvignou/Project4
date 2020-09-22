// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

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
			}
		);

		// some forum post said this might help with some warnings...
		//PublicIncludePaths.Add(Path.Combine("../Plugins/GameLiftServerSDK/Source/GameLiftServerSDK", "Public"));
		//PrivateIncludePaths.Add(Path.Combine("../Plugins/GameLiftServerSDK/Source/GameLiftServerSDK", "Private"));
		
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
