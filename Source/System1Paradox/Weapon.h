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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
    float Damage = 25.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
    float HeadshotMultiplier = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
    float TorsoMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
    float LimbMultiplier = 0.7f;
};

UCLASS()
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

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    FORCEINLINE ES1P_WeaponType GetWeaponType() const { return WeaponType; }

    // Компоненты оружия
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* MuzzleLocation;

    // AAA параметры оружия
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FWeaponHitData HitData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    ES1P_WeaponType WeaponType = ES1P_WeaponType::Pistol;  // Тип оружия

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo", meta = (ClampMin = "1", ClampMax = "200"))
    int32 MaxAmmo = 30;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo")
    int32 CurrentAmmo = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.01", ClampMax = "1.0"))
    float FireRate = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.5", ClampMax = "5.0"))
    float ReloadTime = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float RecoilVertical = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float RecoilHorizontal = 0.05f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    USoundBase* FireSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* MuzzleFlash;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* HitEffect;

    // Состояние оружия
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon State")
    bool bIsFiring = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon State")
    bool bIsReloading = false;

private:
    // Таймеры
    FTimerHandle FireTimerHandle;
    FTimerHandle ReloadTimerHandle;

    // AAA функции
    void ApplyRecoil();
    void ApplyHitEffect(const FHitResult& HitResult);
    float CalculateDamage(const FHitResult& HitResult) const;
    FString GetBodyPartName(const FHitResult& HitResult) const;
};