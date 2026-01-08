#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "S1P_Types.h"
#include "Weapon.generated.h"

USTRUCT(BlueprintType)
struct FWeaponHitData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
    float Damage = 25.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
    float HeadshotMultiplier = 2.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
    float TorsoMultiplier = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
    float LimbMultiplier = 0.7f;
};

USTRUCT(BlueprintType)
struct FWeaponStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxAmmo = 30;

    // ВРЕМЯ МЕЖДУ ВЫСТРЕЛАМИ (секунды)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FireInterval = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReloadTime = 2.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RecoilVertical = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RecoilHorizontal = 0.05f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EffectiveRange = 5000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Spread = 0.5f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponAmmoEvent, int32, Ammo);

UCLASS(Blueprintable)
class SYSTEM1PARADOX_API AWeapon : public AActor
{
    GENERATED_BODY()

public:
    AWeapon();

    virtual void Tick(float DeltaTime) override;

    // ===== ACTIONS =====
    UFUNCTION(BlueprintCallable)
    void StartFire();

    UFUNCTION(BlueprintCallable)
    void StopFire();

    UFUNCTION(BlueprintCallable)
    void Reload();

    UFUNCTION(BlueprintCallable)
    void StartAim();

    UFUNCTION(BlueprintCallable)
    void StopAim();

    // ===== STATE =====
    bool IsFiring() const { return bIsFiring; }
    bool IsReloading() const { return bIsReloading; }
    bool IsAiming() const { return bIsAiming; }

    float GetReloadProgress() const;

    ES1P_WeaponType GetWeaponType() const { return WeaponType; }

    // ===== COMPONENTS =====
    UPROPERTY(VisibleAnywhere)
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* MuzzleLocation;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* ShellEjectLocation;

    // ===== DATA =====
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FWeaponStats Stats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FWeaponHitData HitData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ES1P_WeaponType WeaponType = ES1P_WeaponType::Pistol;

    // ===== EVENTS =====
    UPROPERTY(BlueprintAssignable)
    FWeaponEvent OnWeaponFired;

    UPROPERTY(BlueprintAssignable)
    FWeaponEvent OnWeaponReloaded;

    UPROPERTY(BlueprintAssignable)
    FWeaponEvent OnWeaponEmpty;

    UPROPERTY(BlueprintAssignable)
    FWeaponAmmoEvent OnAmmoChanged;

protected:
    virtual void BeginPlay() override;

private:
    void FireShot();
    bool CanFire() const;
    bool CanReload() const;

    void ApplyRecoil();

    int32 CurrentAmmo;
    bool bIsFiring;
    bool bIsReloading;
    bool bIsAiming;

    float ReloadStartTime;

    FTimerHandle FireTimer;
    FTimerHandle ReloadTimer;
};
