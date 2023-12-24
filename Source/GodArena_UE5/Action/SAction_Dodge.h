// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "Components/TimelineComponent.h"
#include "SAction_Dodge.generated.h"

/**
 * 
 */

class UCurveFloat;
UCLASS()
class GODARENA_UE5_API USAction_Dodge : public USAction
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(ToolTip = "Whether wan to increase the dodge distance or not"))
		float dodgeAnimRate = 1.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Whether wan to increase the dodge distance or not"))
		float DodgeFinishDurationOffset = 0.2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Rotation Speed"))
		float DodgeInterpToDirectionSpeed = 10.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Delay to play dodge montage"))
		float DelayBeforeDodge = 0.5f;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Whether wan to increase the dodge distance or not"))
		//float 

	void Init(TObjectPtr <class AGodsArenaCharacter> owner, FCharacterCombat combatStruct) override;

	//UFUNCTION(BlueprintCallable)
	//	void Dodge(FVector dodgeDirection);

	void OnActionBegin_Implementation() override;
	void OnActionEnd_Implementation() override;
protected:
	void RotationBeforeDodge(const FRotator& rotateDir);
	float PlayDodgeMontage();

	FTimerHandle DodgeRotationHandle;
	FTimerHandle DodgeFinishedHandle;
	float alpha;
	float EndDodgeDuration;

	class APlayerCharacter* playerChar;



};
