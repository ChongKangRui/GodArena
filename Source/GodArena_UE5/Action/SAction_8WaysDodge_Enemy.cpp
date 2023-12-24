// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_8WaysDodge_Enemy.h"
#include "../GodsArenaCharacter.h"
#include "../Enemy/Enemy_Base.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void USAction_8WaysDodge_Enemy::Init(TObjectPtr<class AGodsArenaCharacter> owner, FCharacterCombat combatStruct)
{
	Super::Init(owner, combatStruct);

	EnemyBase = Cast<AEnemy_Base>(owner);
}

void USAction_8WaysDodge_Enemy::OnActionBegin_Implementation()
{
	Super::OnActionBegin_Implementation();

	EnemyBase->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	//ownerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

	FEnvQueryRequest DodgeQueryRequest = FEnvQueryRequest(EnemyBase->DodgeEQS, EnemyBase);

	DodgeQueryRequest.Execute(
		EEnvQueryRunMode::RandomBest25Pct,
		this,
		&USAction_8WaysDodge_Enemy::OnQueryFinished);

}

void USAction_8WaysDodge_Enemy::OnActionEnd_Implementation()
{
	Super::OnActionEnd_Implementation();
}

void USAction_8WaysDodge_Enemy::OnQueryFinished(TSharedPtr<FEnvQueryResult> result)
{
	if (result->IsSuccessful()) {
		
		int temp_rand = FMath::RandRange(0, 6);
		int montageInt = GetClosestAngleMontageInt(result->GetItemAsLocation(temp_rand));
		UAnimMontage* temp_Montage = combatInfo.CombatAnim[montageInt].AttackAnimation;
		if (temp_Montage) {
			float duration = ownerCharacter->PlayAnimMontage(temp_Montage, dodgeAnimRate);
			StartTimer(this, &USAction_8WaysDodge_Enemy::OnActionEnd_Implementation,false, duration);


		}
		else
			OnActionEnd_Implementation();



	}
}

int USAction_8WaysDodge_Enemy::GetClosestAngleMontageInt(FVector DodgeDir)
{
	FVector temp_direction = DodgeDir - ownerCharacter->GetActorLocation();
	temp_direction.Normalize();

	FVector temp_forwardVec = ownerCharacter->GetActorForwardVector();
	FVector temp_rightVec = ownerCharacter->GetActorRightVector();

	float temp_X = FVector::DotProduct(temp_direction, temp_forwardVec);
	float temp_Y = FVector::DotProduct(temp_direction, temp_rightVec);

	FVector2D temp_Results = FVector2D(temp_X, temp_Y);
	float MinDeviation = 10;
	int OutBestInt = 0;


	for (const auto& temp_map : DirectionMontageMap) {
		
		FVector2D temp_ScoreVector = temp_Results - temp_map.Key;
		//ownerCharacter->DebugPrint("????ASDDDDDDDDDDDDDDDD");

		float temp_Deviation = FMath::Abs(temp_ScoreVector.X + temp_ScoreVector.Y);
		if (temp_Deviation < MinDeviation) {
			MinDeviation = temp_Deviation;
			OutBestInt = temp_map.Value;
		}
	}


	
	
	return OutBestInt;
}




