// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PressurePlateBase.generated.h"

class UBoxComponent;

UCLASS()
class ESCAPEPROJECT_API APressurePlateBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APressurePlateBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsActivated();

private:
	//Get Weight on pressure plate function
	float GetWeight() const;
	//bool to check if pressure plate activated
	bool bActivated = false;
	float InitialPosition = 0.f;

	UPROPERTY(EditAnywhere, Category = "Pressure Plate", meta = (AllowPrivateAccess = "true"))
	float ActivationWeight = 50.f;
	UPROPERTY(EditAnywhere, Category = "Pressure Plate", meta = (AllowPrivateAccess = "true"))
	float PressureDistance = 100.f;
	UPROPERTY(EditAnywhere, Category = "Pressure Plate", meta = (AllowPrivateAccess = "true"))
	USoundBase* ActivateSound = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent = nullptr;
};
