// Copyright (c) IdeaShadow Company. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Spitfire.generated.h"

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

	void ApplyControls(float DeltaTime);


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetThrottle(float ThrottleValue);

	UFUNCTION(BlueprintCallable)
	void KillThrottle();

	UFUNCTION(BlueprintCallable)
	float GetThrottle();

	UFUNCTION(BlueprintCallable)
	void YawRight(float YawAmount);

	UFUNCTION(BlueprintCallable)
	void PitchUp(float PitchAmount);

	UFUNCTION(BlueprintCallable)
	void RollRight(float RollAmount);

	UFUNCTION(BlueprintCallable)
	struct FControlSurface& GetControlSurfaces() const;

	UFUNCTION(BlueprintCallable)
	void CenterControls();

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
	float PitchDegreesPerSecond = 45.0;

	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float RollDegreesPerSecond = 60.0;

	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float YawDegreesPerSecond = 60.0;

	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float MaxRollDegPerSec = 300.0;

	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float MaxPitchDegPerSec = 300.0;

	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float MaxYawDegPerSec = 300.0;

	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float DragCoefficient = 10000.0;

	// This value is multiplied with the negative "angular velocity in degrees" vector 
	// and used to add negative torque to the aircraft to stop it from rotating about
	// any of it's three axes
	UPROPERTY(EditAnywhere, Category = "SpitfireControl")
	float AngularMultiplier = 75.0;

	USpitfireEngine* SpitfireEngine = nullptr;

	void ForwardThrust();

	void ApplyDrag();

	FVector GetForwardVector();

	FVector GetComponentLocation();

	// apply torque to airframe in direction and strength given by the TorqueVector
	void ApplyTorque(const FVector& TorqueVector) const;

	void GetControlSurfaces(float& Aileron, float& Elevator, float& Rudder) const;

	// add @DeltaChange to @ControlSurface and limit the result to range bounded by @LowLimit and @HighLimit
	void SetControlSurface(float& ControlSurface, float DeltaChange, float Limit);
};
