// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ChActor.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);
	Box->SetBoxExtent(FVector(100, 100, 100));
	Box->SetSimulatePhysics(true);
	Box->SetNotifyRigidBodyCollision(true);
	Box->BodyInstance.SetCollisionProfileName("BlockAll");
	

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Box);
}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	
	Box->OnComponentHit.AddDynamic(this, &ATestActor::OnHit);
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AChActor* target = Cast<AChActor>(OtherActor);
	
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && (OtherActor == target))
	{
		UE_LOG(LogTemp, Warning, TEXT("Successed OnHit"));
	}
}

