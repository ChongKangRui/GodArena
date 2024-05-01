// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_Defend.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Components/AttributeComponent.h"
#include "../GodsArenaCharacter.h"



void USAction_Defends::Init(TObjectPtr<AGodsArenaCharacter> owner, FCharacterCombat combatStruct)
{
	Super::Init(owner, combatStruct);
	owner->attributeComp->OnStaminaDamaged.AddDynamic(this, &USAction_Defends::HitReactionBind);
	OGSpeed = owner->GetCharacterMovement()->MaxWalkSpeed;
}

void USAction_Defends::OnActionBegin_Implementation()
{
	Super::OnActionBegin_Implementation();
	FTimerDelegate temp_delegate;
	Blocking = true;
	if (!GetWorld()->GetTimerManager().IsTimerActive(Parry_Handle)) {
		
		ownerCharacter->SetCharacterState(combatInfo.state);
		temp_delegate.BindWeakLambda(this, [this]()
			{
				if (ownerCharacter->GetCharacterState() != ECharacterState::Defending)
				{
					if (ownerCharacter->GetCharacterState() == ECharacterState::Idle || ownerCharacter->GetCharacterState() == ECharacterState::Moving) {
						ownerCharacter->SetCharacterState(ECharacterState::Defending);
					}
					else
						Blocking = false;
				}
				else {
					Blocking = true;
				}


			});
		Parry_Handle = StartTimer(temp_delegate,  true);
	}

}

void USAction_Defends::OnActionEnd_Implementation()
{
	Super::OnActionEnd_Implementation();
	
	Blocking = false;

	if (Parry_Handle.IsValid()) {
		if (GetWorld()->GetTimerManager().IsTimerActive(Parry_Handle))
			GetWorld()->GetTimerManager().ClearTimer(Parry_Handle);
	}
}

void USAction_Defends::HitReactionBind(AActor* Instigator,float Amount, FAttackDebuff debuff)
{
	UAnimMontage* temp_HitMontage = combatInfo.CombatAnim[0].AttackAnimation;
	FTimerDelegate temp_Delegate;

	if (temp_HitMontage) {
		GetWorld()->GetTimerManager().ClearTimer(HitReaction_Handle);
		float temp_Duration = ownerCharacter->PlayAnimMontage(temp_HitMontage);
		ownerCharacter->GetCharacterMovement()->MaxWalkSpeed = 0;
		temp_Delegate.BindWeakLambda(this, [this] {
			ownerCharacter->GetCharacterMovement()->MaxWalkSpeed = OGSpeed;
			});

		StartTimer(temp_Delegate,false, temp_Duration);
	}
}
