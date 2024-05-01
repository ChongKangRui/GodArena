// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingComponent.h"
#include "../Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UTargetingComponent::UTargetingComponent() : DisableMeshRotationState({ ECharacterState::Stun, ECharacterState::KnockOut, ECharacterState::Executed, ECharacterState::Dodging })
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<APlayerCharacter>(GetOwner());
	if (player)
	{
		player->OnStateChange.AddDynamic(this, &UTargetingComponent::OnPlayerDeath);
	}
}

void UTargetingComponent::AddTarget(AGodsArenaCharacter* Target)
{
	if (!TargetedList.Contains(Target))
		TargetedList.Add(Target);
}

void UTargetingComponent::RemoveTarget(AGodsArenaCharacter* Target)
{
	if (TargetedList.Contains(Target))
		TargetedList.Remove(Target);
}

void UTargetingComponent::SwitchTarget(bool left)
{
	if (TargetedList.Num() == 0)
	{
		player->DebugPrint("Num none");
		return;
	}

	AGodsArenaCharacter* temp_lActor = nullptr;
	AGodsArenaCharacter* temp_rActor = nullptr;

	AGodsArenaCharacter* temp_OutActor = nullptr;

	float temp_smallestLAngle = -10000;
	float temp_smallestRAngle = 10000;

	for (auto temp_CurrentActor : TargetedList)
	{
		if (!temp_CurrentActor)
			continue;

		if (CurrentTarget == temp_CurrentActor)
			continue;

		//Get direction from potential switch target
		FVector temp_Dir1 = temp_CurrentActor->GetActorLocation() - player->GetActorLocation();

		//Get forward vector of the player, use to determine the angle
		FVector temp_Dir2 = player->GetActorForwardVector();

		//Get right vector of the player, use to determine the left and right
		FVector temp_RightVector = player->GetActorRightVector();

		temp_Dir1.Normalize();
		temp_Dir2.Normalize();
		temp_RightVector.Normalize();

		//Get angle
		float temp_Radian = FMath::Acos(FVector::DotProduct(temp_Dir2, temp_Dir1));
		float temp_Angle = FMath::RadiansToDegrees(temp_Radian);


		//Use right vector to check left and right
		if (FVector::DotProduct(temp_Dir1, temp_RightVector) > 0)
		{
			if (temp_Angle < temp_smallestRAngle)
			{
				temp_smallestRAngle = temp_Angle;
				temp_rActor = temp_CurrentActor;
			}
		}
		else
		{
			if (-temp_Angle > temp_smallestLAngle)
			{
				temp_smallestLAngle = temp_Angle;
				temp_lActor = temp_CurrentActor;
			}
		}
	}

	if (!temp_lActor && !temp_rActor)
	{
		player->DebugPrint("Null left and right actor");
		return;
	}

	if (left)
	{
		if (!temp_lActor)
			temp_OutActor = temp_rActor;
		else
			temp_OutActor = temp_lActor;
	}
	else {
		if (!temp_rActor)
			temp_OutActor = temp_lActor;
		else
			temp_OutActor = temp_rActor;

	}

	if (CurrentPointWidget)
		CurrentPointWidget->SetVisibility(false);

	CurrentTarget = temp_OutActor;
	SetTargetingPointWidget();
}

void UTargetingComponent::ToggleTarget()
{
	if (!IsTargeted)
		BeginTarget();
	else
		EndTarget();
}

void UTargetingComponent::BeginTarget()
{
	if (!player || player->GetCharacterState() == ECharacterState::Death)
	{
		player->DebugPrint("Player Reference invalid");
		return;
	}

	float temp_SmallestAngle = 10000;
	AGodsArenaCharacter* temp_Target = nullptr;

	for (auto Target : TargetedList)
	{
		if (!Target)
			continue;

		if (Target->GetCharacterState() == ECharacterState::Death)
		{
			
			TargetedList.Remove(Target);
			continue;
		}

		//Get direction from potential switch target
		FVector temp_Dir1 = Target->GetActorLocation() - player->GetActorLocation();

		//Get forward vector of the player, use to determine the angle
		FVector temp_Dir2 = player->GetActorForwardVector();

		temp_Dir1.Normalize();
		temp_Dir2.Normalize();

		//Get angle
		float temp_Radian = FMath::Acos(FVector::DotProduct(temp_Dir2, temp_Dir1));
		float temp_Angle = FMath::RadiansToDegrees(temp_Radian);

		if (temp_Angle < temp_SmallestAngle)
		{
			temp_SmallestAngle = temp_Angle;
			temp_Target = Target;
		}

	}

	CurrentTarget = temp_Target;
	if (!CurrentTarget)
		return;

	SetTargetingPointWidget();

	//Start the targeting rotation tick
	if (!GetWorld()->GetTimerManager().IsTimerActive(TargetingHandle))
	{
		FTimerDelegate temp_delegate;
		temp_delegate.BindWeakLambda(this, [this]()
			{
				TargetingTick();
			});
		player->DebugPrint("tick targeting activated");
		IsTargeted = true;
		GetWorld()->GetTimerManager().SetTimer(TargetingHandle, temp_delegate, GetWorld()->GetDeltaSeconds(), true, 0);
	}
}

