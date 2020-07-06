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

}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	switch (DoorType)
	{
		case Door::Rotate:
		{
			InitialPosition = GetOwner()->GetActorRotation().Yaw;
			TargetPosition = InitialPosition + 90.f;
		}
		break;
		case Door::Lift:
		{
			InitialPosition = GetOwner()->GetActorLocation().Z;
			TargetPosition = InitialPosition + 200.f;
		}
		break;
	}
	FindAudioComponent();
	CheckPressurePlateComponent();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	OpenDoor(DeltaTime);
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	if (PressurePlate && TotalMassOfActors() > MassThreshold)
	{
		switch (DoorType)
		{
		case Door::Rotate:
		{
			float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
			CurrentYaw = FMath::Lerp(CurrentYaw, TargetPosition, DoorOpenSpeed * DeltaTime);
			GetOwner()->SetActorRotation(FRotator(0.f, CurrentYaw, 0.f));
		}
		break;
		case Door::Lift:
		{
			float CurrentZ = GetOwner()->GetActorLocation().Z;
			CurrentZ = FMath::Lerp(CurrentZ, TargetPosition, DoorOpenSpeed * DeltaTime);
			FVector CurrentPos = GetOwner()->GetActorLocation();
			CurrentPos.Z = CurrentZ;
			GetOwner()->SetActorLocation(CurrentPos);
		}
		break;
		}
		if (!DoorSound) { return; }
		if (!PlayedOpenSound)
		{
			DoorSound->Play();
			PlayedOpenSound = true;
			PlayedCloseSound = false;
		}
		DoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	else if ((GetWorld()->GetTimeSeconds() - DoorOpenTime) > DoorDelay)
	{
		CloseDoor(DeltaTime);
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	if (PressurePlate && !(TotalMassOfActors() > MassThreshold))
	{
		switch (DoorType)
		{
			case Door::Rotate:
			{
				float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
				CurrentYaw = FMath::Lerp(CurrentYaw, InitialPosition, DoorCloseSpeed * DeltaTime);
				GetOwner()->SetActorRotation(FRotator(0.f, CurrentYaw, 0.f));
			}
			break;
			case Door::Lift:
			{
				float CurrentZ = GetOwner()->GetActorLocation().Z;
				CurrentZ = FMath::Lerp(CurrentZ, InitialPosition, DoorOpenSpeed * DeltaTime);
				FVector CurrentPos = GetOwner()->GetActorLocation();
				CurrentPos.Z = CurrentZ;
				GetOwner()->SetActorLocation(CurrentPos);
			}
			break;
		}

		if (!DoorSound) { return; }
		if (!PlayedCloseSound)
		{
			DoorSound->Play();
			PlayedCloseSound = true;
			PlayedOpenSound = false;
		}
	}
}

void UOpenDoor::FindAudioComponent()
{
	DoorSound = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!DoorSound)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing Audio Component!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::CheckPressurePlateComponent()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing Trigger Volume Component!"), *GetOwner()->GetName());
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	//Find overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) {return 0.f;}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
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

