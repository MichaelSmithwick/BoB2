// Copyright (c) IdeaShadow Company. All Rights Reserved.

#include "SpitfireEngine.h"
#include "Components/PrimitiveComponent.h"
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

// Set Throttle Level
// @ThrottleValue input is between 1.0 (Throttle UP) and -1.0 (Throttle DOWN)
// @USpitfireEngine::SpitfireCurrentThrottle is set and clamped between 0.0 and 1.0
void USpitfireEngine::SetThrottle(float ThrottleValue)
{
	SpitfireCurrentThrottle += FMath::Clamp<float>(ThrottleValue, -1.0, 1.0) * GetWorld()->GetDeltaSeconds();
	SpitfireCurrentThrottle = FMath::Clamp<float>(SpitfireCurrentThrottle, 0.0, 1.0);
}

// getter for throttle setting
float USpitfireEngine::GetThrottle()
{
	return SpitfireCurrentThrottle;
}

// getter for maximum thrust
float USpitfireEngine::GetMaxThrust()
{
	return SpitfireMaxThrust;
}

// Thrust Vector is in one direction only
// The direction is along the forward vector of the engine socket
FVector USpitfireEngine::GetThrustVector()
{
	// set thrust value for engine
	FRotator SocketRotator = GetSocketRotation(FName("Engine"));
	FVector ForwardVector = SocketRotator.Vector();
	FVector ThrustVector = ForwardVector * SpitfireCurrentThrottle * SpitfireMaxThrust;

	// if the throttle is currently at 0.0 then feed in a reverse direction vector
	// to slow the plane to a stop.
	USceneComponent* Root = GetAttachmentRoot();
	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Root);
	FVector LinearVelocity = PrimitiveComponent->GetPhysicsLinearVelocity();
	if (SpitfireCurrentThrottle < 0.001)
	{
		ThrustVector = -10.0 * LinearVelocity;
	}

	return ThrustVector;
}

// The force is being applied at the Engine Socket Location
FVector USpitfireEngine::GetForceLocation()
{
	return GetSocketLocation(FName("Engine"));
}

// getter for thrust location and thrust vector
void USpitfireEngine::GetSocketWorldLocationAndThrust(FVector & Location, FVector & Thrust)
{
	Thrust = GetThrustVector();
	Location = GetForceLocation();
}

