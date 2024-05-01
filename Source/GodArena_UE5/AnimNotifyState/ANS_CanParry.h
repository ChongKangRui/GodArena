// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "../StructAndEnum.h"
#include "ANS_CanParry.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API UANS_CanParry : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual bool ShouldFireInEditor() {
		return false;
	}
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EParryingState NotifyBeginState = EParryingState::CanParry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EParryingState NotifyEndState;
private:
	class AGodsArenaCharacter* ownerCharacter;
};
