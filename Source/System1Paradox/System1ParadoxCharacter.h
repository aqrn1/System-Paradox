#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "System1ParadoxCharacter.generated.h"

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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* SpringArmComponent;

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float WalkSpeed = 400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintSpeed = 600.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsSprinting = false;
};