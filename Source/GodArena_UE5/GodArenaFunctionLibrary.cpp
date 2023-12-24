// Fill out your copyright notice in the Description page of Project Settings.


#include "GodArenaFunctionLibrary.h"
#include "Components/AttributeComponent.h"

bool UGodArenaFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* DamagedTarget, const float& damageAmount, const FAttackDebuff& debuff)
{
	if (!DamageCauser || !DamagedTarget) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString("Invalid Damage causer or target"));
		return false;
	}

	UAttributeComponent* comp = DamagedTarget->FindComponentByClass<UAttributeComponent>();
	if (!comp) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString("Invalid Attribute component"));
		return false;
	}

	comp->ApplyDamage(DamageCauser, damageAmount, debuff);
	return true;
}

bool UGodArenaFunctionLibrary::ApplyStaminaDeduction(AActor* DamageCauser, AActor* DamagedTarget, float StaminaAmount)
{
	return false;
}

template<typename T>
FTimerHandle UGodArenaFunctionLibrary::StartTimer(UObject* owner, void(T::* Function)(), float duration, bool bLoop)
{
	FTimerDelegate Delegate;
	FTimerHandle OutHandle;

	

	//owner->GetWorld()->GetTimerManager().SetTimer(OutHandle, &T::Function, 0.1, bLoop, duration);

	return OutHandle;
}

//bool UGodArenaFunctionLibrary::ApplyStaminaDeduction(AActor* DamageCauser, AActor* DamagedTarget, float StaminaAmount)
//{
//
//	UAttributeComponent* comp = DamagedTarget->FindComponentByClass<UAttributeComponent>();
//	if (!comp)
//		return false;
//
//	float 
//
//	return false;
//}
