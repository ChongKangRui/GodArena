// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_RotationToTarget.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API UANS_RotationToTarget : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual bool ShouldFireInEditor() {
		return false;
	}
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationSpeed = 20.0f;

	class AGodsArenaCharacter* Target;
	class AGodsArenaCharacter* owner;
};
