// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlateBase.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"

// Sets default values
APressurePlateBase::APressurePlateBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pressure Plate"));
	RootComponent = StaticMesh;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APressurePlateBase::BeginPlay()
{
	Super::BeginPlay();

	InitialPosition = StaticMesh->GetRelativeTransform().GetLocation().Z;
}

// Called every frame
void APressurePlateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Get weight and use proportion (PressureDistance, ActivationWeight) to move pressure plate accordingly
	//When activation weight is reached, activate bool, play sound
	float Mass = GetWeight();
	if (Mass > 0)
	{
		float CurrentLocation = StaticMesh->GetRelativeTransform().GetLocation().Z;
		float TargetLocation = FMath::Clamp(Mass/ActivationWeight, 0.f, 1.f);
		UE_LOG(LogTemp, Log, TEXT("Normalized Weight: %f"), TargetLocation);
		TargetLocation = (InitialPosition)-TargetLocation*(PressureDistance);
		CurrentLocation = FMath::FInterpTo(CurrentLocation, TargetLocation, DeltaTime, 3.f);
		UE_LOG(LogTemp, Log, TEXT("Mass: %f, CurrentLocation: %f, TargetLocation: %f"), Mass, CurrentLocation, TargetLocation);

		FVector CurrentPosition = StaticMesh->GetRelativeTransform().GetLocation();
		CurrentPosition.Z = CurrentLocation;
		StaticMesh->SetRelativeLocation(CurrentPosition);

		if (ActivationWeight <= FMath::RoundToInt(Mass))
		{
			bActivated = true;
		}
		else
		{
			bActivated = false;
		}
	}
	else
	{
		FVector CurrentPosition = StaticMesh->GetRelativeTransform().GetLocation();
		CurrentPosition.Z = FMath::FInterpTo(CurrentPosition.Z, InitialPosition, DeltaTime, 3.f);
		StaticMesh->SetRelativeLocation(CurrentPosition);

		bActivated = false;
	}
}

bool APressurePlateBase::IsActivated()
{
	return bActivated;
}

float APressurePlateBase::GetWeight() const
{
	float TotalMass = 0.f;

	//Find overlapping actors
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OUT OverlappingActors);
	for (int32 i = 0; i < OverlappingActors.Num(); i++)
	{
		if (OverlappingActors[i]->ActorHasTag(TEXT("Physics")))
		{
			TotalMass += OverlappingActors[i]->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
	}

	//total the masses
	//UE_LOG(LogTemp, Log, TEXT("Mass: %f"), TotalMass);
	return TotalMass;
}

