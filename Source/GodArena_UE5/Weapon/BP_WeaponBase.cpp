// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_WeaponBase.h"
#include "../GodArenaFunctionLibrary.h"
#include "../GodsArenaCharacter.h"
#include "../Components/ActionComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "../StructAndEnum.h"

using namespace EDrawDebugTrace;

ABP_WeaponBase::ABP_WeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(GetRootComponent());

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail"));
	ParticleSystemComponent->SetupAttachment(mesh);

	BeginWeaponTracePoint = CreateDefaultSubobject<USceneComponent>(TEXT("BeginTrace"));
	EndWeaponTracePoint = CreateDefaultSubobject<USceneComponent>(TEXT("EndTrace"));

	BeginWeaponTracePoint->SetupAttachment(mesh);
	EndWeaponTracePoint->SetupAttachment(mesh);

}
// Called when the game starts or when spawned
void ABP_WeaponBase::BeginPlay()
{
	Super::BeginPlay();
	ownerCharacter = Cast<AGodsArenaCharacter>(GetParentActor());
	OriginalEndPointVec = EndWeaponTracePoint->GetRelativeLocation();
}

void ABP_WeaponBase::BeginTrace_Implementation()
{
	EndTraceTick(currentTraceHandle);
	currentTraceHandle = BeginTraceTick(BeginWeaponTracePoint, EndWeaponTracePoint, CurrentTraceType);
}

void ABP_WeaponBase::EndTrace_Implementation()
{
	EndTraceTick(currentTraceHandle);
}

FTimerHandle ABP_WeaponBase::BeginTraceTick(const USceneComponent* BeginPoint, const USceneComponent* EndPoint, EWeaponTraceType traceType)
{

	FTimerDelegate temp_delegate;
	FTimerHandle traceHandle;

	IgnoredActorList.Empty();

	IgnoredActorList.Add(this);
	IgnoredActorList.Add(ownerCharacter);

	temp_delegate.BindWeakLambda(this, [this, BeginPoint, EndPoint, traceType]()
		{
			FVector temp_BeginVector = (BeginPoint) ? BeginPoint->GetComponentLocation() : FVector::Zero();
			FVector temp_EndVector = (EndPoint) ? EndPoint->GetComponentLocation() : temp_BeginVector;

			Trace(temp_BeginVector, temp_EndVector, traceType);
		});
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString("Begin Trace"));
	if (!GetWorld()->GetTimerManager().IsTimerActive(traceHandle))
	{
		OnWeaponBeginTrace.Broadcast();
		GetWorld()->GetTimerManager().SetTimer(traceHandle, temp_delegate, TraceTickSpeed, true, 0);
	}

	return traceHandle;

}

