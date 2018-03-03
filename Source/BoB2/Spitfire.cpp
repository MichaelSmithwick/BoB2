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

	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	Controls.Aileron = 0.0;
	Controls.Elevator = 0.0;
	Controls.Rudder = 0.0;

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

	ApplyControls(DeltaTime);

	ForwardThrust();
	//ApplyDrag();

}

void ASpitfire::ApplyControls(float DeltaTime)
{
	// calculate roll from the forward vector
	// calculate pitch from the right vector
	// calculate yaw from the up vector
	FVector ForwardVector = GetForwardVector();
	FVector RightVector = GetActorRightVector();
	FVector UpVector = GetActorUpVector();

	// scale the vectors and add together
	ForwardVector *= Controls.Aileron * RollDegreesPerSecond * DeltaTime;
	RightVector *= Controls.Elevator * PitchDegreesPerSecond * DeltaTime;
	UpVector *= Controls.Rudder * YawDegreesPerSecond * DeltaTime;
	FVector ComboControlVector = ForwardVector + RightVector + UpVector;

	// apply torque
	ApplyTorque(ComboControlVector);
}

// Apply torque to the airframe
// @TorqueVector determines direction and strength of torque
void ASpitfire::ApplyTorque(const FVector& TorqueVector) const
{
	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(GetRootComponent());
	if (ensure(PrimitiveComponent))
	{
		PrimitiveComponent->AddTorqueInDegrees(TorqueVector, NAME_None, true);
	}
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

void ASpitfire::KillThrottle()
{
	SetThrottle(0.0);
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
// since the engine is out in front of the spitfire mesh the engine socket must
// be adjusted to offset vector changes when flying in a straight line.
void ASpitfire::ForwardThrust()
{
	FVector ForceApplied, ForceLocation;
	SpitfireEngine->GetSocketWorldLocationAndThrust(ForceLocation, ForceApplied);

	UPrimitiveComponent* SpitfireRoot = Cast<UPrimitiveComponent>(GetRootComponent());
	if (ensure(SpitfireRoot != nullptr))
	{
		SpitfireRoot->AddForceAtLocation(ForceApplied, ForceLocation);
		//SpitfireRoot->AddForce(ForceApplied);
	}
}

void ASpitfire::ApplyDrag()
{
	// calculate and apply airframe (static) drag
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();
	// drag is in opposite direction of forward motion
	Rotation = Rotation * -1.0;

	FVector Drag = Rotation.Vector();

	UE_LOG(LogTemp,Warning,TEXT("BULLDOG Drag Vector: %s"),*(Drag.ToString()))

	// don't apply more drag than there is forward force.
	FVector ForceApplied, ForceLocation;
	SpitfireEngine->GetSocketWorldLocationAndThrust(ForceLocation, ForceApplied);
	float ForwardForce = ForceApplied.Size();
	DragCoefficient = FMath::Clamp<float>(DragCoefficient, 0.0, ForwardForce);

	Drag *= DragCoefficient;

	UPrimitiveComponent* SpitfireRoot = Cast<UPrimitiveComponent>(GetRootComponent());
	if (ensure(SpitfireRoot != nullptr))
	{
		SpitfireRoot->AddForceAtLocation(Drag, Location);
		//SpitfireRoot->AddForce(ForceApplied);
	}

	// calculate parasitic drag which depends on current velocity (in this case throttle setting)
}

// add @YawAmount to the Rudder setting and check limits
void ASpitfire::YawRight(float YawAmount)
{
	SetControlSurface(Controls.Rudder, YawAmount, MaxYawDegPerSec);
}

// add @PitchAmount to Elevator setting and check limits
void ASpitfire::PitchUp(float PitchAmount)
{
	SetControlSurface(Controls.Elevator, PitchAmount, MaxPitchDegPerSec);
}

// add @RollAmount to Aileron setting and check limits
void ASpitfire::RollRight(float RollAmount)
{
	SetControlSurface(Controls.Aileron, RollAmount, MaxRollDegPerSec);
}

// add @DeltaChange to @ControlSurface and limit the result to range bounded by @LowLimit and @HighLimit
void ASpitfire::SetControlSurface(float& ControlSurface, float DeltaChange, float Limit)
{
	ControlSurface += DeltaChange;
	ControlSurface = FMath::Clamp<float>(ControlSurface, -Limit, Limit);
}

void ASpitfire::GetControlSurfaces(float & Aileron, float & Elevator, float & Rudder) const
{
	Aileron = Controls.Aileron;
	Elevator = Controls.Elevator;
	Rudder = Controls.Rudder;
}

FControlSurface& ASpitfire::GetControlSurfaces() const
{
	return (FControlSurface&)Controls;
}

void ASpitfire::CenterControls()
{
	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(GetRootComponent());
	if (ensure(PrimitiveComponent))
	{
		FVector AngularVelocity = PrimitiveComponent->GetPhysicsAngularVelocityInDegrees();
		AngularVelocity *= (-1.0 * AngularMultiplier);
		PrimitiveComponent->AddTorqueInDegrees(AngularVelocity, NAME_None, true);
	}

	Controls.Aileron = 0.0;
	Controls.Elevator = 0.0;
	Controls.Rudder = 0.0;
}
