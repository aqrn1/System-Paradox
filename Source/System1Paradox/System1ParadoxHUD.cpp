#include "System1ParadoxHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"

ASystem1ParadoxHUD::ASystem1ParadoxHUD()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASystem1ParadoxHUD::BeginPlay()
{
    Super::BeginPlay();

    // Инициализация значений
    Health = 100.0f;
    AmmoCurrent = 30;
    AmmoMax = 30;
}

void ASystem1ParadoxHUD::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Обновление таймера сообщения
    if (MessageTimer > 0.0f)
    {
        MessageTimer -= DeltaTime;
        if (MessageTimer <= 0.0f)
        {
            DisplayMessage = "";
        }
    }
}

void ASystem1ParadoxHUD::DrawHUD()
{
    Super::DrawHUD();

    if (Canvas == nullptr) return;

    // Рисуем элементы HUD
    DrawHealthBar();
    DrawAmmoInfo();
    DrawCrosshair();
    DrawMessage();
}

void ASystem1ParadoxHUD::DrawHealthBar()
{
    // Рисуем полоску здоровья
    float HealthBarWidth = 200.0f * HUDSize;
    float HealthBarHeight = 20.0f * HUDSize;
    float HealthBarX = 50.0f;
    float HealthBarY = 50.0f;

    // Фон здоровья
    FVector2D HealthBarBackgroundSize(HealthBarWidth, HealthBarHeight);
    FVector2D HealthBarBackgroundPos(HealthBarX, HealthBarY);
    DrawRect(FLinearColor::Red, HealthBarBackgroundPos.X, HealthBarBackgroundPos.Y, HealthBarBackgroundSize.X, HealthBarBackgroundSize.Y);

    // Текущее здоровье
    float HealthPercent = Health / 100.0f;
    FVector2D HealthBarSize(HealthBarWidth * HealthPercent, HealthBarHeight);
    FVector2D HealthBarPos(HealthBarX, HealthBarY);
    DrawRect(HUDColor, HealthBarPos.X, HealthBarPos.Y, HealthBarSize.X, HealthBarSize.Y);

    // Текст здоровья
    FString HealthText = FString::Printf(TEXT("Health: %.0f"), Health);
    DrawText(HealthText, FLinearColor::White, HealthBarX, HealthBarY + 25.0f);
}

void ASystem1ParadoxHUD::DrawAmmoInfo()
{
    // Рисуем информацию о патронах
    float AmmoX = Canvas->SizeX - 150.0f;
    float AmmoY = 50.0f;

    FString AmmoText = FString::Printf(TEXT("Ammo: %d / %d"), AmmoCurrent, AmmoMax);
    DrawText(AmmoText, FLinearColor::White, AmmoX, AmmoY);
}

void ASystem1ParadoxHUD::DrawCrosshair()
{
    // Простой прицел в центре экрана
    float CenterX = Canvas->SizeX / 2.0f;
    float CenterY = Canvas->SizeY / 2.0f;
    float CrosshairSize = 12.0f * HUDSize;

    // Горизонтальная линия
    DrawLine(CenterX - CrosshairSize, CenterY, CenterX + CrosshairSize, CenterY, HUDColor);

    // Вертикальная линия
    DrawLine(CenterX, CenterY - CrosshairSize, CenterX, CenterY + CrosshairSize, HUDColor);
}

void ASystem1ParadoxHUD::DrawMessage()
{
    if (!DisplayMessage.IsEmpty())
    {
        float MessageX = Canvas->SizeX / 2.0f - 100.0f;
        float MessageY = Canvas->SizeY / 3.0f;
        DrawText(DisplayMessage, FLinearColor::Yellow, MessageX, MessageY);
    }
}

void ASystem1ParadoxHUD::SetHUDSize(float NewSize)
{
    HUDSize = NewSize;
}

void ASystem1ParadoxHUD::SetHUDColor(FLinearColor NewColor)
{
    HUDColor = NewColor;
}

void ASystem1ParadoxHUD::UpdateHealth(float NewHealth)
{
    Health = NewHealth;
}

void ASystem1ParadoxHUD::UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
    this->AmmoCurrent = CurrentAmmo;
    this->AmmoMax = MaxAmmo;
}

void ASystem1ParadoxHUD::ShowMessage(const FString& Message, float Duration)
{
    DisplayMessage = Message;
    MessageTimer = Duration;
}
