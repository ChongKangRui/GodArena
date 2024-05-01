// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController_Base.h"
#include "AIController_ExpertKnight.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API AAIController_ExpertKnight : public AAIController_Base
{
	GENERATED_BODY()
public:
	void BlackBoardValueTick() override;
};
