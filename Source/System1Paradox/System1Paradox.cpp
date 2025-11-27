#include "System1Paradox.h"
#include "Modules/ModuleManager.h"
#include "HAL/IConsoleManager.h"
#include "BlueprintManager.h" // ← ДОБАВЬТЕ ЭТУ СТРОКУ

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


// 🔥 КОМАНДА ДЛЯ СОЗДАНИЯ BLUEPRINTS (ОБНОВЛЕННАЯ ВЕРСИЯ)
static void CreateBlueprintsCommand(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🛠️ НАЧИНАЕМ СОЗДАНИЕ BLUEPRINTS ==="));

    // Создаем менеджер блюпринтов
    UBlueprintManager* BlueprintManager = NewObject<UBlueprintManager>();
    if (BlueprintManager)
    {
        BlueprintManager->CreateAllBlueprints();
        UE_LOG(LogTemp, Warning, TEXT("✅ BlueprintManager успешно создал все блюпринты!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Не удалось создать BlueprintManager!"));
    }

    // Выводим аргументы если есть
    if (Args.Num() > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("📊 Аргументы команды:"));
        for (const FString& Arg : Args)
        {
            UE_LOG(LogTemp, Warning, TEXT("   - %s"), *Arg);
        }
    }
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