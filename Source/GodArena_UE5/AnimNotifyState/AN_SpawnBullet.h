// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_SpawnBullet.generated.h"

/**
 * 
 */

class ABuff_Base;

UCLASS()
class GODARENA_UE5_API UAN_SpawnBullet : public UAnimNotify
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABuff_Base> BulletBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnDistance = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform TransformOffset = {FRotator(0.1f),FVector(0.0f) ,FVector(0.0f) };

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:

};
