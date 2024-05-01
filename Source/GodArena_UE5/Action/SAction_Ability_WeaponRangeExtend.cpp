// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_Ability_WeaponRangeExtend.h"
#include "../GodsArenaCharacter.h"
#include "../Weapon/BP_WeaponBase.h"
#include "../Components/ActionComponent.h"
#include "../Action/SAction_KnightAbility02.h"
#include "../Components/AttributeComponent.h"


void USAction_Ability_WeaponRangeExtend::OnActionBegin_Implementation()
{
	ownerCharacter->attributeComp->HitReactionAbandonState.Add(ECharacterState::Attacking);

	Super::OnActionBegin_Implementation();

	USAction_Ability_WeaponRangeExtend* temp_Action = Cast<USAction_Ability_WeaponRangeExtend>(ownerCharacter->actionComp->GetAction(DisableBuffCombatType));
	if (temp_Action && IsTimerActive(temp_Action->BuffTimerHandle)) {
		ClearTimer(temp_Action->BuffTimerHandle);
	}

	if (ownerCharacter->weapon) {
		ownerCharacter->weapon->ExtendTraceLength(ExtendedWeaponEndPosition);
		OriginalWeaponEndTracePos = ownerCharacter->weapon->GetInitialEndPointLocation();
		ownerCharacter->weapon->TrailActivated = true;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString("Warning: Invalid Weapon"));
	}

	//If not extend weapon attack range forever, then stop the timer
	if (!ForeverExtendWeapon) {
		ClearTimer(BuffTimerHandle);
		FTimerDelegate temp_Delegate;
		temp_Delegate.BindWeakLambda(this, [this]()
			{
				if (ownerCharacter->weapon) {

					ownerCharacter->weapon->EndWeaponTracePoint->SetRelativeLocation(OriginalWeaponEndTracePos);
					ownerCharacter->weapon->TrailActivated = false;
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString("Disable Extended weapon"));
				}
			});

		float temp_MontageDuration = combatInfo.CombatAnim[0].AttackAnimation->GetPlayLength();
		BuffTimerHandle = StartTimer(temp_Delegate, false, BuffEndDuration + temp_MontageDuration);
	}
}

void USAction_Ability_WeaponRangeExtend::OnActionEnd_Implementation()
{
	Super::OnActionEnd_Implementation();
	ownerCharacter->attributeComp->HitReactionAbandonState.Remove(ECharacterState::Attacking);
	
}

void USAction_Ability_WeaponRangeExtend::CombatResetTimer()
{
	if (!ownerCharacter)
		return;

	ECharacterState ownerAction = ownerCharacter->GetCharacterState();
	bool anyMontagePlaying = ownerCharacter->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();

	if ((!anyMontagePlaying && ownerAction == ECharacterState::SkillExecuting || ownerAction != ECharacterState::SkillExecuting)) {
		OnActionEnd_Implementation();
	}
	
}
