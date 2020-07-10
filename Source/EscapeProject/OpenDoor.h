// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/AudioComponent.h"
#include "OpenDoor.generated.h"

class APressurePlateBase;

UENUM()
enum class Door : uint8
{
	Rotate UMETA(DisplayName = "Rotate"),
	Lift UMETA(DisplayName = "Lift"),
	Slide UMETA(DisplayName = "Slide")
};

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
	float InitialPosition;

	UPROPERTY()
	float TargetPosition;

	UPROPERTY(EditAnywhere, Category = "Door")
	Door DoorType = Door::Rotate;

	UPROPERTY(EditAnywhere, Category = "References")
	APressurePlateBase* PressurePlate = nullptr;

	void OpenDoor(float DeltaTime);

	void CloseDoor(float DeltaTime);

	void FindAudioComponent();

	void CheckPressurePlate();

	float DoorOpenTime;

	UPROPERTY(EditAnywhere, Category = "Door")
	float DoorDelay = 2.f;
	UPROPERTY(EditAnywhere, Category = "Door")
	float DoorOpenSpeed = 1.f;
	UPROPERTY(EditAnywhere, Category = "Door")
	float DoorCloseSpeed = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	USoundBase* DoorSound = nullptr;

	//Checks if door sound has been played
	bool PlayedOpenSound = false;
	bool PlayedCloseSound = true;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door", meta = (AllowPrivateAccess = "true"))
	bool bPlayerHasMass = false;
};


