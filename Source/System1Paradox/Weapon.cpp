// Weapon.cpp - ПОЛНОСТЬЮ ИСПРАВЛЕННЫЙ ДЛЯ НОВОГО WEAPON.H
#include "Weapon.h"
#include "System1ParadoxCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"

AWeapon::AWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    // 1. Создаем корневой компонент ПЕРВЫМ
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    // 2. Создаем меш оружия
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    WeaponMesh->SetupAttachment(Root);
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 3. Компонент MuzzleLocation
    MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
    MuzzleLocation->SetupAttachment(WeaponMesh);
    MuzzleLocation->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));
    MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
    MuzzleLocation->bVisualizeComponent = true; // Для отладки в редакторе

    // 4. Компонент для выброса гильз (новая фича из Weapon.h)
    ShellEjectLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ShellEjectLocation"));
    ShellEjectLocation->SetupAttachment(WeaponMesh);
    ShellEjectLocation->SetRelativeLocation(FVector(0.0f, 10.0f, 0.0f));

    // 5. Инициализация структуры Stats (ВМЕСТО старых отдельных переменных!)
    Stats.MaxAmmo = 30;
    Stats.FireRate = 0.1f;
    Stats.ReloadTime = 2.0f;
    Stats.RecoilVertical = 0.1f;
    Stats.RecoilHorizontal = 0.05f;
    Stats.EffectiveRange = 5000.0f;
    Stats.Spread = 0.5f;

    // 6. Инициализация состояния оружия
    CurrentAmmo = Stats.MaxAmmo; // Используем Stats.MaxAmmo вместо старой переменной
    bIsFiring = false;
    bIsReloading = false;
    bIsAiming = false;
    bInfiniteAmmo = false;
    bDebugMode = false;
    TimeSinceLastShot = 0.0f;
    ReloadStartTime = 0.0f;

    // 7. Настройки урона
    HitData.Damage = 25.0f;
    HitData.HeadshotMultiplier = 2.0f;
    HitData.TorsoMultiplier = 1.0f;
    HitData.LimbMultiplier = 0.7f;
}

void AWeapon::BeginPlay()
{
    Super::BeginPlay();

    // Инициализация боезапаса
    CurrentAmmo = Stats.MaxAmmo; // Используем Stats.MaxAmmo

    // DEBUG: Показываем информацию об оружии
    UE_LOG(LogTemp, Log, TEXT("🔫 Weapon Initialized: Type=%d, Ammo=%d/%d, FireRate=%.2f"),
        (int32)WeaponType, CurrentAmmo, Stats.MaxAmmo, Stats.FireRate);

    if (MuzzleLocation)
    {
        UE_LOG(LogTemp, Verbose, TEXT("🔫 MuzzleLocation позиция: %s"),
            *MuzzleLocation->GetRelativeLocation().ToString());
    }
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

    // Обновляем время с последнего выстрела
    if (bIsFiring)
    {
        TimeSinceLastShot += DeltaTime;
    }
}

void AWeapon::StartFire()
{
    if (!CanFire())
    {
        if (CurrentAmmo <= 0 && EmptySound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, EmptySound, GetActorLocation());
        }
        return;
    }

    if (!bIsFiring)
    {
        bIsFiring = true;
        TimeSinceLastShot = 0.0f;

        // Немедленный первый выстрел
        FireShot();

        // Устанавливаем таймер для автоматической стрельбы
        // Используем Stats.FireRate вместо старой переменной
        GetWorldTimerManager().SetTimer(
            FireTimerHandle,
            this,
            &AWeapon::FireShot,
            Stats.FireRate,
            true
        );

        UE_LOG(LogTemp, Verbose, TEXT("Weapon: Started firing"));
    }
}

void AWeapon::StopFire()
{
    if (bIsFiring)
    {
        bIsFiring = false;
        GetWorldTimerManager().ClearTimer(FireTimerHandle);
        UE_LOG(LogTemp, Verbose, TEXT("Weapon: Stopped firing"));
    }
}

