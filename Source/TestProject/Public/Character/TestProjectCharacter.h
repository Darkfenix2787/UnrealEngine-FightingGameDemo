// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestProjectCharacter.generated.h"

class UTP_HealthComponent;
class UAnimMontage;
class UAnimInstance;
class USphereComponent;
class ATestProjectGameMode;

UCLASS(config=Game)
class ATestProjectCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/// <summary>
	/// Health component for the character
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UTP_HealthComponent* HealthComponent;

	/// <summary>
	/// Sphere component for the character right hand, this component will detect the enemy character for damage
	/// </summary>
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
		USphereComponent* RightHandDetectorComponent;

	/// <summary>
	/// Sphere component for the character left hand, this component will detect the enemy character for damage
	/// </summary>
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
		USphereComponent* LeftHandDetectorComponent;

public:
	ATestProjectCharacter();

public:

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	
	/// <summary>
	/// Indicates id the character is doing a melee attack
	/// </summary>
	bool bDoingMelee;

	/// <summary>
	/// Indicates if the character can do the second hit
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
		bool bSecondHitEnabled;

	/// <summary>
	/// Amount of damage dealt by the character
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
		float MeleeDamage;

	/// <summary>
	/// Skeletal Socket name where the RightHandDetectorComponent will be attached
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
		FName RightMeleeSocketName;

	/// <summary>
	/// Skeletal Socket name where the LeftHandDetectorComponent will be attached
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
		FName LeftMeleeSocketName;

protected:

	/// <summary>
	/// Animation Montage for the first hit
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* LeftHitMontage;

	/// <summary>
	/// Animation Montage for the second hit
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* RightHitMontage;	

	/// <summary>
	/// GameMode Reference
	/// </summary>
	UPROPERTY(BlueprintReadWrite)
		ATestProjectGameMode* GameModeReferences;

	UAnimInstance* MyAnimInstance;

protected:
	
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);


	/// <summary>
	/// Function that contains the Melee behaviour
	/// </summary>
	UFUNCTION(BlueprintCallable)
		void StartMelee();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;

	/// <summary>
	/// Overlapp function that makes the damage to a character
	/// </summary>
	UFUNCTION()
		void MakeMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// <summary>
	/// OnHealthChanged function that contains the GameOver call
	/// </summary>
	UFUNCTION()
		void OnHealthChanged(UTP_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

public:

	void SetRightHandDetectorCollision(ECollisionEnabled::Type NewCollisionState);

	void SetLeftHandDetectorCollision(ECollisionEnabled::Type NewCollisionState);

	void SetSecondHitEnabled(bool NewState);

	void SetDoingMelee(bool NewState);
	

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

