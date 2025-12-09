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
            "InputCore"
            // НИЧЕГО БОЛЬШЕ НЕ ДОБАВЛЯЙ!
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            // ПУСТО! НИЧЕГО НЕ НУЖНО!
        });
    }
}