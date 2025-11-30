#include "Weapon.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "TimerManager.h"
#include "System1ParadoxCharacter.h"

AWeapon::AWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    // Создаем корневой компонент
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    // Создаем меш оружия
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    WeaponMesh->SetupAttachment(Root);

    // Создаем точку выстрела
    MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
    MuzzleLocation->SetupAttachment(WeaponMesh);
    MuzzleLocation->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));

    // Настройки по умолчанию для штурмовой винтовки
    WeaponConfig.Damage = 25.0f;
    WeaponConfig.FireRate = 600.0f;
    WeaponConfig.Spread = 1.0f;
    WeaponConfig.Recoil = 0.5f;
    WeaponConfig.Range = 10000.0f;
    WeaponConfig.BulletsPerShot = 1;

    MaxAmmo = 30;
    CurrentAmmo = MaxAmmo;
    MaxReserveAmmo = 90;
    CurrentReserveAmmo = MaxReserveAmmo;

    // Рассчитываем время между выстрелами
    TimeBetweenShots = 60.0f / WeaponConfig.FireRate;
}

void AWeapon::BeginPlay()
{
    Super::BeginPlay();

    // Инициализация
    CurrentAmmo = MaxAmmo;
    CurrentReserveAmmo = MaxReserveAmmo;
    CurrentSpread = 0.0f;
}

void AWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Постепенное уменьшение разброса при не стрельбе
    if (!bIsFiring && CurrentSpread > 0.0f)
    {
        CurrentSpread = FMath::FInterpTo(CurrentSpread, 0.0f, DeltaTime, 5.0f);
    }
}

void AWeapon::StartFire()
{
    if (!CanFire())
    {
        PlayEmptySound();
        return;
    }

    if (!bIsFiring)
    {
        bIsFiring = true;

        // Немедленный первый выстрел
        FireShot();

        // Устанавливаем таймер для автоматической стрельбы
        GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AWeapon::FireShot, TimeBetweenShots, true);
    }
}

void AWeapon::StopFire()
{
    if (bIsFiring)
    {
        bIsFiring = false;
        GetWorldTimerManager().ClearTimer(FireTimerHandle);
    }
}

void AWeapon::FireShot()
{
    if (!CanFire())
    {
        StopFire();
        PlayEmptySound();
        return;
    }

    // Уменьшаем патроны
    CurrentAmmo--;

    // Эффекты выстрела
    SpawnMuzzleFlash();
    PlayFireSound();

    // Увеличиваем разброс
    CurrentSpread = FMath::Min(CurrentSpread + WeaponConfig.Spread * 0.1f, WeaponConfig.Spread);

    // Производим выстрелы (для дробовиков)
    for (int32 i = 0; i < WeaponConfig.BulletsPerShot; i++)
    {
        // Получаем направление выстрела с учетом разброса
        FVector ShotDirection;
        float ShotSpread;
        CalculateSpread(ShotDirection, ShotSpread);

        // Начальная и конечная точки трассировки
        FVector StartLocation = MuzzleLocation->GetComponentLocation();
        FVector EndLocation = StartLocation + (ShotDirection * WeaponConfig.Range);

        // Параметры трассировки
        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(this);
        CollisionParams.AddIgnoredActor(GetOwner());
        CollisionParams.bReturnPhysicalMaterial = true;

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

            // Наносим урон
            AActor* HitActor = HitResult.GetActor();
            if (HitActor)
            {
                UGameplayStatics::ApplyPointDamage(
                    HitActor,
                    WeaponConfig.Damage,
                    ShotDirection,
                    HitResult,
                    GetInstigatorController(),
                    this,
                    UDamageType::StaticClass()
                );
            }

            // Эффект попадания
            SpawnImpactEffect(HitResult.ImpactPoint, HitResult.ImpactNormal);
        }

        // Визуализация выстрела (только в режиме отладки)
        DrawDebugShot(StartLocation, EndLocation, bHit);
    }

    // Отдача
    ASystem1ParadoxCharacter* OwnerCharacter = Cast<ASystem1ParadoxCharacter>(GetOwner());
    if (OwnerCharacter)
    {
        // Простая вертикальная отдача
        FRotator RecoilRotation = FRotator(-WeaponConfig.Recoil * 0.5f, 0.0f, 0.0f);
        OwnerCharacter->AddControllerPitchInput(RecoilRotation.Pitch);
    }

    // Обновляем время последнего выстрела
    LastFireTime = GetWorld()->GetTimeSeconds();

    UE_LOG(LogTemp, Warning, TEXT("Fired! Ammo: %d/%d"), CurrentAmmo, CurrentReserveAmmo);
}

