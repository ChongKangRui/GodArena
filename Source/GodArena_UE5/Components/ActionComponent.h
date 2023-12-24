// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../StructAndEnum.h"
#include "ActionComponent.generated.h"


class USAction;
class AGodsArenaCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAction, AGodsArenaCharacter*, OwnerCharacter, USAction*, CurrentAction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionInitializationFinished);

class UDataTable;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GODARENA_UE5_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UActionComponent();
public:
	UPROPERTY(BlueprintAssignable)
	FAction OnActionBegin;

	UPROPERTY(BlueprintAssignable)
	FAction OnActionEnd;

	UPROPERTY(BlueprintAssignable)
	FActionInitializationFinished OnInitializaActionFinished;

	UPROPERTY(BlueprintReadWrite)
	bool AbandomAllAction = false;

public:
	UFUNCTION(BlueprintCallable, meta = (ToolTip = "This will call the OnActionBegin as well"))
	bool ExecuteAction(ECombatType actionType);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "This will call the OnActionEnd as well"))
	bool TerminateAction(ECombatType actionType);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "This will call the OnActionEnd for all action"))
	void TerminateAllAction();

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "This will call the OnActionEnd for current action"))
	bool TerminateCurrentAction();

	UFUNCTION(BlueprintPure, meta = (ToolTip = "Get USAction class based on enum"))
	USAction* GetAction(ECombatType actionType);

	UFUNCTION(BlueprintPure, meta = (ToolTip = "Get USAction class based on current active action"))
	USAction* GetCurrentActiveAction()
	{
		return currentAction.Get();
	};

	UFUNCTION(BlueprintPure, meta = (ToolTip = "Get USAction class based on current active action"))
	ECombatType GetCurrentActionType()
	{
		if (!currentAction)
			return ECombatType::None;

		return (*actionList.FindKey(currentAction));
	};

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Clear current action"))
	void ClearCurrentAction()
	{
		if (currentAction)
			currentAction = nullptr;
	};


	void ActionInitialization(AGodsArenaCharacter* ownerRef);



private:
	AGodsArenaCharacter* owner;
	TObjectPtr<USAction> currentAction;

	UPROPERTY(VisibleAnywhere)
	TMap<ECombatType, class USAction*> actionList;

private:
	UFUNCTION()
	void OnCharacterDeath(ECharacterState state);

	
};
