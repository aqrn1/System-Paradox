#include "System1Paradox.h"
#include "Modules/ModuleManager.h"
#include "BlueprintManager.h"  // ← ЭТА СТРОКА ДОЛЖНА БЫТЬ
#include "Engine/Engine.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FSystem1ParadoxModule, System1Paradox, "System1Paradox");

void FSystem1ParadoxModule::StartupModule()
{
    UE_LOG(LogTemp, Warning, TEXT("=== SYSTEM1PARADOX MODULE STARTED ==="));

    // АВТОМАТИЧЕСКИ ЗАПУСКАЕМ СОЗДАНИЕ BLUEPRINTS ПРИ ЗАГРУЗКЕ РЕДАКТОРА
    if (GIsEditor && !IsRunningCommandlet())
    {
        UE_LOG(LogTemp, Warning, TEXT("Запускаем автоматическое создание Blueprints..."));

        // Ждем 3 секунды чтобы редактор полностью загрузился
        FTimerHandle TimerHandle;
        GEngine->GetTimerManager().SetTimer(TimerHandle, []()
            {
                // СОЗДАЕМ МЕНЕДЖЕР И ЗАПУСКАЕМ АВТОМАТИЗАЦИЮ
                UBlueprintManager* BlueprintManager = NewObject<UBlueprintManager>();
                if (BlueprintManager)
                {
                    BlueprintManager->CreateAndSetupAllBlueprints();
                    UE_LOG(LogTemp, Warning, TEXT("✅ Автоматическое создание Blueprints завершено!"));
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("❌ Не удалось создать BlueprintManager"));
                }
            }, 3.0f, false);
    }
}

void FSystem1ParadoxModule::ShutdownModule()
{
    UE_LOG(LogTemp, Warning, TEXT("System1Paradox Module Shutdown!"));
}