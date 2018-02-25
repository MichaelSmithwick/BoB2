// Copyright (c) IdeaShadow Company. All Rights Reserved.

#include "LevelStreamerActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"


// Sets default values
ALevelStreamerActor::ALevelStreamerActor()
{
 	// Set this actor to call Tick() every frame. 
	PrimaryActorTick.bCanEverTick = true;

	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	RootComponent = OverlapVolume;

	// bind the callback function used to load the Level after the player enters the LevelStreaming Box
	OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &ALevelStreamerActor::OverlapBegins);

	// bind the callback function used to unload the Level after the player exits the LevelStreamingBox
	OverlapVolume->OnComponentEndOverlap.AddUniqueDynamic(this, &ALevelStreamerActor::OverlapEnds);
}

// Called when the game starts or when spawned
void ALevelStreamerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Callback function used when an actor enters the Level Streaming Box
// @OtherActor is checked agains the first player actor (assuming single player game) and
// @LevelToLoad is checked to see if a Level has been specified,
// If both conditions are met the specified Level in @LevelToLoad is loaded into the Gamespace.
// -----------------------------------------------------------------------
// if main pawn is a character use this, otherwise use GetPlayer()
// ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
void ALevelStreamerActor::OverlapBegins(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
	UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AActor* MyCharacter = GetPlayer(OtherActor->GetName());

	if (OtherActor == MyCharacter && LevelToLoad != "")
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);
		// UE_LOG(LogTemp,Warning,TEXT("DOG Load %s"),*(LevelToLoad.ToString()))
	}
}

// Callback function used when an actor exits the Level Streaming Box
// @OtherActor is checked agains the first player actor (assuming single player game) and
// @LevelToLoad is checked to see if a Level has been specified,
// If both conditions are met the specified Level in @LevelToLoad is unloaded from the Gamespace.
void ALevelStreamerActor::OverlapEnds(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
	UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	AActor* MyCharacter = GetPlayer(OtherActor->GetName());

	if (OtherActor == MyCharacter && LevelToLoad != "")
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::UnloadStreamLevel(this, LevelToLoad, LatentInfo);
		// UE_LOG(LogTemp, Warning, TEXT("DOG Unload %s"), *(LevelToLoad.ToString()))
	}
}

// Iterate through list of pawns and return found pawn if name matches @PlayerName
// or nullptr if no match is found
AActor* ALevelStreamerActor::GetPlayer(const FString PlayerName)
{
	APawn* FoundPawn = nullptr;

	FConstPawnIterator PawnIterator = GetWorld()->GetPawnIterator();
	
	for (; PawnIterator; ++PawnIterator)
	{
		APawn* Pawn = *PawnIterator;
		FString PawnName = Pawn->GetName();

		if (PawnName == PlayerName)
		{
			FoundPawn = Pawn;
			break;
		}
	}

	return FoundPawn;
}

// Called every frame
void ALevelStreamerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

