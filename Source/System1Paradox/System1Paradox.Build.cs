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
            "UMG",           // Для виджетов
            "Slate",         // Для UI
            "SlateCore"      // Для UI
        });

        PrivateDependencyModuleNames.AddRange(new string[] { 
            // Пока пусто
        });

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