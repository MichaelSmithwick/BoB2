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
	
	bCompassActive = false;
}

// get the Engine module associated with this aircraft
void ASpitfire::Initialize(USpitfireEngine * TheEngine)
{
	SpitfireEngine = TheEngine;
}

// Called every frame
void ASpitfire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// calculate Rate Of Climb and capture Last Rotation Vector
	if (MinuteCounter > 1.0)
	{
		MinuteCounter = 0.0;
		RateOfClimb = (GetAltitude() - MinuteAltitude) * 60;
		MinuteAltitude = GetAltitude();

		LastRotation = GetActorRotation();
	}
	else
	{
		MinuteCounter += DeltaTime;
	}

	// apply aircraft controls
	ApplyControls(DeltaTime);

	// apply engine produced thrust to aircraft
	ForwardThrust();

	//ApplyDrag();
}

// applies control surface settings as torque in the Pitch, Roll, and Yaw axises
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

	// combine all control inputs so they can be applied at the same time
	FVector ComboControlVector = ForwardVector + RightVector + UpVector;

	// apply torque
	ApplyTorque(ComboControlVector);
}

// calculate the current Pitch, Roll, or Yaw and return
// the calculated value as specified by the Axis flag
float ASpitfire::RealTimeAircraftAttitude(ERealTimeData Axis)
{
	float RotationValue = 0.5;

	switch (Axis)
	{
	case ERealTimeData::Pitch:
		RotationValue += Controls.Elevator / (2 * MaxPitchDegPerSec);
		break;
	case ERealTimeData::Roll:
		RotationValue += (-1.0 * Controls.Aileron) / (2 * MaxRollDegPerSec);
		break;
	case ERealTimeData::Yaw:
		RotationValue += Controls.Rudder / (2 * MaxYawDegPerSec);
		break;
	default:
		break;
	}

	return RotationValue;
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

// recieves ThrottleValue from player control and passes to the engine
void ASpitfire::SetThrottle(float ThrottleValue)
{
	SpitfireEngine->SetThrottle(ThrottleValue);

	// keeps compass from oscillating like crazy on startup
	if (bCompassActive == false)
	{
		//bCompassActive = true;
	}
}

// kills the throttle
void ASpitfire::KillThrottle()
{
	SetThrottle(0.0);
}

void ASpitfire::KillThrottleRestore()
{
	KillThrottle();
}

// getter for the current throttle setting
float ASpitfire::GetThrottle()
{
	return SpitfireEngine->GetThrottle();
}

// returns the Linear Velocity Vector for this aircraft
FVector ASpitfire::GetLinearVelocity()
{
	USceneComponent* Root = GetRootComponent();
	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Root);
	return PrimitiveComponent->GetPhysicsLinearVelocity();
}

// returns current aircraft velocity nautical miles per hour
int32 ASpitfire::GetKts()
{
	FVector LinearVelocity = GetLinearVelocity();
	return FMath::RoundToInt(LinearVelocity.Size() * CM2KTS);
}

// returns direction of aircraft in degrees
int32 ASpitfire::GetDirectionDegrees()
{
	//UE_LOG(LogTemp,Warning,TEXT("Compass Active : %s"), *(bCompassActive?FString("True"):FString("False")))

	float HeadingAngle = 0.0;
	FVector LinearVelocity = GetLinearVelocity();

	// if aircraft is moving use LinearVelocity to calculate heading angle
	// otherwise use location because it returns a steady (if incorrect) reading
	if(LinearVelocity.Size() > 1.0)
	{
		HeadingAngle = LinearVelocity.HeadingAngle();
	}
	else
	{
		FVector Direction = GetViewRotation().Vector();
		HeadingAngle = Direction.HeadingAngle();
	}

	float Dummy;
	return FMath::RoundToInt(FMath::Modf((HeadingAngle * (180.0 / PI) + 360.0) / 360.0, &Dummy) * 360.0);
}

