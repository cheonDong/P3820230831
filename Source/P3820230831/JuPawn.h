// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "JuPawn.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UArrowComponent;
class UFloatingPawnMovement;
class UChActorComponent;

UCLASS()
class P3820230831_API AJuPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AJuPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* Propeller;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UArrowComponent* Arrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UFloatingPawnMovement* Movement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UChActorComponent* ChActorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UParticleSystemComponent* Smoke_First;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Data")
	class UParticleSystem* SmokeSystem_First;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UParticleSystemComponent* Smoke_Second;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Data")
	class UParticleSystem* SmokeSystem_Second;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UParticleSystemComponent* Smoke_Third;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Data")
	class UParticleSystem* SmokeSystem_Third;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UParticleSystemComponent* DestroyPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UParticleSystem* DestroyPawnSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	float PropellerSpeed = 3600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float BoostValue = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float FallSpeed = 0.0f;

	void EnhancedBoost(const FInputActionValue& Value);

	void EnhancedUnBoost(const FInputActionValue& Value);

	void EnhancedFire(const FInputActionValue& Value);

	void EnhancedPitchAndRoll(const FInputActionValue& Value);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* IA_Boost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* IA_Fire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* IA_PitchAndRoll;

	// 클래스 이름 기억. 클래스 원본의 이름(CDO의 Default 객체)을 가리키기 위해 사용. 그래서 포인터가 아님.
	// C++에서 블루프린트 class를 가져와서 사용가능
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<class AChActor> RocketTemplate;

public:
	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void SpawnSmoke();

	UFUNCTION()
	void ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void FallingPawn();
};
