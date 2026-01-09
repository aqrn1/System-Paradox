#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniMap.generated.h"

UCLASS()
class SYSTEM1PARADOX_API UMiniMap : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
};
