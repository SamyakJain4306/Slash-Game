// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USlashOverlay::SetHealthPercent(float Percent)
{
	if(ProgressHealth)
	{
		ProgressHealth->SetPercent(Percent / 100.0f);
	}
}

void USlashOverlay::SetGoldAmount(int32 Amount)
{
	if (GoldNumber)
	{
		GoldNumber->SetText(FText::AsNumber(Amount));
	}
}

void USlashOverlay::SetSoulsAmount(int32 Amount)
{
	if (SoulsNumber)
	{
		SoulsNumber->SetText(FText::AsNumber(Amount));
	}
}

void USlashOverlay::SetStaminaPercent(float Percent)
{
	if (ProgressStamina)
	{
		ProgressStamina->SetPercent(Percent / 100.0f);
	}
}
