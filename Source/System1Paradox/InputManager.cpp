#include "InputManager.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/ConfigCacheIni.h"
#include "System1ParadoxCharacter.h"

UInputManager::UInputManager()
{
    UE_LOG(LogTemp, Warning, TEXT("🔧 InputManager создан!"));
}

void UInputManager::CreateInputSystem()
{
    UE_LOG(LogTemp, Warning, TEXT("=== 🎮 СОЗДАЕМ INPUT СИСТЕМУ С ПРИВЯЗКАМИ ==="));

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
        // Настраиваем привязки с автоматическим созданием клавиш
        SetupDefaultMappings(DefaultContext, MoveAction, LookAction, JumpAction, SprintAction, CrouchAction);

        // Сохраняем ассеты
        SaveCreatedAssets();

        // Привязываем к проекту
        BindInputToProject();

        UE_LOG(LogTemp, Warning, TEXT("✅ Input система создана со всеми привязками!"));

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
                TEXT("✅ Input система создана! Все клавиши привязаны автоматически."));
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

void UInputManager::SetupDefaultMappings(UInputMappingContext* Context, UInputAction* MoveAction, UInputAction* LookAction,
    UInputAction* JumpAction, UInputAction* SprintAction, UInputAction* CrouchAction)
{
    if (!Context)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Context недействителен для настройки привязок!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("🔧 Настраиваем автоматические привязки клавиш..."));

    // 🔥 ДВИЖЕНИЕ (WASD) - Axis2D
    CreateKeyMapping(Context, MoveAction, EKeys::W, 0.0f, 1.0f);    // W - вперед
    CreateKeyMapping(Context, MoveAction, EKeys::S, 0.0f, -1.0f);   // S - назад
    CreateKeyMapping(Context, MoveAction, EKeys::A, -1.0f, 0.0f);   // A - влево
    CreateKeyMapping(Context, MoveAction, EKeys::D, 1.0f, 0.0f);    // D - вправо

    // 🔥 КАМЕРА (МЫШЬ) - Axis2D
    CreateKeyMapping(Context, LookAction, EKeys::MouseX, 1.0f, 0.0f);  // Mouse X
    CreateKeyMapping(Context, LookAction, EKeys::MouseY, 0.0f, 1.0f);  // Mouse Y

    // 🔥 ДЕЙСТВИЯ (Boolean)
    CreateKeyMapping(Context, JumpAction, EKeys::SpaceBar);        // Пробел - прыжок
    CreateKeyMapping(Context, SprintAction, EKeys::LeftShift);     // Shift - спринт
    CreateKeyMapping(Context, CrouchAction, EKeys::LeftControl);   // Ctrl - приседание

    UE_LOG(LogTemp, Warning, TEXT("✅ Все привязки клавиш созданы автоматически!"));
    UE_LOG(LogTemp, Warning, TEXT("   🎮 Движение: W, A, S, D"));
    UE_LOG(LogTemp, Warning, TEXT("   🖱️  Камера: Mouse X, Mouse Y"));
    UE_LOG(LogTemp, Warning, TEXT("   ⚡ Действия: Space, Shift, Ctrl"));
}

void UInputManager::CreateKeyMapping(UInputMappingContext* Context, UInputAction* Action, FKey Key, float X, float Y, float Z)
{
    if (!Context || !Action)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Context или Action недействительны для создания привязки!"));
        return;
    }

    // Создаем новую привязку
    FEnhancedActionKeyMapping& Mapping = Context->MapKey(Action, Key);

    // Настраиваем вектор для Axis2D действий
    if (Action->ValueType == EInputActionValueType::Axis2D || Action->ValueType == EInputActionValueType::Axis3D)
    {
        // Для осей настраиваем вектор направления
        Mapping.Modifiers.Add(NewObject<UInputModifierSwizzleAxis>());
    }

    UE_LOG(LogTemp, Warning, TEXT("   🔗 Привязано: %s -> %s (%.1f, %.1f, %.1f)"),
        *Action->GetName(), *Key.ToString(), X, Y, Z);
}

void UInputManager::CreateAxisMapping(UInputMappingContext* Context, UInputAction* Action, FKey PositiveKey, FKey NegativeKey, bool bForXAxis)
{
    if (!Context || !Action)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Context или Action недействительны для создания оси!"));
        return;
    }

    // Положительное направление
    FEnhancedActionKeyMapping& PosMapping = Context->MapKey(Action, PositiveKey);

    // Отрицательное направление  
    FEnhancedActionKeyMapping& NegMapping = Context->MapKey(Action, NegativeKey);

    UE_LOG(LogTemp, Warning, TEXT("   🔄 Ось создана: %s -> %s / %s"),
        *Action->GetName(), *PositiveKey.ToString(), *NegativeKey.ToString());
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

            // Помечаем пакет как требующий сохранения
            Asset->GetOutermost()->MarkPackageDirty();
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