void UTargetingComponent::EndTarget()
{
	GetWorld()->GetTimerManager().ClearTimer(TargetingHandle);
	IsTargeted = false;

	if (CurrentPointWidget)
		CurrentPointWidget->SetVisibility(false);

	CurrentTarget = nullptr;
	CurrentPointWidget = nullptr;

	player->GetCharacterMovement()->bOrientRotationToMovement = true;
	player->GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void UTargetingComponent::TargetingTick()
{
	if (!CurrentTarget || CurrentTarget->GetCharacterState() == ECharacterState::Death) {
		EndTarget();
		//BeginTarget();
		return;
	}

	FVector direction = CurrentTarget->GetActorLocation() - player->GetActorLocation();
	bool IsDodging = (player->GetCharacterState() == ECharacterState::Dodging);
	if (dodgeSetting.RotateToEnemyAfterDodge)
	{
		if (!IsDodging)
		{
			player->GetCharacterMovement()->bOrientRotationToMovement = false;
			player->GetCharacterMovement()->bUseControllerDesiredRotation = true;
		}

	}
	else
	{
		float ownerYaw = player->GetActorRotation().Yaw;
		float controllerYaw = player->GetController()->GetControlRotation().Yaw;
		if (!IsDodging)
		{
			if ((ownerYaw - controllerYaw) >= dodgeSetting.ReFollowAngleMin && (ownerYaw - controllerYaw) <= dodgeSetting.ReFollowAngleMax)
			{

				player->GetCharacterMovement()->bOrientRotationToMovement = false;
				player->GetCharacterMovement()->bUseControllerDesiredRotation = true;
			}
		}
	}


	//Prevent mesh keep rotating with controller rotation
	if (DisableMeshRotationState.Contains(player->GetCharacterState())) {
		player->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	}
	else {
		if (!player->GetCharacterMovement()->bUseControllerDesiredRotation)
			player->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}

	direction.Normalize();

	float YawRadian = FMath::Atan2(direction.Y, direction.X);
	float YawAngle = FMath::RadiansToDegrees(YawRadian);

	//Get Pitch Rotation
	float PitchRadian = FMath::Atan2(direction.Z, direction.Size2D());
	float PitchAngle = FMath::RadiansToDegrees(PitchRadian);
	float pitch = FMath::Clamp(PitchAngle, -30, 30);

	//FVector debugVec = FVector(PitchRadian, YawRadian, 0);

	FRotator TargetRotation = FRotator(0, YawAngle, 0.0f);

	FRotator FinalRotation = FMath::RInterpTo(player->GetController()->GetControlRotation(),
		TargetRotation, GetWorld()->GetDeltaSeconds(), RotationInterpSpeed);

	//player->DebugPrint(FinalRotation.ToString());

	player->Controller->SetControlRotation(FinalRotation);
}

void UTargetingComponent::SetTargetingPointWidget()
{
	UActorComponent* temp_PointWidgetAC = CurrentTarget->GetComponentsByTag(USceneComponent::StaticClass(), FName("Point"))[0];
	if (temp_PointWidgetAC)
	{
		CurrentPointWidget = Cast<USceneComponent>(temp_PointWidgetAC);
		if (CurrentPointWidget)
		{
			CurrentPointWidget->SetVisibility(true);
		}
	}
}

void UTargetingComponent::OnPlayerDeath(ECharacterState state)
{
	if (state == ECharacterState::Death)
		EndTarget();
}




