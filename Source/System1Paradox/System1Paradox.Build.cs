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
            "EnhancedInput"
            // Убрали лишние зависимости пока не понадобятся
        });

        // Убрали PrivateDependencyModuleNames - пока не нужны
        // Убрали PublicIncludePaths - ссылались на удалённые папки

        // Раскомментируйте по мере необходимости:
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");
    }
}