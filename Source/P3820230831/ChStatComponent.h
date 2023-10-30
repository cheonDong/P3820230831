// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChStatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDele_UpdateHp_TwoParams, float, CurHp, float, MaxHp);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P3820230831_API UChStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION()
	void OnRep_CurHp();

	UFUNCTION()
	void UpdateHp(float Damage);

public:
	UPROPERTY(ReplicatedUsing = OnRep_CurHp)
	float MaxHp = 100.0f;

	UPROPERTY(ReplicatedUsing = OnRep_CurHp)
	float CurHp = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Coin;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDele_UpdateHp_TwoParams Fuc_Dele_UpdateHp;

};
