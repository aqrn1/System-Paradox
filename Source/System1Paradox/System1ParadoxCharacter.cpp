#include "System1ParadoxCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"
#include "Weapon.h"
#include "System1ParadoxHUD.h"

ASystem1ParadoxCharacter::ASystem1ParadoxCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->TargetArmLength = 0.0f;
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
    CameraComponent->bUsePawnControlRotation = false;
    CameraComponent->SetFieldOfView(90.0f);

    bUseControllerRotationPitch = true;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 300.0f;
    GetCharacterMovement()->AirControl = 0.2f;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    // === НАСТРОЙКИ ТОРМОЖЕНИЯ (ДОБАВИЛИ) ===
    GetCharacterMovement()->BrakingDecelerationWalking = WalkingDeceleration;
    GetCharacterMovement()->GroundFriction = 8.0f;
    GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
    GetCharacterMovement()->MaxAcceleration = 2048.0f;
    GetCharacterMovement()->bUseSeparateBrakingFriction = false;
    // ======================================

    GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

    // Инициализация новых переменных
    SprintMultiplier = 1.5f;
    CrouchSpeed = 200.0f;

    // Начальные настройки скорости
    UpdateMovementSpeed();
}

void ASystem1ParadoxCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Создаем оружие при старте игры
    if (WeaponClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();

        CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnParams);
        if (CurrentWeapon)
        {
            CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("hand_r"));
        }
    }

    // Создаем оружие при старте игры
    if (WeaponClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();

        CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnParams);
        if (CurrentWeapon)
        {
            // Прикрепляем к кости руки
            CurrentWeapon->AttachToComponent(
                GetMesh(),
                FAttachmentTransformRules::SnapToTargetNotIncludingScale,
                FName("hand_r")  // Кость правой руки
            );

            // Настрой позицию если нужно
            // CurrentWeapon->SetActorRelativeLocation(FVector(10.0f, 5.0f, -5.0f));
            // CurrentWeapon->SetActorRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
        }
    }

}

void ASystem1ParadoxCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    // Пустая реализация, но она нужна для компиляции
}

void ASystem1ParadoxCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Автоматически выключаем спринт если условия изменились
    if (bIsSprinting && !CanSprint())
    {
        StopSprint();
    }

    // Обновляем HUD каждый кадр
    UpdateHUD();

    UpdateAnimationParameters();

    // Отладочная информация
    if (GEngine)
    {
        FString AmmoInfo = CurrentWeapon ? FString::Printf(TEXT("Ammo: %d"), CurrentWeapon->CurrentAmmo) : TEXT("Ammo: No Weapon");
        FString HealthInfo = FString::Printf(TEXT("Health: %.0f"), CurrentHealth);
        FString DebugString = FString::Printf(TEXT("Speed: %.0f | %s | %s"),
            GetVelocity().Size(),
            *HealthInfo,
            *AmmoInfo);

        GEngine->AddOnScreenDebugMessage(1, 0, FColor::Green, DebugString);
    }
    
  

}

void ASystem1ParadoxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ASystem1ParadoxCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASystem1ParadoxCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &ASystem1ParadoxCharacter::Turn);
    PlayerInputComponent->BindAxis("LookUp", this, &ASystem1ParadoxCharacter::LookUp);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASystem1ParadoxCharacter::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASystem1ParadoxCharacter::StopJump);

    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASystem1ParadoxCharacter::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASystem1ParadoxCharacter::StopFire);

    PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ASystem1ParadoxCharacter::StartReload);

    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASystem1ParadoxCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASystem1ParadoxCharacter::StopSprint);

    // Приседание (удерживать Ctrl)
    PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASystem1ParadoxCharacter::StartCrouch);
    PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASystem1ParadoxCharacter::StopCrouch);
}

void ASystem1ParadoxCharacter::UpdateAnimationParameters()
{
    // Получаем AnimInstance из Mesh
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (!AnimInstance) return;

    // 1. Передаем скорость
    FProperty* SpeedProp = AnimInstance->GetClass()->FindPropertyByName(TEXT("Speed"));
    if (SpeedProp)
    {
        // Получаем указатель на переменную Speed в AnimInstance
        float* SpeedValue = SpeedProp->ContainerPtrToValuePtr<float>(AnimInstance);
        if (SpeedValue)
        {
            *SpeedValue = GetVelocity().Size(); // Устанавливаем текущую скорость
        }
    }

    // 2. Передаем состояние приседания
    FProperty* CrouchProp = AnimInstance->GetClass()->FindPropertyByName(TEXT("bIsCrouching"));
    if (!CrouchProp)
    {
        // Попробуем другое имя (иногда используют IsCrouching)
        CrouchProp = AnimInstance->GetClass()->FindPropertyByName(TEXT("IsCrouching"));
    }

    if (CrouchProp)
    {
        bool* CrouchValue = CrouchProp->ContainerPtrToValuePtr<bool>(AnimInstance);
        if (CrouchValue)
        {
            *CrouchValue = bIsCrouching;
        }
    }

    // 3. Передаем состояние спринта
    FProperty* SprintProp = AnimInstance->GetClass()->FindPropertyByName(TEXT("bIsSprinting"));
    if (!SprintProp)
    {
        SprintProp = AnimInstance->GetClass()->FindPropertyByName(TEXT("IsSprinting"));
    }

    if (SprintProp)
    {
        bool* SprintValue = SprintProp->ContainerPtrToValuePtr<bool>(AnimInstance);
        if (SprintValue)
        {
            *SprintValue = bIsSprinting;
        }
    }

    // 4. Передаем состояние в воздухе (опционально)
    FProperty* InAirProp = AnimInstance->GetClass()->FindPropertyByName(TEXT("bIsInAir"));
    if (!InAirProp)
    {
        InAirProp = AnimInstance->GetClass()->FindPropertyByName(TEXT("IsInAir"));
    }

    if (InAirProp)
    {
        bool* InAirValue = InAirProp->ContainerPtrToValuePtr<bool>(AnimInstance);
        if (InAirValue)
        {
            *InAirValue = GetCharacterMovement()->IsFalling();
        }
    }
}


