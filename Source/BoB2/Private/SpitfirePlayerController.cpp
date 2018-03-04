// Copyright (c) IdeaShadow Company. All Rights Reserved.

#include "SpitfirePlayerController.h"
#include "Spitfire.h"

ASpitfire * ASpitfirePlayerController::GetControlledSpitfire() const
{
	return Cast<ASpitfire>(GetPawn());
}

void ASpitfirePlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("ASpitfirePlayerController::BeginPlay() called."))

	ASpitfire* PlayerSpitfire = GetControlledSpitfire();

	if (PlayerSpitfire != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player found."))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player not found."))
	}
}
