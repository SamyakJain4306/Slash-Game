// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API ATreasure : public AItem
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Treasure")
	int32 GoldAmount;

protected:
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;


	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* TreasureSound;
	
public:
	FORCEINLINE int32 GetGoldAmount() const { return GoldAmount; }
};
