// Copyright (c) IdeaShadow Company. All Rights Reserved.

#include "SpitfireEngine.h"
#include "Engine/World.h"


// Sets default values for this component's properties
USpitfireEngine::USpitfireEngine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpitfireEngine::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USpitfireEngine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USpitfireEngine::SetThrottle(float ThrottleValue)
{
	ThrottleValue = FMath::Clamp<float>(ThrottleValue, -1.0, 1.0);
	SpitfireCurrentThrottle += ThrottleValue * SpitfireThrottleScaler * GetWorld()->GetTimeSeconds();
	SpitfireCurrentThrottle = FMath::Clamp<float>(SpitfireCurrentThrottle, 0.0, 1.0);
}

float USpitfireEngine::GetThrottle()
{
	return SpitfireCurrentThrottle;
}

float USpitfireEngine::GetMaxThrust()
{
	return SpitfireMaxThrust;
}

FVector USpitfireEngine::GetThrustVector()
{
	FVector EngineVector = GetForwardVector();
	EngineVector.Z = 0;
	EngineVector.Y = 0;
	FVector ForceApplied = EngineVector * SpitfireCurrentThrottle * SpitfireMaxThrust;

	return ForceApplied;
}

FVector USpitfireEngine::GetForceLocation()
{
	return GetSocketLocation(FName("Engine"));
}

void USpitfireEngine::GetSocketWorldLocationAndRotation(FVector & Location, FVector & Thrust)
{
	Thrust = GetThrustVector();
	Location = GetForceLocation();
}

