// Fill out your copyright notice in the Description page of Project Settings.


#include "../Enemy/Enemy_Base.h"
#include "AIController_Base.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Components/TargetingComponent.h"


//#include "Components/WidgetComponent.h"

AEnemy_Base::AEnemy_Base()
{
	Tags.Add("Enemy");
	AIControllerClass = AAIController_Base::StaticClass();
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	//WidgetPoint = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetingPoint"));
	////WidgetPoint->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	//WidgetPoint->SetVisibility(false);

	JogState.Add(ECharacterJogState::Run, 600);
	JogState.Add(ECharacterJogState::Jog, 100);
	JogState.Add(ECharacterJogState::Jump, 700);
	JogState.Add(ECharacterJogState::Dash, 1000);

}

void AEnemy_Base::BeginPlay()
{
	Super::BeginPlay();
}


void AEnemy_Base::SetCharacterJogState(const ECharacterJogState& JogStateToChange)
{
	GetCharacterMovement()->MaxWalkSpeed = *JogState.Find(JogStateToChange);
}



void AEnemy_Base::SetCharacterState(ECharacterState stateToChange)
{
	if (state == ECharacterState::Death)
		return;

	if (GetCharacterState() != stateToChange)
	{
		//DebugPrint("SetState");
		Super::SetCharacterState(stateToChange);
		if (AIController && AIController->blackboardComp)
		{
			uint8 temp_State = static_cast<uint8>(stateToChange);
			AIController->blackboardComp->SetValueAsEnum("CharacterState", temp_State);
			
		}

		//Set rotation based on state
		//Stop Rotation if getting stun or executed
		if (GetCharacterState() == ECharacterState::Stun || GetCharacterState() == ECharacterState::Executed) {
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
		}
		//Begin Rotation if these state
		if (GetCharacterState() == ECharacterState::Idle || GetCharacterState() == ECharacterState::Attacking 
			|| GetCharacterState() == ECharacterState::Moving) {
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
		}
		
	}



}

void AEnemy_Base::Death()
{

	Super::Death();
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	

}



