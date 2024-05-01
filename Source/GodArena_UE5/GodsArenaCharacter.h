// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StructAndEnum.h"
#include "GodsArenaCharacter.generated.h"

class UAnimMontage;
class UActionComponent;
class UAttributeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChange, ECharacterState, state);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExecuteStateChange, EExecutedState, ExecState);

UCLASS(config = Game)
class AGodsArenaCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	AGodsArenaCharacter();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void SetExecutedState(EExecutedState Executed_State);

	UFUNCTION(BlueprintCallable)
	virtual void SetCharacterBuff(FAttackBuff buff);

	UFUNCTION(BlueprintCallable)
	virtual void SetCharacterState(ECharacterState state);

	UFUNCTION(BlueprintCallable)
	virtual void SetCharacterMovementState(ECharacterJogState MovementState);

	UFUNCTION(BlueprintCallable)
	void SetParryingState(EParryingState parryingStateToChange) {
		parryingState = parryingStateToChange;
	};

	UFUNCTION(BlueprintPure)
	const FAttackBuff GetCharacterBuff()
	{
		return CharacterBuff;
	};

	UFUNCTION(BlueprintPure)
	const TMap<ECombatType, FCharacterCombat> GetActionInfoList() {
		return ActionInfoList;
	}

	UFUNCTION(BlueprintPure)
	const FCharacterInfo& GetCharacterInfo() {
		if (characterInfo != nullptr)
		{
			return *characterInfo;
		}
		static const FCharacterInfo DefaultCharacterInfo;
		return DefaultCharacterInfo;
	}

	UFUNCTION(BlueprintPure)
	const EExecutedState GetCharacterExecutedState()
	{
		return executedState;
	};

	UFUNCTION(BlueprintPure)
	EParryingState GetParryingState() const {
		return parryingState;
	};

	UFUNCTION(BlueprintPure)
	ECharacterState GetCharacterState() const
	{
		return state;
	};

	UFUNCTION(BlueprintPure)
	ECharacterJogState GetCharacterMovementState() const
	{
		return characterMovementState;
	};

	UFUNCTION(BlueprintCallable)
	virtual void Death();

	UFUNCTION(BlueprintCallable)
	virtual void StunCharacter(const float& duration);

	UFUNCTION(BlueprintCallable)
	virtual void BindOnWeaponTraceHit(AGodsArenaCharacter* HitCharacter, FCharacterCombatAnimation currentEnemyAttribute);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = "This will stop all timer for this character(including component)"))
	void TerminateCharacter();

	UFUNCTION(BlueprintCallable)
	void DebugPrint(FString text) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, text);
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ECharacterJogState, float> JogState = { {ECharacterJogState::Jog, 300.0f }, {ECharacterJogState::Run, 800.0f } };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	ECharacterType ownerType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<class ABP_WeaponBase> weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<UActionComponent> actionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<UAttributeComponent> attributeComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanBeingParry = true;

	UPROPERTY(BlueprintAssignable)
	FOnStateChange OnStateChange;

	UPROPERTY(BlueprintAssignable)
	FOnExecuteStateChange OnExecuteStageChange;

protected:
	//CharacterState
	ECharacterState state;
	EExecutedState executedState;
	EParryingState parryingState = EParryingState::None;
	ECharacterJogState characterMovementState = ECharacterJogState::Jog;

	//contain character health, combat info and stuff. A summary of everything from datatable
	FCharacterInfo* characterInfo;
	FAttackBuff CharacterBuff;

	TObjectPtr<class UDataTable> DataTableAsset;
	TMap<ECombatType, FCharacterCombat> ActionInfoList;

protected:
	//Put to construction script
	void DataTableInitialization();

	//Put to begin play to make sure sequence correct
	void CharacterTypeInitialization();







};

