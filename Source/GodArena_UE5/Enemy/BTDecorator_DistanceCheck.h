// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "BTDecorator_DistanceCheck.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API UBTDecorator_DistanceCheck : public UBTDecorator_Blackboard
{
	GENERATED_BODY()

public:
	EBlackboardNotificationResult OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID) override;
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxDistance;

	
};