void AWeapon::FireShot()
{
    if (!CanFire())
    {
        StopFire();
        return;
    }

    // Уменьшаем патроны (если не бесконечные)
    if (!bInfiniteAmmo)
    {
        CurrentAmmo--;
    }

    // Вызываем событие изменения патронов
    OnAmmoChanged.Broadcast(CurrentAmmo);

    // Получаем позицию и вращение дула
    FVector StartLocation = MuzzleLocation->GetComponentLocation();
    FRotator StartRotation = MuzzleLocation->GetComponentRotation();

    // Добавляем разброс в зависимости от времени стрельбы
    float CurrentSpread = Stats.Spread * (1.0f + TimeSinceLastShot * 0.5f);
    FRotator SpreadRotation = StartRotation;
    SpreadRotation.Yaw += FMath::RandRange(-CurrentSpread, CurrentSpread);
    SpreadRotation.Pitch += FMath::RandRange(-CurrentSpread, CurrentSpread);

    FVector EndLocation = StartLocation + SpreadRotation.Vector() * Stats.EffectiveRange;

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
            UE_LOG(LogTemp, Verbose, TEXT("Weapon: Hit %s in %s with damage: %.1f"),
                *HitActor->GetName(), *BodyPart, FinalDamage);
        }

        // Эффекты попадания
        ApplyHitEffect(HitResult);
    }

    // Визуальные эффекты выстрела (только в режиме разработки)
#if WITH_EDITOR
    if (GEngine && bDebugMode)
    {
        FColor LineColor = bHit ? FColor::Red : FColor::Green;
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, LineColor, false, 0.1f, 0, 2.0f);

        if (bHit)
        {
            DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 0.1f);
        }
    }
#endif

    // Звук выстрела
    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, StartLocation);
    }

    // Мuzzle flash
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

    // Выброс гильзы
    ApplyShellEjectEffect();

    // Отдача
    ApplyRecoil();

    // Вызываем событие выстрела
    OnWeaponFired.Broadcast();

    UE_LOG(LogTemp, Verbose, TEXT("Weapon: Fired! Ammo: %d/%d"), CurrentAmmo, Stats.MaxAmmo);
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
    ReloadStartTime = GetWorld()->GetTimeSeconds();
    StopFire(); // Прерываем стрельбу при перезарядке

    // Звук начала перезарядки
    if (ReloadSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
    }

    UE_LOG(LogTemp, Log, TEXT("Weapon: Started reloading..."));

    // Таймер перезарядки
    GetWorldTimerManager().SetTimer(ReloadTimerHandle, [this]()
        {
            CurrentAmmo = Stats.MaxAmmo; // Используем Stats.MaxAmmo
            bIsReloading = false;

            // Вызываем событие перезарядки
            OnWeaponReloaded.Broadcast();
            OnAmmoChanged.Broadcast(CurrentAmmo);

            UE_LOG(LogTemp, Log, TEXT("Weapon: Reloaded! Ammo: %d"), CurrentAmmo);
        }, Stats.ReloadTime, false); // Используем Stats.ReloadTime
}

bool AWeapon::CanFire() const
{
    // Не можем стрелять если: нет патронов, перезаряжаемся или на кд
    return (bInfiniteAmmo || CurrentAmmo > 0) && !bIsReloading;
}

bool AWeapon::CanReload() const
{
    // Можем перезаряжать если: не полный магазин и не в процессе перезарядки
    return CurrentAmmo < Stats.MaxAmmo && !bIsReloading; // Используем Stats.MaxAmmo
}

float AWeapon::GetReloadProgress() const
{
    if (!bIsReloading) return 0.0f;

    float Elapsed = GetWorld()->GetTimeSeconds() - ReloadStartTime;
    return FMath::Clamp(Elapsed / Stats.ReloadTime, 0.0f, 1.0f); // Используем Stats.ReloadTime
}

