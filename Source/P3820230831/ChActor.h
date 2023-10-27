// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChActor.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

// 본인이 원하는 함수(델리게이트)도 생성하여 사용 가능.
// DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(FCustomSignature, AActor, OnCustomOverlap, AActor*, OverlappedActor, AActor*, OtherActor);

UCLASS()
class P3820230831_API AChActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* Rocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UProjectileMovementComponent* Movement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UParticleSystem* Particle;

	// 델리게이트 함수는 항상 UFUNCTION(). 안붙이면 꺼짐
	UFUNCTION()
	void ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	// BlueprintImplementableEvent = 블루프린트에서 구현한 함수 호출.
	// 구현은 엔진이 자동으로 블루프린트와 연결해줌.
	// 껍데기만 만들고 내용은 블루프린트로 만들 수 있어 다른사람이 엔진에서만 작업해도 가능
	UFUNCTION(BlueprintImplementableEvent)
	void CallCPPToBP();

	// BlueprintNativeEvent = 블루프린트가 없다면 C++ 함수를 호출.
	// 같은 이름의 _Implementation 함수를 꼭 생성해줘야 함.
	UFUNCTION(BlueprintNativeEvent)
	void CallCPPToBPButCPP(AActor* Target);
	void CallCPPToBPButCPP_Implementation(AActor* Target);

	UFUNCTION()
	void HitMesh(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	
};


