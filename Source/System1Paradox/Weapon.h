#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

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

    // Боеприпасы
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
    int32 MaxAmmo = 30;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo")
    int32 CurrentAmmo = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Damage = 25.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float FireRate = 0.1f;

    // Состояние оружия
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon State")
    bool bIsFiring = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon State")
    bool bIsReloading = false;

    // Таймеры
    FTimerHandle FireTimerHandle;
    FTimerHandle ReloadTimerHandle;
};