// Fill out your copyright notice in the Description page of Project Settings.


#include "ChStatComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UChStatComponent::UChStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	// ...
}


// Called when the game starts
void UChStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UChStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UChStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(UChStatComponent, CurHp);
	DOREPLIFETIME(UChStatComponent, MaxHp);
}

void UChStatComponent::OnRep_CurHp()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_CurHp"));
	if (Fuc_Dele_UpdateHp.IsBound())
		Fuc_Dele_UpdateHp.Broadcast(CurHp, MaxHp);
}

void UChStatComponent::UpdateHp(float Damage)
{
	CurHp -= Damage;

	OnRep_CurHp();
}
