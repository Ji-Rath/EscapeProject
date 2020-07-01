// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/AudioComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEPROJECT_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	float InitialYaw;

	UPROPERTY()
	float TargetYaw;

	UPROPERTY(EditAnywhere, Category = "References")
	ATriggerVolume* PressurePlate = nullptr;

	void OpenDoor(float DeltaTime);

	void CloseDoor(float DeltaTime);

	void FindAudioComponent();

	void CheckPressurePlateComponent();

	
	float DoorOpenTime;

	float TotalMassOfActors() const;

	UPROPERTY(EditAnywhere)
	float MassThreshold = 50.f;

	UPROPERTY(EditAnywhere, Category = "Door")
	float DoorDelay = 2.f;
	UPROPERTY(EditAnywhere, Category = "Door")
	float DoorOpenSpeed = 1.f;
	UPROPERTY(EditAnywhere, Category = "Door")
	float DoorCloseSpeed = 2.f;

	UPROPERTY(EditAnywhere, Category = "Audio")
	UAudioComponent* DoorSound = nullptr;

	//Checks if door sound has been played
	bool PlayedOpenSound = false;
	bool PlayedCloseSound = false;
};
