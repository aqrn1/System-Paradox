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
            "UMG",           // ДЛЯ UI ВИДЖЕТОВ (GameMode)
            "Slate",
            "SlateCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "UnrealEd",      // ДЛЯ BlueprintManager
            "AssetRegistry",
            "Kismet",
            "BlueprintGraph"
        });
    }
}