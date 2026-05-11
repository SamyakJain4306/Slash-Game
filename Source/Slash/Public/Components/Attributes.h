// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Attributes.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UAttributes : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributes();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	virtual void BeginPlay() override;

private:
	//Current health
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxHealth;
	
	UPROPERTY(EditAnywhere, Category = "Attributes")
	int32 Gold;
	
	UPROPERTY(EditAnywhere, Category = "Attributes")
	int32 Souls;


public:
	void RecieveDamage(float Damage);
	float GetHealthPercent();
	bool IsAlive();
	void AddSoul(int32 Amount);
	void AddGold(int32 Amount);

	
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE int32 GetGold() const { return Gold; }
		
};