// returns altitude of aircraft in feet above sea level
int32 ASpitfire::GetAltitude()
{
	FVector Location = GetActorLocation();
	float fAltitude = Location.Z;
	// convert from cm to feet
	fAltitude /= 100.0; // convert to meters
	fAltitude *= 39.39; // convert to inches
	fAltitude /= 12.0;  // convert to feet
	int32 Altitude = FMath::RoundToInt(fAltitude);
	return Altitude;
}

// returns aircraft Rate of Climb in feet per minute - calculated each second
int32 ASpitfire::GetRateOfClimb()
{
	return RateOfClimb;
}

// get the forward unit vector for this aircraft
FVector ASpitfire::GetForwardVector()
{
	return GetActorForwardVector();
}

// get the aircraft location in 3-D space
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
	//SpitfireEngine->SetRestrictor((6000.0 - (float)(FMath::Abs(GetRateOfClimb()))) / 6000.0);
	SpitfireEngine->GetSocketWorldLocationAndThrust(ForceLocation, ForceApplied);

	UPrimitiveComponent* SpitfireRoot = Cast<UPrimitiveComponent>(GetRootComponent());
	if (ensure(SpitfireRoot != nullptr))
	{
		SpitfireRoot->AddForceAtLocation(ForceApplied, ForceLocation);
	}
}

// applys drag to the aircraft -- NOT IMPLEMENTED --
void ASpitfire::ApplyDrag()
{
	// calculate and apply airframe (static) drag
	FVector Location = GetActorLocation();
	//FRotator Rotation = GetActorRotation();
	// drag is in opposite direction of forward motion
	//Rotation = Rotation * -1.0;

	//FVector Drag = Rotation.Vector();

	//UE_LOG(LogTemp,Warning,TEXT("BULLDOG Drag Vector: %s"),*(Drag.ToString()))

	// don't apply more drag than there is forward force.
	FVector ForceApplied, ForceLocation;
	SpitfireEngine->GetSocketWorldLocationAndThrust(ForceLocation, ForceApplied);
	//float ForwardForce = ForceApplied.Size();
	//DragCoefficient = FMath::Clamp<float>(DragCoefficient, 0.0, ForwardForce);

	//Drag *= DragCoefficient;

	ForceApplied *= FMath::Clamp<float>(((MaximumClimbRate - (float)(GetRateOfClimb())) / (2 * MaximumClimbRate)) - 1.0, -1.0, 0.0);

	UPrimitiveComponent* SpitfireRoot = Cast<UPrimitiveComponent>(GetRootComponent());
	if (ensure(SpitfireRoot != nullptr))
	{
		SpitfireRoot->AddForceAtLocation(ForceApplied, ForceLocation);
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

float ASpitfire::ShowLevel()
{
	FRotator Rotation = GetActorRotation();
	Rotation -= FRotator::ZeroRotator;
	Rotation *= 1.0/360.0;
	Rotation.Roll += 0.5;

	return Rotation.Roll;
}

float ASpitfire::ShowPitch()
{
	FRotator Rotation = GetActorRotation();
	Rotation -= FRotator::ZeroRotator;
	Rotation *= 1.0 / 360.0;
	Rotation.Pitch += 0.5;

	return Rotation.Pitch;
}

// add @DeltaChange to @ControlSurface and limit the result to range bounded by @LowLimit and @HighLimit
void ASpitfire::SetControlSurface(float& ControlSurface, float DeltaChange, float Limit)
{
	ControlSurface += DeltaChange;
	ControlSurface = FMath::Clamp<float>(ControlSurface, -Limit, Limit);
}

// returns the control surface values for this aircraft in @Aileron, @Elevator, and @Rudder
void ASpitfire::GetControlSurfaces(float & Aileron, float & Elevator, float & Rudder) const
{
	Aileron = Controls.Aileron;
	Elevator = Controls.Elevator;
	Rudder = Controls.Rudder;
}

// returns the ControlSurface struct with current control surface values
FControlSurface& ASpitfire::GetControlSurfaces() const
{
	return (FControlSurface&)Controls;
}

// Zeros all controls and applies an counter Angular Velocity to stop the axis rotations where they currently are.
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
