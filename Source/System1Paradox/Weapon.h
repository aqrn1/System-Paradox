#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "S1P_Types.h"
#include "Weapon.generated.h"

// Структура для параметров попадания
USTRUCT(BlueprintType)
struct FWeaponHitData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit", meta = (ClampMin = "1.0", ClampMax = "200.0"))
    float Damage = 25.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit", meta = (ClampMin = "1.0", ClampMax = "10.0"))
    float HeadshotMultiplier = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit", meta = (ClampMin = "0.1", ClampMax = "5.0"))
    float TorsoMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit", meta = (ClampMin = "0.1", ClampMax = "3.0"))
    float LimbMultiplier = 0.7f;
};

// Структура для базовых характеристик оружия
USTRUCT(BlueprintType)
struct FWeaponStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "1", ClampMax = "200"))
    int32 MaxAmmo = 30;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.01", ClampMax = "1.0"))
    float FireRate = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.5", ClampMax = "5.0"))
    float ReloadTime = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.0", ClampMax = "5.0"))
    float RecoilVertical = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.0", ClampMax = "2.0"))
    float RecoilHorizontal = 0.05f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "100.0", ClampMax = "10000.0"))
    float EffectiveRange = 5000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.0", ClampMax = "5.0"))
    float Spread = 0.5f;
};

// ★★★ ДЕКЛАРАЦИИ ДЕЛЕГАТОВ ДО КЛАССА ★★★
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponEventDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponAmmoDelegate, int32, NewAmmo);

UCLASS(Blueprintable, BlueprintType, meta = (DisplayName = "System1Paradox Weapon"))
class SYSTEM1PARADOX_API AWeapon : public AActor
{
    GENERATED_BODY()

public:
    AWeapon();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    virtual void Tick(float DeltaTime) override;

    // ==================== ОСНОВНЫЕ ФУНКЦИИ ====================
    UFUNCTION(BlueprintCallable, Category = "Weapon|Actions")
    void StartFire();

    UFUNCTION(BlueprintCallable, Category = "Weapon|Actions")
    void StopFire();

    UFUNCTION(BlueprintCallable, Category = "Weapon|Actions")
    void FireShot();

    UFUNCTION(BlueprintCallable, Category = "Weapon|Actions")
    void Reload();

    // ==================== ПРОВЕРКИ ====================
    UFUNCTION(BlueprintCallable, Category = "Weapon|Checks")
    bool CanFire() const;

    UFUNCTION(BlueprintCallable, Category = "Weapon|Checks")
    bool CanReload() const;

    // ==================== ГЕТТЕРЫ ИНФОРМАЦИИ ====================
    UFUNCTION(BlueprintCallable, Category = "Weapon|Info")
    ES1P_WeaponType GetWeaponType() const { return WeaponType; }

    UFUNCTION(BlueprintCallable, Category = "Weapon|Info")
    int32 GetCurrentAmmo() const { return CurrentAmmo; }

    UFUNCTION(BlueprintCallable, Category = "Weapon|Info")
    int32 GetMaxAmmo() const { return Stats.MaxAmmo; }

    UFUNCTION(BlueprintCallable, Category = "Weapon|Info")
    float GetFireRate() const { return Stats.FireRate; }

    // ==================== ГЕТТЕРЫ СОСТОЯНИЯ (ВАЖНО: ТОЛЬКО ЭТИ!) ====================
    UFUNCTION(BlueprintCallable, Category = "Weapon|State")
    bool IsFiring() const { return bIsFiring; }

    UFUNCTION(BlueprintCallable, Category = "Weapon|State")
    bool IsReloading() const { return bIsReloading; }

    UFUNCTION(BlueprintCallable, Category = "Weapon|State")
    bool IsAiming() const { return bIsAiming; }

    UFUNCTION(BlueprintCallable, Category = "Weapon|State")
    float GetReloadProgress() const;

    // ==================== КОМПОНЕНТЫ ====================
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Components")
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Components")
    UStaticMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Components")
    USceneComponent* MuzzleLocation;

    // ==================== ХАРАКТЕРИСТИКИ ====================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
    FWeaponStats Stats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Damage")
    FWeaponHitData HitData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Info")
    ES1P_WeaponType WeaponType = ES1P_WeaponType::Pistol;

    // ==================== ЭФФЕКТЫ ====================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Effects")
    USoundBase* FireSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Effects")
    USoundBase* ReloadSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Effects")
    USoundBase* EmptySound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Effects")
    UParticleSystem* MuzzleFlash;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Effects")
    UParticleSystem* HitEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Effects")
    UParticleSystem* ShellEjectEffect;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Components")
    USceneComponent* ShellEjectLocation;

    // ==================== СОСТОЯНИЕ ОРУЖИЯ ====================
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|State")
    int32 CurrentAmmo;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|State")
    bool bIsFiring;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|State")
    bool bIsReloading;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|State")
    bool bIsAiming;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|State")
    float TimeSinceLastShot;

    // ==================== ДОПОЛНИТЕЛЬНЫЕ ФУНКЦИИ ====================
    UFUNCTION(Exec, Category = "Weapon|Debug")
    void WeaponDebug(int32 Enable);

    UFUNCTION(Exec, Category = "Weapon|Debug")
    void SetInfiniteAmmo(int32 Enable);

    UFUNCTION(BlueprintCallable, Category = "Weapon|Actions")
    void ToggleFireMode();

    UFUNCTION(BlueprintCallable, Category = "Weapon|Setup")
    void ApplyWeaponStats(const FWeaponStats& NewStats);

    // ==================== ДЕЛЕГАТЫ ДЛЯ BLUEPRINT ====================
    UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
    FWeaponEventDelegate OnWeaponFired;

    UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
    FWeaponEventDelegate OnWeaponReloaded;

    UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
    FWeaponAmmoDelegate OnAmmoChanged;

    UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
    FWeaponEventDelegate OnWeaponEmpty;

private:
    // ТАЙМЕРЫ
    FTimerHandle FireTimerHandle;
    FTimerHandle ReloadTimerHandle;

    // ВНУТРЕННИЕ ПЕРЕМЕННЫЕ
    bool bInfiniteAmmo;
    bool bDebugMode;
    float ReloadStartTime;

    // ВНУТРЕННИЕ ФУНКЦИИ
    void ApplyRecoil();
    void ApplyHitEffect(const FHitResult& HitResult);
    void ApplyShellEjectEffect();
    float CalculateDamage(const FHitResult& HitResult) const;
    FString GetBodyPartName(const FHitResult& HitResult) const;
};