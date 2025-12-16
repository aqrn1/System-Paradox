#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "S1P_Types.h"

#include "System1ParadoxCharacter.generated.h"

class UFPSAnimInstance;
class AWeapon;

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

    // КОМПОНЕНТЫ (protected - доступны в классе и наследниках)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    AWeapon* CurrentWeapon;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsSprinting;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsCrouching;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    ES1P_WeaponType CurrentWeaponType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float WalkSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AWeapon> DefaultWeaponClass;

    void MoveForward(float Value);
    void MoveRight(float Value);
    void LookUp(float Value);
    void Turn(float Value);
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
    void UpdateMovementSpeed();
    bool CanSprint() const;
    void SpawnDefaultWeapon();

public:
    // ГЕТТЕРЫ (public - доступны извне)
    UFUNCTION(BlueprintCallable, Category = "Animation")
    float GetCurrentSpeed() const;

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    FORCEINLINE AWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

    UFUNCTION(BlueprintCallable, Category = "Animation")
    bool GetIsCrouching() const;

    UFUNCTION(BlueprintCallable, Category = "Animation")
    bool GetIsSprinting() const;

    UFUNCTION(BlueprintCallable, Category = "Animation")
    bool GetIsInAir() const;

    UFUNCTION(BlueprintCallable, Category = "Animation")
    ES1P_WeaponType GetCurrentWeaponType() const;

    UFUNCTION(BlueprintCallable, Category = "Animation")
    ES1P_MovementState GetMovementState() const;

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SwitchToPistol();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SwitchToRifle();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SwitchToMelee();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SwitchToUnarmed();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void EquipWeapon(ES1P_WeaponType NewWeaponType);

    UFUNCTION(BlueprintCallable, Category = "Animation")
    UFPSAnimInstance* GetFPSAnimInstance() const;

    // ГЕТТЕРЫ ДЛЯ КОМПОНЕНТОВ
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    AWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

    UFUNCTION(BlueprintCallable, Category = "Camera")
    UCameraComponent* GetCameraComponent() const { return CameraComponent; }

    void PrintDebugInfo() const;
};