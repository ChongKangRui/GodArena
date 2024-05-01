// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "Engine/EngineTypes.h"
#include "SAction_8WaysDodge_Player.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API USAction_8WaysDodge_Player : public USAction
{
	GENERATED_BODY()

public:
	virtual void Init(TObjectPtr <class AGodsArenaCharacter> owner, FCharacterCombat combatStruct) override;
	virtual void OnActionBegin_Implementation() override;
	virtual void OnActionEnd_Implementation() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Whether wan to increase the dodge distance or not"))
	float dodgeAnimRate = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "float is dot product direction, int is the index montage gonna play"))
	TMap<FVector2D, int> DirectionMontageMap = {
		//Forward Backward
		{{0.0, 1.0}, 0 }, { {0.0, -1.0}, 1 },
		//Left Right
		{{-1.0, 0.0}, 2},{{1.0, 0.0}, 3},
		//Upleft UpRight
		{{-1, 1}, 4},{{1, 1}, 5},
		//Downleft DownRight
		{{-1, -1}, 6},{{1, -1}, 7}
	};

private:
	int GetClosestAngleMontageInt(FVector2D inputDir);

private:
	FTimerHandle DodgeFinishedHandle;
	class APlayerCharacter* player;
	float alpha;
	float EndDodgeDuration;

	
};
