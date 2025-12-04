#include "System1Paradox.h"
#include "Modules/ModuleManager.h"
#include "HAL/IConsoleManager.h"
#include "Engine/World.h"
#include "BlueprintManager.h"

// ТЕСТОВАЯ КОМАНДА
static void TestCommand(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("🎯 ТЕСТОВАЯ КОМАНДА РАБОТАЕТ!"));
}

// ПРОВЕРКА СИСТЕМЫ
static void HealthCheck(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("❤️ HEALTH CHECK: SYSTEM1PARADOX"));
    UE_LOG(LogTemp, Warning, TEXT("✅ Модуль загружен"));
}

// КОМАНДА ДЛЯ СОЗДАНИЯ BLUEPRINTS
static void CreateBlueprintsCommand(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🛠️ НАЧИНАЕМ СОЗДАНИЕ BLUEPRINTS ==="));

    UWorld* World = GWorld;
    if (!World) return;

    UBlueprintManager* BlueprintManager = NewObject<UBlueprintManager>(World);
    if (BlueprintManager)
    {
        BlueprintManager->CreateAllBlueprints();
    }
}

// КОМАНДА ДЛЯ ПРИВЯЗКИ BLUEPRINTS
static void AutoBindBlueprintsCommand(const TArray<FString>& Args)
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🔗 ПРИВЯЗЫВАЕМ BLUEPRINTS ==="));

    UWorld* World = GWorld;
    if (!World) return;

    UBlueprintManager* BlueprintManager = NewObject<UBlueprintManager>(World);
    if (BlueprintManager)
    {
        BlueprintManager->AutoBindBlueprints();
    }
}

// РЕГИСТРАЦИЯ КОМАНД
static FAutoConsoleCommand TestCmd(TEXT("sys.Test"), TEXT("Тестовая команда"), FConsoleCommandWithArgsDelegate::CreateStatic(&TestCommand));
static FAutoConsoleCommand HealthCmd(TEXT("sys.Health"), TEXT("Проверка системы"), FConsoleCommandWithArgsDelegate::CreateStatic(&HealthCheck));
static FAutoConsoleCommand CreateBPCmd(TEXT("createbp"), TEXT("Создание Blueprints"), FConsoleCommandWithArgsDelegate::CreateStatic(&CreateBlueprintsCommand));
static FAutoConsoleCommand AutoBindCmd(TEXT("autobind"), TEXT("Привязка Blueprints"), FConsoleCommandWithArgsDelegate::CreateStatic(&AutoBindBlueprintsCommand));
static FAutoConsoleCommand AdjustWeaponCmd(TEXT("weapon.pos"), TEXT("Настройка позиции оружия"), FConsoleCommandWithArgsDelegate::CreateStatic(&AdjustWeaponPos));

IMPLEMENT_PRIMARY_GAME_MODULE(FSystem1ParadoxModule, System1Paradox, "System1Paradox");

void FSystem1ParadoxModule::StartupModule()
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🚀 SYSTEM1PARADOX MODULE STARTED ==="));
}

void FSystem1ParadoxModule::ShutdownModule()
{
    UE_LOG(LogTemp, Warning, TEXT("System1Paradox Module Shutdown"));
}

// Команда для настройки позиции оружия
static void AdjustWeaponPos(const TArray<FString>& Args)
{
    if (Args.Num() >= 3)
    {
        FVector NewOffset(
            FCString::Atof(*Args[0]),
            FCString::Atof(*Args[1]),
            FCString::Atof(*Args[2])
        );

        // Получаем персонажа и меняем позицию
        // ...
    }
}
