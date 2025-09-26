// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
class UBoxComponent;
UCLASS()


class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* TraceStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* TraceEnd;

	TArray<AActor*> IgnoreActors;

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& Location);

	void AttachWeapon(USceneComponent* InParent, FName SocketName, AActor* NewOwner, APawn* NewInstigator);

	void DettachWeapon();


	FORCEINLINE UBoxComponent* GetBoxComponent();


protected:

	virtual void BeginPlay() override;

	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;


	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

	void EmbersDisable();

	void EmbersEnable();
	
};
