#include "System1ParadoxCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"

ASystem1ParadoxCharacter::ASystem1ParadoxCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Создаем и прикрепляем SpringArm
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->TargetArmLength = 300.f; // длина плеча
    SpringArmComponent->bUsePawnControlRotation = true;

    // Создаем и подключаем Камеру
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
    CameraComponent->bUsePawnControlRotation = false; // камера не вращается сама

    // Настройка переменных скорости
    WalkSpeed = 250.f;
    SprintSpeed = 400.f; // Обновите по вашему желанию

    // Настройки движения
    GetCharacterMovement()->bOrientRotationToMovement = false; // вращение за телом
    GetCharacterMovement()->RotationRate = FRotator(0, 540, 0);
    GetCharacterMovement()->JumpZVelocity = JumpHeight;
    GetCharacterMovement()->AirControl = AirControlFactor;

    // Настройка crouch
    GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
    GetCharacterMovement()->CrouchedHalfHeight = 44.f;

    // Инициализация состояния
    bIsSprinting = false;
    bIsCrouching = false;
}

void ASystem1ParadoxCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ASystem1ParadoxCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsSprinting && (GetVelocity().Size() < 10.0f || GetCharacterMovement()->IsFalling()))
    {
        StopSprint();
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
    PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASystem1ParadoxCharacter::StartCrouch);
    PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASystem1ParadoxCharacter::StopCrouch);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASystem1ParadoxCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASystem1ParadoxCharacter::StopSprint);
}

void ASystem1ParadoxCharacter::MoveForward(float Value)
{
    if (Controller && Value != 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRot(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void ASystem1ParadoxCharacter::MoveRight(float Value)
{
    if (Controller && Value != 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRot(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void ASystem1ParadoxCharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value * 0.8f);
}

void ASystem1ParadoxCharacter::Turn(float Value)
{
    AddControllerYawInput(Value * 0.8f);
}

void ASystem1ParadoxCharacter::StartJump()
{
    Jump();
    StopSprint();
}

void ASystem1ParadoxCharacter::StopJump()
{
    StopJumping();
}

void ASystem1ParadoxCharacter::StartCrouch()
{
    if (CanCrouch())
    {
        Crouch();
        bIsCrouching = true;
        GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
        StopSprint();
    }
}

void ASystem1ParadoxCharacter::StopCrouch()
{
    UnCrouch();
    bIsCrouching = false;

    if (bIsSprinting)
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
    else
    {
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    }
}

void ASystem1ParadoxCharacter::StartSprint()
{
    if (!bIsCrouching && (GetVelocity().Size() > 0.1f) && !GetCharacterMovement()->IsFalling())
    {
        bIsSprinting = true;
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void ASystem1ParadoxCharacter::StopSprint()
{
    bIsSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}