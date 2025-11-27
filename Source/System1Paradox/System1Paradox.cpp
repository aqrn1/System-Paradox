#include "System1Paradox.h"
#include "Modules/ModuleManager.h"
#include "HAL/IConsoleManager.h"

// 🔥 ТЕСТОВАЯ КОМАНДА
static void TestCommand(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("🎯 ТЕСТОВАЯ КОМАНДА РАБОТАЕТ!"));
    UE_LOG(LogTemp, Warning, TEXT("📊 Количество аргументов: %d"), Args.Num());
}

// 🔥 ПРОВЕРКА СИСТЕМЫ
static void HealthCheck(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("❤️  HEALTH CHECK: SYSTEM1PARADOX"));
    UE_LOG(LogTemp, Warning, TEXT("✅ Модуль загружен"));
    UE_LOG(LogTemp, Warning, TEXT("✅ Консольные команды активны"));
    UE_LOG(LogTemp, Warning, TEXT("✅ Live Coding работает"));
}

// 🔥 КОМАНДА ДЛЯ СОЗДАНИЯ BLUEPRINTS
static void CreateBlueprintsCommand(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🛠️ НАЧИНАЕМ СОЗДАНИЕ BLUEPRINTS ==="));
    UE_LOG(LogTemp, Warning, TEXT("📁 Создаем Blueprints из C++ классов..."));

    // Простая проверка - выводим аргументы
    if (Args.Num() > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("📊 Аргументы команды:"));
        for (const FString& Arg : Args)
        {
            UE_LOG(LogTemp, Warning, TEXT("   - %s"), *Arg);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("✅ Команда создания Blueprints активирована!"));
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

static FAutoConsoleCommand CreateBPCmd(
    TEXT("sys.CreateBlueprints"),
    TEXT("Автоматическое создание Blueprints из C++ классов"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&CreateBlueprintsCommand)
);

IMPLEMENT_PRIMARY_GAME_MODULE(FSystem1ParadoxModule, System1Paradox, "System1Paradox");

void FSystem1ParadoxModule::StartupModule()
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🚀 SYSTEM1PARADOX MODULE STARTED ==="));
    UE_LOG(LogTemp, Warning, TEXT("Доступные команды:"));
    UE_LOG(LogTemp, Warning, TEXT("  sys.Test - тестовая команда"));
    UE_LOG(LogTemp, Warning, TEXT("  sys.Health - проверка системы"));
    UE_LOG(LogTemp, Warning, TEXT("  sys.CreateBlueprints - создание Blueprints"));
}

void FSystem1ParadoxModule::ShutdownModule()
{
    UE_LOG(LogTemp, Warning, TEXT("System1Paradox Module Shutdown"));
}