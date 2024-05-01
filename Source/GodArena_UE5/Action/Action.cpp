// Fill out your copyright notice in the Description page of Project Settings.


#include "Action.h"
#include "../GodsArenaCharacter.h"
#include "Animation/AnimMontage.h"
#include "../Components/ActionComponent.h"

void USAction::Init(TObjectPtr <AGodsArenaCharacter> owner, FCharacterCombat combatStruct)
{
	if (owner == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("owner invalid init"));
		return;
	}
	ownerCharacter = owner;
	combatInfo = combatStruct;
	world = GetWorld();

	if (!ownerCharacter || !world) {
		AbandoneAction = true;
		return;
	}
}

bool USAction::ClearTimer(FTimerHandle& Handle)
{
	if (IsTimerActive(Handle)) {
		GetWorld()->GetTimerManager().ClearTimer(Handle);
		return true;
	}
	else
		return false;
}

bool USAction::IsTimerActive(FTimerHandle& handle)
{
	return GetWorld()->GetTimerManager().IsTimerActive(handle);
}

void USAction::OnActionBegin_Implementation()
{
	if (!ownerCharacter || AbandoneAction) {
		return;
	}

	ownerCharacter->actionComp->OnActionBegin.Broadcast(ownerCharacter, this);
}

template<typename T>
FTimerHandle USAction::StartTimer(T* Object, void(T::* Function)(), const bool bLoop, const float delay)
{

	FTimerDelegate TimerDelegate;
	// Bind the timer delegate with a lambda
	TimerDelegate.BindWeakLambda(this, [Object, Function]()
		{
			if (Object)
			{
				(Object->*Function)(); // Call the member function on the object
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString("invalid object "));
			}
		});

	// Set the timer with the specified duration and looping flag
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.02, bLoop, delay);

	return TimerHandle;
}

FTimerHandle USAction::StartTimer(const FTimerDelegate& timerDelegate, const bool bLoop, const float delay)
{
	// Set the timer with the specified duration and looping flag
	FTimerHandle TimerHandle;
	world->GetTimerManager().SetTimer(TimerHandle, timerDelegate, 0.02, bLoop, delay);

	return TimerHandle;
}

void USAction::OnActionEnd_Implementation()
{
	if (!ownerCharacter || AbandoneAction)
	{
		return;
	}

	if (GetOwnerCharacter()->GetCharacterState() == combatInfo.state)
		ownerCharacter->SetCharacterState(ECharacterState::Idle);

	ownerCharacter->actionComp->OnActionEnd.Broadcast(ownerCharacter, this);
	ownerCharacter->actionComp->ClearCurrentAction();
}