void AWeapon::CalculateSpread(FVector& ShotDirection, float& CurrentShotSpread)
{
    // Базовое направление - вперед от дула
    ShotDirection = MuzzleLocation->GetForwardVector();

    // Добавляем случайный разброс
    float SpreadRadius = CurrentSpread * 0.1f;
    FVector Spread = FVector(
        FMath::RandRange(-SpreadRadius, SpreadRadius),
        FMath::RandRange(-SpreadRadius, SpreadRadius),
        FMath::RandRange(-SpreadRadius, SpreadRadius)
    );

    ShotDirection += Spread;
    ShotDirection.Normalize();

    CurrentShotSpread = CurrentSpread;
}

void AWeapon::SpawnMuzzleFlash()
{
    if (MuzzleFlash)
    {
        UGameplayStatics::SpawnEmitterAttached(
            MuzzleFlash,
            MuzzleLocation,
            NAME_None,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            EAttachLocation::SnapToTarget,
            true
        );
    }
}

void AWeapon::SpawnImpactEffect(const FVector& Location, const FVector& Normal)
{
    if (ImpactEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            ImpactEffect,
            Location,
            Normal.Rotation(),
            true
        );
    }
}

void AWeapon::PlayFireSound()
{
    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            FireSound,
            GetActorLocation()
        );
    }
}

void AWeapon::PlayReloadSound()
{
    if (ReloadSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            ReloadSound,
            GetActorLocation()
        );
    }
}

void AWeapon::PlayEmptySound()
{
    if (EmptySound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            EmptySound,
            GetActorLocation()
        );
    }
}

void AWeapon::Reload()
{
    if (!CanReload() || bIsReloading)
        return;

    bIsReloading = true;

    // Воспроизводим звук перезарядки
    PlayReloadSound();

    // Симулируем время перезарядки (2 секунды)
    float ReloadTime = 2.0f;
    GetWorldTimerManager().SetTimer(ReloadTimerHandle, [this]()
        {
            int32 AmmoNeeded = MaxAmmo - CurrentAmmo;
            int32 AmmoToReload = FMath::Min(AmmoNeeded, CurrentReserveAmmo);

            CurrentAmmo += AmmoToReload;
            CurrentReserveAmmo -= AmmoToReload;

            bIsReloading = false;

            UE_LOG(LogTemp, Warning, TEXT("Reloaded! Ammo: %d/%d"), CurrentAmmo, CurrentReserveAmmo);

        }, ReloadTime, false);
}

bool AWeapon::CanFire() const
{
    return CurrentAmmo > 0 && !bIsReloading;
}

bool AWeapon::CanReload() const
{
    return CurrentAmmo < MaxAmmo && CurrentReserveAmmo > 0 && !bIsReloading;
}

void AWeapon::DrawDebugShot(const FVector& Start, const FVector& End, bool bHit)
{
    if (GetWorld()->WorldType == EWorldType::Editor || GetWorld()->WorldType == EWorldType::PIE)
    {
        FColor DebugColor = bHit ? FColor::Red : FColor::Green;
        float DebugTime = 1.0f;

        DrawDebugLine(GetWorld(), Start, End, DebugColor, false, DebugTime, 0, 1.0f);

        if (bHit)
        {
            DrawDebugPoint(GetWorld(), End, 8.0f, FColor::Red, false, DebugTime);
        }
    }
}