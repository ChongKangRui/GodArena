// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_AIDodge.h"
#include "../GodsArenaCharacter.h"
#include "../Enemy/Enemy_Base.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"


void USAction_AIDodge::Init(TObjectPtr <AGodsArenaCharacter> owner, FCharacterCombat combatStruct)
{
	Super::Init(owner, combatStruct);

	EnemyBase = Cast<AEnemy_Base>(owner);
}

void USAction_AIDodge::OnActionBegin_Implementation()
{
	Super::OnActionBegin_Implementation();
	EnemyBase->GetCharacterMovement()->bUseControllerDesiredRotation = false;

	FEnvQueryRequest HidingSpotQueryRequest = FEnvQueryRequest(EnemyBase->DodgeEQS, EnemyBase);
	HidingSpotQueryRequest.Execute(
		EEnvQueryRunMode::SingleResult,
		this,
		&USAction_AIDodge::OnQueryFinished);



}

void USAction_AIDodge::OnActionEnd_Implementation()
{
	Super::OnActionEnd_Implementation();
	ClearTimer(DodgeRotationHandle);
	EnemyBase->GetCharacterMovement()->bUseControllerDesiredRotation = true;

}

void USAction_AIDodge::OnQueryFinished(TSharedPtr<FEnvQueryResult> result)
{
	if (result->IsSuccessful()) {
		FVector dir = result->GetItemAsLocation(0) - EnemyBase->GetActorLocation();
		dir.Normalize();
		FRotator TargetRotation = FRotator(0, FMath::Atan2(dir.Y, dir.X), 0);

		EndDodgeDuration = PlayDodgeMontage();
		FTimerDelegate DodgeRotationDelegate;

		//Start dodge rotation timer
		DodgeRotationDelegate.BindWeakLambda(this, [this, TargetRotation]()
			{
				RotationBeforeDodge(TargetRotation);
			});

		if (!IsTimerActive(DodgeRotationHandle))
		{
			DodgeRotationHandle = StartTimer(
				 DodgeRotationDelegate,  0.0, true);
		}

		DodgeFinishedHandle = StartTimer(this, &USAction_AIDodge::OnActionEnd_Implementation, false, EndDodgeDuration);

	}
}

void USAction_AIDodge::RotationBeforeDodge(const FRotator& rotateDir)
{
	FRotator TargetRotation = FRotator(rotateDir.Pitch, rotateDir.Yaw, rotateDir.Roll);
	FRotator FinalRotation = FMath::RInterpConstantTo(ownerCharacter->GetActorRotation(), rotateDir,
		GetWorld()->GetDeltaSeconds(), DodgeInterpToDirectionSpeed);
	ownerCharacter->SetActorRotation(FinalRotation);

}

float USAction_AIDodge::PlayDodgeMontage()
{
	UAnimMontage* temp_Montage = combatInfo.CombatAnim[0].AttackAnimation;

	if (!temp_Montage)
		return 0;
	float temp_duration = temp_Montage->GetPlayLength() / dodgeAnimRate;

	FTimerDelegate DodgeEndDelegate;
	DodgeEndDelegate.BindWeakLambda(this, [this, temp_Montage]()
		{
			ownerCharacter->PlayAnimMontage(temp_Montage, dodgeAnimRate);
		});
	StartTimer( DodgeEndDelegate,false, DelayBeforeDodge);

	return temp_duration;
}
