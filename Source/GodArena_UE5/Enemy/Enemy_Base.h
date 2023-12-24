// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../GodsArenaCharacter.h"
#include "Enemy_Base.generated.h"

/**
 * 
 */
UCLASS()
class GODARENA_UE5_API AEnemy_Base : public AGodsArenaCharacter
{
	GENERATED_BODY()
public:
	AEnemy_Base();

	void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		 TObjectPtr<class UEnvQuery> DodgeEQS;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<AGodsArenaCharacter> Target;

	UFUNCTION(BlueprintCallable)
		void SetCharacterJogState(const ECharacterJogState& JogStateToChange);

	inline void SetAIControllerReference(class AAIController_Base* controllerReference)
	{
		AIController = controllerReference;
	};

	void SetCharacterState(ECharacterState stateToChange) override;
	void Death() override;

protected:
	 TObjectPtr < class AAIController_Base> AIController;
};
