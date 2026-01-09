#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "S1P_Types.h"
#include "System1ParadoxCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AWeapon;
class UFPSAnimInstance;

UCLASS()
class SYSTEM1PARADOX_API ASystem1ParadoxCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ASystem1ParadoxCharacter();

    virtual void Tick(float DeltaTime) override;

    // ===== Input =====
    void Turn(float Value);
    void LookUp(float Value);

    void StartJump();
    void StopJump();

    void StartSprint();
    void StopSprint();

    void StartCrouch();
    void StopCrouch();

    // ===== Movement =====
    bool CanSprint() const;
    void UpdateMovementSpeed();

    // ===== Weapon =====
    void SpawnDefaultWeapon();
    void EquipWeapon(ES1P_WeaponType NewWeaponType);

    // ===== Getters =====
    float GetCurrentSpeed() const;
    bool GetIsCrouching() const;
    bool GetIsSprinting() const;
    bool GetIsInAir() const;

    // Проверки состояния оружия
    bool IsWeaponFiring() const;
    bool IsWeaponReloading() const;
    bool IsWeaponAiming() const;

    ES1P_WeaponType GetCurrentWeaponType() const { return CurrentWeaponType; }
    AWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

    UFPSAnimInstance* GetFPSAnimInstance() const;

    // В System1ParadoxCharacter.cpp
    void MoveForward(float Value);
    void MoveRight(float Value);

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    // ===== Components =====
    UPROPERTY(VisibleAnywhere, Category = "Camera")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, Category = "Camera")
    UCameraComponent* CameraComponent;

    // ===== Movement params =====
    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    float WalkSpeed = 600.f;

    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    float SprintSpeed = 900.f;

    bool bIsSprinting = false;
    bool bIsCrouching = false;

    // ===== Weapon =====
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<AWeapon> DefaultWeaponClass;

    UPROPERTY()
    AWeapon* CurrentWeapon = nullptr;

    ES1P_WeaponType CurrentWeaponType = ES1P_WeaponType::Unarmed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* CrouchAnimMontage;  // Ссылка на анимацию приседания

};