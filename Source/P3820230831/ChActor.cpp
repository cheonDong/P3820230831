// Fill out your copyright notice in the Description page of Project Settings.


#include "ChActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AChActor::AChActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(20, 20, 20));
	Box->SetGenerateOverlapEvents(true);

	Rocket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket"));
	Rocket->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Rocket(TEXT("/Script/Engine.StaticMesh'/Game/P38/Meshes/SM_Rocket.SM_Rocket'"));
	if (SM_Rocket.Succeeded())
	{
		Rocket->SetStaticMesh(SM_Rocket.Object);
	}

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));

	Movement->ProjectileGravityScale = 0;
	Movement->MaxSpeed = 2000.0f;
}

// Called when the game starts or when spawned
void AChActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(3.0f);

	// �������Ʈ�� �̺�Ʈ = ��������Ʈ. �̸��� ���󰡺��� ������ ����. �������� ���ڸ� �����ͼ� �ʿ� �Լ� ����.
	// ������ �� �ѹ��� ���. �����ڰ� �ƴ϶� beginplay����. CDO�� ���� �����̱� ������.
	OnActorBeginOverlap.AddDynamic(this, &AChActor::ProcessBeginOverlap);

	// ��������Ʈ �����.
	// OnActorBeginOverlap.RemoveDynamic(this, &AChActor::ProcessBeginOverlap);
	// OnActorBeginOverlap.RemoveAll(this);

}

// Called every frame
void AChActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChActor::ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap begin"));
	// ������ �������Ʈ�� ������ ���� X
	// CallCPPToBP();

	// ������ �������Ʈ�� ���ٸ� CPP�� �������ִ� �Լ� ����.
	CallCPPToBPButCPP();
}

void AChActor::CallCPPToBPButCPP_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("CPP Excute"));
}

