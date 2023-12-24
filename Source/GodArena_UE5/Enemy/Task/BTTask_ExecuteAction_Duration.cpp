// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ExecuteAction_Duration.h"
#include "../../GodsArenaCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "../../Components/ActionComponent.h"
#include "../../Action/Action.h"
#include "AIController.h"

UBTTask_ExecuteAction_Duration::UBTTask_ExecuteAction_Duration()
{
	NodeName = TEXT("ExecuteAction_Duration");
}

EBTNodeResult::Type UBTTask_ExecuteAction_Duration::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{//Controller
	BTComp = &owner_comp;
	auto const aiController = owner_comp.GetAIOwner();
	if (!aiController)
		return EBTNodeResult::Failed;

	//Character
	actionComp = aiController->GetPawn()->FindComponentByClass<UActionComponent>();
	if (!actionComp)
		return EBTNodeResult::Failed;

	actionComp->ExecuteAction(CombatType);
	

	FTimerDelegate temp_Delegate;

	if (Duration > 0) {
		temp_Delegate.BindWeakLambda(this,[this]()
			{
				actionComp->TerminateCurrentAction();
				EndTask();
			});
		GetWorld()->GetTimerManager().SetTimer(execute_Action_Timer_Handle,
			temp_Delegate, 0.1f, false, Duration);

	}
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_ExecuteAction_Duration::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (actionComp) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString(""));

		actionComp->TerminateCurrentAction();
	}
	return EBTNodeResult::Type();
}

void UBTTask_ExecuteAction_Duration::EndTask()
{
	if (BTComp)
		FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
}
