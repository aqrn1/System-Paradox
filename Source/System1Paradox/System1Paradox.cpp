#include "System1Paradox.h"
#include "Modules/ModuleManager.h"
#include "HAL/IConsoleManager.h"
#include "Engine/World.h"
#include "BlueprintManager.h"

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

    // Создаем менеджер блюпринтов в правильном контексте
    UWorld* World = GWorld;
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ GWorld не доступен!"));
        return;
    }

    UBlueprintManager* BlueprintManager = NewObject<UBlueprintManager>(World);
    if (BlueprintManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("🔧 BlueprintManager создан успешно!"));
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

// 🔥 НОВАЯ КОМАНДА: Автоматическая привязка блюпринтов
static void AutoBindBlueprintsCommand(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🔗 НАЧИНАЕМ АВТОМАТИЧЕСКУЮ ПРИВЯЗКУ BLUEPRINTS ==="));

    UWorld* World = GWorld;
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ GWorld не доступен!"));
        return;
    }

    UBlueprintManager* BlueprintManager = NewObject<UBlueprintManager>(World);
    if (BlueprintManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("🔧 BlueprintManager создан для привязки!"));
        BlueprintManager->AutoBindBlueprints();
        UE_LOG(LogTemp, Warning, TEXT("✅ BlueprintManager завершил автоматическую привязку!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Не удалось создать BlueprintManager для привязки!"));
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

static FAutoConsoleCommand AutoBindCmd(
    TEXT("sys.AutoBindBlueprints"),
    TEXT("Автоматическая привязка созданных Blueprints к настройкам проекта"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&AutoBindBlueprintsCommand)
);

IMPLEMENT_PRIMARY_GAME_MODULE(FSystem1ParadoxModule, System1Paradox, "System1Paradox");

void FSystem1ParadoxModule::StartupModule()
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🚀 SYSTEM1PARADOX MODULE STARTED ==="));
    UE_LOG(LogTemp, Warning, TEXT("Доступные команды:"));
    UE_LOG(LogTemp, Warning, TEXT("  sys.Test - тестовая команда"));
    UE_LOG(LogTemp, Warning, TEXT("  sys.Health - проверка системы"));
    UE_LOG(LogTemp, Warning, TEXT("  sys.CreateBlueprints - создание Blueprints"));
    UE_LOG(LogTemp, Warning, TEXT("  sys.AutoBindBlueprints - автоматическая привязка Blueprints"));
}

void FSystem1ParadoxModule::ShutdownModule()
{
    UE_LOG(LogTemp, Warning, TEXT("System1Paradox Module Shutdown"));
}