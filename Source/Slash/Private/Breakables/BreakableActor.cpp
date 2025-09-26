// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakables/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"
#include "Components/CapsuleComponent.h"
#include "Physics/Experimental/ChaosEventType.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABreakableActor::ABreakableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(FName("GeometryCollection"));
	GeometryCollection->SetGenerateOverlapEvents(true);
	SetRootComponent(GeometryCollection);

	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);


	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(FName("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(GeometryCollection);

	CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

}

// Called when the game starts or when spawned
void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();

	GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &ABreakableActor::OnBreak);
	
}

// Called every frame
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::OnBreak(const FChaosBreakEvent& BreakEvent)
{
	if (bBroken) { return; }
	if(BreakSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BreakSound, GetActorLocation());
	}
	UWorld* World = GetWorld();
	if (World and TreasureBPClasses.Num())
	{
		const FVector SpawnLocation(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 75.f);
		World->SpawnActor<ATreasure>(TreasureBPClasses[FMath::RandRange(0, TreasureBPClasses.Num() - 1)], SpawnLocation, GetActorRotation());

	}
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bBroken = true;
}

