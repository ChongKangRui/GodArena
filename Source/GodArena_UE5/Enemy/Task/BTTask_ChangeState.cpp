// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChangeState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../GodsArenaCharacter.h"
#include "AIController.h"

UBTTask_ChangeState::UBTTask_ChangeState()
{
	NodeName = TEXT("ChangeState");
}

EBTNodeResult::Type UBTTask_ChangeState::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	auto const aiController = owner_comp.GetAIOwner();
		auto const enemy = Cast<AGodsArenaCharacter>(aiController->GetPawn());

	enemy->SetCharacterState(StateToChange);

	return EBTNodeResult::Succeeded;
}
