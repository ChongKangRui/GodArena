// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "NiagaraComponent.h"
#include "BTTask_DashWithParticle.generated.h"

/**
 * 
 */



UCLASS()
class GODARENA_UE5_API UBTTask_DashWithParticle : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* DashEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Spawn at the begin and the end of dash"))
	UNiagaraSystem* BlinkEffect;
private:
	UNiagaraComponent* BlinkComp;
	UNiagaraComponent* DaskComp;

	class AGodsArenaCharacter* owner;
	UBehaviorTreeComponent* BTComp;

	FTimerHandle handle;
	void EndTask();
};