void ABP_WeaponBase::Trace(FVector BeginPoint, FVector EndPoint, EWeaponTraceType traceType)
{
	if (!ownerCharacter)
		return;

	TArray<FHitResult> HitResult;
	HitResult.Empty();
	FCollisionQueryParams TraceParams(FName(TEXT("Trace")), true, GetParentActor());

	FVector Start = BeginPoint;
	FVector End = EndPoint;

	TraceParams.AddIgnoredActors(IgnoredActorList);

	bool bHit;
	bHit = false;

	FCollisionShape shape;
	Type temp_traceType;

	if (DebugDuration <= 0.0f)
	{
		temp_traceType = None;

	}
	else {
		temp_traceType = ForDuration;
	}

	switch (traceType)
	{
	case EWeaponTraceType::SphereTrace:
		bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, SphereRadius,
			CollisionTraceType, false, IgnoredActorList,
			temp_traceType, HitResult, true, FLinearColor::Red, FLinearColor::Green, DebugDuration);
		break;
	case EWeaponTraceType::CapsuleTrace:
		bHit = UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), Start, End, CapsuleRadius, CapsuleHeight,
			CollisionTraceType, false, IgnoredActorList,
			temp_traceType, HitResult, true, FLinearColor::Red, FLinearColor::Green, DebugDuration);

		break;
	case EWeaponTraceType::BoxTrace:
		bHit = UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), Start, End, BoxHalfSize, FQuat::Identity.Rotator(),
			CollisionTraceType, false, IgnoredActorList,
			temp_traceType, HitResult, true, FLinearColor::Red, FLinearColor::Green, DebugDuration);
		break;
	default:
		break;
	}
	
	if (bHit) {

		for (int i = 0; i < HitResult.Num(); i++)
		{

			if (HitResult[i].GetActor() && !IgnoredActorList.Contains(HitResult[i].GetActor()) && HitResult[i].GetActor()->ActorHasTag(TraceTag)) {

				AGodsArenaCharacter* temp_HitChar = Cast<AGodsArenaCharacter>(HitResult[i].GetActor());
				if (!temp_HitChar)
					continue;

				OnWeaponTraceHit.Broadcast(temp_HitChar, attribute);
				//If stun or kena parry
				if (ownerCharacter->GetCharacterState() == ECharacterState::Stun)
				{
					break;
				}

				IgnoredActorList.Add(HitResult[i].GetActor());
				TraceParams.AddIgnoredActor(HitResult[i].GetActor());
				float temp_damageToApply = FMath::RandRange(attribute.MinDamage, attribute.MaxDamage);
				UGodArenaFunctionLibrary::ApplyDamage(GetParentActor(), HitResult[i].GetActor(), temp_damageToApply, attribute.debuff);

			}

		}
	}

}

void ABP_WeaponBase::ExtendTraceLength(const FVector& ExtendedLength)
{
	EndWeaponTracePoint->SetRelativeLocation(OriginalEndPointVec + ExtendedLength);
	
}

void ABP_WeaponBase::EndTraceTick(FTimerHandle EndTimerHandle)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(EndTimerHandle)) {

		//
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString("Clear Success"));
		GetWorld()->GetTimerManager().ClearTimer(EndTimerHandle);
	}
	IgnoredActorList.Empty();
	OnWeaponEndTrace.Broadcast();

}

void ABP_WeaponBase::SetAttribute(const FCharacterCombatAnimation& attributeSet)
{
	attribute = attributeSet;
}

void ABP_WeaponBase::SetAttributeDebuff(const FAttackDebuff& attributeDebuffSet)
{
	attribute.debuff = attributeDebuffSet;
}

void ABP_WeaponBase::BeginTrail()
{
	if (ParticleSystemComponent && ParticleSystemComponent->Template/* && TrailActivated*/) {
		ParticleSystemComponent->BeginTrails(TrailInfo.BeginTrailSocketName, TrailInfo.EndTrailSocketName, TrailInfo.TrailWidthMode, TrailInfo.InWidth);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString("Traillllllllllllllll"));
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString("Begin Trail"));
	}
}

void ABP_WeaponBase::EndTrail()
{
	ParticleSystemComponent->EndTrails();
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString("End Trail"));
}
//not use anymore
void ABP_WeaponBase::DrawDebugTrace(FVector start, FVector end, EWeaponTraceType traceType)
{
	FVector center = (end + start) * 0.5f;
	FVector direction = end - start;
	direction.Normalize();

	FBox DebugBox(start, end);

	switch (traceType)
	{
	case EWeaponTraceType::SphereTrace:
		DrawDebugSphere(GetWorld(), start, SphereRadius, 32, FColor::Green, false, DebugDuration, 0, 10.0f);
		//DrawSphere
		break;
	case EWeaponTraceType::CapsuleTrace:
		DrawDebugCapsule(GetWorld(), start, FVector::Dist(start, end),
			CapsuleRadius, FQuat::FindBetweenNormals(FVector::ForwardVector, GetActorForwardVector()),
			FColor::Green, false, DebugDuration, 0, 10.0f);

		break;
	case EWeaponTraceType::BoxTrace:
		DrawDebugBox(GetWorld(), DebugBox.GetCenter(), BoxHalfSize,
			FQuat::FindBetweenNormals(GetActorForwardVector(), direction),
			FColor::Green, false, DebugDuration, 0, 10.0f);
		break;
	default:
		break;
	}
}




