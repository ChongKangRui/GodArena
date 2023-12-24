// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "../../StructAndEnum.h"
#include "BTTask_ChangeState.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API UBTTask_ChangeState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_ChangeState();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBlackboardKeySelector EnumKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECharacterState StateToChange;

};
