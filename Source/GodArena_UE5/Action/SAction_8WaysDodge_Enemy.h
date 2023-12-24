// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SAction_8WaysDodge_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API USAction_8WaysDodge_Enemy : public USAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Whether wan to increase the dodge distance or not"))
	float dodgeAnimRate = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "float is dot product direction, int is the index montage gonna play"))
	TMap<FVector2D, int> DirectionMontageMap = { 
		//Forward Backward
		{{1.0, 0.0}, 0 }, { {-1.0, 0.0}, 1 },
		//Left Right
		{{0.0, -1.0}, 2},{{0.0, 1.0}, 3},
		//Upleft UpRight
		{{0.5, -0.5}, 4},{{0.5, 0.5}, 5},
		//Downleft DownRight
		{{-0.5, -0.5}, 6},{{-0.5, 0.5}, 7}
	};

	virtual void Init(TObjectPtr <class AGodsArenaCharacter> owner, FCharacterCombat combatStruct) override;

	virtual void OnActionBegin_Implementation() override;
	virtual void OnActionEnd_Implementation() override;

	
private:

	void OnQueryFinished(TSharedPtr<FEnvQueryResult> result);
	int GetClosestAngleMontageInt(FVector DodgeDir);

	template <typename T>
	void acas(void(T::*func)());

	class AEnemy_Base* EnemyBase;
	FTimerHandle DodgeFinishedHandle;

	float alpha;
	float EndDodgeDuration;

	
};


