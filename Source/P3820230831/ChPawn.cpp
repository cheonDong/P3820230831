// Fill out your copyright notice in the Description page of Project Settings.


#include "ChPawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Particles/ParticleSystemComponent.h"
#include "ChActorComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ChActor.h"
#include "ChPlayerController.h"
#include "ChStatComponent.h"

// Sets default values
AChPawn::AChPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(20, 20, 20));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT("/Script/Engine.StaticMesh'/Game/Asset/P38/Meshes/SM_P38_Body.SM_P38_Body'"));
	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}

	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);
	Left->SetRelativeLocation(FVector(37.0f, -21.0f, 1.0f));

	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);
	Right->SetRelativeLocation(FVector(37.0f, 21.0f, 1.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Propeller(TEXT("/Script/Engine.StaticMesh'/Game/Asset/P38/Meshes/SM_P38_Propeller.SM_P38_Propeller'"));
	if (SM_Propeller.Succeeded())
	{
		Left->SetStaticMesh(SM_Propeller.Object);
		Right->SetStaticMesh(SM_Propeller.Object);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetRelativeLocation(FVector(100.0f, 0, 0));

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	ChActorComponent = CreateDefaultSubobject<UChActorComponent>(TEXT("ChActorComponent"));

	// 블루프린트에 생성한 BPClass를 가져오는 방법.
	static ConstructorHelpers::FClassFinder<AChActor> RocketClass(TEXT("/Script/Engine.Blueprint'/Game/Blueprint/BP_Rocket.BP_Rocket_C'"));

	if (!RocketClass.Succeeded())
	{
		// 가져오는 액터의 래퍼런스 주소의 끝에 _C를 붙여줘야 성공.
		UE_LOG(LogTemp, Warning, TEXT("Failed"));
	}
	else
	{
		RocketTemplate = RocketClass.Class;
	}

	Smoke = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke"));
	Smoke->SetupAttachment(RootComponent);


	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Smoke/P_Smoke_C.P_Smoke_C'"));
	if (ParticleAsset.Succeeded())
	{
		ParticleSystem = ParticleAsset.Object;
	}
}

// Called when the game starts or when spawned
void AChPawn::BeginPlay()
{
	Super::BeginPlay();
	
	ChActorComponent->AddSceneComponent(Right);
	ChActorComponent->AddSceneComponent(Left);
	ChActorComponent->RotationValue = FRotator(0, 0, 3200.0);

	SpawnSmoke();
}

// Called every frame
void AChPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(GetActorForwardVector(), BoostValue);

	
}

// Called to bind functionality to input
void AChPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent"));
		EnhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Triggered, this, &AChPawn::EnhancedBoost);
		EnhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Completed, this, &AChPawn::EnhancedUnBoost);
		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &AChPawn::EnhancedFire);
		EnhancedInputComponent->BindAction(IA_PitchAndRoll, ETriggerEvent::Triggered, this, &AChPawn::EnhancedPitchAndRoll);
	}

}

void AChPawn::EnhancedBoost(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("EnhancedBoost"));

	BoostValue = 1.0f;
}

void AChPawn::EnhancedUnBoost(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("EnhancedUnBoost"));

	BoostValue = 0.5f;
}

void AChPawn::EnhancedFire(const FInputActionValue& Value)
{
	if (RocketTemplate != nullptr)
	{
		AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(RocketTemplate,
			Arrow->GetComponentLocation(),
			Arrow->GetComponentRotation());
	}
}

void AChPawn::EnhancedPitchAndRoll(const FInputActionValue& Value)
{
	FVector2D VectorValue = Value.Get<FVector2D>();

	if (!VectorValue.IsZero())
	{
		AddActorLocalRotation(FRotator(VectorValue.Y * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * 60.0f,
			0,
			VectorValue.X * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * 60.0f));
	}
}

float AChPawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	AChPlayerController* PC = Cast<AChPlayerController>(this->GetController());

	if (PC)
	{
		UChStatComponent* StatManager = Cast<UChStatComponent>(PC->GetComponentByClass<UChStatComponent>());

		if (StatManager)
		{
			StatManager->UpdateHp(DamageAmount);
		}
	}

	return DamageAmount;
}

void AChPawn::SpawnSmoke()
{
	if (Smoke)
	{
		if (ParticleSystem)
		{
			FVector DesiredLocation = FVector(100, 20, 20); // 원하는 위치를 지정
			Smoke->SetTemplate(ParticleSystem);
			Smoke->SetWorldLocation(DesiredLocation); // 원하는 위치로 설정
		}
	}
}

