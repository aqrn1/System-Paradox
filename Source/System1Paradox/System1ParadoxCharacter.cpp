// System1ParadoxCharacter.cpp
#include "System1ParadoxCharacter.h"
#include "Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Engine/Engine.h"
#include "S1P_Types.h"
#include "FPSAnimInstance.h"  // ← ДОБАВЬТЕ ЭТО ЗДЕСЬ!

// ==================== КОНСТРУКТОР ====================
ASystem1ParadoxCharacter::ASystem1ParadoxCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // ==================== КАМЕРА AAA ====================
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->TargetArmLength = 0.0f;
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f)); // Высота глаз AAA

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
    CameraComponent->bUsePawnControlRotation = false;
    CameraComponent->SetFieldOfView(90.0f); // Стандартный FOV для FPS

    // ==================== ДВИЖЕНИЕ AAA ====================
    bUseControllerRotationYaw = true;
    bUseControllerRotationPitch = true;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // Плавный поворот
    GetCharacterMovement()->JumpZVelocity = 600.0f;
    GetCharacterMovement()->AirControl = 0.2f;
    GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

    // ==================== ЗНАЧЕНИЯ ПО УМОЛЧАНИЮ ====================
    WalkSpeed = 600.0f;
    SprintSpeed = 900.0f;
    bIsSprinting = false;
    bIsCrouching = false;
    CurrentWeaponType = ES1P_WeaponType::Unarmed; // Исправлено на глобальный тип
    CurrentWeapon = nullptr;
}

// ==================== BEGIN PLAY ====================
void ASystem1ParadoxCharacter::BeginPlay()
{
    Super::BeginPlay();

    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    // Спавн стартового оружия (AAA подход)
    SpawnDefaultWeapon();

    // Debug сообщение
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
            TEXT("🎮 SYSTEM1PARADOX: AAA Character Initialized"));
    }
}

// ==================== TICK ====================
void ASystem1ParadoxCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Автоматическое отключение спринта при невозможности
    if (bIsSprinting && !CanSprint())
    {
        StopSprint();
    }

    // Debug информация каждые 2 секунды
    static float DebugTimer = 0.0f;
    DebugTimer += DeltaTime;
    if (DebugTimer >= 2.0f)
    {
        PrintDebugInfo();
        DebugTimer = 0.0f;
    }
}

// ==================== НАСТРОЙКА ВВОДА ====================
void ASystem1ParadoxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);

    // ==================== ОСИ ДВИЖЕНИЯ ====================
    PlayerInputComponent->BindAxis("MoveForward", this, &ASystem1ParadoxCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASystem1ParadoxCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &ASystem1ParadoxCharacter::Turn);
    PlayerInputComponent->BindAxis("LookUp", this, &ASystem1ParadoxCharacter::LookUp);

    // ==================== ДЕЙСТВИЯ ====================
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASystem1ParadoxCharacter::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASystem1ParadoxCharacter::StopJump);

    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASystem1ParadoxCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASystem1ParadoxCharacter::StopSprint);

    PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASystem1ParadoxCharacter::StartCrouch);
    PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASystem1ParadoxCharacter::StopCrouch);

    // ==================== ОРУЖИЕ (AAA УПРАВЛЕНИЕ) ====================
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASystem1ParadoxCharacter::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASystem1ParadoxCharacter::StopFire);

    PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ASystem1ParadoxCharacter::ReloadWeapon);

    PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ASystem1ParadoxCharacter::StartAim);
    PlayerInputComponent->BindAction("Aim", IE_Released, this, &ASystem1ParadoxCharacter::StopAim);

    // ==================== ПЕРЕКЛЮЧЕНИЕ ОРУЖИЯ ====================
    PlayerInputComponent->BindKey(EKeys::One, IE_Pressed, this, &ASystem1ParadoxCharacter::SwitchToUnarmed);
    PlayerInputComponent->BindKey(EKeys::Two, IE_Pressed, this, &ASystem1ParadoxCharacter::SwitchToPistol);
    PlayerInputComponent->BindKey(EKeys::Three, IE_Pressed, this, &ASystem1ParadoxCharacter::SwitchToRifle);
    PlayerInputComponent->BindKey(EKeys::Four, IE_Pressed, this, &ASystem1ParadoxCharacter::SwitchToMelee);
}

// ==================== ФУНКЦИИ ДВИЖЕНИЯ ====================
void ASystem1ParadoxCharacter::MoveForward(float Value)
{
    if (Value != 0.0f && Controller)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void ASystem1ParadoxCharacter::MoveRight(float Value)
{
    if (Value != 0.0f && Controller)
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

// ==================== СПРИНТ ====================
void ASystem1ParadoxCharacter::StartSprint()
{
    if (CanSprint())
    {
        bIsSprinting = true;
        UpdateMovementSpeed();

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("⚡ SPRINTING"));
        }
    }
}

void ASystem1ParadoxCharacter::StopSprint()
{
    bIsSprinting = false;
    UpdateMovementSpeed();
}

