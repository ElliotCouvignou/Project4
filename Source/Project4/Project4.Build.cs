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
			"OnlineSubsystem", "﻿OnlineSubsystemSteam﻿"
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
			"Slate", "SlateCore",
			"UMG",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"GenericGraphRuntime"
			}
		);


		// TODO: platform specific online subsystems
        //      determine PC platform, setup steamonlinesubsystem if true
		//if (Target.Platform == UnrealTargetPlatform.Win64 || Target.Platform == UnrealTargetPlatform.Linux || Target.Platform == UnrealTargetPlatform.Mac)
		//{
		//	PublicDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "﻿OnlineSubsystemSteam﻿" });
		//
		//}

		if (Target.bBuildEditor)
		{
			//PublicIncludePaths.AddRange(new string[] { "Project4/Public" });
			//PrivateDependencyModuleNames.AddRange(new string[] { "GenericGraphRuntime" });
			PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "GenericGraphRuntime"});
		}

	}
}
