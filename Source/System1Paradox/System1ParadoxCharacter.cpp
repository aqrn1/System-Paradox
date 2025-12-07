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

    // Настройки торможения
    GetCharacterMovement()->BrakingDecelerationWalking = WalkingDeceleration;
    GetCharacterMovement()->GroundFriction = 8.0f;
    GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
    GetCharacterMovement()->MaxAcceleration = 2048.0f;
    GetCharacterMovement()->bUseSeparateBrakingFriction = false;

    GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

    // Инициализация новых переменных
    SprintMultiplier = 1.5f;
    CrouchSpeed = 200.0f;
}

void ASystem1ParadoxCharacter::BeginPlay()
{
    Super::BeginPlay();

    // ДЕБАГ: Проверяем компоненты
    if (!CameraComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("CameraComponent is NULL!"));
        return;
    }

    // Создаем оружие при старте игры
    if (WeaponClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();

        CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnParams);
        if (CurrentWeapon && CameraComponent)
        {
            // ВАЖНО: Сначала задаем позицию, потом прикрепляем
            FVector SpawnLocation = CameraComponent->GetComponentLocation() +
                CameraComponent->GetForwardVector() * 100.0f;
            FRotator SpawnRotation = CameraComponent->GetComponentRotation();

            CurrentWeapon->SetActorLocation(SpawnLocation);
            CurrentWeapon->SetActorRotation(SpawnRotation);

            // 🔴 ПРИКРЕПЛЯЕМ К КАМЕРЕ (FPS стиль)
            CurrentWeapon->AttachToComponent(
                CameraComponent,
                FAttachmentTransformRules::SnapToTargetIncludingScale,
                NAME_None
            );

            // 🟢 НОВЫЕ ЗНАЧЕНИЯ ПОЗИЦИИ ОРУЖИЯ:
            CurrentWeapon->SetActorRelativeLocation(FVector(50.0f, 20.0f, -20.0f));
            CurrentWeapon->SetActorRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
            CurrentWeapon->SetActorScale3D(FVector(1.0f));

            // 🔴 АЛЬТЕРНАТИВА: Прикрепляем к кости скелета (если к камере не работает)
            // Uncomment если оружие все еще не видно:
            /*
            if (GetMesh())
            {
                // Попробуем прикрепить к правой руке
                FName WeaponSocket = TEXT("hand_r");  // Правая рука
                if (GetMesh()->DoesSocketExist(WeaponSocket))
                {
                    CurrentWeapon->AttachToComponent(
                        GetMesh(),
                        FAttachmentTransformRules::SnapToTargetIncludingScale,
                        WeaponSocket
                    );
                    CurrentWeapon->SetActorRelativeLocation(FVector(10.0f, 5.0f, -5.0f));
                    CurrentWeapon->SetActorRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
                }
            }
            */

            // ДЕБАГ: Сообщение для проверки
            if (GEngine)
            {
                FVector WeaponPos = CurrentWeapon->GetActorLocation();
                FString DebugMsg = FString::Printf(
                    TEXT("🔫 WEAPON: Attached to Camera\nPosition: X=%.1f, Y=%.1f, Z=%.1f"),
                    WeaponPos.X, WeaponPos.Y, WeaponPos.Z
                );
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, DebugMsg);
            }
        }
        else
        {
            // Ошибка
            FString ErrorMsg = TEXT("❌ ERROR: Failed to create weapon!");
            if (!CameraComponent) ErrorMsg += TEXT(" (Camera is null)");
            if (!CurrentWeapon) ErrorMsg += TEXT(" (Weapon spawn failed)");

            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, ErrorMsg);
        }
    }
    else
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
            TEXT("❌ WeaponClass not set! Check BP_System1ParadoxCharacter settings"));
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

    // Обновляем параметры анимации
    UpdateAnimationParameters();

    // 🔴 ДЕБАГ: Показываем позицию оружия каждые 2 секунды
    static float WeaponDebugTimer = 0.0f;
    WeaponDebugTimer += DeltaTime;
    if (WeaponDebugTimer >= 2.0f)
    {
        if (CurrentWeapon)
        {
            FVector WeaponPos = CurrentWeapon->GetActorLocation();  // 🟢 ИСПРАВЛЕНО!
            FString PosMsg = FString::Printf(
                TEXT("🔫 Weapon Pos: (%.1f, %.1f, %.1f)"),
                WeaponPos.X, WeaponPos.Y, WeaponPos.Z
            );
            if (GEngine) GEngine->AddOnScreenDebugMessage(10, 2.1f, FColor::Cyan, PosMsg);
        }
        WeaponDebugTimer = 0.0f;
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

    // Спринт (удерживать Shift)
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASystem1ParadoxCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASystem1ParadoxCharacter::StopSprint);

    // Приседание (удерживать Ctrl)
    PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASystem1ParadoxCharacter::StartCrouch);
    PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASystem1ParadoxCharacter::StopCrouch);
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

    return !bIsCrouching &&
        GetVelocity().Size() > 10.0f &&
        !GetCharacterMovement()->IsFalling();
}

void ASystem1ParadoxCharacter::StartCrouch()
{
    if (!bIsCrouching)
    {
        bIsCrouching = true;
        Crouch();  // Встроенная функция Unreal Engine

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
        UnCrouch();  // Встроенная функция

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
        FString SpeedMsg = FString::Printf(TEXT("SPEED: %.0f"), GetCharacterMovement()->MaxWalkSpeed);
        GEngine->AddOnScreenDebugMessage(3, 2.0f, FColor::Cyan, SpeedMsg);
    }
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

// 🔴 ДОБАВЛЯЕМ НОВУЮ ФУНКЦИЮ ДЛЯ ДЕБАГА
void ASystem1ParadoxCharacter::DebugWeaponPosition()
{
    if (!CurrentWeapon || !CameraComponent) return;

    FVector WeaponWorldPos = CurrentWeapon->GetActorLocation();
    FVector CameraWorldPos = CameraComponent->GetComponentLocation();
    FVector RelativePos = WeaponWorldPos - CameraWorldPos;

    // 🟢 ИСПРАВЛЕННЫЙ КОД: Правильное форматирование строки
    FString DebugInfo = FString::Printf(
        TEXT("=== WEAPON DEBUG ===\n") \
        TEXT("Weapon World: (%.1f, %.1f, %.1f)\n") \
        TEXT("Camera World: (%.1f, %.1f, %.1f)\n") \
        TEXT("Relative Offset: (%.1f, %.1f, %.1f)\n") \
        TEXT("Weapon Scale: (%.2f, %.2f, %.2f)"),
        WeaponWorldPos.X, WeaponWorldPos.Y, WeaponWorldPos.Z,
        CameraWorldPos.X, CameraWorldPos.Y, CameraWorldPos.Z,
        RelativePos.X, RelativePos.Y, RelativePos.Z,
        CurrentWeapon->GetActorScale3D().X,
        CurrentWeapon->GetActorScale3D().Y,
        CurrentWeapon->GetActorScale3D().Z
    );

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, DebugInfo);
    }
    UE_LOG(LogTemp, Warning, TEXT("%s"), *DebugInfo);
}