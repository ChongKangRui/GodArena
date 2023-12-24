// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTAsk_Jump.h"
#include "../../Enemy/Enemy_Base.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"



UBTTAsk_Jump::UBTTAsk_Jump()
{
	NodeName = "Jump/Dash";
	BlackboardKey.AddVectorFilter(this, FName("TargetLocation"));
}

EBTNodeResult::Type UBTTAsk_Jump::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	BTComp = &owner_comp;
	auto const aiController = owner_comp.GetAIOwner();
	auto const BlackboardComp = owner_comp.GetBlackboardComponent();
	AEnemy_Base* owner = Cast<AEnemy_Base>(aiController->GetPawn());
	
	FVector jumpLocation = BlackboardComp->GetValueAsVector(BlackboardKey.SelectedKeyName);

	if (!owner) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString("Warning: no owner component ref"));
		return EBTNodeResult::Failed;
	}

	OGSpeed = owner->GetMovementComponent()->GetMaxSpeed();
	

	UCharacterMovementComponent* MC = Cast<UCharacterMovementComponent>(owner->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	if (!MC) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString("Warning: no movement component ref"));
		return EBTNodeResult::Failed;
	}
	
	
	OGJumpHeight = MC->JumpZVelocity;
	
	MC->JumpZVelocity = Height;
	

	float InitDistance = FVector::Dist2D(owner->GetActorLocation(), jumpLocation);
	FVector InitPos = owner->GetActorLocation();

	owner->Jump();
	
	FTimerDelegate TimerDelegate;
	// Bind the timer delegate with a lambda
	TimerDelegate.BindWeakLambda(this, [this,owner, jumpLocation, InitDistance, InitPos]()
		{
		//	FVector temp_Vec1 = { owner->GetActorLocation().X, owner->GetActorLocation().Y, 0.0f };

			float Distance = FVector::Dist2D(owner->GetActorLocation() , jumpLocation);
			
			FVector velocity = (jumpLocation - owner->GetActorLocation());
		

			velocity.Normalize();
			owner->AddMovementInput(velocity);

			if (!(Distance <= 100.0f)) {
				if ((Distance / InitDistance) <= 0.2f)
					owner->StopJumping();
				
			}
			
			if (!owner->GetCharacterMovement()->IsFalling()) {
				GetWorld()->GetTimerManager().ClearTimer(handle);
				EndTask();
			}
			

		});

	// Set the timer with the specified duration and looping flag
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(handle, TimerDelegate, 0.016, true, 0);


	return EBTNodeResult::InProgress;
}

void UBTTAsk_Jump::EndTask()
{

	if (BTComp)
		FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);

}
