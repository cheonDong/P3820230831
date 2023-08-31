// Fill out your copyright notice in the Description page of Project Settings.


#include "ChActor.h"

// Sets default values
AChActor::AChActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

