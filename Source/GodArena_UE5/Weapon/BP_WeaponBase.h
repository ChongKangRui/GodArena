// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../StructAndEnum.h"
#include "Kismet/KismetSystemLibrary.h"

#include "BP_WeaponBase.generated.h"

USTRUCT(BlueprintType)
struct FTrailInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BeginTrailSocketName = "None";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EndTrailSocketName = "None";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETrailWidthMode> TrailWidthMode = ETrailWidthMode::ETrailWidthMode_FromCentre;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InWidth = 1.0f;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHitActor, AGodsArenaCharacter*, HitActor, FCharacterCombatAnimation, currentAttribute);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponTrace);
UCLASS()
class GODARENA_UE5_API ABP_WeaponBase : public AActor
{
	GENERATED_BODY()

public:
	ABP_WeaponBase();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeginTrace();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndTrace();

	UFUNCTION(BlueprintCallable)
	void Trace(FVector BeginPoint, FVector EndPoint, EWeaponTraceType traceType);

	UFUNCTION(BlueprintCallable)
	FTimerHandle BeginTraceTick(const USceneComponent* BeginPoint, const USceneComponent* EndPoint, EWeaponTraceType traceType);
	
	UFUNCTION(BlueprintCallable)
	void EndTraceTick(FTimerHandle EndTimerHandle);

	UFUNCTION(BlueprintCallable)
	void ExtendTraceLength(const FVector& ExtendedLength);

	UFUNCTION(BlueprintCallable)
	void DrawDebugTrace(FVector start, FVector end, EWeaponTraceType traceType);

	UFUNCTION(BlueprintCallable)
	void SetAttribute(const FCharacterCombatAnimation& attributeSet);

	UFUNCTION(BlueprintPure)
	FCharacterCombatAnimation GetCurrentAttribute() const {
		return attribute;
	};

	UFUNCTION(BlueprintCallable)
	void SetAttributeDebuff(const FAttackDebuff& attributeDebuffSet);

	UFUNCTION(BlueprintCallable)
	void BeginTrail();

	UFUNCTION(BlueprintCallable)
	void EndTrail();

	UFUNCTION(BlueprintCallable)
	const FVector GetInitialEndPointLocation() const {
		return OriginalEndPointVec;
	}
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* BeginWeaponTracePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* EndWeaponTracePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponTraceType CurrentTraceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TraceTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool TrailActivated = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTrailInfo TrailInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystemComponent* ParticleSystemComponent;
	////General
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	float TraceTickSpeed = 0.2f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General")
	TArray<AActor*> IgnoredActorList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionTraceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	float DebugDuration = 10.0f;

	//Sphere
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SphereTrace")
	float SphereRadius = 35.0f;

	//Capsule
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CapsuleTrace")
	float CapsuleRadius = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CapsuleTrace")
	float CapsuleHeight = 10.0f;

	//Box
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoxTrace")
	FVector BoxHalfSize = FVector(1.0f);

	UPROPERTY(BlueprintAssignable)
	FOnWeaponTrace OnWeaponBeginTrace;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponTrace OnWeaponEndTrace;

	UPROPERTY(BlueprintAssignable)
	FOnHitActor OnWeaponTraceHit;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AGodsArenaCharacter* ownerCharacter;
	
	FTimerHandle currentTraceHandle;
	FCharacterCombatAnimation attribute;

private:
	FVector OriginalEndPointVec;
	
};
