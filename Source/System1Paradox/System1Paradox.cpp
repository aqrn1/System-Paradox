// System1Paradox.cpp - ИСПРАВЛЕННЫЙ
#include "System1Paradox.h"
#include "Modules/ModuleManager.h"
#include "HAL/IConsoleManager.h"
#include "Engine/World.h"

// УБРАТЬ ВСЕ ССЫЛКИ НА BlueprintManager!
// static UBlueprintManager* BlueprintManager = nullptr; // КОММЕНТИРУЕМ

static void TestCommand(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("🎯 ТЕСТОВАЯ КОМАНДА РАБОТАЕТ!"));
}

static void HealthCheck(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("❤️ HEALTH CHECK: SYSTEM1PARADOX"));
    UE_LOG(LogTemp, Warning, TEXT("✅ Модуль загружен"));
}

static FAutoConsoleCommand TestCmd(TEXT("sys.Test"), TEXT("Тестовая команда"), FConsoleCommandWithArgsDelegate::CreateStatic(&TestCommand));
static FAutoConsoleCommand HealthCmd(TEXT("sys.Health"), TEXT("Проверка системы"), FConsoleCommandWithArgsDelegate::CreateStatic(&HealthCheck));

IMPLEMENT_PRIMARY_GAME_MODULE(FSystem1ParadoxModule, System1Paradox, "System1Paradox");

void FSystem1ParadoxModule::StartupModule()
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🚀 SYSTEM1PARADOX MODULE STARTED ==="));

    // УБРАТЬ ЭТО - BlueprintManager создается в другом месте
    // BlueprintManager = NewObject<UBlueprintManager>();
    // BlueprintManager->CreateAllBlueprints();
}

void FSystem1ParadoxModule::ShutdownModule()
{
    UE_LOG(LogTemp, Warning, TEXT("System1Paradox Module Shutdown"));

    // УБРАТЬ ЭТО
    // if (BlueprintManager)
    // {
    //     BlueprintManager->ConditionalBeginDestroy();
    //     BlueprintManager = nullptr;
    // }
}