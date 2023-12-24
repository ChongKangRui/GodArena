// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_RotationToTarget.h"
#include "../Components/TargetingComponent.h"
#include "../GodsArenaCharacter.h"
#include "../Enemy/Enemy_Base.h"



void UANS_RotationToTarget::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {

    if (!MeshComp->GetOwner())
        return;

    owner = Cast<AGodsArenaCharacter>(MeshComp->GetOwner());
    AEnemy_Base* temp_Enemy = Cast<AEnemy_Base>(MeshComp->GetOwner());

   // owner->DebugPrint(owner->GetName());

    if (temp_Enemy) {
        Target = temp_Enemy->Target;
       // owner->DebugPrint("GotTarget?");
        return;
    }

    UTargetingComponent* temp_TargetComp = MeshComp->GetOwner()->FindComponentByClass<UTargetingComponent>();
    if (temp_TargetComp)
        Target = temp_TargetComp->CurrentTarget;
}

void UANS_RotationToTarget::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
    Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

   

    if (Target && owner) {
       
        FVector direction = Target->GetActorLocation() - owner->GetActorLocation();
        direction.Normalize();

        float YawRadian = FMath::Atan2(direction.Y, direction.X);
        float YawAngle = FMath::RadiansToDegrees(YawRadian);

        FRotator TargetRotation = FRotator(0, YawAngle, 0);

        FRotator FinalRotation = FMath::RInterpTo(owner->GetActorRotation(),
            TargetRotation, FrameDeltaTime, RotationSpeed);;
        owner->SetActorRotation(FinalRotation);
    }

  
}
