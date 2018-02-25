// Copyright (c) IdeaShadow Company. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Spitfire.generated.h"

class USpitfireEngine;

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

	UFUNCTION(BlueprintCallable)
	void SetThrottle(float ThrottleValue);

	UFUNCTION(BlueprintCallable)
	float GetThrottle();

	UFUNCTION(BlueprintCallable)
	void YawRight(float YawAmount);

	UFUNCTION(BlueprintCallable)
	void PitchUp(float PitchAmount);

	UFUNCTION(BlueprintCallable)
	void RollRight(float RollAmount);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Input")
	void Initialize(USpitfireEngine* TheEngine);

private:
	UPROPERTY(EditAnywhere, Category = "Throttle")
	float StaticDrag = 0.001;

	UPROPERTY(EditAnywhere, Category = "Throttle")
	float ParasiticDrag = 0.001;


	UPROPERTY(EditAnywhere, Category = "Trim")
	float TrimYaw = 0.001;

	UPROPERTY(EditAnywhere, Category = "Trim")
	float TrimRoll = 0.001;

	UPROPERTY(EditAnywhere, Category = "Trim")
	float TrimPitch = 0.001;

	FRotator LastRotation = FRotator(0.0, 0.0, 0.0);

	USpitfireEngine* SpitfireEngine = nullptr;

	void ForwardThrust();

	void ApplyDrag();

	FVector GetForwardVector();

	FVector GetComponentLocation();

};