bool ASystem1ParadoxCharacter::CanSprint() const
{
    // AAA логика: можно спринтовать только на земле и не приседая
    return !bIsCrouching && GetCharacterMovement()->IsMovingOnGround();
}

// ==================== ПРИСЕДАНИЕ ====================
void ASystem1ParadoxCharacter::StartCrouch()
{
    if (!bIsCrouching)
    {
        bIsCrouching = true;
        Crouch();
        UpdateMovementSpeed();

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("⬇ CROUCHING"));
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
    }
}

// ==================== ОБНОВЛЕНИЕ СКОРОСТИ ====================
void ASystem1ParadoxCharacter::UpdateMovementSpeed()
{
    if (bIsCrouching)
    {
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * 0.5f; // 50% скорости при приседании
    }
    else if (bIsSprinting)
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
    else
    {
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    }
}

// ==================== СИСТЕМА ОРУЖИЯ AAA ====================


void ASystem1ParadoxCharacter::StopFire()
{
    if (CurrentWeapon)
    {
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


void ASystem1ParadoxCharacter::SpawnDefaultWeapon()
{
    if (DefaultWeaponClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();

        CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultWeaponClass, SpawnParams);
        if (CurrentWeapon && CameraComponent)
        {
            // КРИТИЧЕСКАЯ ПРОБЛЕМА: Возможно оружие прикрепляется к корню, а не к камере
            FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

            // УБЕДИТЕСЬ что прикрепляем к CameraComponent, а не к RootComponent!
            CurrentWeapon->AttachToComponent(CameraComponent, AttachmentRules);

            // AAA позиционирование оружия (как в CS:GO)
            CurrentWeapon->SetActorRelativeLocation(FVector(35.0f, 8.0f, -30.0f));
            CurrentWeapon->SetActorRelativeRotation(FRotator(2.0f, -95.0f, -5.0f));
            CurrentWeapon->SetActorScale3D(FVector(0.8f));

            CurrentWeaponType = ES1P_WeaponType::Pistol;

            // DEBUG: Проверка прикрепления
            UE_LOG(LogTemp, Warning, TEXT("🔫 Weapon attached to: %s"),
                *CurrentWeapon->GetAttachParentActor()->GetName());

            // Проверка позиции
            FVector WeaponWorldPos = CurrentWeapon->GetActorLocation();
            FVector CameraWorldPos = CameraComponent->GetComponentLocation();
            UE_LOG(LogTemp, Warning, TEXT("📏 Weapon Pos: %s, Camera Pos: %s"),
                *WeaponWorldPos.ToString(), *CameraWorldPos.ToString());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ Cannot attach weapon!"));
            UE_LOG(LogTemp, Error, TEXT("CurrentWeapon: %s, CameraComponent: %s"),
                CurrentWeapon ? TEXT("Valid") : TEXT("NULL"),
                CameraComponent ? TEXT("Valid") : TEXT("NULL"));
        }
    }
}

// ==================== ПЕРЕКЛЮЧЕНИЕ ОРУЖИЯ ====================
void ASystem1ParadoxCharacter::SwitchToPistol()
{
    EquipWeapon(ES1P_WeaponType::Pistol); // Исправлено
}

void ASystem1ParadoxCharacter::SwitchToRifle()
{
    EquipWeapon(ES1P_WeaponType::Rifle); // Исправлено
}

void ASystem1ParadoxCharacter::SwitchToMelee()
{
    EquipWeapon(ES1P_WeaponType::Melee); // Исправлено
}

void ASystem1ParadoxCharacter::SwitchToUnarmed()
{
    EquipWeapon(ES1P_WeaponType::Unarmed); // Исправлено
}

void ASystem1ParadoxCharacter::EquipWeapon(ES1P_WeaponType NewWeaponType) // Исправлено
{
    if (CurrentWeaponType == NewWeaponType) return;

    CurrentWeaponType = NewWeaponType;

    // Временное решение - скрыть/показать оружие
    if (CurrentWeapon)
    {
        CurrentWeapon->SetActorHiddenInGame(NewWeaponType == ES1P_WeaponType::Unarmed); // Исправлено
    }

    FString WeaponName;
    switch (NewWeaponType)
    {
    case ES1P_WeaponType::Pistol: WeaponName = TEXT("Pistol"); break;
    case ES1P_WeaponType::Rifle: WeaponName = TEXT("Rifle"); break;
    case ES1P_WeaponType::Melee: WeaponName = TEXT("Melee"); break;
    default: WeaponName = TEXT("Unarmed"); break;
    }

    if (GEngine)
    {
        FString Msg = FString::Printf(TEXT("🔫 Weapon: %s"), *WeaponName);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, Msg);
    }
}

// ==================== ГЕТТЕРЫ ДЛЯ АНИМАЦИЙ ====================
float ASystem1ParadoxCharacter::GetCurrentSpeed() const
{
    float Speed = GetVelocity().Size2D();

    // ВРЕМЕННАЯ ОТЛАДКА
    if (GEngine && Speed > 10.0f)
    {
        FString Msg = FString::Printf(TEXT("C++ Speed: %.0f"), Speed);
        GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Green, Msg);
    }

    return Speed;
}

