// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Attributes.h"

UAttributes::UAttributes()
{
	
	PrimaryComponentTick.bCanEverTick = true;

}



void UAttributes::BeginPlay()
{
	Super::BeginPlay();

	
}

void UAttributes::RecieveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	
}

float UAttributes::GetHealthPercent()
{
	return Health / MaxHealth;
}

bool UAttributes::IsAlive()
{
	return Health > 0;
}



void UAttributes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

