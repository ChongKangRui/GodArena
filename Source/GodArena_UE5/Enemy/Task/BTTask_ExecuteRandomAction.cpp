// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ExecuteRandomAction.h"
#include "../../GodsArenaCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "../../Components/ActionComponent.h"
#include "../../Action/Action.h"
#include "AIController.h"


UBTTask_ExecuteRandomAction::UBTTask_ExecuteRandomAction()
{
	NodeName = TEXT("ExecuteRandomAction");

}

EBTNodeResult::Type UBTTask_ExecuteRandomAction::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	//Controller
	BTComp = &owner_comp;
	auto const aiController = owner_comp.GetAIOwner();
	if (!aiController)
		return EBTNodeResult::Failed;

	//Character
	actionComp = aiController->GetPawn()->FindComponentByClass<UActionComponent>();
	if (!actionComp)
		return EBTNodeResult::Failed;

	FRandomStream RandomStream;
	RandomStream.GenerateNewSeed();

	int32 temp_RandomIndex = RandomStream.RandRange(0, CombatType.Num() - 1);

	if (CombatType.IsValidIndex(temp_RandomIndex)) {
		actionComp->ExecuteAction(CombatType[temp_RandomIndex]);
		actionComp->OnActionEnd.AddUniqueDynamic(this, &UBTTask_ExecuteRandomAction::OnActionEndBind);
		currentAction = actionComp->GetCurrentActiveAction();
	}


	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_ExecuteRandomAction::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (actionComp) {
		actionComp->OnActionEnd.RemoveDynamic(this, &UBTTask_ExecuteRandomAction::OnActionEndBind);
		actionComp->TerminateCurrentAction();
	}

	return EBTNodeResult::Type();
}

void UBTTask_ExecuteRandomAction::OnActionEndBind(AGodsArenaCharacter* OwnerCharacter, USAction* CurrentAction)
{
	if (currentAction == CurrentAction) {
		actionComp->OnActionEnd.RemoveDynamic(this, &UBTTask_ExecuteRandomAction::OnActionEndBind);
		EndTask();
	}
}

void UBTTask_ExecuteRandomAction::EndTask()
{
	if (BTComp)
		FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
}


