// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Soul.generated.h"

class UNiagaraSystem;

UCLASS()
class SLASH_API ASoul : public AItem
{
	GENERATED_BODY()
	
private:
	
	UPROPERTY(EditAnywhere)
	int32 SoulsAmount;
	
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* PickupEffect;
	UPROPERTY(EditAnywhere)
	USoundBase* PickupSound;
	
	
	
	void SpawnPickupEffect();
	void PlayPickupSound();
	
protected:
	
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) ;

	
public:
	FORCEINLINE int32 GetSoulsAmount() { return SoulsAmount; };
};
