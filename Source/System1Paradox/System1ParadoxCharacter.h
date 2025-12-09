#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "S1P_Types.h" 
#include "System1ParadoxCharacter.generated.h"


UCLASS()
class SYSTEM1PARADOX_API ASystem1ParadoxCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ASystem1ParadoxCharacter();

    // ВСЕ ГЕТТЕРЫ И ФУНКЦИИ МЕНЯЮТ ТИПЫ НА ГЛОБАЛЬНЫЕ:
    UFUNCTION(BlueprintCallable, Category = "Animation")
    FORCEINLINE ES1P_WeaponType GetCurrentWeaponType() const { return CurrentWeaponType; } // Исправлено

    UFUNCTION(BlueprintCallable, Category = "Animation")
    ES1P_MovementState GetMovementState() const; // Исправлено

    void EquipWeapon(ES1P_WeaponType NewWeaponType); // Исправлено

protected:
    virtual void BeginPlay() override;

    ES1P_WeaponType CurrentWeaponType;
    // Компоненты
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArmComponent;

    // Состояния персонажа
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsSprinting;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsCrouching;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    EWeaponType CurrentWeaponType;

    // Настройки движения
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float WalkSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintSpeed;

    // Ввод
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

    // Вспомогательные функции
    void UpdateMovementSpeed();
    bool CanSprint() const;

public:
    // Геттеры для анимаций
    UFUNCTION(BlueprintCallable, Category = "Animation")
    FORCEINLINE float GetCurrentSpeed() const { return GetVelocity().Size2D(); }

    UFUNCTION(BlueprintCallable, Category = "Animation")
    FORCEINLINE bool GetIsCrouching() const { return bIsCrouching; }

    UFUNCTION(BlueprintCallable, Category = "Animation")
    FORCEINLINE bool GetIsSprinting() const { return bIsSprinting; }

    UFUNCTION(BlueprintCallable, Category = "Animation")
    bool GetIsInAir() const;

    UFUNCTION(BlueprintCallable, Category = "Animation")
    FORCEINLINE EWeaponType GetCurrentWeaponType() const { return CurrentWeaponType; }

    // Смена оружия
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SwitchToPistol();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SwitchToRifle();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SwitchToUnarmed();
};