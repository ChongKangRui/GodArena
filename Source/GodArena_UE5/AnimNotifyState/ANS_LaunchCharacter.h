// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "../StructAndEnum.h"
#include "ANS_LaunchCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API UANS_LaunchCharacter : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float launchForce = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "LaunchDirection ==  ELaunchDirection::EnemyDirection", EditConditionHides, ToolTip = "Only use for Enemy Direction"))
	float DistanceToEnemy = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELaunchDirection LaunchDirection = ELaunchDirection::Forward;

protected:
	virtual bool ShouldFireInEditor() {
		return false;
	}
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

	FVector GetEnemyDirection(AActor* owner);
	FVector GetToEnemyVelocity(AActor* owner, AActor* Target);

protected:
	FVector TargetDirection;
};
