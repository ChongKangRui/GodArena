// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_DistanceCheck.h"
#include "../GodsArenaCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy_Base.h"

EBlackboardNotificationResult UBTDecorator_DistanceCheck::OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	return EBlackboardNotificationResult::RemoveObserver;
}

bool UBTDecorator_DistanceCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto const aiController = OwnerComp.GetAIOwner();
	if (!aiController)
		return false;

	//Character
	auto const ownerPawn = Cast<AEnemy_Base>(aiController->GetPawn());
	if (!ownerPawn)
		return false;

	float temp_Distance = 0;
	if(ownerPawn->Target)
		temp_Distance = FVector::Dist(ownerPawn->GetActorLocation(), ownerPawn->Target->GetActorLocation());
	
    
	return temp_Distance > MaxDistance;
}
