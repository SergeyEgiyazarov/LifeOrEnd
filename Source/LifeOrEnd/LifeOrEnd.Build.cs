// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LifeOrEnd : ModuleRules
{
	public LifeOrEnd(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		PublicIncludePaths.AddRange(new string[]
		{
			"LifeOrEnd/Public/Components",
			"LifeOrEnd/Public/Player",
			"LifeOrEnd/Public/Items",
			"LifeOrEnd/Public/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
