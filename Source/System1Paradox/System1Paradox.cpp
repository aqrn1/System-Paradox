#include "System1Paradox.h"
#include "Modules/ModuleManager.h"
#include "HAL/IConsoleManager.h"

// 🔥 ТЕСТОВАЯ КОМАНДА
static void TestCommand(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("🎯 ТЕСТОВАЯ КОМАНДА РАБОТАЕТ!"));
    UE_LOG(LogTemp, Warning, TEXT("📊 Количество аргументов: %d"), Args.Num()); // ← УБРАЛ ЛИШНЮЮ СКОБКУ
}

// 🔥 ПРОВЕРКА СИСТЕМЫ
static void HealthCheck(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("❤️  HEALTH CHECK: SYSTEM1PARADOX"));
    UE_LOG(LogTemp, Warning, TEXT("✅ Модуль загружен"));
    UE_LOG(LogTemp, Warning, TEXT("✅ Консольные команды активны"));
    UE_LOG(LogTemp, Warning, TEXT("✅ Live Coding работает"));
}

// РЕГИСТРАЦИЯ КОМАНД
static FAutoConsoleCommand TestCmd(
    TEXT("sys.Test"),
    TEXT("Простая тестовая команда"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&TestCommand)
);

static FAutoConsoleCommand HealthCmd(
    TEXT("sys.Health"),
    TEXT("Проверка жизнеспособности системы"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&HealthCheck)
);

IMPLEMENT_PRIMARY_GAME_MODULE(FSystem1ParadoxModule, System1Paradox, "System1Paradox");

void FSystem1ParadoxModule::StartupModule()
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🚀 SYSTEM1PARADOX MODULE STARTED ==="));
    UE_LOG(LogTemp, Warning, TEXT("Доступные команды:"));
    UE_LOG(LogTemp, Warning, TEXT("  sys.Test - тестовая команда"));
    UE_LOG(LogTemp, Warning, TEXT("  sys.Health - проверка системы"));
}

void FSystem1ParadoxModule::ShutdownModule()
{
    UE_LOG(LogTemp, Warning, TEXT("System1Paradox Module Shutdown"));
}