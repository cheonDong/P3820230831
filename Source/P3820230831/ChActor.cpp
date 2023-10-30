// Fill out your copyright notice in the Description page of Project Settings.


#include "ChActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AChActor::AChActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(20, 20, 20));
	Box->SetGenerateOverlapEvents(true);
	Box->SetSimulatePhysics(true);
	Box->SetNotifyRigidBodyCollision(true);
	Box->BodyInstance.SetCollisionProfileName("BlockAll");
	

	Rocket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket"));
	Rocket->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Rocket(TEXT("/Script/Engine.StaticMesh'/Game/Asset/P38/Meshes/SM_Rocket.SM_Rocket'"));
	if (SM_Rocket.Succeeded())
	{
		Rocket->SetStaticMesh(SM_Rocket.Object);
	}

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));

	Movement->ProjectileGravityScale = 0;
	Movement->MaxSpeed = 3000.0f;

	Particle = CreateDefaultSubobject<UParticleSystem>(TEXT("Particle"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_A.P_Explosion_Big_A"));
	if (ParticleAsset.Succeeded())
	{
		Particle = ParticleAsset.Object;
	}
}

// Called when the game starts or when spawned
void AChActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(3.0f);

	// 블루프린트의 이벤트 = 델리게이트. 이름을 따라가보면 원형이 있음. 원형에서 인자를 가져와서 필요 함수 생성.
	// 시작할 때 한번만 등록. 생성자가 아니라 beginplay에서. CDO에 없는 상태이기 때문에.
	// OnActorBeginOverlap.AddDynamic(this, &AChActor::ProcessBeginOverlap);
	Rocket->OnComponentHit;
	Box->SetCollisionProfileName("Box");
	Box->OnComponentHit.AddDynamic(this, &AChActor::OnHit);
	
	// 델리게이트 지우기.
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
	// 구현된 블루프린트가 없으면 동작 X
	// CallCPPToBP();

	// 구현된 블루프린트가 없다면 CPP이 가지고있는 함수 실행.
	CallCPPToBPButCPP(OtherActor);

}

void AChActor::CallCPPToBPButCPP_Implementation(AActor* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("CPP Excute"));

	if (Particle && Target)
	{
		// Spawn the particle system at the location of the overlapping actor
		UParticleSystemComponent* ParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, Target->GetActorLocation());

		// Activate the particle system
		if (ParticleComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("ActivateSystem"));
		}
	}
}

void AChActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FVector HitLocation = Hit.ImpactPoint;
	UE_LOG(LogTemp, Warning, TEXT("OnHit"));

	this->Destroy();

	if (Particle)
	{
		UParticleSystemComponent* ParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, HitLocation);

		if (ParticleComponent)
		{
			ParticleComponent->SetWorldScale3D(FVector(1.0f)); // 크기 설정
		}
	}

	UGameplayStatics::ApplyDamage(OtherActor, 20.0f, GetWorld()->GetFirstPlayerController(), nullptr, UDamageType::StaticClass());
	UE_LOG(LogClass, Warning, TEXT("ApplyDamage"));
}

