// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetEnemyForwardVector.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../GodsArenaCharacter.h"
#include "../Enemy_Base.h"
#include "AIController.h"

UBTTask_GetEnemyForwardVector::UBTTask_GetEnemyForwardVector() {


	NodeName = TEXT("Get Enemy Forward Vector");
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetEnemyForwardVector, BlackboardKey), AGodsArenaCharacter::StaticClass());
	Out_TargetForwardVector.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetEnemyForwardVector, Out_TargetForwardVector));
}

EBTNodeResult::Type UBTTask_GetEnemyForwardVector::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	auto const BlackboardComp = owner_comp.GetBlackboardComponent();

	if (!BlackboardComp)
		return EBTNodeResult::Failed;


	 auto const Target = Cast<AGodsArenaCharacter>(BlackboardComp->GetValueAsObject(BlackboardKey.SelectedKeyName));

	 if(!Target)
		 return EBTNodeResult::Failed;


	 FVector temp_Vector = Target->GetActorLocation() + (Target->GetActorForwardVector() * ForwardDistance);

	 BlackboardComp->SetValueAsVector(Out_TargetForwardVector.SelectedKeyName, temp_Vector);


	return EBTNodeResult::Succeeded;
}
