#include "MiniMap.h"
#include "Engine/World.h"
#include "Engine/Canvas.h"

UMiniMap::UMiniMap()
{
    ScaleFactor = 1.0f;
    MapCenter = FVector(0.0f, 0.0f, 0.0f);
}

void UMiniMap::NativePaint(FPaintContext& Context)
{
    if (Canvas == nullptr || Players.Num() == 0) return;

    for (AActor* Player : Players)
    {
        FVector PlayerLocation = Player->GetActorLocation();
        FVector2D PositionOnMap = FVector2D((PlayerLocation.X - MapCenter.X) * ScaleFactor, (PlayerLocation.Y - MapCenter.Y) * ScaleFactor);
        DrawRect(FLinearColor::Green, PositionOnMap.X, PositionOnMap.Y, 10.0f, 10.0f);
    }
}
