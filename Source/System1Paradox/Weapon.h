#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "S1P_Types.h"  // Подключаем наши глобальные типы
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

    // ==================== ОСНОВНЫЕ ФУНКЦИИ ОРУЖИЯ ====================

    UFUNCTION(BlueprintCallable, Category = "Weapon|Actions")
    void StartFire();

    UFUNCTION(BlueprintCallable, Category = "Weapon|Actions")
    void StopFire();

    UFUNCTION(BlueprintCallable, Category = "Weapon|Actions")
    void FireShot();

    UFUNCTION(BlueprintCallable, Category = "Weapon|Actions")
    void Reload();

    // ==================== ПРОВЕРОЧНЫЕ ФУНКЦИИ ====================

    UFUNCTION(BlueprintCallable, Category = "Weapon|Checks")
    bool CanFire() const;

    UFUNCTION(BlueprintCallable, Category = "Weapon|Checks")
    bool CanReload() const;

    UFUNCTION(BlueprintCallable, Category = "Weapon|Checks")
    float GetReloadProgress() const;

    // ==================== ГЕТТЕРЫ ====================

    UFUNCTION(BlueprintCallable, Category = "Weapon|Info")
    FORCEINLINE ES1P_WeaponType GetWeaponType() const { return WeaponType; }

    UFUNCTION(BlueprintCallable, Category = "Weapon|Info")
    FORCEINLINE int32 GetCurrentAmmo() const { return CurrentAmmo; }

    UFUNCTION(BlueprintCallable, Category = "Weapon|Info")
    FORCEINLINE int32 GetMaxAmmo() const { return Stats.MaxAmmo; }

    UFUNCTION(BlueprintCallable, Category = "Weapon|Info")
    FORCEINLINE bool IsFiring() const { return bIsFiring; }

    UFUNCTION(BlueprintCallable, Category = "Weapon|Info")
    FORCEINLINE bool IsReloading() const { return bIsReloading; }

    UFUNCTION(BlueprintCallable, Category = "Weapon|Info")
    FORCEINLINE float GetFireRate() const { return Stats.FireRate; }

    // ==================== КОМПОНЕНТЫ ====================

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Components")
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Components")
    UStaticMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Components")
    USceneComponent* MuzzleLocation;

    // ==================== AAA ХАРАКТЕРИСТИКИ ====================

    // Основные характеристики (используйте эту структуру вместо отдельных полей)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
    FWeaponStats Stats;

    // Настройки урона
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Damage")
    FWeaponHitData HitData;

    // Тип оружия
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Effects")
    USceneComponent* ShellEjectLocation;

    // ==================== СОСТОЯНИЕ ОРУЖИЯ ====================

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|State")
    int32 CurrentAmmo = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|State")
    bool bIsFiring = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|State")
    bool bIsReloading = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|State")
    bool bIsAiming = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|State")
    float TimeSinceLastShot = 0.0f;

    // ==================== AAA ДОПОЛНИТЕЛЬНЫЕ ФУНКЦИИ ====================

    // Консольные команды для отладки
    UFUNCTION(Exec, Category = "Weapon|Debug")
    void WeaponDebug(int32 Enable);

    UFUNCTION(Exec, Category = "Weapon|Debug")
    void SetInfiniteAmmo(int32 Enable);

    // Режимы стрельбы
    UFUNCTION(BlueprintCallable, Category = "Weapon|Actions")
    void ToggleFireMode();

    // Настройка параметров
    UFUNCTION(BlueprintCallable, Category = "Weapon|Setup")
    void ApplyWeaponStats(const FWeaponStats& NewStats);

private:
    // Таймеры
    FTimerHandle FireTimerHandle;
    FTimerHandle ReloadTimerHandle;

    // AAA внутренние переменные
    bool bInfiniteAmmo = false;
    bool bDebugMode = false;
    float ReloadStartTime = 0.0f;

    // ==================== AAA ВНУТРЕННИЕ ФУНКЦИИ ====================

    void ApplyRecoil();
    void ApplyHitEffect(const FHitResult& HitResult);
    void ApplyShellEjectEffect();
    float CalculateDamage(const FHitResult& HitResult) const;
    FString GetBodyPartName(const FHitResult& HitResult) const;
    void UpdateSpread(float DeltaTime);
    void UpdateWeaponSway(float DeltaTime);

    // Делегаты для событий
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponFired);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponReloaded);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32, NewAmmo);

public:
    // ==================== AAA ДЕЛЕГАТЫ ДЛЯ BLUEPRINT ====================

    UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
    FOnWeaponFired OnWeaponFired;

    UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
    FOnWeaponReloaded OnWeaponReloaded;

    UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
    FOnAmmoChanged OnAmmoChanged;

    UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
    FOnWeaponFired OnWeaponEmpty;
};