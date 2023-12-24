// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetRandomBehavior.h"
#include "../../GodsArenaCharacter.h"
#include "AIController.h"


UBTTask_SetRandomBehavior::UBTTask_SetRandomBehavior()
{
    NodeName = TEXT("Set Random AI Behavior");
}

EBTNodeResult::Type UBTTask_SetRandomBehavior::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
    if (AI_Behavior.Num() <= 0)
        return EBTNodeResult::Failed;

    FRandomStream RandomStream;
    RandomStream.GenerateNewSeed();

    int32 temp_RandomIndex = RandomStream.RandRange(0, AI_Behavior.Num() - 1);

    if (AI_Behavior.IsValidIndex(temp_RandomIndex)) {

        auto const aiController = owner_comp.GetAIOwner();
        auto const enemy = Cast<AGodsArenaCharacter>(aiController->GetPawn());


        enemy->SetCharacterState(AI_Behavior[temp_RandomIndex]);

        return EBTNodeResult::Succeeded;
    }
    

    return EBTNodeResult::Failed;
}
