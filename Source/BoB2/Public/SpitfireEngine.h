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

	UPROPERTY(EditAnywhere, Category = "Throttle")
	float SpitfireMaxThrust = 35000000.0;

};
