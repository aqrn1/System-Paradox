using UnrealBuildTool;

public class System1Paradox : ModuleRules
{
    public System1Paradox(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // ВАЖНО: Раскомментируйте эти строки если они закомментированы
        PublicIncludePaths.AddRange(new string[] { "System1Paradox/Public" });
        PrivateIncludePaths.AddRange(new string[] { "System1Paradox/Private" });

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "AnimGraphRuntime",
            "BlueprintGraph",
            "UMG",           // Для виджетов
            "Slate",         // Для UI
            "SlateCore",     // Для UI
            "GameplayTasks", // Для задач
            "AIModule",      // Для AI
            "NavigationSystem" // Для навигации
        });

        PrivateDependencyModuleNames.AddRange(new string[] { 
            // Добавьте сюда модули только для вашего кода
        });

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(new string[] {
                "UnrealEd",
                "AnimGraph",
                "KismetCompiler",
                "Kismet",
                "KismetWidgets"
            });
        }
    }
}