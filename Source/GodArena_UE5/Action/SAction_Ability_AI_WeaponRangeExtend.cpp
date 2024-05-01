// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_Ability_AI_WeaponRangeExtend.h"
#include "../GodsArenaCharacter.h"
#include "../Weapon/BP_WeaponBase.h"
#include "../Components/ActionComponent.h"
#include "../Action/SAction_KnightAbility02.h"
#include "../Components/AttributeComponent.h"

void USAction_Ability_AI_WeaponRangeExtend::OnActionBegin_Implementation()
{
	ownerCharacter->attributeComp->HitReactionAbandonState.Add(ECharacterState::Attacking);

	ABP_WeaponBase* weapon = ownerCharacter->weapon;

	if (weapon) {
		weapon->ExtendTraceLength(ExtendedWeaponEndPosition);
		OriginalWeaponEndTracePos = ownerCharacter->weapon->GetInitialEndPointLocation();
		weapon->TrailActivated = true;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString("Set Trace"));

	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString("Warning: no weapon reference"));
		return;
	}

	ClearTimer(BuffTimerHandle);
	float temp_MontageDuration = combatInfo.CombatAnim[0].AttackAnimation->GetPlayLength();
	BuffTimerHandle = StartTimer(this, &USAction_Ability_AI_WeaponRangeExtend::OnActionEnd_Implementation, 
		false, BuffEndDuration + temp_MontageDuration);

	Super::OnActionBegin_Implementation();

}

void USAction_Ability_AI_WeaponRangeExtend::OnActionEnd_Implementation()
{
	Super::OnActionEnd_Implementation();

	if (ownerCharacter && ownerCharacter->weapon) {

		if (!ForeverExtendWeapon) {
			ownerCharacter->attributeComp->HitReactionAbandonState.Remove(ECharacterState::Attacking);
			ownerCharacter->weapon->TrailActivated = false;

			ownerCharacter->weapon->EndWeaponTracePoint->SetRelativeLocation(OriginalWeaponEndTracePos);
		}
	}

}


