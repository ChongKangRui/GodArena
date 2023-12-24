// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_AICombatAttack.h"
#include "../GodsArenaCharacter.h"
#include "Animation/AnimMontage.h"
#include "../Components/ActionComponent.h"
#include "../Components/AttributeComponent.h"
#include "../Weapon/BP_WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

void USAction_AICombatAttack::OnActionBegin_Implementation()
{
	Super::OnActionBegin_Implementation();
	//Return if owner invalid or combat integer hit limit
	if (!ownerCharacter)
	{
		return;
	}

	auto const AIController = Cast<AAIController>(ownerCharacter->GetController());
	auto const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (!AIController && !player)
		return;

	int rand_AttackMontageInt = FMath::RandRange(0, combatInfo.CombatAnim.Max() - 1);
	
	if (CanMove) {
		FTimerDelegate temp_MoveDelegate;
		UAnimMontage* temp_Movingmontage = combatInfo.CombatAnim[rand_AttackMontageInt].AttackAnimation;

		float temp_AttackDistance = combatInfo.CombatAnim[rand_AttackMontageInt].AttackDistance;
		temp_MoveDelegate.BindWeakLambda(this,[this, temp_AttackDistance, player, rand_AttackMontageInt, AIController]()
			{
				float Distance = FVector::Dist(ownerCharacter->GetActorLocation(), player->GetActorLocation());
				//UE_LOG(LogTemp, Warning, TEXT("Current Distance: %f & TargetDistance: %f"), Distance, temp_AttackDistance);
				if (Distance <= temp_AttackDistance) {
					GetWorld()->GetTimerManager().ClearTimer(MoveHandle);
					PlayAttackMontage(rand_AttackMontageInt);
				}
				else {
					if (ownerCharacter->GetCharacterState() != ECharacterState::Stun)
						AIController->MoveToActor(player);
					else {
						ClearTimer(MoveHandle);
						AIController->StopMovement();
					}


				}

			});
		MoveHandle = StartTimer(temp_MoveDelegate, true);
	}
	else
		PlayAttackMontage(rand_AttackMontageInt);
}

void USAction_AICombatAttack::OnActionEnd_Implementation()
{
	Super::OnActionEnd_Implementation();
	ClearTimer(MoveHandle);
	if (!ownerCharacter)
		return;

}

void USAction_AICombatAttack::PlayAttackMontage(const int& combatInfoInt)
{
	UAnimMontage* temp_montage = combatInfo.CombatAnim[combatInfoInt].AttackAnimation;

	//Return & Reset if montage is nullptr
	if (!temp_montage)
	{
		return;
	}

	//Set weapon attribute(damage, debuff, etc)
	if (ownerCharacter->weapon)
	{
		ownerCharacter->weapon->SetAttribute(combatInfo.CombatAnim[combatInfoInt]);
	}

	//Play montage, set character & combat state
	float duration = ownerCharacter->PlayAnimMontage(temp_montage, ownerCharacter->GetCharacterBuff().AttackSpeedRate);
	ownerCharacter->SetCharacterState(combatInfo.state);

	//Call End Action
	FTimerHandle combatEndHandle;
	FTimerDelegate temp_delegate;

	StartTimer(this, &USAction_AICombatAttack::OnActionEnd_Implementation, false, duration);
}

