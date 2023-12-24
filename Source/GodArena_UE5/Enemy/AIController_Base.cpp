// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_Base.h"
#include "../Enemy/Enemy_Base.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"




AAIController_Base::AAIController_Base() : UseControllerRotationState({ECharacterState::Defending, ECharacterState::Moving})
{
	behaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));

	blackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));

	SetActorTickInterval(0.1f);


	DefaultInitRandomCharacteterState.Add(ECharacterState::Moving);
	DefaultInitRandomCharacteterState.Add(ECharacterState::Attacking);
}

void AAIController_Base::BeginPlay()
{
	Super::BeginPlay();
	//BlackBoardValueInit();

}

void AAIController_Base::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	BlackBoardValueTick();

	if (ownerCharacter) {
		SetRotationState();

		if (ownerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation && ownerCharacter->Target) {
			RotateToTarget();
		}
	}
}

void AAIController_Base::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	UE_LOG(LogTemp, Display, TEXT("Possesss"));
	if (!IsValid(BehaviorTree))
		return;

	if (!behaviorTreeComponent->IsRegistered())
		return;

	ownerCharacter = Cast<AEnemy_Base>(pawn);

	blackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	behaviorTreeComponent->StartTree(*BehaviorTree);
	BlackBoardValueInit();
}

ECharacterState AAIController_Base::GetRandomBehavior(const TArray<ECharacterState>& StateList)
{
	if (StateList.Num() <= 0)
		return ECharacterState::Idle;

	int temp_randBehaviorInt = FMath::RandRange(0, StateList.Num() - 1);
	ECharacterState temp_State = StateList[temp_randBehaviorInt];


	return temp_State;

}

void AAIController_Base::BlackBoardValueInit()
{
	//UE_LOG(LogTemp, Display, TEXT("Recursion now"));
	
	if (!ownerCharacter)
	{
		return;
	}
	ownerCharacter->SetAIControllerReference(this);
	

	
	if (!GetWorld()->GetTimerManager().IsTimerActive(BlackboardInitialization)) {

		//loop this so this can find target certainly
		FTimerDelegate temp_delegate;
		//Bind event by lamdba expression
		temp_delegate.BindWeakLambda(this, [this]()
			{
				if (!ownerCharacter->Target)
				{
					APawn* player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
					if (!player) {
						return;
					}
					else {
						ownerCharacter->Target = Cast<AGodsArenaCharacter>(player);
						if (!ownerCharacter->Target)
							return;
						else
						{
							blackboardComp->SetValueAsObject("Target", ownerCharacter->Target);

							//ownerCharacter->SetCharacterState(GetRandomBehavior(DefaultInitRandomCharacteterState));
							//SetFocus(ownerCharacter->Target);
							GetWorld()->GetTimerManager().ClearTimer(BlackboardInitialization);

						}
					}

				}
			});
		GetWorld()->GetTimerManager().SetTimer(BlackboardInitialization,
			temp_delegate, 0.1f, true, 0);
	}

	


}

void AAIController_Base::BlackBoardValueTick()
{
	if (!ownerCharacter || !ownerCharacter->Target || !behaviorTreeComponent->TreeHasBeenStarted())
	{
		return;
	}

	if (ownerCharacter->Target->GetCharacterState() == ECharacterState::Death || ownerCharacter->GetCharacterState() == ECharacterState::Death) {
		blackboardComp->ClearValue("Target");
		SetActorTickEnabled(false);
		return;
	}

	float temp_Distance = FVector::Dist(ownerCharacter->GetActorLocation(), ownerCharacter->Target->GetActorLocation());
	blackboardComp->SetValueAsFloat("DistanceToTarget", temp_Distance);

	uint8 temp_TargetState = static_cast<uint8>(ownerCharacter->Target->GetCharacterState());
	blackboardComp->SetValueAsEnum("TargetState", temp_TargetState);

	bool temp_IsMontagePlaying = ownerCharacter->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();
	blackboardComp->SetValueAsBool("IsAnyMontagePlaying", temp_IsMontagePlaying);
	

}

void AAIController_Base::SetRotationState()
{
	if (ownerCharacter->GetCharacterState() == ECharacterState::Death) {
		ownerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = false;
		return;
	}

	if (UseControllerRotationState.Contains(ownerCharacter->GetCharacterState())) {
		ownerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = true;
		ownerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else {
		ownerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = false;
		ownerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	}


}

void AAIController_Base::RotateToTarget()
{
	
	FVector temp_dir = ownerCharacter->Target->GetActorLocation() - ownerCharacter->GetActorLocation();
	temp_dir.Normalize();
	float YawRadian = FMath::Atan2(temp_dir.Y, temp_dir.X);
	float YawAngle = FMath::RadiansToDegrees(YawRadian);

	FRotator TargetRotation = FRotator(0, YawAngle, 0.0f);

	FRotator FinalRotation = FMath::RInterpTo(ownerCharacter->GetActorRotation(),
		TargetRotation, GetWorld()->GetDeltaSeconds(), RotationInterpSpeed);

	//UE_LOG(LogTemp, Warning, TEXT("MyFloatValue: %f"), FinalRotation.Yaw);

	SetControlRotation(FinalRotation);

}