bool ASystem1ParadoxCharacter::GetIsCrouching() const
{
    return bIsCrouching;
}

bool ASystem1ParadoxCharacter::GetIsSprinting() const
{
    return bIsSprinting;
}

bool ASystem1ParadoxCharacter::GetIsInAir() const
{
    return GetCharacterMovement() ? GetCharacterMovement()->IsFalling() : false;
}

ES1P_WeaponType ASystem1ParadoxCharacter::GetCurrentWeaponType() const // Исправлено
{
    return CurrentWeaponType;
}

ES1P_MovementState ASystem1ParadoxCharacter::GetMovementState() const // Исправлено
{
    if (GetIsInAir())
    {
        return ES1P_MovementState::Jumping; // Исправлено
    }
    else if (bIsCrouching)
    {
        return ES1P_MovementState::Crouching; // Исправлено
    }
    else if (bIsSprinting && GetCurrentSpeed() > 100.0f)
    {
        return ES1P_MovementState::Sprinting; // Исправлено
    }
    else if (GetCurrentSpeed() > 10.0f)
    {
        return ES1P_MovementState::Walking; // Исправлено
    }

    return ES1P_MovementState::Idle; // Исправлено
}

// ==================== DEBUG ФУНКЦИИ ====================
void ASystem1ParadoxCharacter::PrintDebugInfo() const
{
    if (GEngine)
    {
        FString WeaponStr;
        switch (CurrentWeaponType)
        {
        case ES1P_WeaponType::Pistol: WeaponStr = "Pistol"; break;
        case ES1P_WeaponType::Rifle: WeaponStr = "Rifle"; break;
        case ES1P_WeaponType::Melee: WeaponStr = "Melee"; break;
        default: WeaponStr = "Unarmed"; break;
        }

        FString DebugMsg = FString::Printf(
            TEXT("AAA DEBUG | Speed: %.0f | Weapon: %s | Sprint: %s | Crouch: %s | State: %d"),
            GetCurrentSpeed(),
            *WeaponStr,
            bIsSprinting ? TEXT("Yes") : TEXT("No"),
            bIsCrouching ? TEXT("Yes") : TEXT("No"),
            (int32)GetMovementState()
        );

        GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Cyan, DebugMsg);
    }
}

UFPSAnimInstance* ASystem1ParadoxCharacter::GetFPSAnimInstance() const
{
    if (!GetMesh()) return nullptr;

    UAnimInstance* BaseAnim = GetMesh()->GetAnimInstance();
    if (!BaseAnim) return nullptr;

    // Безопасный каст
    return Cast<UFPSAnimInstance>(BaseAnim);
}

void ASystem1ParadoxCharacter::StartAim()
{
    // Временная реализация - уменьшаем FOV для эффекта прицеливания
    if (CameraComponent)
    {
        CameraComponent->SetFieldOfView(FMath::Lerp(90.0f, 75.0f, 0.3f)); // Плавное изменение
        UE_LOG(LogTemp, Warning, TEXT("🎯 AIM: ON (FOV: %.1f)"), CameraComponent->FieldOfView);
    }
}

void ASystem1ParadoxCharacter::StopAim()
{
    if (CameraComponent)
    {
        CameraComponent->SetFieldOfView(FMath::Lerp(75.0f, 90.0f, 0.3f)); // Плавное изменение
        UE_LOG(LogTemp, Warning, TEXT("🎯 AIM: OFF (FOV: %.1f)"), CameraComponent->FieldOfView);
    }
}

void ASystem1ParadoxCharacter::StartFire()
{
    UE_LOG(LogTemp, Warning, TEXT("=== START FIRE CALLED ==="));

    // Debug: Проверка всех условий
    if (!Controller)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ No Controller!"));
        return;
    }

    if (!CurrentWeapon)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ No CurrentWeapon!"));

        // Попробуем найти оружие в дочерних акторах
        TArray<AActor*> AttachedActors;  // <-- ВМЕСТО Children
        GetAttachedActors(AttachedActors);
        UE_LOG(LogTemp, Warning, TEXT("Attached actors: %d"), AttachedActors.Num());
        for (AActor* Child : AttachedActors)
        {
            UE_LOG(LogTemp, Warning, TEXT("  - %s"), *Child->GetName());
        }
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("✅ Weapon found: %s"), *CurrentWeapon->GetName());
    UE_LOG(LogTemp, Warning, TEXT("✅ Weapon Owner: %s"), *GetName());

    // Проверка типа оружия
    UE_LOG(LogTemp, Warning, TEXT("✅ Weapon Type: %d"), (int32)CurrentWeapon->GetWeaponType());

    CurrentWeapon->StartFire();

    // Debug сообщение на экран
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
            FString::Printf(TEXT("🔥 FIRE! Weapon: %s"), *CurrentWeapon->GetName()));
    }
}