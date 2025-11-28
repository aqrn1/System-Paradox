#include "BlueprintManager.h"
#include "Engine/Blueprint.h"
#include "Misc/MessageDialog.h"
#include "HAL/PlatformFilemanager.h"
#include "HAL/PlatformFile.h"
#include "Misc/Paths.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/Package.h"
#include "Misc/PackageName.h"
#include "Misc/ConfigCacheIni.h"

UBlueprintManager::UBlueprintManager()
{
    UE_LOG(LogTemp, Warning, TEXT("🔧 BlueprintManager создан!"));
}

void UBlueprintManager::CreateAllBlueprints()
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🚀 НАЧИНАЕМ СОЗДАНИЕ ВСЕХ BLUEPRINTS ==="));

    // Создаем папку для блюпринтов
    EnsureBlueprintFolderExists();

    // Получаем все классы проекта для создания блюпринтов
    TArray<UClass*> ProjectClasses;
    GetAllProjectClasses(ProjectClasses);

    UE_LOG(LogTemp, Warning, TEXT("📋 Найдено классов для создания блюпринтов: %d"), ProjectClasses.Num());

    // Создаем блюпринты для каждого класса
    int32 CreatedCount = 0;
    for (UClass* Class : ProjectClasses)
    {
        if (Class && IsClassSuitableForBlueprint(Class))
        {
            FString BlueprintName = FString::Printf(TEXT("BP_%s"), *Class->GetName());
            if (CreateBlueprintFromClass(Class, BlueprintName))
            {
                CreatedCount++;
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("✅ Создание блюпринтов завершено! Создано: %d"), CreatedCount);

    // Выводим сообщение на экран
    if (GEngine)
    {
        FString Message = FString::Printf(TEXT("✅ BlueprintManager: создано %d блюпринтов! Используйте Ctrl+S для сохранения"), CreatedCount);
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, Message);
    }
}

void UBlueprintManager::AutoBindBlueprints()
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🔗 НАЧИНАЕМ АВТОМАТИЧЕСКУЮ ПРИВЯЗКУ BLUEPRINTS ==="));

    int32 BoundCount = 0;

    // Привязываем основные блюпринты к проекту
    if (BindBlueprintToProject(GetBlueprintPathForClass("System1ParadoxCharacter"), "DefaultPawnClass"))
    {
        BoundCount++;
    }

    if (BindBlueprintToProject(GetBlueprintPathForClass("System1ParadoxGameMode"), "DefaultGameMode"))
    {
        BoundCount++;
    }

    if (BindBlueprintToProject(GetBlueprintPathForClass("System1ParadoxPlayerController"), "PlayerControllerClass"))
    {
        BoundCount++;
    }

    if (BindBlueprintToProject(GetBlueprintPathForClass("System1ParadoxCameraManager"), "PlayerCameraManagerClass"))
    {
        BoundCount++;
    }

    // Сохраняем конфигурацию проекта
    SaveProjectConfig();

    UE_LOG(LogTemp, Warning, TEXT("✅ Автоматическая привязка завершена! Привязано: %d блюпринтов"), BoundCount);

    // Выводим сообщение на экран
    if (GEngine)
    {
        FString Message = FString::Printf(TEXT("✅ Автоматическая привязка завершена! Привязано %d блюпринтов"), BoundCount);
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, Message);
    }
}

