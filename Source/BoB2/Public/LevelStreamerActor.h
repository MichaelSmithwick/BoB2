// Copyright (c) IdeaShadow Company. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelStreamerActor.generated.h"

class UBoxComponent;

//
// This class loads/unloads Levels when a player enters/exits
// the associated Level Streaming Box
// 2 callback functions are provided for the system to use,
// one to load the Level and one to unload the Level. 
// The callback functions are assigned in the class constructor.
//

UCLASS()
class BOB2_API ALevelStreamerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelStreamerActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when actor enters streaming box
	UFUNCTION()
	void OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	// Called when actor leaves streaming box
	UFUNCTION()
	void OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Name of the level to load, specified in blueprints.
	UPROPERTY(EditAnywhere)
	FName LevelToLoad;

private:

	// Overlap volume to trigger level streaming
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* OverlapVolume;

	AActor* GetPlayer(const FString PlayerName);

};
