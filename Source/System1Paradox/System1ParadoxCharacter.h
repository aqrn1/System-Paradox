#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "System1ParadoxCharacter.generated.h"

class UFPSAnimInstance;
class AWeapon;
class UCameraComponent;

UCLASS()
class SYSTEM1PARADOX_API ASystem1ParadoxCharacter : public ACharacter {
    GENERATED_BODY()

public:
    ASystem1ParadoxCharacter();

    // Главный метод для обновления состояния персонажа
    virtual void Tick(float DeltaTime) override;

    // Настройка ввода
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    virtual void BeginPlay() override;

    // Камера персонажа
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* CameraComponent;

    // Текущее оружие
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    AWeapon* CurrentWeapon;

    // Метод для смены оружия
    void EquipWeapon(ES1P_WeaponType NewWeaponType);

    // Метод для обновления скорости движения
    void UpdateMovementSpeed();

    // Методы для управления перемещением
    void MoveForward(float Value);
    void MoveRight(float Value);

    // Методы для управления состоянием оружия
    void SwitchToPistol();
    void SwitchToRifle();
    void SwitchToMelee();
    void SwitchToUnarmed();

    // Получаем текущую скорость
    UFUNCTION(BlueprintCallable, Category = "Animation")
    float GetCurrentSpeed() const;

    // Получаем состояние приседания
    UFUNCTION(BlueprintCallable, Category = "Animation")
    bool GetIsCrouching() const;

    // Получаем текущее оружие
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    AWeapon* GetCurrentWeapon() const;

    // События для переключения состояний оружия
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void StartFire();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void StopFire();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void ReloadWeapon();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void StartAim();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void StopAim();

    // Дополнительные параметры для анимации, такие как наклон прицеливания и т. д.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bIsFiring;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bIsReloading;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bIsAiming;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float FireAnimationTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float ReloadProgress;
};
