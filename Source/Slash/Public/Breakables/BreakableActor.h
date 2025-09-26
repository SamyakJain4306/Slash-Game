// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class SLASH_API ABreakableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();



protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
	void OnBreak(const struct FChaosBreakEvent& BreakEvent);

private:
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;


	UPROPERTY(EditAnywhere, Category = "Treasure")
	TArray<TSubclassOf<class ATreasure>> TreasureBPClasses;

	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase* BreakSound;


	bool bBroken = false;

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* CapsuleComponent;

};
