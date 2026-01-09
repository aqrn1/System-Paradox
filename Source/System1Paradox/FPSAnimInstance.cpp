#include "FPSAnimInstance.h"

void UFPSAnimInstance::SetDebug(bool bEnable)
{
    bDebugEnabled = bEnable;

    UE_LOG(LogTemp, Warning, TEXT("FPSAnimInstance Debug: %s"),
        bDebugEnabled ? TEXT("ON") : TEXT("OFF"));
}
