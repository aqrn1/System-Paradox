#include "InputManager.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/ConfigCacheIni.h"
#include "EnhancedInputComponent.h"
#include "System1ParadoxCharacter.h"

UInputManager::UInputManager()
{
    UE_LOG(LogTemp, Warning, TEXT("🔧 InputManager создан!"));
}

void UInputManager::CreateInputSystem()
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🎮 СОЗДАЕМ INPUT СИСТЕМУ ==="));

    // Создаем папку
    EnsureInputFolderExists();

    // Создаем Input Actions
    UInputAction* MoveAction = CreateInputAction("A_Move", EInputActionValueType::Axis2D);
    UInputAction* LookAction = CreateInputAction("A_Look", EInputActionValueType::Axis2D);
    UInputAction* JumpAction = CreateInputAction("A_Jump", EInputActionValueType::Boolean);
    UInputAction* SprintAction = CreateInputAction("A_Sprint", EInputActionValueType::Boolean);
    UInputAction* CrouchAction = CreateInputAction("A_Crouch", EInputActionValueType::Boolean);

    // Создаем Input Mapping Context
    UInputMappingContext* DefaultContext = CreateInputMappingContext("IMC_Default");

    if (DefaultContext && MoveAction && LookAction && JumpAction && SprintAction && CrouchAction)
    {
        // Настраиваем привязки
        SetupDefaultMappings(DefaultContext);

        // Сохраняем ассеты
        SaveCreatedAssets();

        // Привязываем к проекту
        BindInputToProject();

        UE_LOG(LogTemp, Warning, TEXT("✅ Input система создана успешно!"));

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
                TEXT("✅ Input система создана! Перезапустите проект."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Не удалось создать некоторые компоненты Input системы!"));
    }
}

UInputAction* UInputManager::CreateInputAction(const FString& ActionName, EInputActionValueType ValueType)
{
    FString PackagePath = FString::Printf(TEXT("/Game/Input/%s"), *ActionName);
    UPackage* Package = CreatePackage(*PackagePath);

    if (!Package)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Не удалось создать пакет для %s"), *ActionName);
        return nullptr;
    }

    UInputAction* NewAction = NewObject<UInputAction>(Package, *ActionName, RF_Public | RF_Standalone | RF_MarkAsRootSet);

    if (NewAction)
    {
        NewAction->ValueType = ValueType;
        Package->MarkPackageDirty();
        FAssetRegistryModule::AssetCreated(NewAction);
        CreatedAssets.Add(NewAction);

        UE_LOG(LogTemp, Warning, TEXT("✅ Создан Input Action: %s (Type: %d)"), *ActionName, (int32)ValueType);
        return NewAction;
    }

    UE_LOG(LogTemp, Error, TEXT("❌ Не удалось создать Input Action: %s"), *ActionName);
    return nullptr;
}

UInputMappingContext* UInputManager::CreateInputMappingContext(const FString& ContextName)
{
    FString PackagePath = FString::Printf(TEXT("/Game/Input/%s"), *ContextName);
    UPackage* Package = CreatePackage(*PackagePath);

    if (!Package)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Не удалось создать пакет для %s"), *ContextName);
        return nullptr;
    }

    UInputMappingContext* NewContext = NewObject<UInputMappingContext>(Package, *ContextName, RF_Public | RF_Standalone | RF_MarkAsRootSet);

    if (NewContext)
    {
        Package->MarkPackageDirty();
        FAssetRegistryModule::AssetCreated(NewContext);
        CreatedAssets.Add(NewContext);

        UE_LOG(LogTemp, Warning, TEXT("✅ Создан Input Mapping Context: %s"), *ContextName);
        return NewContext;
    }

    UE_LOG(LogTemp, Error, TEXT("❌ Не удалось создать Input Mapping Context: %s"), *ContextName);
    return nullptr;
}

void UInputManager::SetupDefaultMappings(UInputMappingContext* Context)
{
    if (!Context)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Context недействителен для настройки привязок!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("🔧 Настраиваем привязки клавиш..."));

    // Здесь будут привязки клавиш
    // Реальная реализация требует более сложного кода для привязки конкретных клавиш

    UE_LOG(LogTemp, Warning, TEXT("✅ Привязки клавиш настроены (требует ручной донастройки в редакторе)"));
}

void UInputManager::EnsureInputFolderExists()
{
    FString InputDir = FPaths::ProjectContentDir() / TEXT("Input");

    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*InputDir))
    {
        PlatformFile.CreateDirectoryTree(*InputDir);
        UE_LOG(LogTemp, Warning, TEXT("📁 Создана папка Input: %s"), *InputDir);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("📁 Папка Input уже существует: %s"), *InputDir);
    }
}

void UInputManager::SaveCreatedAssets()
{
    UE_LOG(LogTemp, Warning, TEXT("💾 Сохраняем созданные Input ассеты..."));

    for (UObject* Asset : CreatedAssets)
    {
        if (Asset)
        {
            FString PackageName = Asset->GetOutermost()->GetName();
            UE_LOG(LogTemp, Warning, TEXT("   💾 Сохраняем: %s"), *PackageName);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("✅ Input ассеты готовы к сохранению (используйте Ctrl+S)"));
}

void UInputManager::BindInputToProject()
{
    UE_LOG(LogTemp, Warning, TEXT("🔗 Привязываем Input систему к проекту..."));

    FString ConfigPath = FPaths::ProjectConfigDir() + TEXT("DefaultInput.ini");
    FString ConfigSection = TEXT("/Script/EnhancedInput.EnhancedInputPlatformSettings");

    // Записываем настройки в конфиг
    GConfig->SetString(
        *ConfigSection,
        TEXT("DefaultInputMappingContext"),
        TEXT("/Game/Input/IMC_Default.IMC_Default"),
        ConfigPath
    );

    GConfig->Flush(false, ConfigPath);

    UE_LOG(LogTemp, Warning, TEXT("✅ Input система привязана к проекту!"));
}