void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    // Получаем персонажа
    PlayerCharacter = Cast<ASystem1ParadoxCharacter>(TryGetPawnOwner());

    // Если персонажа нет - выходим
    if (!PlayerCharacter) return;

    // Получаем скорость
    Speed = PlayerCharacter->GetVelocity().Size();

    // Получаем состояние приседания (через публичную функцию)
    bIsCrouching = PlayerCharacter->GetIsCrouching();

    // Получаем состояние спринта (через публичную функцию)
    bIsSprinting = PlayerCharacter->GetIsSprinting();

    // Проверяем в воздухе ли персонаж
    bIsInAir = PlayerCharacter->GetCharacterMovement()->IsFalling();

    // Получаем тип оружия (через публичную функцию)
    CurrentWeaponType = PlayerCharacter->GetCurrentWeaponType();

    // Получаем состояние смены оружия (через публичную функцию)
    bIsSwitchingWeapon = PlayerCharacter->GetIsSwitchingWeapon();

    // Конвертируем тип оружия в строку для Blueprint
    switch (CurrentWeaponType)
    {
    case EWeaponType::Pistol:
        WeaponState = "Pistol";
        break;
    case EWeaponType::Rifle:
        WeaponState = "Rifle";
        break;
    default:
        WeaponState = "Unarmed";
        break;
    }

    // Определяем состояние движения
    if (bIsInAir)
    {
        MovementState = "Jumping";
    }
    else if (bIsCrouching)
    {
        MovementState = "Crouching";
    }
    else if (bIsSprinting && Speed > 100.0f)
    {
        MovementState = "Sprinting";
    }
    else if (Speed > 10.0f)
    {
        MovementState = "Walking";
    }
    else
    {
        MovementState = "Idle";
    }
}