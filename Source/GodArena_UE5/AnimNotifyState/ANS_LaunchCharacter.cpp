// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_LaunchCharacter.h"
#include "../Components/TargetingComponent.h"
#include "../Enemy/Enemy_Base.h"




void UANS_LaunchCharacter::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ACharacter* owner = Cast<ACharacter>(MeshComp->GetOwner());
	if (!owner)
		return;




	TargetDirection = FVector::Zero();

	switch (LaunchDirection)
	{
	case ELaunchDirection::EnemyDirection:
		TargetDirection = GetEnemyDirection(owner);
		break;
	case ELaunchDirection::Forward:
		TargetDirection = owner->GetActorForwardVector() * launchForce;
		break;
	case ELaunchDirection::Left:
		TargetDirection = (owner->GetActorRightVector() * -1) * launchForce;
		break;
	case ELaunchDirection::Back:
		TargetDirection = (owner->GetActorForwardVector() * -1) * launchForce;
		break;
	case ELaunchDirection::Right:
		TargetDirection = owner->GetActorRightVector() * launchForce;
		break;
	default:
		break;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TargetDirection.ToString());

	if (TargetDirection != FVector::Zero()) {
		if (LaunchDirection != ELaunchDirection::EnemyDirection) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString("Launch chacra"));
			owner->LaunchCharacter(TargetDirection, true, false);
		}
	}

}
void UANS_LaunchCharacter::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	AActor* owner = MeshComp->GetOwner();
	if (!owner) {
		
		return;
	}


	if (LaunchDirection == ELaunchDirection::EnemyDirection) {
		if (TargetDirection != FVector::Zero()) {
			if (FVector::Dist(owner->GetActorLocation(), TargetDirection) > DistanceToEnemy) {
				
				FVector temp_Velocity = FMath::VInterpTo(owner->GetActorLocation(), TargetDirection, FrameDeltaTime, launchForce);
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, temp_Velocity.ToString());
				owner->SetActorLocation(temp_Velocity);
			}
		}
	}


}

FVector UANS_LaunchCharacter::GetToEnemyVelocity(AActor* owner, AActor* Target)
{

	if (!owner || !Target)
		return FVector::Zero();


	FVector temp_dir = Target->GetActorLocation() - owner->GetActorLocation();
	temp_dir.Normalize();

	return temp_dir * launchForce;
}

FVector UANS_LaunchCharacter::GetEnemyDirection(AActor* owner)
{
	UTargetingComponent* TargetComp = owner->FindComponentByClass<UTargetingComponent>();
	AEnemy_Base* enemy = nullptr;


	if (!TargetComp)
		enemy = Cast<AEnemy_Base>(owner);


	if (TargetComp)
	{
		return TargetComp->CurrentTarget->GetActorLocation();
	}
	else {
		if (enemy) {
			return enemy->Target->GetActorLocation();
		}
	}
	return FVector::Zero();
}
