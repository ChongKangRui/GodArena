// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_8WaysDodge_Player.h"
#include "../GodsArenaCharacter.h"
#include "../Player/PlayerCharacter.h"
#include "../Components/AttributeComponent.h"
#include "../Components/TargetingComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"


void USAction_8WaysDodge_Player::Init(TObjectPtr<class AGodsArenaCharacter> owner, FCharacterCombat combatStruct)
{
	Super::Init(owner, combatStruct);
	player = Cast<APlayerCharacter>(owner);
}

void USAction_8WaysDodge_Player::OnActionBegin_Implementation()
{
	Super::OnActionBegin_Implementation();
	int MontageInt = 0;
	if(player->TargetComponent->CurrentTarget)
		MontageInt = GetClosestAngleMontageInt(player->GetPlayerLastInputMovementValue());
	
	UAnimMontage* temp_Montage = combatInfo.CombatAnim[MontageInt].AttackAnimation;
	player->SetCharacterState(combatInfo.state);

	player->attributeComp->ApplyStaminaCost(combatInfo.CombatAnim[MontageInt].Cost);

	if (temp_Montage) {
		float Duration = ownerCharacter->PlayAnimMontage(temp_Montage, dodgeAnimRate);
		StartTimer(this, &USAction_8WaysDodge_Player::OnActionEnd_Implementation, false, Duration);
	}
	else
		OnActionEnd_Implementation();

}

void USAction_8WaysDodge_Player::OnActionEnd_Implementation()
{
	Super::OnActionEnd_Implementation();
	ownerCharacter->SetCharacterState(ECharacterState::Idle);
}

int USAction_8WaysDodge_Player::GetClosestAngleMontageInt(FVector2D inputDir)
{
	ownerCharacter->DebugPrint(inputDir.ToString());

	int OutBestInt = 1;

	for (const auto& temp_map : DirectionMontageMap) {
		if (inputDir == temp_map.Key) {
			OutBestInt = temp_map.Value;
			break;
		}
	}
	return OutBestInt;
}