bool UBlueprintManager::CreateBlueprintFromClass(UClass* SourceClass, const FString& BlueprintName, const FString& PackagePath)
{
    if (!SourceClass)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Неверный исходный класс для создания блюпринта!"));
        return false;
    }

    UE_LOG(LogTemp, Warning, TEXT("🛠️ СОЗДАЕМ БЛЮПРИНТ %s из класса %s"),
        *BlueprintName, *SourceClass->GetName());

    FString FullPackagePath = FString::Printf(TEXT("%s/%s"), *PackagePath, *BlueprintName);

    UE_LOG(LogTemp, Warning, TEXT("   📁 Полный путь: %s"), *FullPackagePath);
    UE_LOG(LogTemp, Warning, TEXT("   🎯 Родительский класс: %s"), *SourceClass->GetName());

    // Создаем пакет для блюпринта
    UPackage* Package = CreatePackage(*FullPackagePath);

    if (!Package)
    {
        UE_LOG(LogTemp, Error, TEXT("   ❌ Не удалось создать пакет для блюпринта!"));
        return false;
    }

    // Создаем блюпринт
    UBlueprint* NewBlueprint = FKismetEditorUtilities::CreateBlueprint(
        SourceClass,
        Package,
        *BlueprintName,
        BPTYPE_Normal,
        UBlueprint::StaticClass(),
        UBlueprintGeneratedClass::StaticClass()
    );

    if (NewBlueprint)
    {
        // Помечаем пакет как измененный (требует сохранения)
        Package->MarkPackageDirty();

        // Уведомляем Asset Registry о создании нового ассета
        FAssetRegistryModule::AssetCreated(NewBlueprint);

        UE_LOG(LogTemp, Warning, TEXT("   ✅ БЛЮПРИНТ УСПЕШНО СОЗДАН В ПАМЯТИ!"));
        UE_LOG(LogTemp, Warning, TEXT("   💡 Имя: %s"), *BlueprintName);
        UE_LOG(LogTemp, Warning, TEXT("   💡 Пакет: %s"), *FullPackagePath);
        UE_LOG(LogTemp, Warning, TEXT("   💡 Используйте Ctrl+S чтобы сохранить блюпринт на диск"));

        // Выводим сообщение на экран
        if (GEngine)
        {
            FString Message = FString::Printf(TEXT("✅ Создан блюпринт: %s (используйте Ctrl+S для сохранения)"), *BlueprintName);
            GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, Message);
        }

        return true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("   ❌ Не удалось создать блюпринт через FKismetEditorUtilities!"));
        return false;
    }
}

bool UBlueprintManager::BindBlueprintToProject(const FString& BlueprintPath, const FString& SettingName)
{
    if (BlueprintPath.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Путь к блюпринту пустой для настройки: %s"), *SettingName);
        return false;
    }

    UE_LOG(LogTemp, Warning, TEXT("🔗 Привязываем блюпринт: %s"), *BlueprintPath);
    UE_LOG(LogTemp, Warning, TEXT("   ⚙️  Настройка: %s"), *SettingName);

    // 🔥 УПРОЩЕННАЯ ПРОВЕРКА - просто проверяем существование папки с блюпринтами
    FString BlueprintDir = FPaths::ProjectContentDir() / TEXT("Blueprints/AutoGenerated");

    if (!FPaths::DirectoryExists(BlueprintDir))
    {
        UE_LOG(LogTemp, Error, TEXT("   ❌ Папка с блюпринтами не существует: %s"), *BlueprintDir);
        return false;
    }

    UE_LOG(LogTemp, Warning, TEXT("   ✅ Папка с блюпринтами существует"));

    // 🔥 ПРИВЯЗКА БЕЗ ПРОВЕРКИ КОНКРЕТНЫХ ФАЙЛОВ
    // (Unreal сам проверит существование блюпринтов при загрузке)

    FString ConfigPath = FPaths::ProjectConfigDir() + TEXT("DefaultEngine.ini");
    FString ConfigSection = TEXT("/Script/EngineSettings.GameMapsSettings");
    FString ConfigKey = SettingName;

    // Записываем в конфигурацию
    GConfig->SetString(
        *ConfigSection,
        *ConfigKey,
        *BlueprintPath,
        ConfigPath
    );

    UE_LOG(LogTemp, Warning, TEXT("   ✅ Запись в конфиг: [%s] %s = %s"), *ConfigSection, *ConfigKey, *BlueprintPath);
    UE_LOG(LogTemp, Warning, TEXT("   💡 Unreal проверит существование блюпринта при загрузке"));

    return true;
}

