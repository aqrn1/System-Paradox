#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "Engine/Engine.h"
#include "InputTriggers.h"
#include "InputModifiers.h"
#include "InputManager.generated.h"


UCLASS()
class SYSTEM1PARADOX_API UInputManager : public UObject
{
	GENERATED_BODY()

public:
	UInputManager();

	// Основная функция создания всей Input системы
	UFUNCTION(BlueprintCallable, Category = "Input Automation")
	void CreateInputSystem();

	// Создание отдельных компонентов
	UFUNCTION(BlueprintCallable, Category = "Input Automation")
	UInputAction* CreateInputAction(const FString& ActionName, EInputActionValueType ValueType);

	UFUNCTION(BlueprintCallable, Category = "Input Automation")
	UInputMappingContext* CreateInputMappingContext(const FString& ContextName);

	// Настройка привязок клавиш с автоматическим созданием
	UFUNCTION(BlueprintCallable, Category = "Input Automation")
	void SetupDefaultMappings(UInputMappingContext* Context, UInputAction* MoveAction, UInputAction* LookAction,
		UInputAction* JumpAction, UInputAction* SprintAction, UInputAction* CrouchAction);

	// Создание ключевых привязок
	void CreateKeyMapping(UInputMappingContext* Context, UInputAction* Action, FKey Key, float X = 0.0f, float Y = 0.0f, float Z = 0.0f);
	void CreateAxisMapping(UInputMappingContext* Context, UInputAction* Action, FKey PositiveKey, FKey NegativeKey, bool bForXAxis = true);

private:
	// Создание папки для Input ассетов
	void EnsureInputFolderExists();

	// Сохранение созданных ассетов
	void SaveCreatedAssets();

	// Привязка Input системы к проекту
	void BindInputToProject();

	TArray<UObject*> CreatedAssets;
};