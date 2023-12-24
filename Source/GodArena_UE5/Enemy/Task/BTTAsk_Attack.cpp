// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTAsk_Attack.h"
#include "../../GodsArenaCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "../../Components/ActionComponent.h"
#include "AIController.h"

UBTTAsk_Attack::UBTTAsk_Attack()
{
	NodeName = TEXT("ExecuteNormalAttackAction");
}

EBTNodeResult::Type UBTTAsk_Attack::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	//Controller
	auto const aiController = owner_comp.GetAIOwner();
	if(!aiController)
		return EBTNodeResult::Failed;

	//Character
	auto const ownerPawn = Cast<AGodsArenaCharacter>(aiController->GetPawn());
	if (!ownerPawn)
		return EBTNodeResult::Failed;


	ownerPawn->actionComp->ExecuteAction(ECombatType::NormalAttack);

	IsMontagePlaying = true;

	FTimerDelegate temp_delegate;
	FTimerHandle temp_Handle;
	//Bind event by lamdba expression
	
	temp_delegate.BindWeakLambda(this,[this,&owner_comp,&ownerPawn]()
		{
			IsMontagePlaying = ownerPawn->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();
			if (!IsMontagePlaying)
			{
				//TimerChecking(owner_comp);
				//FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
				FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
				
			}
		});
	GetWorld()->GetTimerManager().SetTimer(temp_Handle,
		temp_delegate, 0.1f, true, 0);




	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTAsk_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Controller
	auto const aiController = OwnerComp.GetAIOwner();
	if (!aiController)
		return EBTNodeResult::Failed;

	//Character
	auto const ownerPawn = Cast<AGodsArenaCharacter>(aiController->GetPawn());
	if (!ownerPawn)
		return EBTNodeResult::Failed;

	ownerPawn->actionComp->TerminateAction(ECombatType::NormalAttack);



	return EBTNodeResult::Type();
}


