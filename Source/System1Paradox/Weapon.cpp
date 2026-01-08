#include "Weapon.h"
#include "System1ParadoxCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AWeapon::AWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>("Root");
    RootComponent = Root;

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
    WeaponMesh->SetupAttachment(Root);

    MuzzleLocation = CreateDefaultSubobject<USceneComponent>("Muzzle");
    MuzzleLocation->SetupAttachment(WeaponMesh);

    ShellEjectLocation = CreateDefaultSubobject<USceneComponent>("ShellEject");
    ShellEjectLocation->SetupAttachment(WeaponMesh);

    bIsFiring = false;
    bIsReloading = false;
    bIsAiming = false;
}

void AWeapon::BeginPlay()
{
    Super::BeginPlay();
    CurrentAmmo = Stats.MaxAmmo;
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
            OnWeaponEmpty.Broadcast();
        }
        return;
    }

    bIsFiring = true;
    FireShot();

    GetWorldTimerManager().SetTimer(
        FireTimer,
        this,
        &AWeapon::FireShot,
        Stats.FireInterval,
        true
    );
}

void AWeapon::StopFire()
{
    bIsFiring = false;
    GetWorldTimerManager().ClearTimer(FireTimer);
}

void AWeapon::FireShot()
{
    if (!CanFire())
    {
        StopFire();
        return;
    }

    CurrentAmmo--;
    OnAmmoChanged.Broadcast(CurrentAmmo);
    OnWeaponFired.Broadcast();

    ApplyRecoil();
}

void AWeapon::Reload()
{
    if (!CanReload())
        return;

    bIsReloading = true;
    ReloadStartTime = GetWorld()->GetTimeSeconds();
    StopFire();

    GetWorldTimerManager().SetTimer(
        ReloadTimer,
        [this]()
        {
            CurrentAmmo = Stats.MaxAmmo;
            bIsReloading = false;
            OnAmmoChanged.Broadcast(CurrentAmmo);
            OnWeaponReloaded.Broadcast();
        },
        Stats.ReloadTime,
        false
    );
}

void AWeapon::StartAim()
{
    bIsAiming = true;
}

void AWeapon::StopAim()
{
    bIsAiming = false;
}

bool AWeapon::CanFire() const
{
    return !bIsReloading && CurrentAmmo > 0;
}

bool AWeapon::CanReload() const
{
    return !bIsReloading && CurrentAmmo < Stats.MaxAmmo;
}

float AWeapon::GetReloadProgress() const
{
    if (!bIsReloading) return 0.f;

    float Elapsed = GetWorld()->GetTimeSeconds() - ReloadStartTime;
    return FMath::Clamp(Elapsed / Stats.ReloadTime, 0.f, 1.f);
}

void AWeapon::ApplyRecoil()
{
    if (ASystem1ParadoxCharacter* Char = Cast<ASystem1ParadoxCharacter>(GetOwner()))
    {
        Char->AddControllerPitchInput(-Stats.RecoilVertical);
        Char->AddControllerYawInput(
            FMath::RandRange(-Stats.RecoilHorizontal, Stats.RecoilHorizontal)
        );
    }
}
