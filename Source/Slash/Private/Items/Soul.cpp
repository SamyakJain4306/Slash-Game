// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Soul.h"
#include "NiagaraFunctionLibrary.h"
#include "Interfaces/PickupInterface.h"
#include "Kismet/GameplayStatics.h"

void ASoul::SpawnPickupEffect()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		PickupEffect,
		GetActorLocation()
	);
}

void ASoul::PlayPickupSound()
{
	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
}

void ASoul::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (IPickupInterface* PickupInteface = Cast<IPickupInterface>(OtherActor))
	{
		PickupInteface->AddSoul(this);
		SpawnPickupEffect();
		PlayPickupSound();
		Destroy();
	}
}

