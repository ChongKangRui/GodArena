// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_CombatAttack.h"
#include "Action.h"
#include "Animation/AnimMontage.h"
#include "TimerManager.h"
#include "../GodsArenaCharacter.h"
#include "../Components/ActionComponent.h"
#include "../Components/AttributeComponent.h"
#include "../Components/TargetingComponent.h"
#include "../Weapon/BP_WeaponBase.h"
#include "../GodArenaFunctionLibrary.h"

void USAction_CombatAttack::Init(TObjectPtr <AGodsArenaCharacter> owner, FCharacterCombat combatStruct)
{
	USAction::Init(owner, combatStruct);
	combatState = ECombatState::NoCombat;
	currentCombatInt = 0;

	TargetingComp = ownerCharacter->FindComponentByClass<UTargetingComponent>();
}

bool USAction_CombatAttack::SetCombatState(ECombatState combatStateToChange)
{
	if (IsTimerActive(ResetCombatHandle)) {
		this->combatState = combatStateToChange;
		return true;
	}
	else
		return false;
}

bool USAction_CombatAttack::PlayCombatMontage()
{
	//Return if owner invalid or combat integer hit limit
	if (!ownerCharacter || currentCombatInt >= combatInfo.CombatAnim.Num())
	{
		UE_LOG(LogTemp, Display, TEXT("combat int more thant a"));
		return false;
	}

	ECharacterState currentState = ownerCharacter->GetCharacterState();
	UAnimMontage* temp_montage = combatInfo.CombatAnim[currentCombatInt].AttackAnimation;

	//Return if combat state not meet condition
	if (combatState == ECombatState::FinishedCombo || combatState == ECombatState::StartCombo)
	{
		FString EnumValueString = UEnum::GetValueAsString<ECombatState>(combatState);
		UE_LOG(LogTemp, Display, TEXT("Combat State: %s"), *EnumValueString);
		return false;
	}

	//Return & Reset if montage is nullptr
	if (!temp_montage)
	{
		ResetCombatMontage();
		UE_LOG(LogTemp, Display, TEXT("invalid montage"));
		return false;
	}
	bool staminaCost = ownerCharacter->attributeComp->ApplyStaminaCost(combatInfo.CombatAnim[currentCombatInt].Cost);
	if (!staminaCost)
	{
		UE_LOG(LogTemp, Display, TEXT("stamina not enough"));
		return false;
	}
	
	//Make sure to clear Reset Combat timer before start a new timer event
	GetWorld()->GetTimerManager().ClearTimer(ResetCombatHandle);

	//Set weapon attribute(damage, debuff, etc)
	if (ownerCharacter->weapon) {
		ownerCharacter->weapon->SetAttribute(combatInfo.CombatAnim[currentCombatInt]);
	}

	//Make sure defend was cancel
	ownerCharacter->actionComp->TerminateAction(ECombatType::Defense);
	
	
	//Play montage, set character & combat state
	combatState = ECombatState::StartCombo;
	ownerCharacter->PlayAnimMontage(temp_montage, ownerCharacter->GetCharacterBuff().AttackSpeedRate);
	ownerCharacter->SetCharacterState(combatInfo.state);
	currentCombatInt++;
	UE_LOG(LogTemp, Warning, TEXT("CombatInt: %d"), currentCombatInt);
	//Start a new reset combat event

	if (!GetWorld()->GetTimerManager().IsTimerActive(ResetCombatHandle)) {

		ResetCombatHandle = StartTimer(this, &USAction_CombatAttack::CombatResetTimer,true);
	}

	return true;
}

void USAction_CombatAttack::ResetCombatMontage()
{
	//Return if owner invalid
	if (!ownerCharacter)
		return;

	if (ResetCombatHandle.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(ResetCombatHandle);
		ResetCombatHandle.Invalidate();
	}

	currentCombatInt = 0;
	combatState = ECombatState::NoCombat;

	if(ownerCharacter->GetCharacterState() == ECharacterState::Attacking)
		ownerCharacter->SetCharacterState(ECharacterState::Idle);
	
}

bool USAction_CombatAttack::CanExecuteTarget()
{
	//Get direction from potential switch target
	if (!TargetingComp)
		return false;


	AGodsArenaCharacter* Target = TargetingComp->CurrentTarget;

	if (!Target)
		return false;

	FVector temp_Dir1 = Target->GetActorLocation() - ownerCharacter->GetActorLocation();

	//Get forward vector of the player, use to determine the angle
	FVector temp_Dir2 = ownerCharacter->GetActorForwardVector();

	temp_Dir1.Normalize();
	temp_Dir2.Normalize();

	//Get angle
	float temp_Radian = FMath::Acos(FVector::DotProduct(temp_Dir2, temp_Dir1));
	float temp_Angle = FMath::RadiansToDegrees(temp_Radian);

	float temp_Distance = FVector::Dist(Target->GetActorLocation(), ownerCharacter->GetActorLocation());

	return temp_Distance <= Execute_Distance && temp_Angle <= Execute_Angle 
		&& Target->GetCharacterState() == ECharacterState::Stun;
}

void USAction_CombatAttack::OnActionBegin_Implementation()
{
	Super::OnActionBegin_Implementation();
	if (!CanExecuteTarget())
		PlayCombatMontage();
	else
		ownerCharacter->actionComp->ExecuteAction(ECombatType::ExecuteAttack);
}

void USAction_CombatAttack::OnActionEnd_Implementation()
{
	ResetCombatMontage();
	Super::OnActionEnd_Implementation();
}

void USAction_CombatAttack::CombatResetTimer()
{
	if (!ownerCharacter || !this)
		return;
	
	ECharacterState ownerAction = ownerCharacter->GetCharacterState();
	bool anyMontagePlaying = ownerCharacter->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();

	if ((!anyMontagePlaying && /*ownerAction == ECharacterState::Attacking &&*/ combatState == ECombatState::FinishedCombo) || ownerAction == ECharacterState::KnockOut || ownerAction == ECharacterState::Stun
		|| ownerAction != ECharacterState::Attacking) {
		OnActionEnd_Implementation();
	}
}



