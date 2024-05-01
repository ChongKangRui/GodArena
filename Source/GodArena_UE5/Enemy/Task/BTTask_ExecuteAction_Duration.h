// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "../../StructAndEnum.h"
#include "BTTask_ExecuteAction_Duration.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API UBTTask_ExecuteAction_Duration : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_ExecuteAction_Duration();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	void EndTask();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECombatType CombatType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "0 mean loop, more than 0 will set a duration"))
	float Duration = 1.0f;

private:
	UBehaviorTreeComponent* BTComp;
	class UActionComponent* actionComp;
	class USAction* currentAction;

	FTimerHandle execute_Action_Timer_Handle;
};
