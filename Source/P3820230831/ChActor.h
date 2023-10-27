// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChActor.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

// ������ ���ϴ� �Լ�(��������Ʈ)�� �����Ͽ� ��� ����.
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

	// ��������Ʈ �Լ��� �׻� UFUNCTION(). �Ⱥ��̸� ����
	UFUNCTION()
	void ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	// BlueprintImplementableEvent = �������Ʈ���� ������ �Լ� ȣ��.
	// ������ ������ �ڵ����� �������Ʈ�� ��������.
	// �����⸸ ����� ������ �������Ʈ�� ���� �� �־� �ٸ������ ���������� �۾��ص� ����
	UFUNCTION(BlueprintImplementableEvent)
	void CallCPPToBP();

	// BlueprintNativeEvent = �������Ʈ�� ���ٸ� C++ �Լ��� ȣ��.
	// ���� �̸��� _Implementation �Լ��� �� ��������� ��.
	UFUNCTION(BlueprintNativeEvent)
	void CallCPPToBPButCPP(AActor* Target);
	void CallCPPToBPButCPP_Implementation(AActor* Target);

	UFUNCTION()
	void HitMesh(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	
};


