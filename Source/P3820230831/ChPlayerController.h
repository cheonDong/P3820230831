// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class P3820230831_API AChPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AChPlayerController();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSecond) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UChStatComponent* StatManager;

	bool bAlive;
};
