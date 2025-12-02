#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

UCLASS()
class SYSTEM1PARADOX_API UCharacterAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    // Обновляемые извне переменные состояния
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float Speed = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bIsInAir = false;

    UPROPERTY(BlueprintReadOnly, Category = "Weapon")
    FName CurrentWeaponType = TEXT("Unarmed"); // "Unarmed", "Pistol", "Rifle"

    UPROPERTY(BlueprintReadOnly, Category = "Action")
    bool bIsReloading = false;

    // Референсы на анимации (заполняются в редакторе)
    // Пример для одного состояния - по аналогии добавьте все свои
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Pistol")
    UAnimSequence* Pistol_Idle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Pistol")
    UAnimSequence* Pistol_Jog_Fwd;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Rifle")
    UAnimSequence* Rifle_Walk_Fwd;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Unarmed")
    UAnimSequence* Unarmed_Jog_Fwd;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations|Actions")
    UAnimMontage* Montage_HitReact_Front;

    // Функция для запуска анимации действия (например, реакции на удар)
    UFUNCTION(BlueprintCallable, Category = "Animations")
    void PlayHitReact(FName Direction = TEXT("Front"));
};