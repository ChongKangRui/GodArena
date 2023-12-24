// Fill out your copyright notice in the Description page of Project Settings.


#include "Buff_Base.h"
#include "Particles/ParticleSystemComponent.h"
#include "../GodsArenaCharacter.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ABuff_Base::ABuff_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(GetRootComponent());

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
	NiagaraComponent->SetupAttachment(mesh);
	CascadeComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CascadeEffect"));
	CascadeComponent->SetupAttachment(mesh);

}

// Called when the game starts or when spawned
void ABuff_Base::BeginPlay()
{
	Super::BeginPlay();
	AGodsArenaCharacter* owner = Cast<AGodsArenaCharacter>( GetInstigator());
	OnBeginBuff.Broadcast();

	if (owner) {
		OriginalBuff = owner->GetCharacterBuff();
		owner->SetCharacterBuff(BuffToApply);

		if (BuffDuration > 0) {
			FTimerHandle temp_Handle;
			FTimerDelegate temp_Delegate;
			//Bind event by lamdba expression
			temp_Delegate.BindWeakLambda(this,[this, owner]()
				{
					owner->SetCharacterBuff(OriginalBuff);
					OnEndBuff.Broadcast();
					Destroy();
				});
			GetWorld()->GetTimerManager().SetTimer(temp_Handle,
				temp_Delegate, 0.1f, false, BuffDuration);
		}

	}
}

// Called every frame
void ABuff_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

