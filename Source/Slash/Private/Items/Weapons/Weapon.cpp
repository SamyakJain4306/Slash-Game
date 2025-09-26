 // Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/SlashCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	Box = CreateDefaultSubobject<UBoxComponent>(FName("Box"));
	Box->SetupAttachment(GetRootComponent());
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);

	TraceStart = CreateDefaultSubobject<USceneComponent>(FName("TraceStart"));
	TraceStart->SetupAttachment(GetRootComponent());
	TraceEnd = CreateDefaultSubobject<USceneComponent>(FName("TraceEnd"));
	TraceEnd->SetupAttachment(GetRootComponent());

	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxBeginOverlap);
}

void AWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}

void AWeapon::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(this->GetOwner()->ActorHasTag("Enemy") and OtherActor->ActorHasTag("Enemy"))
	{
		
		return;
	}
	const FVector Start = TraceStart->GetComponentLocation();
	const FVector End = TraceEnd->GetComponentLocation();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(this->GetOwner());

	for (AActor* Actors : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actors);
		UE_LOG(LogTemp, Warning, TEXT("Ignored Actor: %s"), *Actors->GetName());
	}

	FHitResult HitResult;
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(5.f),
		TraceStart->GetComponentRotation(),
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);
		if(HitResult.GetActor())
		{
			if (this->GetOwner()->ActorHasTag("Enemy") and OtherActor->ActorHasTag("Enemy"))
			{
				return;
			}
			UGameplayStatics::ApplyDamage(HitResult.GetActor(), Damage, GetInstigator()->GetController(), GetOwner(), UDamageType::StaticClass());

			IHitInterface* HitInterface = Cast<IHitInterface>(HitResult.GetActor());
			if (HitInterface)
			{
				HitInterface->Execute_GetHit(HitResult.GetActor(), HitResult.ImpactPoint, GetOwner());
				IgnoreActors.AddUnique(HitResult.GetActor());
			}
			CreateFields(HitResult.Location);
			if (Cast<ABaseCharacter>(GetOwner()) and Cast<ABaseCharacter>(HitResult.GetActor()))
			{
				ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());
				Character->CombatTarget = HitResult.GetActor();
			}
		}
}

void AWeapon::AttachWeapon(USceneComponent* InParent, FName SocketName, AActor* NewOwner, APawn* NewInstigator)
{
	ItemState = EItemStates::Equipped;
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(InParent, TransformRules, SocketName);
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	EmbersDisable();
}

void AWeapon::EmbersDisable()
{
	EmbersEffect->Deactivate();
}

void AWeapon::DettachWeapon()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetOwner(nullptr);
	SetInstigator(nullptr);
	EmbersEnable();
	ItemState = EItemStates::Hovering;
	SetActorRotation(FRotator(0, 0, 0));
}

void AWeapon::EmbersEnable()
{
	EmbersEffect->Activate();
}

UBoxComponent* AWeapon::GetBoxComponent()
{
	return Box;
}
