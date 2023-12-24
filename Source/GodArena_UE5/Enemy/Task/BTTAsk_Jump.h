// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTAsk_Jump.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API UBTTAsk_Jump : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTAsk_Jump();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Height = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool RotateToTarget;

	void EndTask();
	


private:
	float OGSpeed;
	float OGJumpHeight;
	UBehaviorTreeComponent* BTComp;
	FTimerHandle handle;
};
