// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StructAndEnum.h"
#include "GodArenaFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API UGodArenaFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		static bool ApplyDamage(AActor* DamageCauser, AActor* DamagedTarget, const float& damageAmount, const FAttackDebuff& debuff);

	UFUNCTION(BlueprintCallable)
		static bool ApplyStaminaDeduction(AActor* DamageCauser, AActor* DamagedTarget, float StaminaAmount);

	template <typename T>
	static FTimerHandle StartTimer(UObject* owner,void(T::* Function)(), float duration, bool bLoop);

};


