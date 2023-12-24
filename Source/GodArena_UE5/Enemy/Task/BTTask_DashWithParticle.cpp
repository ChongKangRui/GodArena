// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DashWithParticle.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NiagaraFunctionLibrary.h"
#include "../../GodsArenaCharacter.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type UBTTask_DashWithParticle::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	BTComp = &owner_comp;
	auto const aiController = owner_comp.GetAIOwner();
	auto const BlackboardComp = owner_comp.GetBlackboardComponent();
	owner = Cast<AGodsArenaCharacter>(aiController->GetPawn());

	FVector MoveLocation = BlackboardComp->GetValueAsVector(BlackboardKey.SelectedKeyName);

	if (!owner) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString("Warning: no owner component ref"));
		return EBTNodeResult::Failed;
	}

	aiController->MoveToLocation(MoveLocation);
	

	if(BlinkEffect)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(owner->GetWorld(), BlinkEffect,
			owner->GetActorLocation(),
			FRotator::ZeroRotator, FVector(1.0f), true, true, ENCPoolMethod::None, true);


	owner->GetMesh()->SetVisibility(false, true);
	if (!GetWorld()->GetTimerManager().IsTimerActive(handle)) {

		FTimerDelegate TimerDelegate;
		// Bind the timer delegate with a lambda
		TimerDelegate.BindWeakLambda(this, [this, MoveLocation, aiController]()
			{
				float Dist = FVector::Dist2D(owner->GetActorLocation(), MoveLocation);
				if (DashEffect) {
					//if (!SpawnedEffect) {
					FRotator rotation = UKismetMathLibrary::MakeRotFromX((MoveLocation - owner->GetActorLocation()).GetSafeNormal());
					DaskComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(owner->GetWorld(), DashEffect,
						owner->GetActorLocation(),
						rotation, FVector(1.0f), true, true, ENCPoolMethod::None, true);
					//}
				}
				
				if (owner->GetVelocity().Length() <= 0.0f && Dist <=200) {

					EndTask();
				}


			});

		GetWorld()->GetTimerManager().SetTimer(handle, TimerDelegate, 0.1, true, 0);
	}


	return EBTNodeResult::InProgress;
}

void UBTTask_DashWithParticle::EndTask()
{
	if (BTComp) {
		GetWorld()->GetTimerManager().ClearTimer(handle);
		owner->GetMesh()->SetVisibility(true, true);
		if (DaskComp)
			DaskComp->DestroyComponent();

		if (BlinkEffect)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(owner->GetWorld(), BlinkEffect,
				owner->GetActorLocation(),
				FRotator::ZeroRotator, FVector(1.0f), true, true, ENCPoolMethod::None, true);

		FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
	}
}
