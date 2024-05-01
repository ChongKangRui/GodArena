#pragma once
#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "StructAndEnum.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Idle,
	Attacking,
	Dodging,
	Moving,
	Defending,
	Parrying,
	Executing,
	SkillExecuting,
	//Refer to laying on the ground
	KnockOut,
	GetHit,
	//Standing without doing anything
	Stun,
	//Getting stab or executed
	Executed,
	Death
};

UENUM(BlueprintType)
enum class ECharacterJogState : uint8
{
	Run,
	Jog,
	Jump,
	Dash,
	NoMovement

};

UENUM(BlueprintType)
enum class ELockTargetState : uint8
{
	Targeted,
	Untargeted

};

UENUM(BlueprintType)
enum class EWeaponDrawnState : uint8
{
	Drawn,
	Undrawn

};

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	Player_Assassin,
	Player_knight,
	Player_Wizard,
	Player_Kwang,
	Enemy_SamuraiBoss,
	Enemy_WingOfSonBoss,
	Enemy_TutorialDummy,
	Enemy_ExpertKnight

};

UENUM(BlueprintType)
enum class ECombatType : uint8
{
	None,
	NormalAttack,
	HeavyAttack,
	Defense,
	Dodge,
	Parry,
	ExecuteAttack,
	SpecialAbility_01,
	SPecialAbility_02,
	SpecialAbility_03,
	SPecialAbility_04,
	SpecialAbility_05,
	SPecialAbility_06


};

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	NoCombat,
	StartCombo,
	FinishedCombo,
	NextCombo

};

UENUM(BlueprintType)
enum class EExecutedState : uint8
{
	NoExecuted,
	ExecuteReaction,
	Executed,
	StandingUp

};

UENUM(BlueprintType)
enum class EWeaponTraceType : uint8
{
	SphereTrace,
	CapsuleTrace,
	BoxTrace

};

UENUM(BlueprintType)
enum class ELaunchDirection : uint8
{
	EnemyDirection,
	Forward,
	Left,
	Back,
	Right

};

UENUM(BlueprintType)
enum class EParryingState : uint8
{
	None,
	CanParry,
	Parrying

};

UENUM(BlueprintType)
enum class ETurnInPlace : uint8
{
	NoTurn,
	TurnLeft,
	TurnRight

};

USTRUCT(BlueprintType)
struct FAttackDebuff
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool stun = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bleeding = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Executed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Impulse = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaminaCost = 10.0f;

};

USTRUCT(BlueprintType)
struct FAttackBuff
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeedRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDamage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageReduceRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaminaCostRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageStaminaCostRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool NoParry = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool NoHitReaction = false;


};

USTRUCT(BlueprintType)
struct FCharacterCombatAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDamage = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cost = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Attack Distance (AIOnly)")
	float AttackDistance = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanParry = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "debuff apply to enemy"))
	FAttackDebuff debuff;

};

USTRUCT(BlueprintType)
struct FCharacterCombat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCharacterCombatAnimation> CombatAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "use to set the state"))
	ECharacterState state;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "State to abandon action"))
	TArray<ECharacterState> abandonActionState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class USAction> action;

	FCharacterCombat()
	{
		abandonActionState.Add(ECharacterState::KnockOut);
		abandonActionState.Add(ECharacterState::Stun);
		abandonActionState.Add(ECharacterState::Executed);
		abandonActionState.Add(ECharacterState::Executing);
	}
};

USTRUCT(BlueprintType)
struct FCharacterInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Health = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Stamina = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "For Enemy, stamina will deduct. For player, stamina will increase"))
	float StaminaModificationDelay = 1.0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ToolTip = "Rate for stamina to increase or decrease"))
	float StaminaRate = 0.01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "HitDuration"))
	float HitDuration = 0.7f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "HitReaction"))
	TArray<UAnimMontage*> HitReaction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "HitReaction"))
	TArray<class UNiagaraSystem*> DamagedBlood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "ParryDuration"))
	float StunDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ECombatType, FCharacterCombat> ActionInfo;

	FCharacterInfo() : Health(100), Stamina(100)
	{

	}
};

