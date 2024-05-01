// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_SetCombatState.h"
#include "../Components/ActionComponent.h"
#include "GameFramework/Actor.h"
#include "../Action/SAction_CombatAttack.h"

void UANS_SetCombatState::CombatContinuation(USkeletalMeshComponent* MeshComp, bool isCombatContinue)
{
	if (!MeshComp || !MeshComp->GetOwner())
		return;


	UActionComponent* Comp = MeshComp->GetOwner()->FindComponentByClass<UActionComponent>();
	if (Comp)
	{
		USAction_CombatAttack* CA = Cast<USAction_CombatAttack>(Comp->GetAction(combatType));
		if(!CA) {
			UE_LOG(LogTemp, Display, TEXT("DataTable not contain this combat type"));
			return;
		}

		if (isCombatContinue)
		{
			CA->SetCombatState(BeginState);
			UE_LOG(LogTemp, Display, TEXT("combat continue"));
		}
		else
		{
			CA->SetCombatState(EndState);
			UE_LOG(LogTemp, Display, TEXT("combat not continue"));
		}

	}

}

void UANS_SetCombatState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{	
	CombatContinuation(MeshComp, true);
}

void UANS_SetCombatState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	CombatContinuation(MeshComp, false);
}


