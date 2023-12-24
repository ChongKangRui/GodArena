// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
//#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Animation/AnimMontage.h"
#include "../Components/ActionComponent.h"
#include "../Components/AttributeComponent.h"
#include "../Components/TargetingComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "../Weapon/BP_WeaponBase.h"




APlayerCharacter::APlayerCharacter() {

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, FName("SpringEndpoint")); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	TargetComponent = CreateDefaultSubobject<UTargetingComponent>(TEXT("Targeting Comp"));

	//weapon = Cast<ABP_WeaponBase>(FindComponentByClass<UChildActorComponent>()->GetChildActor());
	Tags.Add(FName("Player"));
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	// Set up action bindings


	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//EnhancedInputComponent.get
		//Jumping

		//EnhancedInputComponent->BindAction(Dodge, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

	}

}


void APlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}



}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GetWorld()->GetTimerManager().ListTimers();
}


void APlayerCharacter::SetCharacterState(ECharacterState stateToChange)
{
	if (state == ECharacterState::Death)
		return;

	Super::SetCharacterState(stateToChange);

	/*TargetComponent->EndTarget();
	TargetComponent->BeginTarget();*/
}

FVector APlayerCharacter::GetPlayerLastInputMovementVector()
{
	return MovementInputVector.GetSafeNormal();
}

FVector2D APlayerCharacter::GetPlayerLastInputMovementValue()
{
	return MovementInputValue;
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MV = Value.Get<FVector2D>();
	MovementInputValue = MV;
	
	if (Controller != nullptr)
	{

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		MovementInputVector = (ForwardDirection * MovementInputValue.Y) + (RightDirection * MovementInputValue.X);

		if (MovementAllowState.Contains(GetCharacterState()) && characterMovementState != ECharacterJogState::NoMovement)
		{
			// add movement 
			AddMovementInput(ForwardDirection, MovementInputValue.Y);
			AddMovementInput(RightDirection, MovementInputValue.X);

			if (state != ECharacterState::Defending)
			{
				if (GetVelocity().Length() == 0)
				{
					SetCharacterState(ECharacterState::Idle);
				}
				else
				{
					SetCharacterState(ECharacterState::Moving);
				}
			}
		}
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	//DebugPrint(LookAxisVector.ToString());

	if (Controller != nullptr)
	{
		if (TargetComponent && TargetComponent->CheckIsTargeted())
		{
			return;
		}
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(-LookAxisVector.Y);
	}
}

void APlayerCharacter::Death()
{
	TargetComponent->EndTarget();
	Super::Death();
	APlayerController* temp_PC = Cast<APlayerController>(Controller);
	
}






