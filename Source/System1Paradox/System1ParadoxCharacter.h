#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "S1P_Types.h"
#include "System1ParadoxCharacter.generated.h"

// UE компоненты
class UCameraComponent;
class USpringArmComponent;
class UCharacterMovementComponent;

// Игра
class AWeapon;
class UFPSAnimInstance;

UCLASS()
class SYSTEM1PARADOX_API ASystem1ParadoxCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ASystem1ParadoxCharacter();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    // ===== GETTERS (для AnimInstance и других систем) =====
    float GetCurrentSpeed() const;
    bool GetIsCrouching() const;
    bool GetIsSprinting() const;
    bool GetIsInAir() const;
    ES1P_WeaponType GetCurrentWeaponType() const;
    AWeapon* GetCurrentWeapon() const;
    UFPSAnimInstance* GetFPSAnimInstance() const;

    // ===== INPUT =====
    void MoveForward(float Value);
    void MoveRight(float Value);
    void Turn(float Value);
    void LookUp(float Value);

    void StartJump();
    void StopJump();

    void StartSprint();
    void StopSprint();

    void StartCrouch();
    void StopCrouch();

    void StartFire();
    void StopFire();
    void ReloadWeapon();

    void StartAim();
    void StopAim();

    // ===== WEAPON SWITCH =====
    void SwitchToPistol();
    void SwitchToRifle();
    void SwitchToMelee();
    void SwitchToUnarmed();

protected:
    // ===== CAMERA =====
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* CameraComponent;

    // ===== MOVEMENT =====
    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    float WalkSpeed = 600.f;

    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    float SprintSpeed = 900.f;

    bool bIsSprinting = false;
    bool bIsCrouching = false;

    // ===== WEAPON =====
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    AWeapon* CurrentWeapon = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<AWeapon> DefaultWeaponClass;

    ES1P_WeaponType CurrentWeaponType = ES1P_WeaponType::Unarmed;

protected:
    // ===== INTERNAL =====
    void UpdateMovementSpeed();
    bool CanSprint() const;

    void SpawnDefaultWeapon();
    void EquipWeapon(ES1P_WeaponType NewWeaponType);
};
