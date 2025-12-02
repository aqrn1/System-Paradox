#include "System1ParadoxCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"
#include "Weapon.h"
#include "System1ParadoxHUD.h"
#include "CharacterAnimInstance.h" // Подключите новый заголовочный файл
#include "GameFramework/CharacterMovementComponent.h"

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
            UE_LOG(LogTemp, Warning, TEXT("Weapon spawned and attached!"));
        }
    }
}

void ASystem1ParadoxCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Обновляем HUD каждый кадр
    UpdateHUD();

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

    // Добавляем стрельбу
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASystem1ParadoxCharacter::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASystem1ParadoxCharacter::StopFire);

    // Добавляем перезарядку
    PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ASystem1ParadoxCharacter::StartReload);
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

void ASystem1ParadoxCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    // Получаем наш кастомный AnimInstance
    if (GetMesh())
    {
        AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
    }
}

void ASystem1ParadoxCharacter::UpdateAnimationState()
{
    if (AnimInstance)
    {
        // Передаем базовые данные
        AnimInstance->Speed = GetVelocity().Size2D();
        AnimInstance->bIsInAir = GetCharacterMovement()->IsFalling();

        // Здесь логика определения типа оружия
        // AnimInstance->CurrentWeaponType = ...;

        // Здесь логика перезарядки
        // AnimInstance->bIsReloading = ...;
    }
}

// В Tick() добавьте вызов обновления анимации
void ASystem1ParadoxCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateAnimationState(); // Добавьте эту строку
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
        UE_LOG(LogTemp, Warning, TEXT("Started firing"));
    }
}

void ASystem1ParadoxCharacter::StopFire()
{
    if (bIsFiring)
    {
        bIsFiring = false;
        CurrentWeapon->StopFire();
        UE_LOG(LogTemp, Warning, TEXT("Stopped firing"));
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

// Остальные функции (sprint, crouch) пока остаются пустыми
void ASystem1ParadoxCharacter::StartSprint() {}
void ASystem1ParadoxCharacter::StopSprint() {}
void ASystem1ParadoxCharacter::StartCrouch() {}
void ASystem1ParadoxCharacter::StopCrouch() {}