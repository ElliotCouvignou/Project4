using UnrealBuildTool;
using System.IO;

public class Project4Editor : ModuleRules
{
	public Project4Editor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
		   new string[]
		   {
				"Project4Editor/Public"
		   }
	    );

		PrivateIncludePaths.AddRange(
			new string[]
			{
				"Project4Editor/Private"
			}
		);

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"Project4"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"UnrealEd",  "GenericGraphRuntime",  "GenericGraphEditor",  "Project4"
			});

		//if (Target.bBuildEditor)
		//{
		//	PrivateDependencyModuleNames.AddRange(
		//	new string[]
		//	{
		//			"UnrealEd",  "GenericGraphRuntime",  "GenericGraphEditor",  "Project4"
		//	});
		//}

		PrivateIncludePathModuleNames.AddRange(
				new string[]
				{
				});

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			});

		PrivatePCHHeaderFile = "Public/Project4EditorModule.h";
	}
}
