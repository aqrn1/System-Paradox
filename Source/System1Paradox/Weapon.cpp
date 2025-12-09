#include "Weapon.h"
#include "System1ParadoxCharacter.h"  // Теперь подключаем!
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"

AWeapon::AWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    WeaponMesh->SetupAttachment(Root);
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
    MuzzleLocation->SetupAttachment(WeaponMesh);
    MuzzleLocation->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));

    // AAA настройки по умолчанию
    WeaponType = ES1P_WeaponType::Pistol;
    MaxAmmo = 30;
    CurrentAmmo = MaxAmmo;
    FireRate = 0.1f;
    ReloadTime = 2.0f;
    RecoilVertical = 0.1f;
    RecoilHorizontal = 0.05f;

    // Настройки урона
    HitData.Damage = 25.0f;
    HitData.HeadshotMultiplier = 2.0f;
    HitData.TorsoMultiplier = 1.0f;
    HitData.LimbMultiplier = 0.7f;
}

void AWeapon::BeginPlay()
{
    Super::BeginPlay();
    CurrentAmmo = MaxAmmo;
}

void AWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    // Очищаем таймеры при уничтожении
    GetWorldTimerManager().ClearTimer(FireTimerHandle);
    GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
}

void AWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AWeapon::StartFire()
{
    if (!CanFire())
    {
        if (CurrentAmmo <= 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("Weapon: Out of ammo!"));
        }
        return;
    }

    if (!bIsFiring)
    {
        bIsFiring = true;

        // Немедленный первый выстрел (AAA - мгновенная обратная связь)
        FireShot();

        // Устанавливаем таймер для автоматической стрельбы
        GetWorldTimerManager().SetTimer(
            FireTimerHandle,
            this,
            &AWeapon::FireShot,
            FireRate,
            true
        );

        UE_LOG(LogTemp, Warning, TEXT("Weapon: Started firing"));
    }
}

void AWeapon::StopFire()
{
    if (bIsFiring)
    {
        bIsFiring = false;
        GetWorldTimerManager().ClearTimer(FireTimerHandle);
        UE_LOG(LogTemp, Warning, TEXT("Weapon: Stopped firing"));
    }
}

void AWeapon::FireShot()
{
    if (!CanFire())
    {
        StopFire();
        return;
    }

    // Уменьшаем патроны
    CurrentAmmo--;

    // Получаем направление выстрела
    FVector StartLocation = MuzzleLocation->GetComponentLocation();
    FRotator StartRotation = MuzzleLocation->GetComponentRotation();

    // AAA: добавляем случайное отклонение для реализма
    float Spread = 0.5f; // В градусах
    FRotator SpreadRotation = StartRotation;
    SpreadRotation.Yaw += FMath::RandRange(-Spread, Spread);
    SpreadRotation.Pitch += FMath::RandRange(-Spread, Spread);

    FVector EndLocation = StartLocation + SpreadRotation.Vector() * 10000.0f;

    // Параметры трассировки
    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);
    CollisionParams.AddIgnoredActor(GetOwner());

    // Выполняем трассировку луча
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        ECC_Visibility,
        CollisionParams
    );

    // Обрабатываем попадание
    if (bHit)
    {
        EndLocation = HitResult.ImpactPoint;

        // Вычисляем урон в зависимости от части тела
        float FinalDamage = CalculateDamage(HitResult);

        // Наносим урон
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            UGameplayStatics::ApplyPointDamage(
                HitActor,
                FinalDamage,
                MuzzleLocation->GetForwardVector(),
                HitResult,
                GetInstigatorController(),
                this,
                UDamageType::StaticClass()
            );

            FString BodyPart = GetBodyPartName(HitResult);
            UE_LOG(LogTemp, Warning, TEXT("Weapon: Hit %s in %s with damage: %.1f"),
                *HitActor->GetName(), *BodyPart, FinalDamage);
        }

        // AAA: Эффекты попадания
        ApplyHitEffect(HitResult);
    }

    // AAA: Визуальные эффекты выстрела (только в режиме разработки)
#if WITH_EDITOR
    if (GEngine)
    {
        FColor LineColor = bHit ? FColor::Red : FColor::Green;
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, LineColor, false, 0.1f, 0, 2.0f);

        if (bHit)
        {
            DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 0.1f);
        }
    }
