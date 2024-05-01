// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTAsk_Attack.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API UBTTAsk_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTAsk_Attack();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; 

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* testMontage;

private:
	bool IsMontagePlaying;
	
};
