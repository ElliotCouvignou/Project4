// Project4 Copyright (Elliot Couvignou) Dont steal this mayne :(


#include "UI/CrosshairWidget.h"
#include "Characters/Project4Character.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Green,text)


void UCrosshairWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	TryFindCrosshairTarget();

	if (DistanceMeterType == EDistanceMeterType::DistanceToTarget && PlayerRef && PlayerRef->SelectedTarget)
	{
		SetDistanceText(PlayerRef->GetDistanceTo(PlayerRef->SelectedTarget));
	}
}
