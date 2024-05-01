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
	Super::OnNodeActivation(SearchD);
}
