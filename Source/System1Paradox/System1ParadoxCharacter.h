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

    UFUNCTION(BlueprintCallable, Category = "Animation")
    EWeaponType GetCurrentWeaponType() const { return CurrentWeaponType; }

    UFUNCTION(BlueprintCallable, Category = "Animation")
    bool GetIsCrouching() const { return bIsCrouching; }

    UFUNCTION(BlueprintCallable, Category = "Animation")
    bool GetIsSprinting() const { return bIsSprinting; }

    UFUNCTION(BlueprintCallable, Category = "Animation")
    bool GetIsSwitchingWeapon() const { return bIsSwitchingWeapon; }

    // Состояния персонажа
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsSprinting = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsCrouching = false;

    // Система оружия
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    EWeaponType CurrentWeaponType = EWeaponType::Unarmed;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    bool bIsSwitchingWeapon = false;

protected:
    virtual void BeginPlay() override;

    // Компоненты камеры
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* SpringArmComponent;

    // Оружие
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    class AWeapon* CurrentWeapon;

    // 🟢 СТАРАЯ ПЕРЕМЕННАЯ (для совместимости)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<class AWeapon> WeaponClass;

    // 🟢 НОВЫЕ ПЕРЕМЕННЫЕ ДЛЯ СИСТЕМЫ СМЕНЫ ОРУЖИЯ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<class AWeapon> PistolClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<class AWeapon> RifleClass;

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

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void StartFire();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void StopFire();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void ReloadWeapon();

    UFUNCTION(BlueprintCallable, Category = "Input")
    void StartReload();

    // HUD функции
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHUD();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    class ASystem1ParadoxHUD* GetSystemHUD() const;

    // Функции спринта
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StartSprint();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StopSprint();

    // Функции приседания
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StartCrouch();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StopCrouch();

    // Вспомогательные функции
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void UpdateMovementSpeed();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool CanSprint() const;

    // Функция для обновления параметров анимации
    void UpdateAnimationParameters();

    // 🟢 ФУНКЦИИ СМЕНЫ ОРУЖИЯ
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SwitchToPistol();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SwitchToRifle();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SwitchToUnarmed();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void EquipWeapon(EWeaponType NewWeaponType);

    // 🟢 ФУНКЦИЯ ОБНОВЛЕНИЯ АНИМАЦИЙ
    UFUNCTION(BlueprintCallable, Category = "Animation")
    void UpdateWeaponAnimations();

    // 🟢 ДЕБАГ ФУНКЦИЯ
    UFUNCTION(BlueprintCallable, Category = "Debug")
    void DebugWeaponPosition();

    // Таймер для автоматической стрельбы
    FTimerHandle FireTimerHandle;
    bool bIsFiring = false;

    // Свойства движения
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float WalkSpeed = 400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintSpeed = 600.0f;

    // Параметры торможения
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float WalkingDeceleration = 2048.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintingDeceleration = 1024.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float CrouchingDeceleration = 512.0f;

    // Состояния персонажа
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsSprinting = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsCrouching = false;

    // Свойства спринта и приседания
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintMultiplier = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float CrouchSpeed = 200.0f;

    // Настройки позиции оружия
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FVector WeaponOffset = FVector(30.0f, 10.0f, -10.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FRotator WeaponRotation = FRotator(0.0f, 90.0f, 0.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FVector WeaponScale = FVector(0.5f);

    // Здоровье
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float CurrentHealth = 100.0f;

    // 🟢 ПЕРЕМЕННЫЕ ДЛЯ СИСТЕМЫ ОРУЖИЯ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    EWeaponType CurrentWeaponType = EWeaponType::Unarmed;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    bool bIsSwitchingWeapon = false;
};