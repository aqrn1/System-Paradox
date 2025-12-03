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
        }
    }
}

void ASystem1ParadoxCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Обновляем HUD каждый кадр
    UpdateHUD();

    // ======== ЛОГИКА ВЫБОРА АНИМАЦИИ ========
    // Выбираем анимацию в зависимости от скорости
    float Speed = GetVelocity().Size2D();
    bool bIsFalling = GetCharacterMovement()->IsFalling();

    UAnimSequence* DesiredAnimation = IdleAnimation; // По умолчанию

    if (bIsFalling)
    {
        // Если есть анимация прыжка - можно добавить
    }
    else if (Speed > 300.0f && RunAnimation) // Бег
    {
        DesiredAnimation = RunAnimation;
    }
    else if (Speed > 10.0f && WalkAnimation) // Ходьба
    {
        DesiredAnimation = WalkAnimation;
    }

    // Проигрываем выбранную анимацию
    if (DesiredAnimation)
    {
        PlayAnimation(DesiredAnimation);
    }
    // =========================================

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

// ======== ФУНКЦИЯ PLAYANIMATION ========
void ASystem1ParadoxCharacter::PlayAnimation(UAnimSequence* NewAnimation)
{
    if (!GetMesh() || !GetMesh()->GetAnimInstance() || !NewAnimation)
        return;

    // Если уже проигрывается эта анимация - ничего не делаем
    if (CurrentAnimation == NewAnimation)
        return;

    CurrentAnimation = NewAnimation;

    // Проигрываем анимацию
    GetMesh()->GetAnimInstance()->PlaySlotAnimationAsDynamicMontage(
        NewAnimation,        // Сама анимация
        FName("DefaultSlot"), // Слот для проигрывания
        0.25f,               // Плавное начало
        0.25f,               // Плавный конец
        1.0f,                // Скорость проигрывания
        1,                   // Сколько раз проиграть (1 = бесконечно)
        0.0f,                // Начальное смещение
        0.0f                 // Стартовое время
    );
}

void ASystem1ParadoxCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    // Пустая реализация - просто удовлетворяем линкер
}