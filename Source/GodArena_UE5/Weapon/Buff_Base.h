// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../StructAndEnum.h"
#include "Buff_Base.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuff);
UCLASS()
class GODARENA_UE5_API ABuff_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuff_Base();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraComponent* NiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystemComponent* CascadeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAttackBuff BuffToApply;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BuffDuration;

	UPROPERTY(BlueprintAssignable)
	FOnBuff OnBeginBuff;

	UPROPERTY(BlueprintAssignable, meta= (ToolTip = "Only called when this actor get destroyed"))
	FOnBuff OnEndBuff;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FAttackBuff OriginalBuff;

};
