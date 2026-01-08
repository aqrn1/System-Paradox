#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "MiniMap.generated.h"

UCLASS()
class SYSTEM1PARADOX_API UMiniMap : public UWidget
{
    GENERATED_BODY()

public:
    UMiniMap();

protected:
    virtual void NativePaint(FPaintContext& Context) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniMap")
    TArray<AActor*> Players; // Игроки на миникарте

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniMap")
    FVector MapCenter; // Центр карты

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniMap")
    float ScaleFactor; // Масштаб миникарты
};
