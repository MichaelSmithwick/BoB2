// Copyright (c) IdeaShadow Company. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpitfirePlayerController.generated.h"

class ASpitfire;

/**
 * 
 */
UCLASS()
class BOB2_API ASpitfirePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Setup")
	ASpitfire * GetControlledSpitfire() const;

protected:
	virtual void BeginPlay() override;
	
};
