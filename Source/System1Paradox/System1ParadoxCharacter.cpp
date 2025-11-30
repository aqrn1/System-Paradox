#include "System1ParadoxCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"

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
    GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

void ASystem1ParadoxCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ASystem1ParadoxCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Îòëàäî÷íàÿ èíôîðìàöèÿ
    if (GEngine)
    {
        FString DebugString = FString::Printf(TEXT("Speed: %.0f | Sprint: %s | Crouch: %s"),
            GetVelocity().Size(),
            bIsSprinting ? TEXT("ON") : TEXT("OFF"),
            bIsCrouching ? TEXT("ON") : TEXT("OFF"));

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

    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASystem1ParadoxCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASystem1ParadoxCharacter::StopSprint);

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

void ASystem1ParadoxCharacter::StartSprint()
{
    UE_LOG(LogTemp, Warning, TEXT("StartSprint called")); // ÄÎÁÀÂÜÒÅ ÝÒÓ ÑÒÐÎÊÓ

    if (!bIsCrouching)
    {
        bIsSprinting = true;
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
        UE_LOG(LogTemp, Warning, TEXT("SPRINT ACTIVATED - Speed: %f"), SprintSpeed);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot sprint while crouching"));
    }
}

void ASystem1ParadoxCharacter::StopSprint()
{
    UE_LOG(LogTemp, Warning, TEXT("StopSprint called")); // ÄÎÁÀÂÜÒÅ ÝÒÓ ÑÒÐÎÊÓ

    bIsSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    UE_LOG(LogTemp, Warning, TEXT("SPRINT DEACTIVATED - Speed: %f"), WalkSpeed);
}

void ASystem1ParadoxCharacter::StartCrouch()
{
    UE_LOG(LogTemp, Warning, TEXT("StartCrouch called")); // ÄÎÁÀÂÜÒÅ ÝÒÓ ÑÒÐÎÊÓ

    bIsCrouching = true;

    if (bIsSprinting)
    {
        GetCharacterMovement()->MaxWalkSpeed = CrouchSprintSpeed;
        UE_LOG(LogTemp, Warning, TEXT("CROUCH SPRINT - Speed: %f"), CrouchSprintSpeed);
    }
    else
    {
        GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
        UE_LOG(LogTemp, Warning, TEXT("CROUCH - Speed: %f"), CrouchSpeed);
    }

    Crouch();
}

void ASystem1ParadoxCharacter::StopCrouch()
{
    UE_LOG(LogTemp, Warning, TEXT("StopCrouch called")); // ÄÎÁÀÂÜÒÅ ÝÒÓ ÑÒÐÎÊÓ

    bIsCrouching = false;

    if (bIsSprinting)
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
        UE_LOG(LogTemp, Warning, TEXT("STAND UP (Sprinting) - Speed: %f"), SprintSpeed);
    }
    else
    {
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
        UE_LOG(LogTemp, Warning, TEXT("STAND UP - Speed: %f"), WalkSpeed);
    }

    UnCrouch();
}}