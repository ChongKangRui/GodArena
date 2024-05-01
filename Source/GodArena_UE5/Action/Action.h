// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../StructAndEnum.h"
#include "Engine/EngineTypes.h"
#include "Action.generated.h"


class AGodsArenaCharacter;
/**
 *
 */
UCLASS(Blueprintable)
class GODARENA_UE5_API USAction : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init(TObjectPtr<class AGodsArenaCharacter> owner, FCharacterCombat combatStruct);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void OnActionBegin();

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void OnActionEnd();

	UFUNCTION(BlueprintCallable)
	const FCharacterCombat GetCombatInfo() {
		return combatInfo;
	}
	UFUNCTION(BlueprintCallable)
	const class AGodsArenaCharacter* GetOwnerCharacter() {
		return ownerCharacter;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AbandoneAction = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool NoEffectFromDebuff = false;

protected:
	template <typename T>
	FTimerHandle StartTimer(T* Object, void(T::*Function)(), const bool bLoop, const float delay = 0);

	FTimerHandle StartTimer(const FTimerDelegate& timerDelegate, const bool bLoop, const float delay = 0);

	bool ClearTimer(FTimerHandle& Handle);

	bool IsTimerActive(FTimerHandle& handle);

protected:
	TObjectPtr<class AGodsArenaCharacter> ownerCharacter;
	FCharacterCombat combatInfo;
	UWorld* world;
};


