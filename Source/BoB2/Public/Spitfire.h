// Copyright (c) IdeaShadow Company. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Spitfire.generated.h"

// Multiplier used to convert cm/sec to knots (nm/hr) example: 8000 cm/sec * CM2KTS = 155.6917 kts
const float CM2KTS = 0.019461462;

UENUM()
enum class ERealTimeData : uint8
{
	Pitch,
	Roll,
	Yaw
};

class USpitfireEngine;

USTRUCT(BlueprintType)
struct FControlSurface 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	float Aileron;
	UPROPERTY(BlueprintReadOnly)
	float Elevator;
	UPROPERTY(BlueprintReadOnly)
	float Rudder;
};

UCLASS()
class BOB2_API ASpitfire : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpitfire();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// apply current control inputs to this aircraft
	void ApplyControls(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	float RealTimeAircraftAttitude(ERealTimeData Axis);

	UFUNCTION(BlueprintCallable)
	void SetThrottle(float ThrottleValue);

	UFUNCTION(BlueprintCallable)
	void KillThrottle();

	UFUNCTION(BlueprintCallable)
	void KillThrottleRestore();

	UFUNCTION(BlueprintCallable)
	float GetThrottle();

	UFUNCTION(BlueprintCallable)
	int32 GetKts();

	UFUNCTION(BlueprintCallable)
	int32 GetDirectionDegrees();

	UFUNCTION(BlueprintCallable)
	int32 GetAltitude();

	UFUNCTION(BlueprintCallable)
	int32 GetRateOfClimb();

	UFUNCTION(BlueprintCallable)
	void YawRight(float YawAmount);

	UFUNCTION(BlueprintCallable)
	void PitchUp(float PitchAmount);

	UFUNCTION(BlueprintCallable)
	void RollRight(float RollAmount);

	UFUNCTION(BlueprintCallable)
	float ShowLevel();

	UFUNCTION(BlueprintCallable)
	float ShowPitch();

	UFUNCTION(BlueprintCallable)
	void CenterControls();

	UFUNCTION(BlueprintCallable)
	struct FControlSurface& GetControlSurfaces() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "SpitfireControl")
	void Initialize(USpitfireEngine* TheEngine);

	UPROPERTY(BlueprintReadOnly)
	struct FControlSurface Controls;

private:
	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float StaticDrag = 0.001;

	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float ParasiticDrag = 0.001;


	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float TrimYaw = 0.001;

	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float TrimRoll = 0.001;

	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float TrimPitch = 0.001;

	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float MaxElevator = 10.;

	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float MinElevator = -10.0;

	UPROPERTY(VisibleAnywhere, Category = "SpitfireControl")
	float CurrentElevator = 0.0;

	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float PitchDegreesPerSecond = 20.0;

	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float RollDegreesPerSecond = 60.0;

	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float YawDegreesPerSecond = 60.0;

	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float MaxRollDegPerSec = 300.0;

	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float MaxPitchDegPerSec = 200.0;

	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float MaxYawDegPerSec = 300.0;

	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float DragCoefficient = 10000.0;

	// This value is multiplied with the negative "angular velocity in degrees" vector 
	// and used to add negative torque to the aircraft to stop it from rotating about
	// any of it's three axes
	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float AngularMultiplier = 45.0;

	// This number is used to restrict the rate of climb of the aircraft
	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float MaximumClimbRate = 2200.0; // feet per minute

	USpitfireEngine* SpitfireEngine = nullptr;

	FRotator LastRotation;
	bool bToggle = false;

	float MinuteCounter = 0.0; // seconds
	int32 MinuteAltitude = 0;  // feet
	int32 RateOfClimb = 0;     // feet per minute

	// set true after first aircraft movement - keeps compass from oscillating like crazy on startup
	bool bCompassActive = false;

	void ForwardThrust();

	void ApplyDrag();

	FVector GetForwardVector();

	FVector GetComponentLocation();

	// apply torque to airframe in direction and strength given by the TorqueVector
	void ApplyTorque(const FVector& TorqueVector) const;

	void GetControlSurfaces(float& Aileron, float& Elevator, float& Rudder) const;

	// add @DeltaChange to @ControlSurface and limit the result to range bounded by @LowLimit and @HighLimit
	void SetControlSurface(float& ControlSurface, float DeltaChange, float Limit);

	// return the Linear Velocity vector for this Spitfire
	FVector GetLinearVelocity();
};
