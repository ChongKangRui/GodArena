// Copyright Epic Games, Inc. All Rights Reserved.

#include "GodsArenaCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/ActionComponent.h"
#include "Components/AttributeComponent.h"
#include "Engine/Datatable.h"
#include "Weapon/BP_WeaponBase.h"
#include "Action/Action.h"
#include "AIController.h"

AGodsArenaCharacter::AGodsArenaCharacter() : state(ECharacterState::Idle)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	actionComp = CreateDefaultSubobject<UActionComponent>(TEXT("ActionComponent"));
	attributeComp = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	DataTableInitialization();
}

void AGodsArenaCharacter::BeginPlay()
{
	Super::BeginPlay();
	CharacterTypeInitialization();

	//Get Weapon Component
	UChildActorComponent* WeaponComp = FindComponentByClass<UChildActorComponent>();
	if (WeaponComp && WeaponComp->GetChildActor())
	{
		weapon = Cast<ABP_WeaponBase>(WeaponComp->GetChildActor());
		if (weapon) {
			weapon->OnWeaponTraceHit.AddDynamic(this, &AGodsArenaCharacter::BindOnWeaponTraceHit);
		}
	}
}

void AGodsArenaCharacter::DataTableInitialization()
{
	//Get datatable
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTablePath(TEXT("/Script/Engine.DataTable'/Game/Blueprint/Blueprint/DataAsset/DT_Character.DT_Character'"));
	DataTableAsset = DataTablePath.Object;
}

void AGodsArenaCharacter::CharacterTypeInitialization()
{
	if (!DataTableAsset)
	{
		return;
	}
	FString characterTypeString = UEnum::GetValueAsString(ownerType);
	characterTypeString.RemoveFromStart("ECharacterType::");

	characterInfo = DataTableAsset->FindRow<FCharacterInfo>(FName(characterTypeString), "");

	//Set action info from character info
	if (!characterInfo)
	{
		DebugPrint("Failed to get character info = " + characterTypeString);
		return;
	}

	ActionInfoList = characterInfo->ActionInfo;

	attributeComp->InitializeVariable(this);
	actionComp->ActionInitialization(this);
}

void AGodsArenaCharacter::SetCharacterState(ECharacterState State)
{
	if (state == ECharacterState::Death)
	{
		Death();
		OnStateChange.Broadcast(this->state);
		return;
	}
	if (this->state != State) {
		this->state = State;
		OnStateChange.Broadcast(this->state);
	}
}

void AGodsArenaCharacter::SetExecutedState(EExecutedState Executed_State)
{
	if (GetCharacterState() == ECharacterState::Executed)
	{
		executedState = Executed_State;
		OnExecuteStageChange.Broadcast(Executed_State);
		if (Executed_State == EExecutedState::NoExecuted) {
			SetCharacterState(ECharacterState::Idle);
		}
	}
}

void AGodsArenaCharacter::SetCharacterBuff(FAttackBuff buff)
{
	if(GetCharacterState() != ECharacterState::Death)
		CharacterBuff = buff;
}

void AGodsArenaCharacter::SetCharacterMovementState(ECharacterJogState MovementState) {
	characterMovementState = MovementState;

	if(MovementState != ECharacterJogState::NoMovement)
		GetCharacterMovement()->MaxWalkSpeed = *JogState.Find(MovementState);

}

void AGodsArenaCharacter::TerminateCharacter()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	actionComp->TerminateAllAction();
	GetWorld()->GetTimerManager().ClearAllTimersForObject(attributeComp);
	GetWorld()->GetTimerManager().ListTimers();
}

void AGodsArenaCharacter::Death()
{
	SetCharacterState(ECharacterState::Death);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	StopAnimMontage(GetCurrentMontage());
	TerminateCharacter();
}

void AGodsArenaCharacter::StunCharacter(const float& duration)
{
	if (state != ECharacterState::Stun)
	{
		auto const AIController = Cast<AAIController>(GetController());

		if (AIController)
			AIController->StopMovement();

		SetCharacterState(ECharacterState::Stun);
		actionComp->TerminateCurrentAction();
		StopAnimMontage(GetCurrentMontage());

		FTimerDelegate temp_delegate;
		FTimerHandle temp_timerHandle;

		temp_delegate.BindWeakLambda(this,[this]()
			{
				SetParryingState(EParryingState::None);
				if(GetCharacterState() != ECharacterState::Executed)
					SetCharacterState(ECharacterState::Idle);
			});
		GetWorld()->GetTimerManager().SetTimer(temp_timerHandle,
			temp_delegate, 0.1f, false, duration);
	}
}

void AGodsArenaCharacter::BindOnWeaponTraceHit(AGodsArenaCharacter* HitCharacter, FCharacterCombatAnimation currentEnemyAttribute)
{
	if (HitCharacter)
	{
		//Get direction from Hit Target to self
		FVector temp_Dir1 = GetActorLocation() - HitCharacter->GetActorLocation();

		//Get forward vector of the Hit target, use to determine the angle
		FVector temp_Dir2 = HitCharacter->GetActorForwardVector();

		temp_Dir1.Normalize();
		temp_Dir2.Normalize();

		//Get angle
		float temp_Radian = FMath::Acos(FVector::DotProduct(temp_Dir2, temp_Dir1));
		float temp_Angle = FMath::RadiansToDegrees(temp_Radian);

		if (!CharacterBuff.NoParry) {
			if (HitCharacter->GetCharacterState() == ECharacterState::Parrying && HitCharacter->GetParryingState() == EParryingState::Parrying
				&& parryingState == EParryingState::CanParry && currentEnemyAttribute.CanParry && temp_Angle <= 45.0f)
			{
				StunCharacter(GetCharacterInfo().StunDuration);

			}
		}
	}
}

