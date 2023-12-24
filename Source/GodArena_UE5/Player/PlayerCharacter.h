// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../GodsArenaCharacter.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UTargetingComponent;

UCLASS()
class GODARENA_UE5_API APlayerCharacter : public AGodsArenaCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTargetingComponent* TargetComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Dodge;
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TArray<ECharacterState> MovementAllowState = {ECharacterState::Idle, ECharacterState::Moving, ECharacterState::Defending};

public:
	void SetCharacterState(ECharacterState stateToChange) override;

	UFUNCTION(BlueprintPure)
	FVector GetPlayerLastInputMovementVector();

	UFUNCTION(BlueprintPure)
	FVector2D GetPlayerLastInputMovementValue();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Death() override;

	// APawn Input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Begin Play
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime);

private:
	float InputValueX = 0;
	FVector MovementInputVector;
	FVector2D MovementInputValue;
};
