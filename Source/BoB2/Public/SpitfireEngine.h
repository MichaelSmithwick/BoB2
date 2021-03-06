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
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetThrottle(float ThrottleValue);
	float GetThrottle();
	float GetMaxThrust();
	void SetRestrictor(float RestrictionLevel);

	UFUNCTION(BlueprintCallable)
	FVector GetThrustVector();

	UFUNCTION(BlueprintCallable)
	FVector GetForceLocation();

	void GetSocketWorldLocationAndThrust(FVector& Location, FVector& Thrust);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Throttle")
	float SpitfireCurrentThrottle = 0.0;

	// The max speed of the spitfire was 320 kts @ 4300 lbs (2000 kg)
	// Add enough thrust to get that speed
	UPROPERTY(EditAnywhere, Category = "Throttle")
	float SpitfireMaxThrust = 40000000.0;

	float Restrictor = 1.0;

};
