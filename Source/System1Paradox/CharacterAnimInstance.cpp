#include "CharacterAnimInstance.h"
#include "Animation/AnimMontage.h"

void UCharacterAnimInstance::PlayHitReact(FName Direction)
{
    // Простая логика выбора монтажа по направлению
    if (Montage_HitReact_Front && Direction == TEXT("Front"))
    {
        Montage_Play(Montage_HitReact_Front);
    }
    // Добавьте логику для других направлений (Back, Left, Right)
}