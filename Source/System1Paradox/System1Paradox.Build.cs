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
            "AnimGraphRuntime",
            "BlueprintGraph",
            "UMG",           // ← ДОБАВЬТЕ ЭТО! (для UUserWidget)
            "Slate",        // ← ДОБАВЬТЕ ЭТО!
            "SlateCore"     // ← ДОБАВЬТЕ ЭТО!
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(new string[] {
                "UnrealEd",
                "AnimGraph",
                "KismetCompiler"
            });
        }
    }
}