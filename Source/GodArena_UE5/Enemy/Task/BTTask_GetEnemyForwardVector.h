// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GetEnemyForwardVector.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API UBTTask_GetEnemyForwardVector : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_GetEnemyForwardVector();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ForwardDistance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector Out_TargetForwardVector;
		
	
};