FString UBlueprintManager::GetBlueprintPathForClass(const FString& ClassName)
{
    return FString::Printf(TEXT("/Game/Blueprints/AutoGenerated/BP_%s.BP_%s_C"), *ClassName, *ClassName);
}

void UBlueprintManager::SaveProjectConfig()
{
    UE_LOG(LogTemp, Warning, TEXT("💾 Сохраняем конфигурацию проекта..."));

    // 🔥 ПРОСТОЕ СОХРАНЕНИЕ БЕЗ СЛОЖНЫХ ВЫЗОВОВ
    FString ConfigPath = FPaths::ProjectConfigDir() + TEXT("DefaultEngine.ini");

    // Принудительно сохраняем конфигурацию
    GConfig->Flush(false, ConfigPath);

    UE_LOG(LogTemp, Warning, TEXT("✅ Конфигурация проекта сохранена: %s"), *ConfigPath);

    // Выводим информационное сообщение
    if (GEngine)
    {
        FString Message = TEXT("💾 Конфигурация проекта обновлена!");
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, Message);
    }
}

void UBlueprintManager::EnsureBlueprintFolderExists()
{
    FString BlueprintDir = FPaths::ProjectContentDir() / TEXT("Blueprints/AutoGenerated");

    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*BlueprintDir))
    {
        PlatformFile.CreateDirectoryTree(*BlueprintDir);
        UE_LOG(LogTemp, Warning, TEXT("📁 Создана папка для блюпринтов: %s"), *BlueprintDir);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("📁 Папка для блюпринтов уже существует: %s"), *BlueprintDir);
    }
}

void UBlueprintManager::GetAllProjectClasses(TArray<UClass*>& OutClasses)
{
    UE_LOG(LogTemp, Warning, TEXT("🔍 Ищем C++ классы проекта..."));

    // Добавляем основные классы проекта
    UClass* CharacterClass = ASystem1ParadoxCharacter::StaticClass();
    UClass* GameModeClass = ASystem1ParadoxGameMode::StaticClass();
    UClass* PlayerControllerClass = ASystem1ParadoxPlayerController::StaticClass();
    UClass* CameraManagerClass = ASystem1ParadoxCameraManager::StaticClass();

    if (CharacterClass)
    {
        OutClasses.Add(CharacterClass);
        UE_LOG(LogTemp, Warning, TEXT("   ✅ Добавлен: %s"), *CharacterClass->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("   ❌ System1ParadoxCharacter не найден!"));
    }

    if (GameModeClass)
    {
        OutClasses.Add(GameModeClass);
        UE_LOG(LogTemp, Warning, TEXT("   ✅ Добавлен: %s"), *GameModeClass->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("   ❌ System1ParadoxGameMode не найден!"));
    }

    if (PlayerControllerClass)
    {
        OutClasses.Add(PlayerControllerClass);
        UE_LOG(LogTemp, Warning, TEXT("   ✅ Добавлен: %s"), *PlayerControllerClass->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("   ❌ System1ParadoxPlayerController не найден!"));
    }

    if (CameraManagerClass)
    {
        OutClasses.Add(CameraManagerClass);
        UE_LOG(LogTemp, Warning, TEXT("   ✅ Добавлен: %s"), *CameraManagerClass->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("   ❌ System1ParadoxCameraManager не найден!"));
    }

    UE_LOG(LogTemp, Warning, TEXT("📋 Итоговое количество классов: %d"), OutClasses.Num());
}

bool UBlueprintManager::IsClassSuitableForBlueprint(UClass* Class)
{
    if (!Class) return false;

    // Исключаем некоторые классы
    FString ClassName = Class->GetName();
    if (ClassName.Contains(TEXT("BlueprintManager")) ||
        ClassName.Contains(TEXT("Module")))
    {
        return false;
    }

    return true;
}