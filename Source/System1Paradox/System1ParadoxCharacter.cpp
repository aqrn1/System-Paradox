#include "System1ParadoxCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"

ASystem1ParadoxCharacter::ASystem1ParadoxCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Камера
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->TargetArmLength = 0.0f;
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
    CameraComponent->bUsePawnControlRotation = false;

    // Настройки движения
    bUseControllerRotationYaw = true;
    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->JumpZVelocity = 600.0f;
    GetCharacterMovement()->AirControl = 0.2f;

    // Значения по умолчанию
    WalkSpeed = 600.0f;
    SprintSpeed = 900.0f;
    bIsSprinting = false;
    bIsCrouching = false;
    CurrentWeaponType = EWeaponType::Unarmed;
}

void ASystem1ParadoxCharacter::BeginPlay()
{
    Super::BeginPlay();
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ASystem1ParadoxCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Автоматическое отключение спринта если нельзя спринтовать
    if (bIsSprinting && !CanSprint())
    {
        StopSprint();
    }
}

void ASystem1ParadoxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Движение
    PlayerInputComponent->BindAxis("MoveForward", this, &ASystem1ParadoxCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASystem1ParadoxCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &ASystem1ParadoxCharacter::Turn);
    PlayerInputComponent->BindAxis("LookUp", this, &ASystem1ParadoxCharacter::LookUp);

    // Действия
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASystem1ParadoxCharacter::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASystem1ParadoxCharacter::StopJump);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASystem1ParadoxCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASystem1ParadoxCharacter::StopSprint);
    PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASystem1ParadoxCharacter::StartCrouch);
    PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASystem1ParadoxCharacter::StopCrouch);

    // Оружие
    PlayerInputComponent->BindKey(EKeys::One, IE_Pressed, this, &ASystem1ParadoxCharacter::SwitchToUnarmed);
    PlayerInputComponent->BindKey(EKeys::Two, IE_Pressed, this, &ASystem1ParadoxCharacter::SwitchToPistol);
    PlayerInputComponent->BindKey(EKeys::Three, IE_Pressed, this, &ASystem1ParadoxCharacter::SwitchToRifle);
}

// Движение
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

// Спринт
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

bool ASystem1ParadoxCharacter::CanSprint() const
{
    return !bIsCrouching && GetCharacterMovement()->IsMovingOnGround();
}

// Приседание
void ASystem1ParadoxCharacter::StartCrouch()
{
    if (!bIsCrouching)
    {
        bIsCrouching = true;
        Crouch();
        UpdateMovementSpeed();
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

void ASystem1ParadoxCharacter::UpdateMovementSpeed()
{
    if (bIsCrouching)
    {
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * 0.5f;
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

bool ASystem1ParadoxCharacter::GetIsInAir() const
{
    return GetCharacterMovement() ? GetCharacterMovement()->IsFalling() : false;
}

// Смена оружия
void ASystem1ParadoxCharacter::SwitchToPistol()
{
    CurrentWeaponType = EWeaponType::Pistol;
}

void ASystem1ParadoxCharacter::SwitchToRifle()
{
    CurrentWeaponType = EWeaponType::Rifle;
}

void ASystem1ParadoxCharacter::SwitchToUnarmed()
{
    CurrentWeaponType = EWeaponType::Unarmed;
}