#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "S1P_Types.h"
#include "System1ParadoxCharacter.generated.h"

class AWeapon;
class UCameraComponent;

UCLASS()
class SYSTEM1PARADOX_API ASystem1ParadoxCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ASystem1ParadoxCharacter();

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    virtual void BeginPlay() override;

    // ================= CAMERA =================
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* CameraComponent;

    // ================= WEAPON =================
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    AWeapon* CurrentWeapon;

    void EquipWeapon(ES1P_WeaponType NewWeaponType);

    // ================= MOVEMENT =================
    void MoveForward(float Value);
    void MoveRight(float Value);

    // ================= WEAPON INPUT =================
    void StartFire();
    void StopFire();
    void ReloadWeapon();
    void StartAim();
    void StopAim();

public:
    // =====================================================
    // === 🔥 API ДЛЯ ANIM INSTANCE (AAA-СТАНДАРТ) ==========
    // =====================================================

    bool IsWeaponFiring() const;
    bool IsWeaponReloading() const;
    bool IsWeaponAiming() const;
    ES1P_WeaponType GetCurrentWeaponType() const;
};