void AWeapon::ApplyRecoil()
{
    ASystem1ParadoxCharacter* OwnerCharacter = Cast<ASystem1ParadoxCharacter>(GetOwner());
    if (OwnerCharacter)
    {
        // Используем Stats.RecoilVertical и Stats.RecoilHorizontal
        float VerticalRecoil = FMath::FRandRange(Stats.RecoilVertical * 0.8f, Stats.RecoilVertical * 1.2f);
        float HorizontalRecoil = FMath::FRandRange(-Stats.RecoilHorizontal, Stats.RecoilHorizontal);

        OwnerCharacter->AddControllerPitchInput(-VerticalRecoil);
        OwnerCharacter->AddControllerYawInput(HorizontalRecoil);
    }
}

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

void AWeapon::ApplyShellEjectEffect()
{
    if (ShellEjectEffect && ShellEjectLocation)
    {
        UGameplayStatics::SpawnEmitterAttached(
            ShellEjectEffect,
            ShellEjectLocation,
            NAME_None,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            EAttachLocation::KeepRelativeOffset,
            true
        );
    }
}

float AWeapon::CalculateDamage(const FHitResult& HitResult) const
{
    float BaseDamage = HitData.Damage;
    float Multiplier = 1.0f;

    FString HitBoneName = HitResult.BoneName.ToString().ToLower();

    if (HitBoneName.Contains("head"))
    {
        Multiplier = HitData.HeadshotMultiplier;
    }
    else if (HitBoneName.Contains("spine") ||
        HitBoneName.Contains("chest") ||
        HitBoneName.Contains("torso") ||
        HitBoneName.Contains("pelvis"))
    {
        Multiplier = HitData.TorsoMultiplier;
    }
    else if (HitBoneName.Contains("arm") ||
        HitBoneName.Contains("leg") ||
        HitBoneName.Contains("hand") ||
        HitBoneName.Contains("foot") ||
        HitBoneName.Contains("shoulder") ||
        HitBoneName.Contains("hip"))
    {
        Multiplier = HitData.LimbMultiplier;
    }

    return BaseDamage * Multiplier;
}

FString AWeapon::GetBodyPartName(const FHitResult& HitResult) const
{
    FString BoneName = HitResult.BoneName.ToString().ToLower();

    if (BoneName.Contains("head"))
        return TEXT("Head");
    else if (BoneName.Contains("spine"))
        return TEXT("Torso");
    else if (BoneName.Contains("arm"))
        return TEXT("Arm");
    else if (BoneName.Contains("leg"))
        return TEXT("Leg");
    else if (BoneName.Contains("hand"))
        return TEXT("Hand");
    else if (BoneName.Contains("foot"))
        return TEXT("Foot");

    return TEXT("Unknown");
}

void AWeapon::WeaponDebug(int32 Enable)
{
    bDebugMode = (Enable != 0);
    UE_LOG(LogTemp, Warning, TEXT("🔧 Weapon Debug: %s"), bDebugMode ? TEXT("ON") : TEXT("OFF"));
}

void AWeapon::SetInfiniteAmmo(int32 Enable)
{
    bInfiniteAmmo = (Enable != 0);
    UE_LOG(LogTemp, Warning, TEXT("🔫 Infinite Ammo: %s"), bInfiniteAmmo ? TEXT("ON") : TEXT("OFF"));
}

void AWeapon::ToggleFireMode()
{
    // Заглушка для будущей реализации режимов стрельбы
    UE_LOG(LogTemp, Warning, TEXT("Weapon: ToggleFireMode - Not implemented yet"));
}

void AWeapon::ApplyWeaponStats(const FWeaponStats& NewStats)
{
    Stats = NewStats;

    // Корректируем текущий боезапас, если он превышает новый максимум
    if (CurrentAmmo > Stats.MaxAmmo)
    {
        CurrentAmmo = Stats.MaxAmmo;
    }

    UE_LOG(LogTemp, Log, TEXT("Weapon: Stats updated"));
}