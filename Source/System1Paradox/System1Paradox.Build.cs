using UnrealBuildTool;

public class System1Paradox : ModuleRules
{
    public System1Paradox(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // днаюбкъел мсфмше лндскх дкъ юбрнлюрхгюжхх
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "GameplayTags",
            "UnrealEd",           // дкъ педюйрнпю
            "BlueprintGraph",     // дкъ BLUEPRINTS
            "KismetCompiler",     // дкъ йнлохкъжхх BLUEPRINTS
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Slate",
            "SlateCore",
            "EditorFramework",
            "AssetTools"
        });
    }
}