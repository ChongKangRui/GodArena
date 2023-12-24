// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_CanParry.h"
#include "../GodsArenaCharacter.h"



void UANS_CanParry::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AActor* owner = MeshComp->GetOwner();
	if (!owner)
		return;

	ownerCharacter = Cast<AGodsArenaCharacter>(owner);

	if (!ownerCharacter)
	{
		return;
	}
	ownerCharacter->SetParryingState(NotifyBeginState);
}


void UANS_CanParry::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* owner = MeshComp->GetOwner();
	if (!owner)
		return;

	if (!ownerCharacter)
	{
		return;
	}

	if(ownerCharacter->GetCharacterState() != ECharacterState::Stun)
		ownerCharacter->SetParryingState(NotifyEndState);
}
