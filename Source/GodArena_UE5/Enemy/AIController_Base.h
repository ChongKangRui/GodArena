// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIController_Base.generated.h"


UCLASS()
class GODARENA_UE5_API AAIController_Base : public AAIController
{
	GENERATED_BODY()

public:
	AAIController_Base();

	UFUNCTION(BlueprintPure)
	ECharacterState GetRandomBehavior(const TArray<ECharacterState>& StateList);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	 TObjectPtr < class UBehaviorTreeComponent> behaviorTreeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	 TObjectPtr <class UBlackboardComponent> blackboardComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TArray<ECharacterState> DefaultInitRandomCharacteterState;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TArray<ECharacterState> UseControllerRotationState = {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SkillExecuteChance = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AEnemy_Base> ownerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationInterpSpeed = 20.0;

protected:
	void Tick(float deltaTime) override;
	void OnPossess(APawn* pawn) override;

	virtual void BlackBoardValueInit();
	virtual void BlackBoardValueTick();
	virtual void SetRotationState();
	virtual void RotateToTarget();


private:
	FTimerHandle BlackboardInitialization;
};
