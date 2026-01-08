#include "System1ParadoxCharacter.h"

// UE
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"

// Game
#include "Weapon.h"
#include "FPSAnimInstance.h"

ASystem1ParadoxCharacter::ASystem1ParadoxCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->TargetArmLength = 0.f;
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SetRelativeLocation(FVector(0, 0, 70));

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringArmComponent);
    CameraComponent->bUsePawnControlRotation = false;
    CameraComponent->SetFieldOfView(90.f);

    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ASystem1ParadoxCharacter::BeginPlay()
{
    Super::BeginPlay();
    SpawnDefaultWeapon();
}

void ASystem1ParadoxCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASystem1ParadoxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    PlayerInputComponent->BindAxis("MoveForward", this, &ASystem1ParadoxCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASystem1ParadoxCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &ASystem1ParadoxCharacter::Turn);
    PlayerInputComponent->BindAxis("LookUp", this, &ASystem1ParadoxCharacter::LookUp);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASystem1ParadoxCharacter::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASystem1ParadoxCharacter::StopJump);

    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASystem1ParadoxCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASystem1ParadoxCharacter::StopSprint);

    PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASystem1ParadoxCharacter::StartCrouch);
    PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASystem1ParadoxCharacter::StopCrouch);
}

// ===== MOVEMENT =====

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

void ASystem1ParadoxCharacter::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void ASystem1ParadoxCharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

// ===== STATES =====

void ASystem1ParadoxCharacter::StartJump() { Jump(); }
void ASystem1ParadoxCharacter::StopJump() { StopJumping(); }

void ASystem1ParadoxCharacter::StartSprint()
{
    if (CanSprint())
    {
        bIsSprinting = true;
        UpdateMovementSpeed();
    }
}

void ASystem1ParadoxCharacter::StopSprint()
{
    bIsSprinting = false;
    UpdateMovementSpeed();
}

void ASystem1ParadoxCharacter::StartCrouch()
{
    bIsCrouching = true;
    Crouch();
    UpdateMovementSpeed();
}

void ASystem1ParadoxCharacter::StopCrouch()
{
    bIsCrouching = false;
    UnCrouch();
    UpdateMovementSpeed();
}

bool ASystem1ParadoxCharacter::CanSprint() const
{
    return !bIsCrouching && GetCharacterMovement()->IsMovingOnGround();
}

void ASystem1ParadoxCharacter::UpdateMovementSpeed()
{
    if (bIsCrouching)
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * 0.5f;
    else if (bIsSprinting)
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    else
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

// ===== WEAPON =====

void ASystem1ParadoxCharacter::SpawnDefaultWeapon()
{
    if (!DefaultWeaponClass) return;

    CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultWeaponClass);
    if (CurrentWeapon)
    {
        CurrentWeapon->AttachToComponent(GetMesh(),
            FAttachmentTransformRules::SnapToTargetNotIncludingScale,
            TEXT("WeaponSocket"));
    }
}

void ASystem1ParadoxCharacter::EquipWeapon(ES1P_WeaponType NewWeaponType)
{
    CurrentWeaponType = NewWeaponType;
}

// ===== WEAPON STATE CHECKS =====

bool ASystem1ParadoxCharacter::IsWeaponFiring() const
{
    return CurrentWeapon && CurrentWeapon->IsFiring();
}

bool ASystem1ParadoxCharacter::IsWeaponFiring() const
{
    return CurrentWeapon ? CurrentWeapon->IsFiring() : false;
}

bool ASystem1ParadoxCharacter::IsWeaponReloading() const
{
    return CurrentWeapon && CurrentWeapon->IsReloading();
}

bool ASystem1ParadoxCharacter::IsWeaponReloading() const
{
    return CurrentWeapon ? CurrentWeapon->IsReloading() : false;
}

bool ASystem1ParadoxCharacter::IsWeaponAiming() const
{
    return CurrentWeapon && CurrentWeapon->IsAiming();
}

bool ASystem1ParadoxCharacter::IsWeaponAiming() const
{
    return CurrentWeapon ? CurrentWeapon->IsAiming() : false;
}
// ===== GETTERS =====

float ASystem1ParadoxCharacter::GetCurrentSpeed() const
{
    return GetVelocity().Size();
}

bool ASystem1ParadoxCharacter::GetIsCrouching() const { return bIsCrouching; }
bool ASystem1ParadoxCharacter::GetIsSprinting() const { return bIsSprinting; }
bool ASystem1ParadoxCharacter::GetIsInAir() const { return GetCharacterMovement()->IsFalling(); }

UFPSAnimInstance* ASystem1ParadoxCharacter::GetFPSAnimInstance() const
{
    return Cast<UFPSAnimInstance>(GetMesh()->GetAnimInstance());
}
