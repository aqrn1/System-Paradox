#include "Weapon.h"
// УДАЛИТЕ #include "System1ParadoxCharacter.h" если он есть
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    WeaponMesh->SetupAttachment(Root);

    MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
    MuzzleLocation->SetupAttachment(WeaponMesh);
    MuzzleLocation->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));

    // Простые настройки
    Damage = 25.0f;
    FireRate = 0.1f;
    MaxAmmo = 30;
    CurrentAmmo = MaxAmmo;
}

void AWeapon::BeginPlay()
{
    Super::BeginPlay();
    CurrentAmmo = MaxAmmo;
}

void AWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AWeapon::StartFire()
{
    if (!CanFire())
    {
        UE_LOG(LogTemp, Warning, TEXT("No ammo!"));
        return;
    }

    if (!bIsFiring)
    {
        bIsFiring = true;

        // Немедленный первый выстрел
        FireShot();

        // Устанавливаем таймер для автоматической стрельбы
        GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AWeapon::FireShot, FireRate, true);

        UE_LOG(LogTemp, Warning, TEXT("Started firing"));
    }
}

void AWeapon::StopFire()
{
    if (bIsFiring)
    {
        bIsFiring = false;
        GetWorldTimerManager().ClearTimer(FireTimerHandle);
        UE_LOG(LogTemp, Warning, TEXT("Stopped firing"));
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
    FVector EndLocation = StartLocation + MuzzleLocation->GetForwardVector() * 10000.0f;

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

        // Наносим урон
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            UGameplayStatics::ApplyPointDamage(
                HitActor,
                Damage,
                MuzzleLocation->GetForwardVector(),
                HitResult,
                GetInstigatorController(),
                this,
                UDamageType::StaticClass()
            );

            UE_LOG(LogTemp, Warning, TEXT("Hit: %s with damage: %.0f"), *HitActor->GetName(), Damage);
        }
    }

    // Визуализация выстрела
    FColor LineColor = bHit ? FColor::Red : FColor::Green;
    DrawDebugLine(GetWorld(), StartLocation, EndLocation, LineColor, false, 1.0f, 0, 2.0f);

    if (bHit)
    {
        DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 1.0f);
    }

    // Простая отдача
    ASystem1ParadoxCharacter* OwnerCharacter = Cast<ASystem1ParadoxCharacter>(GetOwner());
    if (OwnerCharacter)
    {
        // Легкая вертикальная отдача
        OwnerCharacter->AddControllerPitchInput(-0.1f);
    }

    UE_LOG(LogTemp, Warning, TEXT("Fired! Ammo: %d"), CurrentAmmo);
}

void AWeapon::Reload()
{
    if (bIsReloading)
    {
        UE_LOG(LogTemp, Warning, TEXT("Already reloading!"));
        return;
    }

    if (!CanReload())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot reload - already full or no ammo"));
        return;
    }

    bIsReloading = true;
    UE_LOG(LogTemp, Warning, TEXT("Started reloading..."));

    // Симулируем время перезарядки (2 секунды)
    GetWorldTimerManager().SetTimer(ReloadTimerHandle, [this]()
        {
            CurrentAmmo = MaxAmmo;
            bIsReloading = false;
            UE_LOG(LogTemp, Warning, TEXT("Reloaded! Ammo: %d"), CurrentAmmo);

        }, 2.0f, false);
}

bool AWeapon::CanFire() const
{
    return CurrentAmmo > 0 && !bIsReloading;
}

bool AWeapon::CanReload() const
{
    return CurrentAmmo < MaxAmmo && !bIsReloading;
}