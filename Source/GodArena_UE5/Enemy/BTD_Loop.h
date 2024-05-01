// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Loop.h"
#include "BTD_Loop.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API UBTD_Loop : public UBTDecorator_Loop
{
	GENERATED_BODY()
public:

	UBTD_Loop();
	void OnNodeActivation(FBehaviorTreeSearchData& SearchD) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MinLoop = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxLoop = 0;

	
};
