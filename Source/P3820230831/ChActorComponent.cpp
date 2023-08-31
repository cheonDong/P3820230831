// Fill out your copyright notice in the Description page of Project Settings.


#include "ChActorComponent.h"

// Sets default values for this component's properties
UChActorComponent::UChActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UChActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UChActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*for (int32 index = 0; index < Components.Num(); ++index)
	{
		Components[index]->AddLocalRotation(FRotator(0, 0, 3200.0 * DeltaTime));
	}*/

	for (auto Propeller : Components)
	{
		Propeller->AddLocalRotation(FRotator(0, 0, 3200.0 * DeltaTime));
	}
}

void UChActorComponent::AddSceneComponent(USceneComponent* uSceneComponent)
{
	if (uSceneComponent)
	{
		Components.Add(uSceneComponent);
	}
}


