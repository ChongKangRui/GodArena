// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_WeaponTrace.h"
#include "../Weapon/BP_WeaponBase.h"
#include "../GodsArenaCharacter.h"

void UANS_WeaponTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AActor* owner = MeshComp->GetOwner();
	if (!owner)
		return;

	ownerCharacter = Cast<AGodsArenaCharacter>(owner);
	
	if (!ownerCharacter || !ownerCharacter->weapon) {
		return;
	}

	weapon = ownerCharacter->weapon;

	if (CanParry)
		ownerCharacter->SetParryingState(EParryingState::CanParry);

	if (ApplyDebuff) {
		prevAttribute = weapon->GetCurrentAttribute();
		weapon->SetAttributeDebuff(DebuffToApply);
	}
	
	weapon->BeginTrace();
	if (BeginTrailActivated)
		weapon->BeginTrail();
	
}

void UANS_WeaponTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* owner = MeshComp->GetOwner();
	if (!owner)
		return;

	ownerCharacter = Cast<AGodsArenaCharacter>(owner);

	if (!ownerCharacter || !ownerCharacter->weapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString("no owner or wea"));
		return;
	}

	if (weapon)
	{

		if (CanParry)
			ownerCharacter->SetParryingState(EParryingState::None);

		if (ApplyDebuff)
			weapon->SetAttributeDebuff(prevAttribute.debuff);

		if (BeginTrailActivated)
			weapon->EndTrail();

		weapon->EndTrace();
	}
	
}
