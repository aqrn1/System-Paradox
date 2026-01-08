#include "System1ParadoxCharacter.h"
#include "Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Engine/Engine.h"
#include "S1P_Types.h"
#include "FPSAnimInstance.h"

ASystem1ParadoxCharacter::ASystem1ParadoxCharacter() {
    PrimaryActorTick.bCanEverTick = true;

    // Настройка камеры
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->TargetArmLength = 0.0f;
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
    CameraComponent->bUsePawnControlRotation = false;
    CameraComponent->SetFieldOfView(90.0f);

    bUseControllerRotationYaw = true;
    bUseControllerRotationPitch = true;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 600.0f;
    GetCharacterMovement()->AirControl = 0.2f;
    GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

    WalkSpeed = 600.0f;
    SprintSpeed = 900.0f;
    bIsSprinting = false;
    bIsCrouching = false;
    CurrentWeaponType = ES1P_WeaponType::Unarmed;
    CurrentWeapon = nullptr;
}

void ASystem1ParadoxCharacter::BeginPlay() {
    Super::BeginPlay();
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    SpawnDefaultWeapon();

    if (GEngine) {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("🎮 SYSTEM1PARADOX: AAA Character Initialized"));
    }
}

void ASystem1ParadoxCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (bIsSprinting && !CanSprint()) StopSprint();

    static float DebugTimer = 0.0f;
    DebugTimer += DeltaTime;
    if (DebugTimer >= 2.0f) {
        PrintDebugInfo();
        DebugTimer = 0.0f;
    }
}

void ASystem1ParadoxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);

    // Привязка входных данных
    PlayerInputComponent->BindAxis("MoveForward", this, &ASystem1ParadoxCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASystem1ParadoxCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &ASystem1ParadoxCharacter::Turn);
    PlayerInputComponent->BindAxis("LookUp", this, &ASystem1ParadoxCharacter::LookUp);

    PlayerInputComponent->BindAction("Weapon1", IE_Pressed, this, &ASystem1ParadoxCharacter::SwitchToUnarmed);
    PlayerInputComponent->BindAction("Weapon2", IE_Pressed, this, &ASystem1ParadoxCharacter::SwitchToPistol);
    PlayerInputComponent->BindAction("Weapon3", IE_Pressed, this, &ASystem1ParadoxCharacter::SwitchToRifle);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASystem1ParadoxCharacter::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASystem1ParadoxCharacter::StopJump);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASystem1ParadoxCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASystem1ParadoxCharacter::StopSprint);
    PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASystem1ParadoxCharacter::StartCrouch);
    PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASystem1ParadoxCharacter::StopCrouch);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASystem1ParadoxCharacter::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASystem1ParadoxCharacter::StopFire);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ASystem1ParadoxCharacter::ReloadWeapon);
    PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ASystem1ParadoxCharacter::StartAim);
    PlayerInputComponent->BindAction("Aim", IE_Released, this, &ASystem1ParadoxCharacter::StopAim);
    PlayerInputComponent->BindKey(EKeys::One, IE_Pressed, this, &ASystem1ParadoxCharacter::SwitchToUnarmed);
    PlayerInputComponent->BindKey(EKeys::Two, IE_Pressed, this, &ASystem1ParadoxCharacter::SwitchToPistol);
    PlayerInputComponent->BindKey(EKeys::Three, IE_Pressed, this, &ASystem1ParadoxCharacter::SwitchToRifle);
    PlayerInputComponent->BindKey(EKeys::Four, IE_Pressed, this, &ASystem1ParadoxCharacter::SwitchToMelee);

    UE_LOG(LogTemp, Warning, TEXT("[DEBUG] Input bindings setup complete"));
}

