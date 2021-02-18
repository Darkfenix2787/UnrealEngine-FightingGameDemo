// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/TestProjectCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TP_HealthComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/SphereComponent.h"
#include "TestProject/TestProject.h"
#include "Kismet/GameplayStatics.h"
#include "Core/TestProjectGameMode.h"


//////////////////////////////////////////////////////////////////////////
// ATestProjectCharacter

ATestProjectCharacter::ATestProjectCharacter()
{		
	RightMeleeSocketName = "SCK_RightMelee";
	LeftMeleeSocketName = "SCK_LeftMelee";
	MeleeDamage = 15.0f;
	bSecondHitEnabled = false;
	bDoingMelee = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Create the Health Component
	HealthComponent = CreateDefaultSubobject<UTP_HealthComponent>(TEXT("HealthComponent"));

	RightHandDetectorComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RightHandDetectorComponent"));
	RightHandDetectorComponent->SetupAttachment(GetMesh(), RightMeleeSocketName);
	RightHandDetectorComponent->SetSphereRadius(15.0f);
	RightHandDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	RightHandDetectorComponent->SetCollisionResponseToChannel(COLLISION_ENEMY, ECR_Overlap);
	RightHandDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	LeftHandDetectorComponent = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandDetectorComponent"));
	LeftHandDetectorComponent->SetupAttachment(GetMesh(), LeftMeleeSocketName);
	LeftHandDetectorComponent->SetSphereRadius(15.0f);
	LeftHandDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	LeftHandDetectorComponent->SetCollisionResponseToChannel(COLLISION_ENEMY, ECR_Overlap);
	LeftHandDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATestProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Hit", IE_Pressed, this, &ATestProjectCharacter::StartMelee);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATestProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATestProjectCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATestProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATestProjectCharacter::LookUpAtRate);
}

//================================================================================================================================================//
void ATestProjectCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Get the player's AnimInstance
	if (IsValid(GetMesh()))
	{
		MyAnimInstance = GetMesh()->GetAnimInstance();
	}

	RightHandDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &ATestProjectCharacter::MakeMeleeDamage);
	LeftHandDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &ATestProjectCharacter::MakeMeleeDamage);
	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &ATestProjectCharacter::OnHealthChanged);	
	GameModeReferences = Cast<ATestProjectGameMode>(GetWorld()->GetAuthGameMode());
}


//================================================================================================================================================//
/// <summary>
/// Overlap fuction for the hands sphere components
/// </summary>
void ATestProjectCharacter::MakeMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, MeleeDamage, SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);
	}
}

//================================================================================================================================================//
/// <summary>
/// OnHealthChanged function that contains the GameOver call
/// </summary>
void ATestProjectCharacter::OnHealthChanged(UTP_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{	
	if (HealthComponent->IsDead())
	{
		ATestProjectCharacter* CauserCharacter = Cast<ATestProjectCharacter>(DamageCauser);
		if (IsValid(CauserCharacter) && IsValid(GameModeReferences))
		{
			GameModeReferences->GameOver(this, CauserCharacter);
		}		
	}
}

//================================================================================================================================================//
void ATestProjectCharacter::SetRightHandDetectorCollision(ECollisionEnabled::Type NewCollisionState)
{
	RightHandDetectorComponent->SetCollisionEnabled(NewCollisionState);
}

//================================================================================================================================================//
void ATestProjectCharacter::SetLeftHandDetectorCollision(ECollisionEnabled::Type NewCollisionState)
{
	LeftHandDetectorComponent->SetCollisionEnabled(NewCollisionState);
}

//================================================================================================================================================//
void ATestProjectCharacter::SetSecondHitEnabled(bool NewState)
{
	bSecondHitEnabled = NewState;
}

void ATestProjectCharacter::SetDoingMelee(bool NewState)
{
	bDoingMelee = NewState;
}

//================================================================================================================================================//
void ATestProjectCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

//================================================================================================================================================//
void ATestProjectCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

//================================================================================================================================================//
// Melee behaviour whit the UAnimInstance I Play the montages. It depends on the boolean that indicates if the second hit is enabled
void ATestProjectCharacter::StartMelee()
{
	if (IsValid(MyAnimInstance) && IsValid(LeftHitMontage) && IsValid(RightHitMontage))
	{
		if (bSecondHitEnabled)
		{
			if (bDoingMelee)
			{
				MyAnimInstance->Montage_Play(RightHitMontage);
			}
		}

		if (!bDoingMelee)
		{
			MyAnimInstance->Montage_Play(LeftHitMontage);
		}		
	}	
}

//================================================================================================================================================//
void ATestProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

//================================================================================================================================================//
void ATestProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

//================================================================================================================================================//
void ATestProjectCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

//================================================================================================================================================//
void ATestProjectCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
