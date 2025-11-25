// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class System1Paradox : ModuleRules
{
	public System1Paradox(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"System1Paradox",
			"System1Paradox/Variant_Horror",
			"System1Paradox/Variant_Horror/UI",
			"System1Paradox/Variant_Shooter",
			"System1Paradox/Variant_Shooter/AI",
			"System1Paradox/Variant_Shooter/UI",
			"System1Paradox/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
