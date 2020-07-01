// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "OpenDoor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DoorDelay = 2.f;
	DoorOpenTime = 0.f;

	DoorOpenSpeed = 0.5f;
	DoorCloseSpeed = 1.f;

	MassThreshold = 50.f;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	TargetYaw = InitialYaw + 90.f;

	Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	// ...
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	OpenDoor(DeltaTime);
	// ...
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	if(Player && PressurePlate && TotalMassOfActors() > MassThreshold)
	{
		float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
		CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, DoorOpenSpeed * DeltaTime);
		GetOwner()->SetActorRotation(FRotator(0.f, CurrentYaw, 0.f));
		DoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	else if((GetWorld()->GetTimeSeconds() - DoorOpenTime) > DoorDelay)
	{
		CloseDoor(DeltaTime);
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	if (Player && PressurePlate && !PressurePlate->IsOverlappingActor(Player))
	{
		float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
		CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DoorCloseSpeed * DeltaTime);
		GetOwner()->SetActorRotation(FRotator(0.f, CurrentYaw, 0.f));
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	//Find overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OverlappingActors);
	for (int32 i = 0; i < OverlappingActors.Num(); i++)
	{
		TotalMass += OverlappingActors[i]->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	//total the masses
	//UE_LOG(LogTemp, Log, TEXT("Mass: %f"), TotalMass);
	return TotalMass;
}

