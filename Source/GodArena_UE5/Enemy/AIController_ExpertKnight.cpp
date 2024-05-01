// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_ExpertKnight.h"
#include "../Enemy/Enemy_Base.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AAIController_ExpertKnight::BlackBoardValueTick()
{
	Super::BlackBoardValueTick();
	if (!ownerCharacter || !ownerCharacter->Target)
	{
		return;
	}

	FVector temp_Dir1 = ownerCharacter->Target->GetActorLocation() - ownerCharacter->GetActorLocation();
	FVector temp_Dir2 = ownerCharacter->GetActorForwardVector();

	temp_Dir1.Normalize();
	temp_Dir2.Normalize();

	//Get angle
	float temp_Radian = FMath::Acos(FVector::DotProduct(temp_Dir2, temp_Dir1));
	float temp_Angle = FMath::RadiansToDegrees(temp_Radian);
	blackboardComp->SetValueAsFloat("AngleToEnemy", temp_Angle);

	uint8 temp_CanParry = static_cast<uint8>(ownerCharacter->Target->GetParryingState());
	blackboardComp->SetValueAsEnum("EnemyParryState", temp_CanParry);
}
