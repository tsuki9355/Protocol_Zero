// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI/PZHudFormat.h"

FText PZHudFormat::HealthText(float Current, float Max)
{
	const int32 MaxShown = FMath::Max(FMath::CeilToInt(Max), 0);
	const int32 CurrentShown = FMath::Clamp(FMath::CeilToInt(Current), 0, MaxShown);
	return FText::Format(NSLOCTEXT("ProtocolZero", "HealthValue", "{0} / {1}"),
		FText::AsNumber(CurrentShown), FText::AsNumber(MaxShown));
}
