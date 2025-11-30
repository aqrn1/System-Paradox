#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Engine/Engine.h"
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

	// Настройка привязок клавиш
	UFUNCTION(BlueprintCallable, Category = "Input Automation")
	void SetupDefaultMappings(UInputMappingContext* Context);

private:
	// Создание папки для Input ассетов
	void EnsureInputFolderExists();

	// Сохранение созданных ассетов
	void SaveCreatedAssets();

	// Привязка Input системы к проекту
	void BindInputToProject();

	TArray<UObject*> CreatedAssets;
};