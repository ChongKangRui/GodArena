// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_SpawnBullet.h"
#include "../Weapon/Buff_Base.h"
#include "../GodsArenaCharacter.h"
#include "../Weapon/BP_WeaponBase.h"

void UAN_SpawnBullet::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (BulletBase && MeshComp) {
		FVector spawnLocation = MeshComp->GetOwner()->GetActorLocation() + (MeshComp->GetOwner()->GetActorForwardVector() * SpawnDistance);
		FRotator spawnRotation = MeshComp->GetOwner()->GetActorRotation();
		FTransform FinalTransform = { spawnRotation , spawnLocation , FVector(1.0f)};

		UWorld* const World = MeshComp->GetOwner()->GetWorld();
		if (World) {
			FActorSpawnParameters spawnparam;
			spawnparam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AActor* temp_SpawnActor = World->SpawnActor<AActor>(BulletBase, FinalTransform, spawnparam);
			AGodsArenaCharacter* temp_owner = Cast<AGodsArenaCharacter>(MeshComp->GetOwner());

			if (temp_owner->weapon && temp_owner)
				temp_SpawnActor->SetOwner(temp_owner->weapon);
			else
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString("Invalid owner or owner weapon"));
		}

	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString("Invalid meshcomp or bullet class"));
	}

}
