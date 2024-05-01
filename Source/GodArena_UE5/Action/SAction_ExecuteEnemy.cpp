// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ExecuteEnemy.h"
#include "../GodsArenaCharacter.h"
#include "../Player/PlayerCharacter.h"
#include "../Enemy/Enemy_Base.h"
#include "../Components/TargetingComponent.h"
#include "../GodArenaFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void USAction_ExecuteEnemy::OnActionBegin_Implementation()
{
	Super::OnActionBegin_Implementation();

	APlayerCharacter* temp_player = nullptr;
	AEnemy_Base* temp_EnemyOwner = nullptr;
	//For Player
	temp_player = Cast<APlayerCharacter>(ownerCharacter);
	if (temp_player)
	{
		if (temp_player->TargetComponent->CurrentTarget)
		{
			ExecuteTarget(temp_player->TargetComponent->CurrentTarget);
			return;
		}
	}


	//For enemy
	temp_EnemyOwner = Cast<AEnemy_Base>(ownerCharacter);
	if (temp_EnemyOwner)
	{
		ownerCharacter->DebugPrint("EnemyExecute");
		if(temp_EnemyOwner->Target)
			ExecuteTarget(temp_EnemyOwner->Target);
	}
	else {
		OnActionEnd_Implementation();
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString("NoTarget"));
	}


}

void USAction_ExecuteEnemy::OnActionEnd_Implementation()
{
	Super::OnActionEnd_Implementation();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString("Done Execute"));

	ownerCharacter->SetCharacterState(ECharacterState::Idle);
}

void USAction_ExecuteEnemy::ExecuteTarget(AGodsArenaCharacter* Target)
{
	if (Target)
	{
		if (Target->GetCharacterState() != ECharacterState::Stun) {
			OnActionEnd_Implementation();
			return;
		}

		Target->SetCharacterState(ECharacterState::Executed);
		Target->StopAnimMontage(Target->GetCurrentMontage());

		FVector temp_ExecuteVector = Target->GetActorLocation() + (Target->GetActorForwardVector() * ExecuteDistance);
		ownerCharacter->SetActorLocation(temp_ExecuteVector);
		FVector temp_Direction = Target->GetActorLocation() - ownerCharacter->GetActorLocation();
		temp_Direction.Normalize();

		float YawRadian = FMath::Atan2(temp_Direction.Y, temp_Direction.X);
		float YawAngle = FMath::RadiansToDegrees(YawRadian);

		ownerCharacter->SetActorRotation(FRotator(0, YawAngle, 0));
		ownerCharacter->SetCharacterState(combatInfo.state);
		//Play Anim montage
		UAnimMontage* temp_montage = combatInfo.CombatAnim[0].AttackAnimation;
		float temp_EndDuration = 0;
		if (temp_montage)
		{
			temp_EndDuration = ownerCharacter->PlayAnimMontage(temp_montage) - EndActionOffset;
		}
		StartTimer(this, &USAction_ExecuteEnemy::OnActionEnd_Implementation, false, temp_EndDuration);

		//Apply Damage
		FTimerDelegate temp_delegate_ApplyDamage;
		float temp_DamageToApply = FMath::RandRange(combatInfo.CombatAnim[0].MinDamage, combatInfo.CombatAnim[0].MaxDamage);
		
		temp_delegate_ApplyDamage.BindWeakLambda(this, [this, Target, temp_DamageToApply]()
			{
				if (!Target)
					ownerCharacter->DebugPrint("No Target");

				UGodArenaFunctionLibrary::ApplyDamage(ownerCharacter, Target,
					temp_DamageToApply, combatInfo.CombatAnim[0].debuff);
			});
		
		StartTimer(temp_delegate_ApplyDamage, false, ApplyDamageDelay);

		//Apply enemy execute state
		EnemyExecutedState(EnemyDamageReactionState_Timer, Target, EExecutedState::ExecuteReaction);
		EnemyExecutedState(EnemyExecutedState_Timer, Target, EExecutedState::Executed);
		EnemyExecutedState(EnemyStandingUpState_Timer, Target, EExecutedState::StandingUp);
		EnemyExecutedState(EnemyIdleState_Timer, Target, EExecutedState::NoExecuted);

	}
}

void USAction_ExecuteEnemy::EnemyExecutedState(float Timer_Duration, AGodsArenaCharacter* TargetToSetState, EExecutedState execState)
{
	FTimerHandle temp_ApplyDamage_Handle;
	FTimerDelegate temp_delegate_ApplyDamage;
	
	temp_delegate_ApplyDamage.BindWeakLambda(this, [this, TargetToSetState, execState]()
		{
			if (!TargetToSetState)
				ownerCharacter->DebugPrint("No Target");

			ownerCharacter->DebugPrint("Executing");
			TargetToSetState->SetExecutedState(execState);
		});
	StartTimer(temp_delegate_ApplyDamage, false, Timer_Duration);
}
