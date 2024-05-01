// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "../../StructAndEnum.h"
#include "BTTask_ExecuteAction.generated.h"

UCLASS()
class GODARENA_UE5_API UBTTask_ExecuteAction : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_ExecuteAction();

	//Alway make sure it is a ufunction in order to bind dynamic delegate
	UFUNCTION()
	void OnActionEndBind(class AGodsArenaCharacter* OwnerCharacter, class USAction* CurrentAction);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void EndTask();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECombatType CombatType;

private:
	TObjectPtr<UBehaviorTreeComponent> BTComp;
	 TObjectPtr<class UActionComponent> actionComp;
	 TObjectPtr<class USAction> currentAction;
	
};
