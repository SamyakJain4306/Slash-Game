#pragma once
#include "DrawDebugHelpers.h"

#define DRAW_SQUARE(Location) if(GetWorld()) DrawDebugBox(GetWorld(), Location, FVector(10, 10, 10), FColor::Blue, true, -1.f, 0, 1.f )
#define DRAW_SQUARE_SINGLE_FRAME(Location) if(GetWorld()) DrawDebugBox(GetWorld(), Location, FVector(10, 10, 10), FColor::Blue, false, -1.f, 0, 1.f )
#define DRAW_LINE(Location, forward) if(GetWorld()) DrawDebugLine(GetWorld(), Location, forward*50, FColor::Cyan, true, -1.f, 0, 1.f)
#define DRAW_LINE_SINGLE_FRAME(Location, forward) if(GetWorld()) DrawDebugLine(GetWorld(), Location, forward*50, FColor::Cyan, false, -1.f, 0, 1.f)