void ASystem1ParadoxCharacter::MoveForward(float Value)
{
    if (Controller != nullptr && Value != 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void ASystem1ParadoxCharacter::MoveRight(float Value)
{
    if (Controller != nullptr && Value != 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void ASystem1ParadoxCharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

void ASystem1ParadoxCharacter::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void ASystem1ParadoxCharacter::StartJump()
{
    Jump();
}

void ASystem1ParadoxCharacter::StopJump()
{
    StopJumping();
}

void ASystem1ParadoxCharacter::StartFire()
{
    if (CurrentWeapon && !bIsFiring)
    {
        bIsFiring = true;
        CurrentWeapon->StartFire();
    }
}

void ASystem1ParadoxCharacter::StopFire()
{
    if (bIsFiring)
    {
        bIsFiring = false;
        CurrentWeapon->StopFire();
    }
}

void ASystem1ParadoxCharacter::ReloadWeapon()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->Reload();
    }
}

void ASystem1ParadoxCharacter::StartReload()
{
    ReloadWeapon();
}

ASystem1ParadoxHUD* ASystem1ParadoxCharacter::GetSystemHUD() const
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC)
    {
        return Cast<ASystem1ParadoxHUD>(PC->GetHUD());
    }
    return nullptr;
}

void ASystem1ParadoxCharacter::UpdateHUD()
{
    ASystem1ParadoxHUD* HUD = GetSystemHUD();
    if (HUD && CurrentWeapon)
    {
        HUD->UpdateHealth(CurrentHealth);
        HUD->UpdateAmmo(CurrentWeapon->CurrentAmmo, CurrentWeapon->MaxAmmo);
    }
}

void ASystem1ParadoxCharacter::StartSprint()
{
    if (CanSprint())
    {
        bIsSprinting = true;
        UpdateMovementSpeed();

        // Отладочное сообщение (английский)
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("SPRINT: ON"));
        }
    }
}

void ASystem1ParadoxCharacter::StopSprint()
{
    bIsSprinting = false;
    UpdateMovementSpeed();

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("SPRINT: OFF"));
    }
}

bool ASystem1ParadoxCharacter::CanSprint() const
{
    // Можно спринтовать только когда:
    // 1. Не присели
    // 2. Двигаемся вперед
    // 3. Не в воздухе
    // 4. Не перезаряжаемся (если есть оружие)

    return !bIsCrouching &&
        GetVelocity().Size() > 10.0f &&
        !GetCharacterMovement()->IsFalling();
}

void ASystem1ParadoxCharacter::StartCrouch()
{
    if (!bIsCrouching)
    {
        bIsCrouching = true;
        Crouch();

        UpdateMovementSpeed();

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("CROUCH: ON"));
        }
    }
}

void ASystem1ParadoxCharacter::StopCrouch()
{
    if (bIsCrouching)
    {
        bIsCrouching = false;
        UnCrouch();

        UpdateMovementSpeed();

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("CROUCH: OFF"));
        }
    }
}

void ASystem1ParadoxCharacter::UpdateMovementSpeed()
{
    if (bIsCrouching)
    {
        // В приседе
        GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
        GetCharacterMovement()->BrakingDecelerationWalking = CrouchingDeceleration;
    }
    else if (bIsSprinting)
    {
        // Спринт
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed * SprintMultiplier;
        GetCharacterMovement()->BrakingDecelerationWalking = SprintingDeceleration;
    }
    else
    {
        // Обычная ходьба/бег
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
        GetCharacterMovement()->BrakingDecelerationWalking = WalkingDeceleration;
    }

    // Отладка
    if (GEngine)
    {
        FString SpeedMsg = FString::Printf(TEXT("СКОРОСТЬ: %.0f"), GetCharacterMovement()->MaxWalkSpeed);
        GEngine->AddOnScreenDebugMessage(3, 2.0f, FColor::Cyan, SpeedMsg);
    }
}