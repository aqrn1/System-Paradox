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
            "UnrealEd",
            "AssetRegistry",
            "Kismet",
            "AnimGraphRuntime" // <-- Убедитесь, что перед этой строкой есть запятая
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}