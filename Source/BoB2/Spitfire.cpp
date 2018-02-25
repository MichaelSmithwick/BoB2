// Copyright (c) IdeaShadow Company. All Rights Reserved.

#include "Spitfire.h"
#include "SpitfireEngine.h"
#include "Math/UnrealMath.h"
#include "Engine/GameInstance.h"
#include "Components/PrimitiveComponent.h"
#include "Math/Rotator.h"
#include "Engine/World.h"


// Sets default values
ASpitfire::ASpitfire()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpitfire::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpitfire::Initialize(USpitfireEngine * TheEngine)
{
	SpitfireEngine = TheEngine;
}

// Called every frame
void ASpitfire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator CurrentRotation = GetActorRotation().GetNormalized();
	FRotator DeltaRotation = CurrentRotation - LastRotation;

	ForwardThrust();
	ApplyDrag();

}

// Called to bind functionality to input
void ASpitfire::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASpitfire::SetThrottle(float ThrottleValue)
{
	SpitfireEngine->SetThrottle(ThrottleValue);
}

float ASpitfire::GetThrottle()
{
	return SpitfireEngine->GetThrottle();
}

FVector ASpitfire::GetForwardVector()
{
	return GetActorForwardVector();
}

FVector ASpitfire::GetComponentLocation()
{
	return GetActorLocation();
}

// moves spitfire by applying force based on Throttle value
void ASpitfire::ForwardThrust()
{
	FVector ForceApplied, ForceLocation;
	SpitfireEngine->GetSocketWorldLocationAndRotation(ForceLocation, ForceApplied);

	FVector Fuselage = GetActorLocation();
	FVector Delta = Fuselage - ForceLocation;

	UE_LOG(LogTemp,Warning,TEXT("GOOSE Fuselage: %s, Thrust: %s, Delta: %s, Applied Force Vector: %s"),
		*(Fuselage.ToString()),*(ForceLocation.ToString()),*(Delta.ToString()),*(ForceApplied.ToString()))

	UPrimitiveComponent* SpitfireRoot = Cast<UPrimitiveComponent>(GetRootComponent());
	if (ensure(SpitfireRoot != nullptr))
	{
		SpitfireRoot->AddForceAtLocation(ForceApplied, ForceLocation);
	}
}

void ASpitfire::ApplyDrag()
{
	// calculate and apply airframe (static) drag
	FRotator SpitfireRotator = GetActorRotation().GetNormalized();
	FVector SpitfireVector = GetActorForwardVector();
	float VelocityVector = FVector::DotProduct(SpitfireVector, FVector(1, 1, 1));

	//UE_LOG(LogTemp,Warning,TEXT("DUCK %f"),VelocityVector)

	SpitfireRotator.Roll = 0.0;
	SpitfireRotator.Pitch = 0.0;
	SpitfireRotator.Yaw += TrimYaw * VelocityVector;  // Stop the Induced Yaw

	SetActorRotation(SpitfireRotator);

	//UE_LOG(LogTemp,Warning,TEXT("PENGUIN Rotator: %s, Vector: %s"),*(SpitfireRotator.ToString()),*(SpitfireVector.ToString()))

	// calculate parasitic drag which depends on current velocity (in this case throttle setting)
}

void ASpitfire::YawRight(float YawAmount)
{
	FRotator CurrentRotation = GetActorRotation().GetNormalized();
	YawAmount = FMath::Clamp<float>(YawAmount, -180.0, 180.0);

	CurrentRotation.Yaw += YawAmount;

	SetActorRotation(CurrentRotation);
}

void ASpitfire::PitchUp(float PitchAmount)
{
}

void ASpitfire::RollRight(float RollAmount)
{
}
