// Fill out your copyright notice in the Description page of Project Settings.


#include "JuPawn.h"
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
AJuPawn::AJuPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(20, 20, 20));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT("/Script/Engine.StaticMesh'/Game/P38/Meshes/SM_P38_Body.SM_P38_Body'"));
	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}

	Propeller = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Propeller"));
	Propeller->SetupAttachment(Body);
	Propeller->SetRelativeLocation(FVector(20.0f, -21.0f, 1.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Propeller(TEXT("/Script/Engine.StaticMesh'/Game/P38/Meshes/SM_P38_Propeller.SM_P38_Propeller''"));
	if (SM_Propeller.Succeeded())
	{
		Propeller->SetStaticMesh(SM_Propeller.Object);
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

	static ConstructorHelpers::FClassFinder<AChActor> RocketClass(TEXT("/Script/Engine.Blueprint'/Game/Blueprint/BP_Rocket.BP_Rocket_C'"));

	if (!RocketClass.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed A"));
	}
	else
	{
		RocketTemplate = RocketClass.Class;
	}

	Smoke_First = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke_First"));
	Smoke_First->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset1(TEXT("'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Smoke/P_Smoke_A.P_Smoke_A'"));
	if (ParticleAsset1.Succeeded())
	{
		SmokeSystem_First = ParticleAsset1.Object;
	}

	Smoke_Second = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke_Second"));
	Smoke_Second->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset2(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Smoke/P_Smoke_C.P_Smoke_C'"));
	if (ParticleAsset2.Succeeded())
	{
		SmokeSystem_Second = ParticleAsset2.Object;
	}

	Smoke_Third = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke_Third"));
	Smoke_Third->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset3(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Smoke/P_Smoke_C.P_Smoke_C'"));
	if (ParticleAsset3.Succeeded())
	{
		SmokeSystem_Third = ParticleAsset3.Object;
	}

	DestroyPawn = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DestroyPawn"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> DestroyParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_A.P_Explosion_Big_A'"));
	if (DestroyParticleAsset.Succeeded())
	{
		DestroyPawnSystem = DestroyParticleAsset.Object;
	}

}

void AJuPawn::BeginPlay()
{
	Super::BeginPlay();

	ChActorComponent->AddSceneComponent(Propeller);
	ChActorComponent->RotationValue = FRotator(3200.0, 0, 0);

	SpawnSmoke();

	OnActorBeginOverlap.AddDynamic(this, &AJuPawn::ProcessBeginOverlap);

}

// Called every frame
void AJuPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(GetActorForwardVector(), BoostValue);

}

// Called to bind functionality to input
void AJuPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent"));
		EnhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Triggered, this, &AJuPawn::EnhancedBoost);
		EnhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Completed, this, &AJuPawn::EnhancedUnBoost);
		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &AJuPawn::EnhancedFire);
		EnhancedInputComponent->BindAction(IA_PitchAndRoll, ETriggerEvent::Triggered, this, &AJuPawn::EnhancedPitchAndRoll);
	}
}

void AJuPawn::EnhancedBoost(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("EnhancedBoost"));

	BoostValue = 1.0f;
}

void AJuPawn::EnhancedUnBoost(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("EnhancedUnBoost"));

	BoostValue = 0.5f;
}

void AJuPawn::EnhancedFire(const FInputActionValue& Value)
{
	if (RocketTemplate != nullptr)
	{
		AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(RocketTemplate,
			Arrow->GetComponentLocation(),
			Arrow->GetComponentRotation());
	}
}

void AJuPawn::EnhancedPitchAndRoll(const FInputActionValue& Value)
{
	FVector2D VectorValue = Value.Get<FVector2D>();

	if (!VectorValue.IsZero())
	{
		AddActorLocalRotation(FRotator(VectorValue.Y * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * 60.0f,
			0,
			VectorValue.X * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * 60.0f));
	}
}

float AJuPawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

void AJuPawn::SpawnSmoke()
{
	if (Smoke_First)
	{
		if (SmokeSystem_First)
		{
			Smoke_First->RegisterComponent();
			FVector DesiredLocation = FVector(20, 0, 0); // 원하는 위치를 지정
			Smoke_First->SetTemplate(SmokeSystem_First);
			Smoke_First->SetWorldLocation(GetActorLocation()); // 원하는 위치로 설정
		}
	}

	if (Smoke_Second)
	{
		if (SmokeSystem_Second)
		{
			Smoke_Second->RegisterComponent();
			FVector DesiredLocation = FVector(100, 100, 100); // 원하는 위치를 지정
			Smoke_Second->SetTemplate(SmokeSystem_Second);
			Smoke_Second->SetWorldLocation(GetActorLocation()); // 원하는 위치로 설정
		}
	}

	if (Smoke_Third)
	{
		if (SmokeSystem_Third)
		{
			Smoke_Third->RegisterComponent();
			FVector DesiredLocation = FVector(20, 0, 0); // 원하는 위치를 지정
			Smoke_Third->SetTemplate(SmokeSystem_Third);
			Smoke_Third->SetWorldLocation(GetActorLocation()); // 원하는 위치로 설정
		}
	}
}

void AJuPawn::ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("ProcessBeginOverlap"));

	this->Destroy();

	if (DestroyPawn)
	{
		if (DestroyPawnSystem)
		{
			DestroyPawn->RegisterComponent();
			DestroyPawn->SetTemplate(DestroyPawnSystem);
			DestroyPawn->SetWorldLocation(GetActorLocation()); // 원하는 위치로 설정
		}
	}
}

void AJuPawn::FallingPawn()
{

	this->SetActorRelativeRotation(FRotator(0.0, -45.0, 0.0));
}

