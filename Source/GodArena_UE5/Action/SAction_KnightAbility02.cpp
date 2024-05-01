// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_KnightAbility02.h"
#include "../Weapon/BP_WeaponBase.h"
#include "../Components/AttributeComponent.h"
#include "../Components/TargetingComponent.h"
#include "../GodsArenaCharacter.h"
#include "../Components/ActionComponent.h"
#include "../Weapon/Buff_Base.h"

void USAction_KnightAbility02::OnActionBegin_Implementation()
{
	if (BuffActor) {
		return;
	}

	Super::OnActionBegin_Implementation();

	FActorSpawnParameters temp_SpawnParams;
	temp_SpawnParams.Owner = ownerCharacter;
	temp_SpawnParams.Instigator = ownerCharacter;
	
	FTimerHandle temp_SpawnHandle;
	FTimerDelegate temp_delegate;

	temp_delegate.BindWeakLambda(this, [this, temp_SpawnParams]()
		{
			if (ShieldBuff && ownerCharacter) {
				BuffActor = GetWorld()->SpawnActor<ABuff_Base>(ShieldBuff, ownerCharacter->GetActorLocation(), ownerCharacter->GetActorRotation(), temp_SpawnParams);

				if (BuffActor) {
					BuffActor->OnEndBuff.AddDynamic(this, &USAction_KnightAbility02::OnBuffEndBind);
					BuffActor->AttachToActor(ownerCharacter, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				}
			}
		});
	StartTimer(temp_delegate, false, SpawnDelay);

	//Stun and launch enemy
	StartTimer(this, &USAction_KnightAbility02::StunEnemy, false, 0.5f);
}


void USAction_KnightAbility02::OnBuffEndBind()
{
	BuffActor = nullptr;
}

void USAction_KnightAbility02::StunEnemy()
{
	UTargetingComponent* temp_TC = Cast<UTargetingComponent>(ownerCharacter->GetComponentByClass(UTargetingComponent::StaticClass()));

	if (temp_TC) {
		if (temp_TC->CurrentTarget) {
			temp_TC->CurrentTarget->StunCharacter(2.0f);
			temp_TC->CurrentTarget->attributeComp->ApplyImpulse(5000, ownerCharacter);
		}
	}
}


