// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_Parry.h"
#include "../GodsArenaCharacter.h"
#include "Animation/AnimMontage.h"
#include "../Components/AttributeComponent.h"
#include "../Player/PlayerCharacter.h"
#include "TimerManager.h"

void USAction_Parry::Init(TObjectPtr<AGodsArenaCharacter> owner, FCharacterCombat combatStruct)
{
	Super::Init(owner, combatStruct);
	APlayerCharacter* player = Cast<APlayerCharacter>(owner);

	if (player)
		IsPlayer = true;
	else
		IsPlayer = false;

}

void USAction_Parry::OnActionBegin_Implementation()
{
	Super::OnActionBegin_Implementation();
	if (!ownerCharacter) {
		OnActionEnd_Implementation();
		return;
	}

	UAnimMontage* temp_montage = combatInfo.CombatAnim[0].AttackAnimation;

	if (!temp_montage) {
		OnActionEnd_Implementation();
		return;
	}

	if (IsPlayer) {
		bool staminaCost = ownerCharacter->attributeComp->ApplyStaminaCost(combatInfo.CombatAnim[0].Cost);
	}
	
	ownerCharacter->SetCharacterState(combatInfo.state);
	//ownerCharacter->DebugPrint("Parry?>>>>")
	float duration = ownerCharacter->PlayAnimMontage(temp_montage);
	duration -= ParryDurationOffset;


	ClearTimer(ParryTimerHandle);
	if (!IsTimerActive(ParryTimerHandle))
	{
		
		StartTimer(this, &USAction_Parry::OnActionEnd_Implementation, false, duration);
	}
}

void USAction_Parry::OnActionEnd_Implementation()
{
	Super::OnActionEnd_Implementation();

}
