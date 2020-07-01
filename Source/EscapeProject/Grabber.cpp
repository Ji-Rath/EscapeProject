// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Grab physics object when there is a grabbed component
	if (PhysicsHandle && PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetGrabLocation());
	}
}

//Grab physics object in view
void UGrabber::Grab()
{
	if (!PhysicsHandle) { return; }
	FHitResult HitResult = GetPhysicsBodyInReach();
	if (HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(HitResult.GetComponent(), NAME_None, GetGrabLocation());
	}
}

//Release grab key
void UGrabber::Release()
{
	//Release component if grabbed
	if (!PhysicsHandle) {return;}
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

//Check physics handle component exists
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a physics handle."), *GetOwner()->GetName());
	}
}

//Check input component exists and bind actions
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing an input component."), *GetOwner()->GetName());
	}
}

FHitResult UGrabber::GetPhysicsBodyInReach() const
{
	//Ray-cast to detect object
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, GetOwner()->GetActorLocation(), GetGrabLocation(), FCollisionObjectQueryParams{ ECollisionChannel::ECC_PhysicsBody }, TraceParams);

	//Draw debug line
	//DrawDebugLine(GetWorld(), PlayerViewLocation, LineTraceEnd, FColor(0, 255, 0), false, 1.f);

	return Hit;
}

FVector UGrabber::GetGrabLocation() const
{
	//Get player viewport
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewLocation, OUT PlayerViewRotation);

	return PlayerViewLocation + (PlayerViewRotation.Vector() * Reach);
}


