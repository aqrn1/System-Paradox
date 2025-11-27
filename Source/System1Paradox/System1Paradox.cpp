#include "System1Paradox.h"
#include "Modules/ModuleManager.h"
#include "HAL/IConsoleManager.h"
#include "BlueprintManager.h"

// 🔥 ПРОСТАЯ ТЕСТОВАЯ КОМАНДА
static void TestCommand(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("🎯 ТЕСТОВАЯ КОМАНДА РАБОТАЕТ! Аргументы: %d"), Args.Num());

    for (const FString& Arg : Args)
    {
        UE_LOG(LogTemp, Warning, TEXT("Аргумент: %s"), *Arg);
    }
}

// 🔥 КОМАНДА ДЛЯ СОЗДАНИЯ BLUEPRINTS
static void CreateBPCommand(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("🚀 КОМАНДА CreateBlueprints ВЫЗВАНА!"));

    // Создаем менеджер и запускаем создание блюпринтов
    UBlueprintManager* Manager = NewObject<UBlueprintManager>();
    if (Manager)
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ BlueprintManager создан успешно!"));
        Manager->CreateAndSetupAllBlueprints();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Ошибка создания BlueprintManager!"));
    }
}

// Регистрация консольных команд
static FAutoConsoleCommand TestCmd(
    TEXT("sys.Test"),
    TEXT("Простая тестовая команда: sys.Test [arg1] [arg2]"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&TestCommand)
);

static FAutoConsoleCommand CreateBPCmd(
    TEXT("sys.CreateBlueprints"),
    TEXT("Создает все автоматические блюпринты"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&CreateBPCommand)
);

IMPLEMENT_PRIMARY_GAME_MODULE(FSystem1ParadoxModule, System1Paradox, "System1Paradox");

void FSystem1ParadoxModule::StartupModule()
{
    UE_LOG(LogTemp, Warning, TEXT("=== SYSTEM1PARADOX MODULE STARTED ==="));
    UE_LOG(LogTemp, Warning, TEXT("Доступные команды:"));
    UE_LOG(LogTemp, Warning, TEXT("  sys.Test - простая тестовая команда"));
    UE_LOG(LogTemp, Warning, TEXT("  sys.CreateBlueprints - создание блюпринтов"));
}

void FSystem1ParadoxModule::ShutdownModule()
{
    UE_LOG(LogTemp, Warning, TEXT("System1Paradox Module Shutdown!"));
}