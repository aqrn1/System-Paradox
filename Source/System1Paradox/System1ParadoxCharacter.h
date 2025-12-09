#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "System1ParadoxCharacter.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Unarmed     UMETA(DisplayName = "Unarmed"),
    Pistol      UMETA(DisplayName = "Pistol"),
    Rifle       UMETA(DisplayName = "Rifle"),
    Melee       UMETA(DisplayName = "Melee")
};

UCLASS()
class SYSTEM1PARADOX_API ASystem1ParadoxCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ASystem1ParadoxCharacter();
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void PostInitializeComponents() override;

    // ==================== ПУБЛИЧНЫЕ ГЕТТЕРЫ ДЛЯ АНИМАЦИЙ ====================
    UFUNCTION(BlueprintCallable, Category = "Animation")
    FORCEINLINE float GetCurrentSpeed() const { return GetVelocity().Size(); }

    UFUNCTION(BlueprintCallable, Category = "Animation")
    FORCEINLINE bool GetIsCrouching() const { return bIsCrouching; }

    UFUNCTION(BlueprintCallable, Category = "Animation")
    FORCEINLINE bool GetIsSprinting() const { return bIsSprinting; }

    UFUNCTION(BlueprintCallable, Category = "Animation")
    FORCEINLINE bool GetIsInAir() const { return GetCharacterMovement()->IsFalling(); }

    UFUNCTION(BlueprintCallable, Category = "Animation")
    FORCEINLINE EWeaponType GetCurrentWeaponType() const { return CurrentWeaponType; }

    UFUNCTION(BlueprintCallable, Category = "Animation")
    FORCEINLINE bool GetIsSwitchingWeapon() const { return bIsSwitchingWeapon; }

    // ==================== ОСНОВНЫЕ ФУНКЦИИ ====================
    UFUNCTION(BlueprintCallable, Category = "Input")
    void MoveForward(float Value);

    UFUNCTION(BlueprintCallable, Category = "Input")
    void MoveRight(float Value);

    UFUNCTION(BlueprintCallable, Category = "Input")
    void LookUp(float Value);

    UFUNCTION(BlueprintCallable, Category = "Input")
    void Turn(float Value);

    UFUNCTION(BlueprintCallable, Category = "Input")
    void StartJump();

    UFUNCTION(BlueprintCallable, Category = "Input")
    void StopJump();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void StartFire();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void StopFire();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void ReloadWeapon();

    UFUNCTION(BlueprintCallable, Category = "Input")
    void StartReload();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StartSprint();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StopSprint();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StartCrouch();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StopCrouch();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SwitchToPistol();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SwitchToRifle();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SwitchToUnarmed();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void EquipWeapon(EWeaponType NewWeaponType);

protected:
    virtual void BeginPlay() override;

    // ==================== КОМПОНЕНТЫ ====================
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    class AWeapon* CurrentWeapon;

    // ==================== НАСТРОЙКИ ====================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class AWeapon> WeaponClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float WalkSpeed = 400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float SprintSpeed = 600.0f;

    // ==================== СОСТОЯНИЯ (PROTECTED) ====================
protected:
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    bool bIsSprinting = false;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    bool bIsCrouching = false;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    EWeaponType CurrentWeaponType = EWeaponType::Unarmed;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    bool bIsSwitchingWeapon = false;

private:
    FTimerHandle FireTimerHandle;
    bool bIsFiring = false;
};