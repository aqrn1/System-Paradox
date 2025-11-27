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
            "GameplayTags",
            "UnrealEd",           // Для редакторских функций
            "BlueprintGraph",     // Для работы с блюпринтами
            "KismetCompiler",     // Для компиляции блюпринтов
            "Kismet",            // Для KismetEditorUtilities
            "AssetTools"         // Для создания ассетов
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}