// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Grabber.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APlayerBase::APlayerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create camera components
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Grabber = CreateDefaultSubobject<UGrabber>(TEXT("Grabber"));

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Physics Handle"));
}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
	GetCapsuleComponent()->GetScaledCapsuleSize(InitialCapsuleRadius, InitialCapsuleHalfHeight);
}

// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleSmoothCrouch(DeltaTime);
}

// Called to bind functionality to input
void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Setup movement and camera input
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerBase::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerBase::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &APlayerBase::LookRight);

	//Setup action inputs
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerBase::SmoothCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerBase::SmoothUnCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerBase::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerBase::StopJumping);

}

void APlayerBase::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), AxisValue);
	
}

void APlayerBase::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), AxisValue);
}

void APlayerBase::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void APlayerBase::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}


void APlayerBase::SmoothCrouch()
{
	bCrouch = true;
}

void APlayerBase::SmoothUnCrouch()
{
	bCrouch = false;
}

void APlayerBase::HandleSmoothCrouch(float DeltaTime)
{
	if (bCrouch)
	{
		float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		CapsuleHalfHeight = FMath::FInterpTo(CapsuleHalfHeight, CrouchHalfHeight, DeltaTime, CrouchSpeed);

		GetCapsuleComponent()->SetCapsuleHalfHeight(CapsuleHalfHeight);
	}
	else if (!GetWorld()->OverlapBlockingTestByChannel(GetActorLocation(), FQuat(0.f, 0.f, 0.f, 0.f), ECollisionChannel::ECC_Visibility, FCollisionShape::MakeCapsule(InitialCapsuleRadius-10.f, InitialCapsuleHalfHeight)))
	{

		float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		CapsuleHalfHeight = FMath::FInterpTo(CapsuleHalfHeight, InitialCapsuleHalfHeight, DeltaTime, CrouchSpeed);

		GetCapsuleComponent()->SetCapsuleHalfHeight(CapsuleHalfHeight);
	}
}

