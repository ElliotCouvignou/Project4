// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProceduralDungeons : ModuleRules
{
	public ProceduralDungeons(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"PrefabricatorRuntime",
				"ConstructionSystemRuntime",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"PrefabricatorRuntime",
				"ConstructionSystemRuntime",

				// ... add private dependencies that you statically link with here ...	
			}
			);

		//if (Target.bBuildEditor)
		//{
		//	PrivateDependencyModuleNames.AddRange(new string[] { "PrefabricatorEditor", "PrefabricatorEditorPostInit", "ConstructionSystemEditor"});
		//}


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
