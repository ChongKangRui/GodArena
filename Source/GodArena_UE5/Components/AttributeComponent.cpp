// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Components/ActionComponent.h"
#include "Animation/AnimMontage.h"
#include "../Enemy/Enemy_Base.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent() : Dead(false)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttributeComponent::InitializeVariable(TObjectPtr<AGodsArenaCharacter> ownerRef)
{
	if (!ownerRef)
		return;

	ownerCharacter = ownerRef;

	BloodList = ownerCharacter->GetCharacterInfo().DamagedBlood;
	MaxHealth = ownerCharacter->GetCharacterInfo().Health;
	CurrentHealth = MaxHealth;
	MaxStamina = ownerCharacter->GetCharacterInfo().Stamina;
	CurrentStamina = MaxStamina;

	ownerCharacter->OnStateChange.AddDynamic(this, &UAttributeComponent::OnCharacterStateBind);

	class AEnemy_Base* enemy = Cast<AEnemy_Base>(ownerCharacter);
	if (enemy)
		IsEnemy = true;

}

bool UAttributeComponent::ApplyHealthRegen(const float& Amount)
{
	return false;
}

void UAttributeComponent::ApplyImpulse(const float& Impulse, const AActor* Instigator)
{
	if (!NoImpulseApply) {
		FVector temp_AttackedDir = ownerCharacter->GetActorLocation() - Instigator->GetActorLocation();
		temp_AttackedDir.Normalize();
		FVector velocity = temp_AttackedDir * Impulse;
		ownerCharacter->LaunchCharacter(velocity, false, false);
	}
}

void UAttributeComponent::ApplyStaminaRegen()
{
	if (!ownerCharacter)
		return;

	if (CurrentStamina < MaxStamina) {
		CurrentStamina += ownerCharacter->GetCharacterInfo().StaminaRate;
	}
	else {
		CurrentStamina = MaxStamina;
		if (StaminaRegenHandle.IsValid() && GetWorld()->GetTimerManager().IsTimerActive(StaminaRegenHandle)) {
			GetWorld()->GetTimerManager().ClearTimer(StaminaRegenHandle);
			StaminaRegenActivated = false;
		}
	}
}

bool UAttributeComponent::ApplyDamage(AActor* Instigator, const float& Amount, const FAttackDebuff& debuff)
{
	if (!ownerCharacter || ownerCharacter->GetCharacterState() == ECharacterState::Death)
		return false;

	if (CurrentHealth <= 0)
		return false;

	if (DamageIgnoredState.Contains(ownerCharacter->GetCharacterState()))
		return false;

	if (ownerCharacter->GetCharacterState() == ECharacterState::Defending) {
		ApplyDamageStaminaCost(debuff.StaminaCost);
		ApplyImpulse(debuff.Impulse, Instigator);
		OnStaminaDamaged.Broadcast(Instigator, Amount, debuff);
		return false;
	}

	CurrentHealth -= Amount * ownerCharacter->GetCharacterBuff().DamageReduceRate;
	SpawnBloodVFX();

	if (IsEnemy)
		ApplyDamageStaminaCost(debuff.StaminaCost);

	//Add Impulse
	ApplyImpulse(debuff.Impulse, Instigator);

	if (CurrentHealth <= 0)
	{
		//Death animation was set inside animation blueprint
		ownerCharacter->SetCharacterState(ECharacterState::Death);
		ownerCharacter->Death();
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
		return true;
	}

	GetWorld()->GetTimerManager().ClearTimer(HitReactionHandle);

	//Hit Reaction
	if (!HitReactionAbandonState.Contains(ownerCharacter->GetCharacterState()) && !ownerCharacter->GetCharacterBuff().NoHitReaction)
	{
		int temp_HitReactionInt = FMath::RandRange(0, ownerCharacter->GetCharacterInfo().HitReaction.Num() - 1);
		auto temp_HitReaction = ownerCharacter->GetCharacterInfo().HitReaction[temp_HitReactionInt];

		ownerCharacter->actionComp->TerminateCurrentAction();
		ownerCharacter->actionComp->TerminateAction(ECombatType::NormalAttack);

		if (temp_HitReaction)
		{
			ownerCharacter->PlayAnimMontage(temp_HitReaction);
			ownerCharacter->SetCharacterState(ECharacterState::GetHit);

			FTimerDelegate temp_delegate;
			ownerCharacter->SetCharacterMovementState(ECharacterJogState::NoMovement);
			//Bind event by lamdba expression
			temp_delegate.BindWeakLambda(this, [this]()
				{
					ownerCharacter->SetCharacterMovementState(ECharacterJogState::Jog);
					ownerCharacter->SetCharacterState(ECharacterState::Idle);
				});

			GetWorld()->GetTimerManager().SetTimer(HitReactionHandle,
				temp_delegate, 0.1, false, ownerCharacter->GetCharacterInfo().HitDuration);

		}
	}

	OnDamaged.Broadcast(Instigator, Amount, debuff);
	return true;
}

bool UAttributeComponent::ApplyStaminaCost(const float& Amount)
{
	if (CurrentStamina < Amount)
		return false;

	if (CurrentStamina >= Amount)
	{
		CurrentStamina -= Amount * ownerCharacter->GetCharacterBuff().StaminaCostRate;
	}
	return true;
}

bool UAttributeComponent::ApplyDamageStaminaCost(const float& Amount)
{
	if (!ownerCharacter || ownerCharacter->GetCharacterState() == ECharacterState::KnockOut
		|| ownerCharacter->GetCharacterState() == ECharacterState::Stun || ownerCharacter->GetCharacterState() == ECharacterState::Executed)
	{
		return false;
	}
	CurrentStamina -= Amount * ownerCharacter->GetCharacterBuff().DamageStaminaCostRate;

	if (CurrentStamina <= 0) {
		ownerCharacter->StunCharacter(ownerCharacter->GetCharacterInfo().StunDuration);
	}

	return true;
}

void UAttributeComponent::OnCharacterStateBind(ECharacterState state)
{
	if (!StaminaRegenState.Contains(state)) {
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenHandle);
	}
	
	if (StaminaRegenState.Contains(state) && CurrentStamina < MaxStamina && !GetWorld()->GetTimerManager().IsTimerActive(StaminaRegenHandle)) {
		
		FTimerDelegate temp_delegate;
		temp_delegate.BindWeakLambda(this, [this] {
			ApplyStaminaRegen();
			});
		GetWorld()->GetTimerManager().SetTimer(StaminaRegenHandle,
			temp_delegate, GetWorld()->GetDeltaSeconds(), true, ownerCharacter->GetCharacterInfo().StaminaModificationDelay);
	}
}

void UAttributeComponent::SpawnBloodVFX()
{
	int temp_RandomInt = FMath::RandRange(0, BloodList.Num() - 1);
	UNiagaraSystem* temp_Niagara = BloodList[temp_RandomInt];

	if (temp_Niagara) {

		UNiagaraFunctionLibrary::SpawnSystemAttached(temp_Niagara,
			ownerCharacter->GetMesh(), TEXT("BloodPoint"), 
			BloodSpawnUpOffset, ownerCharacter->GetActorRotation(), EAttachLocation::SnapToTarget, true);

		//ownerCharacter->DebugPrint("BloodSpawn");
	}
}




