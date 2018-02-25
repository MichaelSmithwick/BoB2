// Copyright (c) IdeaShadow Company. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "SpitfireEngine.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOB2_API USpitfireEngine : public UStaticMeshComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpitfireEngine();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetThrottle(float ThrottleValue);
	float GetThrottle();
	float GetMaxThrust();
	FVector GetThrustVector();
	FVector GetForceLocation();

	void GetSocketWorldLocationAndRotation(FVector& Location, FVector& Thrust);

private:
	// How much to scale the throttle input
	// Assumes a throttle input range of -1.0 to 1.0 
	UPROPERTY(EditAnywhere, Category = "Throttle")
	float SpitfireThrottleScaler = 0.0003;

	UPROPERTY(VisibleAnywhere, Category = "Throttle")
	float SpitfireCurrentThrottle = 0.0;

	UPROPERTY(EditAnywhere, Category = "Throttle")
	float SpitfireMaxThrust = 100000.0;



	
};
