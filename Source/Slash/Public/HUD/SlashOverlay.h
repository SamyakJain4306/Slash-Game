// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlashOverlay.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API USlashOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthPercent(float Percent);
	void SetGoldAmount(int32 Amount);
	void SetSoulsAmount(int32 Amount);
	void SetStaminaPercent(float Percent);
	
	
private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressHealth;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressStamina;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GoldNumber;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SoulsNumber;
};
