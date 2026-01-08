// System1Paradox.cpp - ИСПРАВЛЕННЫЙ
#include "System1Paradox.h"
#include "HAL/IConsoleManager.h"

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

static FAutoConsoleCommand TestCmd(
    TEXT("sys.Test"),
    TEXT("Тестовая команда System1Paradox"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&TestCommand)
);

static FAutoConsoleCommand HealthCmd(
    TEXT("sys.Health"),
    TEXT("Проверка состояния System1Paradox"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&HealthCheck)
);

IMPLEMENT_PRIMARY_GAME_MODULE(
    FSystem1ParadoxModule,
    System1Paradox,
    "System1Paradox"
);

void FSystem1ParadoxModule::StartupModule()
{
    UE_LOG(LogTemp, Log, TEXT("=== SYSTEM1PARADOX MODULE STARTED ==="));
}

void FSystem1ParadoxModule::ShutdownModule()
{
    UE_LOG(LogTemp, Log, TEXT("=== SYSTEM1PARADOX MODULE SHUTDOWN ==="));
}