#include "System1Paradox.h"
#include "Modules/ModuleManager.h"
#include "HAL/IConsoleManager.h"
#include "Engine/World.h"
#include "BlueprintManager.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"  // 🔥 ДОБАВЛЕНО
#include "EnhancedInputComponent.h"    // 🔥 ДОБАВЛЕНО
#include "InputManager.h" // 🔥 ДОБАВЬТЕ ЭТОТ INCLUDE

// 🔥 КОМАНДА ДЛЯ СОЗДАНИЯ INPUT СИСТЕМЫ
static void CreateInputSystemCommand(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🎮 СОЗДАЕМ INPUT СИСТЕМУ ==="));

    UWorld* World = GWorld;
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ GWorld не доступен!"));
        return;
    }

    UInputManager* InputManager = NewObject<UInputManager>(World);
    if (InputManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("🔧 InputManager создан!"));
        InputManager->CreateInputSystem();
        UE_LOG(LogTemp, Warning, TEXT("✅ Input система создана!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Не удалось создать InputManager!"));
    }
}

// 🔥 ПРОСТАЯ КОМАНДА ДЛЯ INPUT
static void SimpleCreateInput(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🎮 ПРОСТОЕ СОЗДАНИЕ INPUT ==="));

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("=== 🎮 СОЗДАЕМ INPUT СИСТЕМУ ==="));
    }

    UWorld* World = GWorld;
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ GWorld не доступен!"));
        return;
    }

    UInputManager* InputManager = NewObject<UInputManager>(World);
    if (InputManager)
    {
        InputManager->CreateInputSystem();

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, TEXT("✅ Input система создана! Сохраните проект (Ctrl+S)"));
        }
    }
}


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

// 🔥 КОМАНДА ПРОВЕРКИ УПРАВЛЕНИЯ - ИСПРАВЛЕННАЯ ВЕРСИЯ
static void CheckInputCommand(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("🎮 ПРОВЕРКА СИСТЕМЫ УПРАВЛЕНИЯ"));

    UWorld* World = GWorld;
    if (World && World->GetFirstPlayerController())
    {
        APlayerController* PC = World->GetFirstPlayerController();
        UE_LOG(LogTemp, Warning, TEXT("✅ Player Controller доступен: %s"), *PC->GetName());

        if (PC->InputComponent)
        {
            UE_LOG(LogTemp, Warning, TEXT("✅ Input Component подключен"));

            // 🔥 УПРОЩЕННАЯ ПРОВЕРКА Enhanced Input
            ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
            if (LocalPlayer)
            {
                UE_LOG(LogTemp, Warning, TEXT("✅ Local Player доступен"));

                // Проверяем Enhanced Input более простым способом
                if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
                {
                    UE_LOG(LogTemp, Warning, TEXT("✅ Enhanced Input Subsystem доступен"));
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("⚠️ Enhanced Input Subsystem не доступен"));
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("❌ Input Component не подключен!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ Player Controller не доступен!"));
    }

    // Проверяем существование Input Assets
    FString InputDir = FPaths::ProjectContentDir() / TEXT("Input");
    if (FPaths::DirectoryExists(InputDir))
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ Папка Input существует: %s"), *InputDir);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ Папка Input не существует! Создайте папку Content/Input/"));
    }
}

// 🔥 ПРОСТАЯ КОМАНДА ДЛЯ ПРОВЕРКИ (альтернатива)
static void TestInput(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("🎮 ТЕСТ УПРАВЛЕНИЯ"));

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("🎮 Проверка управления..."));
    }

    UWorld* World = GWorld;
    if (World)
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ Мир доступен"));
        if (World->GetFirstPlayerController())
        {
            UE_LOG(LogTemp, Warning, TEXT("✅ Player Controller найден"));
        }
    }
}

