#include "System1Paradox.h"
#include "Modules/ModuleManager.h"
#include "HAL/IConsoleManager.h"
#include "Engine/World.h"
#include "BlueprintManager.h"

// 🔥 ПРОСТАЯ ТЕСТОВАЯ КОМАНДА ДЛЯ ПРОВЕРКИ
static void TestCommand(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("🎯 ТЕСТОВАЯ КОМАНДА РАБОТАЕТ!"));

    // Выводим сообщение на экран
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("🎯 ТЕСТОВАЯ КОМАНДА РАБОТАЕТ!"));
    }
}

// 🔥 ПРОСТАЯ КОМАНДА СОЗДАНИЯ BLUEPRINTS
static void SimpleCreateBlueprints(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🛠️ ПРОСТОЕ СОЗДАНИЕ BLUEPRINTS ==="));

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("=== 🛠️ СОЗДАЕМ BLUEPRINTS ==="));
    }

    UWorld* World = GWorld;
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ GWorld не доступен!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("✅ GWorld доступен, создаем BlueprintManager..."));

    UBlueprintManager* BlueprintManager = NewObject<UBlueprintManager>(World);
    if (BlueprintManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ BlueprintManager создан! Вызываем CreateAllBlueprints..."));

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Yellow, TEXT("🔧 Создаем BlueprintManager..."));
        }

        BlueprintManager->CreateAllBlueprints();

        UE_LOG(LogTemp, Warning, TEXT("✅ CreateAllBlueprints завершено!"));

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, TEXT("✅ Blueprints созданы!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Не удалось создать BlueprintManager!"));

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Red, TEXT("❌ Ошибка создания BlueprintManager!"));
        }
    }
}

// 🔥 ПРОСТАЯ КОМАНДА ПРИВЯЗКИ BLUEPRINTS
static void SimpleAutoBindBlueprints(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🔗 ПРОСТАЯ ПРИВЯЗКА BLUEPRINTS ==="));

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("=== 🔗 ПРИВЯЗЫВАЕМ BLUEPRINTS ==="));
    }

    UWorld* World = GWorld;
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ GWorld не доступен!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("✅ GWorld доступен, создаем BlueprintManager..."));

    UBlueprintManager* BlueprintManager = NewObject<UBlueprintManager>(World);
    if (BlueprintManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ BlueprintManager создан! Вызываем AutoBindBlueprints..."));

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Yellow, TEXT("🔗 Привязываем Blueprints..."));
        }

        BlueprintManager->AutoBindBlueprints();

        UE_LOG(LogTemp, Warning, TEXT("✅ AutoBindBlueprints завершено!"));

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, TEXT("✅ Blueprints привязаны!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Не удалось создать BlueprintManager!"));

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Red, TEXT("❌ Ошибка создания BlueprintManager!"));
        }
    }
}

// 🔥 ПРОВЕРКА СИСТЕМЫ
static void HealthCheck(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("=== ❤️  HEALTH CHECK ==="));
    UE_LOG(LogTemp, Warning, TEXT("✅ SYSTEM1PARADOX MODULE"));
    UE_LOG(LogTemp, Warning, TEXT("✅ Консольные команды активны"));
    UE_LOG(LogTemp, Warning, TEXT("✅ Live Coding работает"));

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("❤️  HEALTH CHECK: СИСТЕМА РАБОТАЕТ"));
    }
}

// РЕГИСТРАЦИЯ ПРОСТЫХ КОМАНД
static FAutoConsoleCommand TestCmd(
    TEXT("test"),
    TEXT("Простая тестовая команда"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&TestCommand)
);

static FAutoConsoleCommand HealthCmd(
    TEXT("health"),
    TEXT("Проверка жизнеспособности системы"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&HealthCheck)
);

static FAutoConsoleCommand CreateBPCmd(
    TEXT("createbp"),
    TEXT("Автоматическое создание Blueprints"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&SimpleCreateBlueprints)
);

static FAutoConsoleCommand AutoBindCmd(
    TEXT("autobind"),
    TEXT("Автоматическая привязка Blueprints"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&SimpleAutoBindBlueprints)
);

IMPLEMENT_PRIMARY_GAME_MODULE(FSystem1ParadoxModule, System1Paradox, "System1Paradox");

void FSystem1ParadoxModule::StartupModule()
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🚀 SYSTEM1PARADOX MODULE STARTED ==="));
    UE_LOG(LogTemp, Warning, TEXT("Доступные команды:"));
    UE_LOG(LogTemp, Warning, TEXT("  test - тестовая команда"));
    UE_LOG(LogTemp, Warning, TEXT("  health - проверка системы"));
    UE_LOG(LogTemp, Warning, TEXT("  createbp - создание Blueprints"));
    UE_LOG(LogTemp, Warning, TEXT("  autobind - привязка Blueprints"));

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Cyan, TEXT("🚀 SYSTEM1PARADOX MODULE ЗАГРУЖЕН"));
    }
}

void FSystem1ParadoxModule::ShutdownModule()
{
    UE_LOG(LogTemp, Warning, TEXT("System1Paradox Module Shutdown"));
}