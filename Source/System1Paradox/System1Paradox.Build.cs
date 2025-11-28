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
            "UnrealEd",           // Для FKismetEditorUtilities
            "AssetRegistry",      // Для FAssetRegistryModule
            "Kismet"             // Для работы с блюпринтами
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}