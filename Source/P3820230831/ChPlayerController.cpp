// Fill out your copyright notice in the Description page of Project Settings.


#include "ChPlayerController.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void AChPlayerController::BeginPlay()
{
	Super::BeginPlay();

	
	UEnhancedInputLocalPlayerSubsystem* LocalPlayerSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (LocalPlayerSystem && DefaultMappingContext)
	{
		LocalPlayerSystem->AddMappingContext(DefaultMappingContext, 0);
	}

}

void AChPlayerController::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);
}
