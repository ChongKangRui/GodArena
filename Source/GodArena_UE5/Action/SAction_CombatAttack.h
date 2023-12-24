// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "../StructAndEnum.h"
#include "SAction_CombatAttack.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API USAction_CombatAttack : public USAction
{
	GENERATED_BODY()
public:
	void Init(TObjectPtr <class AGodsArenaCharacter> owner, FCharacterCombat combatStruct) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Execute_Distance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Execute_Angle = 35.0f;


	UFUNCTION(BlueprintCallable)
		bool SetCombatState(ECombatState combatState);


	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Usually used to play combat animation and continue combat"))
		bool PlayCombatMontage();


	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Usually used to reset everything "))
		void ResetCombatMontage();

	UFUNCTION(BlueprintCallable)
		ECombatState GetCombatState() const
	{
		return combatState;
	}

	void ExecuteEnemy();
	bool CanExecuteTarget();
	
	virtual void OnActionBegin_Implementation() override;
	virtual void OnActionEnd_Implementation() override;

protected:
	virtual void CombatResetTimer();
	ECombatState combatState;
	FTimerHandle ResetCombatHandle;
	int currentCombatInt;

	class UTargetingComponent* TargetingComp;
	
};
