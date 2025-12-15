// BlueprintManager.h - МИНИМАЛЬНАЯ РАБОЧАЯ ВЕРСИЯ
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BlueprintManager.generated.h"

UCLASS()
class SYSTEM1PARADOX_API UBlueprintManager : public UObject
{
    GENERATED_BODY()

public:
    UBlueprintManager();

    // Статический метод для генерации Blueprint
    UFUNCTION(BlueprintCallable, Category = "Blueprint Manager")
    static void GenerateAllBlueprints();

    // Проверка существования Blueprint классов
    UFUNCTION(BlueprintCallable, Category = "Blueprint Manager")
    static bool CheckBlueprintClasses();
};