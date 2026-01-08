#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "System1ParadoxTypes.h"
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

    // ===== Getters =====
    float GetCurrentSpeed() const;
    bool GetIsCrouching() const;
    bool GetIsSprinting() const;
    bool GetIsInAir() const;

    ES1P_WeaponType GetCurrentWeaponType() const;
    AWeapon* GetCurrentWeapon() const;

    UFPSAnimInstance* GetFPSAnimInstance() const;

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
};
