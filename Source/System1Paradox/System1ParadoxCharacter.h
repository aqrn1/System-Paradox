#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "System1ParadoxCharacter.generated.h"

// Предварительное объявление класса Weapon чтобы избежать циклических зависимостей
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

    // Компоненты камеры
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* SpringArmComponent;

    // Оружие
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    AWeapon* CurrentWeapon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<class AActor> WeaponClass;

    // Функции ввода
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

    // Функции стрельбы
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void StartFire();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void StopFire();

    // Функции для управления оружием
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void ReloadWeapon();

    UFUNCTION(BlueprintCallable, Category = "Input")
    void StartReload();

    // HUD функции
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHUD();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    class ASystem1ParadoxHUD* GetSystemHUD() const;

    // Таймер для автоматической стрельбы
    FTimerHandle FireTimerHandle;
    bool bIsFiring = false;

    // Sprint функции (пока не активны)
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StartSprint();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StopSprint();

    // Crouch функции (пока не активны)
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StartCrouch();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StopCrouch();

    // Свойства движения
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float WalkSpeed = 400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintSpeed = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float CrouchSpeed = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float CrouchSprintSpeed = 300.0f;

    // Здоровье
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float CurrentHealth = 100.0f;

    // Переменные состояния
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsSprinting = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsCrouching = false;
};