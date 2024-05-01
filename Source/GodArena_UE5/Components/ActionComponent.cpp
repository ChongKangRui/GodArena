// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionComponent.h"
#include "Animation/AnimMontage.h"
#include "../GodsArenaCharacter.h"
#include "../Action/Action.h"
#include "Components/SkeletalMeshComponent.h"




// Sets default values for this component's properties
UActionComponent::UActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UActionComponent::ActionInitialization(AGodsArenaCharacter* ownerRef)
{
	if (!ownerRef)
		return;

	owner = ownerRef;
	owner->OnStateChange.AddDynamic(this, &UActionComponent::OnCharacterDeath);

	if (owner->GetActionInfoList().Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Get Action info list failed"));
		return;
	}

	//For loop to initializae action
	for (auto Temp_ActionInfoList : owner->GetActionInfoList())
	{
		TSubclassOf<USAction> classType = Temp_ActionInfoList.Value.action.Get();
		if (!ensure(classType))
			continue;

		//owner->DebugPrint(classType.Get()->GetFullName());
		USAction* ac = NewObject<USAction>(GetOwner(), classType);

		ac->Init(owner, Temp_ActionInfoList.Value);
		//owner->DebugPrint(FString::FromInt(Temp_ActionInfoList.Value.Damage));
		actionList.Add(Temp_ActionInfoList.Key, ac);
	}

	OnInitializaActionFinished.Broadcast();
}

bool UActionComponent::ExecuteAction(ECombatType actionType)
{
	if (owner->GetCharacterState() == ECharacterState::Death)
		return false;


	USAction* Temp_Action = GetAction(actionType);
	currentAction = Temp_Action;

	//failed to get action
	if (!currentAction) {
		return false;
	}

	//Abandone Individual Action
	if (currentAction->AbandoneAction) {
		return false;
	}

	//failed because character state not allowed
	if(owner->GetActionInfoList().Find(actionType)->abandonActionState.Contains(owner->GetCharacterState()))
	{
		return false;
	}

	currentAction->OnActionBegin_Implementation();
	return true;

}

bool UActionComponent::TerminateAction(ECombatType actionType)
{

	if (owner->GetCharacterState() == ECharacterState::Death)
		return false;

	USAction* Temp_Action = GetAction(actionType);

	if (!Temp_Action)
	{
		return false;
	}
	if (Temp_Action->AbandoneAction)
	{
		return false;
	}


	Temp_Action->OnActionEnd_Implementation();

	if (currentAction == Temp_Action) {
		currentAction = nullptr;
	}
	return true;
}

void UActionComponent::TerminateAllAction()
{
	for (auto& Temp_ActionInfoList : actionList)
	{
		USAction* temp_Action = Temp_ActionInfoList.Value;
		if (temp_Action) {
			temp_Action->OnActionEnd_Implementation();
		}

	}
}

bool UActionComponent::TerminateCurrentAction()
{
	if (!currentAction)
		return false;

	currentAction->OnActionEnd_Implementation();
	currentAction = nullptr;
	return true;
}

USAction* UActionComponent::GetAction(ECombatType actionName)
{
	if (actionList.Num() <= 0)
	{
		return nullptr;
	}
	USAction** ActionPtr_Execute = actionList.Find(actionName);
	if (!ActionPtr_Execute || !(*ActionPtr_Execute))
	{
		UE_LOG(LogTemp, Warning, TEXT("No action find!!!"));
		return nullptr;
	}
	
	

	return (*ActionPtr_Execute);
}

void UActionComponent::OnCharacterDeath(ECharacterState state)
{
	if (state == ECharacterState::Death)
		AbandomAllAction = true;
}






