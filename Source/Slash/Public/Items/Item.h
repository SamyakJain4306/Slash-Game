// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	float Amplitude = 5.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeConstant = 5.f;

	UFUNCTION(BlueprintPure)
	float TransformedSin();

	UFUNCTION(BlueprintPure)
	float TransformedCos();
	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	template <typename T>
	T Avg(T First, T Second);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* ItemEffect;

	
	
	enum EItemStates : uint8
	{
		Hovering,
		Equipped
	};
	EItemStates ItemState = EItemStates::Hovering;

private:
	

	UPROPERTY(VisibleAnywhere)
	float RunningTime = 0;


	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	
	
};

template<typename T>
inline T AItem::Avg(T First, T Second)
{
	return (First + Second)/2;
}
