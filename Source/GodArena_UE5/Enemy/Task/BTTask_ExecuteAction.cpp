// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ExecuteAction.h"
#include "../../GodsArenaCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "../../Components/ActionComponent.h"
#include "../../Action/Action.h"
#include "AIController.h"

UBTTask_ExecuteAction::UBTTask_ExecuteAction()
{
	NodeName = TEXT("ExecuteAction");
}

EBTNodeResult::Type UBTTask_ExecuteAction::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
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

	actionComp->OnActionEnd.AddUniqueDynamic(this, &UBTTask_ExecuteAction::OnActionEndBind);
	actionComp->ExecuteAction(CombatType);
	currentAction = actionComp->GetCurrentActiveAction();


	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_ExecuteAction::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (actionComp) {
		actionComp->OnActionEnd.RemoveDynamic(this, &UBTTask_ExecuteAction::OnActionEndBind);
		actionComp->TerminateCurrentAction();
	}

	return EBTNodeResult::Type();
}

void UBTTask_ExecuteAction::OnActionEndBind(AGodsArenaCharacter* OwnerCharacter, USAction* CurrentAction)
{
	UE_LOG(LogTemp, Display, TEXT("Trigger"));
	if (currentAction == CurrentAction) {
		actionComp->OnActionEnd.RemoveDynamic(this, &UBTTask_ExecuteAction::OnActionEndBind);
		EndTask();
	}
}

void UBTTask_ExecuteAction::EndTask()
{
	UE_LOG(LogTemp, Display, TEXT("EndTask"));
	if(BTComp)
		FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
}