#endif

    // AAA: Звук выстрела
    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, StartLocation);
    }

    // AAA: Мuzzle flash
    if (MuzzleFlash)
    {
        UGameplayStatics::SpawnEmitterAttached(
            MuzzleFlash,
            MuzzleLocation,
            NAME_None,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            EAttachLocation::KeepRelativeOffset,
            true
        );
    }

    // AAA: Отдача
    ApplyRecoil();

    UE_LOG(LogTemp, Warning, TEXT("Weapon: Fired! Ammo: %d/%d"), CurrentAmmo, MaxAmmo);
}

void AWeapon::Reload()
{
    if (bIsReloading)
    {
        UE_LOG(LogTemp, Warning, TEXT("Weapon: Already reloading!"));
        return;
    }

    if (!CanReload())
    {
        UE_LOG(LogTemp, Warning, TEXT("Weapon: Cannot reload"));
        return;
    }

    bIsReloading = true;
    StopFire(); // AAA: Прерываем стрельбу при перезарядке

    UE_LOG(LogTemp, Warning, TEXT("Weapon: Started reloading..."));

    // Симулируем время перезарядки
    GetWorldTimerManager().SetTimer(ReloadTimerHandle, [this]()
        {
            CurrentAmmo = MaxAmmo;
            bIsReloading = false;
            UE_LOG(LogTemp, Warning, TEXT("Weapon: Reloaded! Ammo: %d"), CurrentAmmo);

            // AAA: Можно добавить звук завершения перезарядки здесь
        }, ReloadTime, false);
}

bool AWeapon::CanFire() const
{
    // AAA: Можем стрелять только если есть патроны, не перезаряжаемся и не на кд
    return CurrentAmmo > 0 && !bIsReloading;
}

bool AWeapon::CanReload() const
{
    // AAA: Можем перезаряжать только если не полный магазин и не в процессе перезарядки
    return CurrentAmmo < MaxAmmo && !bIsReloading;
}

// AAA: Применение отдачи
void AWeapon::ApplyRecoil()
{
    ASystem1ParadoxCharacter* OwnerCharacter = Cast<ASystem1ParadoxCharacter>(GetOwner());
    if (OwnerCharacter)
    {
        // Легкая вертикальная отдача
        float VerticalRecoil = FMath::RandRange(RecoilVertical * 0.8f, RecoilVertical * 1.2f);
        float HorizontalRecoil = FMath::RandRange(-RecoilHorizontal, RecoilHorizontal);

        OwnerCharacter->AddControllerPitchInput(-VerticalRecoil);
        OwnerCharacter->AddControllerYawInput(HorizontalRecoil);
    }
}

// AAA: Эффекты при попадании
void AWeapon::ApplyHitEffect(const FHitResult& HitResult)
{
    if (HitEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            HitEffect,
            HitResult.ImpactPoint,
            HitResult.ImpactNormal.Rotation()
        );
    }
}

// AAA: Расчет урона в зависимости от части тела
float AWeapon::CalculateDamage(const FHitResult& HitResult) const
{
    float BaseDamage = HitData.Damage;
    float Multiplier = 1.0f;

    // Простой способ определения части тела (в реальной AAA игре нужно проверять bone name)
    FString HitBoneName = HitResult.BoneName.ToString();

    if (HitBoneName.Contains("head", ESearchCase::IgnoreCase) ||
        HitBoneName.Contains("Head"))
    {
        Multiplier = HitData.HeadshotMultiplier;
    }
    else if (HitBoneName.Contains("spine", ESearchCase::IgnoreCase) ||
        HitBoneName.Contains("chest") ||
        HitBoneName.Contains("torso"))
    {
        Multiplier = HitData.TorsoMultiplier;
    }
    else if (HitBoneName.Contains("arm") ||
        HitBoneName.Contains("leg") ||
        HitBoneName.Contains("hand") ||
        HitBoneName.Contains("foot"))
    {
        Multiplier = HitData.LimbMultiplier;
    }

    return BaseDamage * Multiplier;
}

// AAA: Получение имени части тела для отладки
FString AWeapon::GetBodyPartName(const FHitResult& HitResult) const
{
    FString BoneName = HitResult.BoneName.ToString();

    if (BoneName.Contains("head", ESearchCase::IgnoreCase))
        return TEXT("Head");
    else if (BoneName.Contains("spine", ESearchCase::IgnoreCase))
        return TEXT("Torso");
    else if (BoneName.Contains("arm", ESearchCase::IgnoreCase))
        return TEXT("Arm");
    else if (BoneName.Contains("leg", ESearchCase::IgnoreCase))
        return TEXT("Leg");

    return TEXT("Unknown");
}