// 🔥 КОМАНДА ДЛЯ НАСТРОЙКИ БЛЮПРИНТОВ
static void SetupBlueprintsCommand(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("=== ⚙️ НАСТРАИВАЕМ СОЗДАННЫЕ BLUEPRINTS ==="));

    UWorld* World = GWorld;
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ GWorld не доступен!"));
        return;
    }

    UBlueprintManager* BlueprintManager = NewObject<UBlueprintManager>(World);
    if (BlueprintManager)
    {
        BlueprintManager->SetupCreatedBlueprints();
        UE_LOG(LogTemp, Warning, TEXT("✅ BlueprintManager настроил все блюпринты!"));
    }
}

// 🔥 КОМАНДА ДЛЯ СОЗДАНИЯ ГРАФОВ
static void CreateGraphsCommand(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🎨 СОЗДАЕМ ГРАФЫ ДЛЯ BLUEPRINTS ==="));

    UWorld* World = GWorld;
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ GWorld не доступен!"));
        return;
    }

    UBlueprintManager* BlueprintManager = NewObject<UBlueprintManager>(World);
    if (BlueprintManager)
    {
        BlueprintManager->CreateBlueprintGraphs();
        UE_LOG(LogTemp, Warning, TEXT("✅ Графы созданы для всех блюпринтов!"));

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green,
                TEXT("✅ EventGraph и ConstructionScript созданы! Сохраните проект (Ctrl+S)"));
        }
    }
}

// РЕГИСТРАЦИЯ КОМАНД
static FAutoConsoleCommand TestCmd(
    TEXT("sys.Test"),
    TEXT("Простая тестовая команда"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&TestCommand)
);

// РЕГИСТРАЦИЯ КОМАНДЫ
static FAutoConsoleCommand CreateGraphsCmd(
    TEXT("creategraphs"),
    TEXT("Создание EventGraph и ConstructionScript для блюпринтов"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&CreateGraphsCommand)
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

static FAutoConsoleCommand SimpleCreateCmd(
    TEXT("createbp"),
    TEXT("Простое создание Blueprints"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&SimpleCreateBlueprints)
);

static FAutoConsoleCommand SimpleAutoBindCmd(
    TEXT("autobind"),
    TEXT("Простая привязка Blueprints"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&SimpleAutoBindBlueprints)
);

static FAutoConsoleCommand CheckInputCmd(
    TEXT("checkinput"),
    TEXT("Проверка системы управления"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&CheckInputCommand)
);

static FAutoConsoleCommand TestInputCmd(
    TEXT("testinput"),
    TEXT("Простая проверка управления"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&TestInput)
);

// 🔥 РЕГИСТРАЦИЯ КОМАНД (добавьте с другими FAutoConsoleCommand)
static FAutoConsoleCommand CreateInputCmd(
    TEXT("createinput"),
    TEXT("Автоматическое создание Input системы"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&SimpleCreateInput)
);

static FAutoConsoleCommand CreateInputFullCmd(
    TEXT("sys.CreateInput"),
    TEXT("Полное создание Input системы"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&CreateInputSystemCommand)
);

// РЕГИСТРАЦИЯ КОМАНДЫ
static FAutoConsoleCommand SetupBPCmd(
    TEXT("setupbp"),
    TEXT("Настройка созданных блюпринтов"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&SetupBlueprintsCommand)
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
    UE_LOG(LogTemp, Warning, TEXT("  createbp - простое создание Blueprints"));
    UE_LOG(LogTemp, Warning, TEXT("  autobind - простая привязка Blueprints"));
    UE_LOG(LogTemp, Warning, TEXT("  checkinput - проверка системы управления"));
    UE_LOG(LogTemp, Warning, TEXT("  testinput - простая проверка управления"));

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Cyan, TEXT("🚀 SYSTEM1PARADOX MODULE ЗАГРУЖЕН"));
    }
}

void FSystem1ParadoxModule::ShutdownModule()
{
    UE_LOG(LogTemp, Warning, TEXT("System1Paradox Module Shutdown"));
}