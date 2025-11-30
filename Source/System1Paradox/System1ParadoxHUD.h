#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "System1ParadoxHUD.generated.h"

UCLASS()
class SYSTEM1PARADOX_API ASystem1ParadoxHUD : public AHUD
{
    GENERATED_BODY()

public:
    ASystem1ParadoxHUD();

    virtual void DrawHUD() override;

    // Функции для обновления информации на HUD
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHealth(float NewHealth);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowMessage(const FString& Message, float Duration = 3.0f);

protected:
    // Переменные для отображения
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
    float Health = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
    int32 AmmoCurrent = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
    int32 AmmoMax = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
    FString DisplayMessage;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
    float MessageTimer = 0.0f;

    // Шрифт для текста
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    UFont* HudFont;

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    void DrawHealthBar();
    void DrawAmmoInfo();
    void DrawCrosshair();
    void DrawMessage();
};