// ==================== ФУНКЦИИ ДВИЖЕНИЯ ====================
void ASystem1ParadoxCharacter::MoveForward(float Value) {
    if (Value != 0.0f && Controller) {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void ASystem1ParadoxCharacter::MoveRight(float Value) {
    if (Value != 0.0f && Controller) {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void ASystem1ParadoxCharacter::LookUp(float Value) { AddControllerPitchInput(Value); }
void ASystem1ParadoxCharacter::Turn(float Value) { AddControllerYawInput(Value); }
void ASystem1ParadoxCharacter::StartJump() { Jump(); }
void ASystem1ParadoxCharacter::StopJump() { StopJumping(); }

// ==================== СПРИНТ ====================
void ASystem1ParadoxCharacter::StartSprint() {
    if (CanSprint()) {
        bIsSprinting = true;
        UpdateMovementSpeed();
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("⚡ SPRINTING"));
    }
}

void ASystem1ParadoxCharacter::StopSprint() {
    bIsSprinting = false;
    UpdateMovementSpeed();
}

bool ASystem1ParadoxCharacter::CanSprint() const {
    return !bIsCrouching && GetCharacterMovement()->IsMovingOnGround();
}

// ==================== ПРИСЕДАНИЕ ====================
void ASystem1ParadoxCharacter::StartCrouch() {
    if (!bIsCrouching) {
        bIsCrouching = true;
        Crouch();
        UpdateMovementSpeed();
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("⬇ CROUCHING"));
    }
}

void ASystem1ParadoxCharacter::StopCrouch() {
    if (bIsCrouching) {
        bIsCrouching = false;
        UnCrouch();
        UpdateMovementSpeed();
    }
}

void ASystem1ParadoxCharacter::UpdateMovementSpeed() {
    if (bIsCrouching)
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * 0.5f;
    else if (bIsSprinting)
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    else
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

// ==================== ОРУЖИЕ ====================
void ASystem1ParadoxCharacter::StartFire() {
    if (CurrentWeapon) CurrentWeapon->StartFire();
}

void ASystem1ParadoxCharacter::StopFire() {
    if (CurrentWeapon) CurrentWeapon->StopFire();
}

void ASystem1ParadoxCharacter::ReloadWeapon() {
    if (CurrentWeapon) CurrentWeapon->Reload();
}

void ASystem1ParadoxCharacter::StartAim() {
    if (CameraComponent) {
        CameraComponent->SetFieldOfView(75.0f);
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("🎯 AIM: ON"));
    }
}

void ASystem1ParadoxCharacter::StopAim() {
    if (CameraComponent) {
        CameraComponent->SetFieldOfView(90.0f);
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("🎯 AIM: OFF"));
    }
}

// ВАША ФУНКЦИЯ SpawnDefaultWeapon (ОСТАВЛЯЕМ ЕЕ КАК ЕСТЬ)
void ASystem1ParadoxCharacter::SpawnDefaultWeapon() {
    if (!DefaultWeaponClass) {
        UE_LOG(LogTemp, Error, TEXT("DefaultWeaponClass is NULL!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("[DEBUG] Spawning weapon..."));

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();

    CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(
        DefaultWeaponClass,
        GetActorLocation(),
        GetActorRotation(),
        SpawnParams
    );

    if (CurrentWeapon) {
        // ПРИКРЕПИТЬ ОРУЖИЕ К РУКЕ
        FName SocketName = TEXT("hand_r"); // или "WeaponSocket"
        CurrentWeapon->AttachToComponent(
            GetMesh(),
            FAttachmentTransformRules::SnapToTargetNotIncludingScale, // ИЗМЕНИЛИ!
            SocketName
        );

        // Настройка положения оружия в руке
        CurrentWeapon->SetActorRelativeLocation(FVector(-11.0f, 8.0f, -6.0f));
        CurrentWeapon->SetActorRelativeRotation(FRotator(0.0f, 90.0f, 0.0f)); // Yaw=90 чтобы повернуть
        // ★★★★★★★★★★★★★★★★★★★★★★★

        UE_LOG(LogTemp, Warning, TEXT("[DEBUG] Weapon attached and transformed"));

        UE_LOG(LogTemp, Warning, TEXT("[DEBUG] Weapon spawned and attached to socket: %s"),
            *SocketName.ToString());
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("[DEBUG] Failed to spawn weapon!"));
    }
}

// ==================== ПЕРЕКЛЮЧЕНИЕ ОРУЖИЯ ====================
void ASystem1ParadoxCharacter::SwitchToPistol() { EquipWeapon(ES1P_WeaponType::Pistol); }
void ASystem1ParadoxCharacter::SwitchToRifle() { EquipWeapon(ES1P_WeaponType::Rifle); }
void ASystem1ParadoxCharacter::SwitchToMelee() { EquipWeapon(ES1P_WeaponType::Melee); }
void ASystem1ParadoxCharacter::SwitchToUnarmed() { EquipWeapon(ES1P_WeaponType::Unarmed); }

void ASystem1ParadoxCharacter::EquipWeapon(ES1P_WeaponType NewWeaponType) {
    CurrentWeaponType = NewWeaponType;

    // ОТЛАДКА: выводим в лог
    UE_LOG(LogTemp, Warning, TEXT("[DEBUG] EquipWeapon called: %s"),
        *UEnum::GetValueAsString(NewWeaponType));

    if (UFPSAnimInstance* AnimInstance = GetFPSAnimInstance()) {
        // Сбросить все Alpha
        AnimInstance->AnimState.UnarmedAlpha = 0.0f;
        AnimInstance->AnimState.PistolAlpha = 0.0f;
        AnimInstance->AnimState.RifleAlpha = 0.0f;

        // Установить нужную Alpha
        switch (NewWeaponType) {
        case ES1P_WeaponType::Unarmed:
            AnimInstance->AnimState.UnarmedAlpha = 1.0f;
            UE_LOG(LogTemp, Warning, TEXT("[DEBUG] Setting UnarmedAlpha = 1.0"));
            break;

        case ES1P_WeaponType::Pistol:
            AnimInstance->AnimState.PistolAlpha = 1.0f;
            UE_LOG(LogTemp, Warning, TEXT("[DEBUG] Setting PistolAlpha = 1.0"));
            // Спавним оружие
            if (!CurrentWeapon) {
                SpawnDefaultWeapon();
            }
            break;

        case ES1P_WeaponType::Rifle:
            AnimInstance->AnimState.RifleAlpha = 1.0f;
            UE_LOG(LogTemp, Warning, TEXT("[DEBUG] Setting RifleAlpha = 1.0"));
            // Для винтовки тоже спавним
            if (!CurrentWeapon) {
                SpawnDefaultWeapon();
            }
            break;
        }
    }
}

// ==================== ГЕТТЕРЫ ====================
float ASystem1ParadoxCharacter::GetCurrentSpeed() const
{
    return GetVelocity().Size2D();
}

bool ASystem1ParadoxCharacter::GetIsCrouching() const { return bIsCrouching; }
bool ASystem1ParadoxCharacter::GetIsSprinting() const { return bIsSprinting; }
bool ASystem1ParadoxCharacter::GetIsInAir() const { return GetCharacterMovement()->IsFalling(); }
ES1P_WeaponType ASystem1ParadoxCharacter::GetCurrentWeaponType() const { return CurrentWeaponType; }

ES1P_MovementState ASystem1ParadoxCharacter::GetMovementState() const
{
    if (GetIsInAir()) return ES1P_MovementState::Jumping;
    else if (bIsCrouching) return ES1P_MovementState::Crouching;
    else if (bIsSprinting && GetCurrentSpeed() > 100.0f) return ES1P_MovementState::Sprinting;
    else if (GetCurrentSpeed() > 10.0f) return ES1P_MovementState::Walking;
    return ES1P_MovementState::Idle;
}

UFPSAnimInstance* ASystem1ParadoxCharacter::GetFPSAnimInstance() const
{
    if (!GetMesh()) return nullptr;
    return Cast<UFPSAnimInstance>(GetMesh()->GetAnimInstance());
}

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
            GetCurrentSpeed(), *WeaponStr,
            bIsSprinting ? TEXT("Yes") : TEXT("No"),
            bIsCrouching ? TEXT("Yes") : TEXT("No"),
            (int32)GetMovementState()
        );
        GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Cyan, DebugMsg);
    }
}
