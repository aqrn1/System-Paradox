#include "System1ParadoxCameraManager.h"
#include "System1ParadoxCharacter.h"

ASystem1ParadoxCameraManager::ASystem1ParadoxCameraManager()
{
    // Базовый FOV как в CS:GO
    DefaultFOV = 90.0f;
}

// УБИРАЕМ проблемную функцию - она нам не нужна сейчас
// Мы добавим ее позже, когда реализуем спринт