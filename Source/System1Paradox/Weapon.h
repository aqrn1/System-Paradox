#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Weapon.generated.h"

// Структура для информации о выстреле
USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Damage = 25.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float FireRate = 600.0f; // Выстрелов в минуту

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Spread = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Recoil = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Range = 10000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 BulletsPerShot = 1;
};

UCLASS()
class SYSTEM1PARADOX_API AWeapon : public AActor
{
    GENERATED_BODY()

public:
    AWeapon();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Основные функции оружия
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void StartFire();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void StopFire();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void FireShot();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void Reload();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    bool CanFire() const;

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    bool CanReload() const;

    // Компоненты оружия
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* MuzzleLocation;

    // Данные оружия
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Config")
    FWeaponData WeaponConfig;

    // Боеприпасы
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
    int32 MaxAmmo = 30;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo")
    int32 CurrentAmmo = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
    int32 MaxReserveAmmo = 90;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo")
    int32 CurrentReserveAmmo = 0;

    // Состояние оружия
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon State")
    bool bIsFiring = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon State")
    bool bIsReloading = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon State")
    float LastFireTime = 0.0f;

    // Эффекты
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* MuzzleFlash;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* ImpactEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    USoundBase* FireSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    USoundBase* ReloadSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    USoundBase* EmptySound;

    // Таймеры
    FTimerHandle FireTimerHandle;
    FTimerHandle ReloadTimerHandle;

    // Вспомогательные функции
    void CalculateSpread(FVector& ShotDirection, float& CurrentSpread);
    void SpawnMuzzleFlash();
    void SpawnImpactEffect(const FVector& Location, const FVector& Normal);
    void PlayFireSound();
    void PlayReloadSound();
    void PlayEmptySound();

    // Отладочные функции
    void DrawDebugShot(const FVector& Start, const FVector& End, bool bHit);

private:
    float CurrentSpread = 0.0f;
    float TimeBetweenShots = 0.0f;
};