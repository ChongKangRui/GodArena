// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_Dodge.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "../Components/AttributeComponent.h"
#include "../Components/ActionComponent.h"
#include "../Player/PlayerCharacter.h"
#include "../GodsArenaCharacter.h"
#include "../Player/PlayerCharacter.h"
#include "../Action/SAction_CombatAttack.h"

void USAction_Dodge::Init(TObjectPtr <AGodsArenaCharacter> owner, FCharacterCombat combatStruct)
{
	Super::Init(owner, combatStruct);
	playerChar = Cast<APlayerCharacter>(owner);
}

float USAction_Dodge::PlayDodgeMontage()
{
	UAnimMontage* temp_Montage = combatInfo.CombatAnim[0].AttackAnimation;
	if (!temp_Montage)
		return 0;
	float temp_duration = (temp_Montage->GetPlayLength() / dodgeAnimRate) * (1 / dodgeAnimRate) - DodgeFinishDurationOffset;


	FTimerDelegate DodgeEndDelegate;
	DodgeEndDelegate.BindWeakLambda(this, [this, temp_Montage]()
		{
			ownerCharacter->PlayAnimMontage(temp_Montage, dodgeAnimRate);
		});
	StartTimer(DodgeEndDelegate, false, DelayBeforeDodge);

	return temp_duration;
}

void USAction_Dodge::OnActionBegin_Implementation()
{
	//Making sure that attacking can dodge but only no combo
	USAction* temp_currentAction = ownerCharacter->actionComp->GetCurrentActiveAction();
	if (temp_currentAction) {
		USAction_CombatAttack* temp_combatAtk = Cast<USAction_CombatAttack>(temp_currentAction);
		if (temp_combatAtk && (temp_combatAtk->GetCombatState() == ECombatState::NextCombo ||
			temp_combatAtk->GetCombatState() == ECombatState::StartCombo))
			return;
	}

	bool canDodge = ownerCharacter->attributeComp->ApplyStaminaCost(combatInfo.CombatAnim[0].Cost);
	if (!canDodge)
	{
		return;
	}
	Super::OnActionBegin_Implementation();

	ownerCharacter->actionComp->TerminateAction(ECombatType::NormalAttack);
	ownerCharacter->SetCharacterState(ECharacterState::Dodging);
	ownerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;


	FVector DodgeRotationDir = Cast<APlayerCharacter>(ownerCharacter)->GetPlayerLastInputMovementVector();

	FRotator TargetRotation = UKismetMathLibrary::MakeRotFromX(DodgeRotationDir);
	EndDodgeDuration = PlayDodgeMontage();

	if (!IsTimerActive(DodgeRotationHandle))
	{
		FTimerDelegate DodgeRotationDelegate;
		//Start dodge rotation timer
		DodgeRotationDelegate.BindWeakLambda(this, [this, TargetRotation]()
			{
				RotationBeforeDodge(TargetRotation);
			});

		DodgeRotationHandle = StartTimer(DodgeRotationDelegate, true);
	}

	//Start Reset State Timer
	StartTimer(this, &USAction_Dodge::OnActionEnd_Implementation, false, EndDodgeDuration);

}

void USAction_Dodge::OnActionEnd_Implementation()
{
	Super::OnActionEnd_Implementation();
	if (DodgeRotationHandle.IsValid()) {
		if (IsTimerActive(DodgeRotationHandle))
		{
			ClearTimer(DodgeRotationHandle);

		}
	}
}

void USAction_Dodge::RotationBeforeDodge(const FRotator& rotateDir)
{
	//FRotator MeshRotation = ownerCharacter->GetMesh()->GetComponentRotation();
	ownerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	FRotator TargetRotation = FRotator(rotateDir.Pitch, rotateDir.Yaw, rotateDir.Roll);

	FRotator FinalRotation = FMath::RInterpConstantTo(ownerCharacter->GetActorRotation(), rotateDir,
		GetWorld()->GetDeltaSeconds(), DodgeInterpToDirectionSpeed);

	ownerCharacter->SetActorRotation(FinalRotation);

}

