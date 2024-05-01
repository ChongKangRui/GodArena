// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../StructAndEnum.h"
#include "../GodsArenaCharacter.h"
#include "AttributeComponent.generated.h"


class UDataTable;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamaged, AActor*, DamageCauser, float, Damage, FAttackDebuff, debuff);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaCost, float, Damage);
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GODARENA_UE5_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAttributeComponent();

	UFUNCTION(BlueprintPure)
	const float GetMaxHealth() {
		return MaxHealth;
	}

	UFUNCTION(BlueprintPure)
	const float GetCurrentHealth()
	{
		return CurrentHealth;
	}

	UFUNCTION(BlueprintPure)
	const float GetMaxStamina()
	{
		return MaxStamina;
	}

	UFUNCTION(BlueprintPure)
	const float GetCurrentStamina()
	{
		return CurrentStamina;
	}

	UFUNCTION(BlueprintPure)
	const bool IsDead()
	{
		return Dead;
	}

	//Apply Damage
	UFUNCTION(BlueprintCallable)
	bool ApplyDamage(AActor* Instigator, const float& Amount, const FAttackDebuff& debuff);

	//Apply general action
	UFUNCTION(BlueprintCallable)
	bool ApplyStaminaCost(const float& Amount);

	//Apply from enemy attack
	UFUNCTION(BlueprintCallable)
	bool ApplyDamageStaminaCost(const float& Amount);

	UFUNCTION(BlueprintCallable)
	bool ApplyHealthRegen(const float& Amount);

	UFUNCTION(BlueprintCallable)
	void ApplyImpulse(const float& Impulse, const AActor* Instigator);

	//Initialize from datatable
	void InitializeVariable(TObjectPtr<AGodsArenaCharacter> ownerRef);


public:
	UPROPERTY(BlueprintAssignable)
	FOnDamaged OnDamaged;

	UPROPERTY(BlueprintAssignable, meta = (ToolTip = "Only work for character who have defending mode for stamina deduction"))
	FOnDamaged OnStaminaDamaged;

	UPROPERTY(BlueprintAssignable)
	FOnStaminaCost OnStaminaDeduct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Abandone hit reaction when character state is one of these state"))
	TArray<ECharacterState> HitReactionAbandonState = { ECharacterState::KnockOut, ECharacterState::Executed,
		ECharacterState::Stun, ECharacterState::Executing, ECharacterState::SkillExecuting, ECharacterState::Dodging, ECharacterState::Parrying };

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Ignore damage when character state is one of these state"))
	TArray<ECharacterState> DamageIgnoredState = { ECharacterState::KnockOut, ECharacterState::Executing };

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Stop stamina regen when character state is one of these state"))
	TArray<ECharacterState> StaminaRegenState = { ECharacterState::Idle, ECharacterState::Moving,
		ECharacterState::KnockOut, ECharacterState::Stun, ECharacterState::Executed };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterState hitEndState = ECharacterState::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "speed per frame"))
	float RegenStaminaSpeed = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "avoid impulse"))
	bool NoImpulseApply = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BloodSpawnUpOffset = FVector(0, 0, 100.0f);
private:
	UFUNCTION()
	void OnCharacterStateBind(ECharacterState state);

	void ApplyStaminaRegen();
	void SpawnBloodVFX();

private:
	float CurrentHealth;
	float MaxHealth;
	float CurrentStamina;
	float MaxStamina;

	bool Dead;
	bool IsEnemy;
	bool StaminaRegenActivated;

	TObjectPtr<class AGodsArenaCharacter> ownerCharacter;

	FTimerHandle HitReactionHandle;
	FTimerHandle StaminaRegenHandle;

	TArray<class UNiagaraSystem*> BloodList;

};
