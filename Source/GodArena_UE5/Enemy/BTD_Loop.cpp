// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_Loop.h"

UBTD_Loop::UBTD_Loop()
{
	NodeName = "RandomLoop";
	
}

void UBTD_Loop::OnNodeActivation(FBehaviorTreeSearchData& SearchD)
{
	int loopCount = FMath::RandRange(MinLoop, MaxLoop);
	
	NumLoops = loopCount;

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("RandomNum, %d"), NumLoops));

	Super::OnNodeActivation(SearchD);
}
