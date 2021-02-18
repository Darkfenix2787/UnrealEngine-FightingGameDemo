// This component will manage the healt of an Actor (For this demo it is attached to the Enemy and Player Actors)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TP_HealthComponent.generated.h"

//I create a Custom Six Params Delegate to manage the Health Change with the OnTakeAnyDamage params
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealtChangeSignature, UTP_HealthComponent*, HealthComponent, AActor*, DamagedActor, float, Damage, const UDamageType*, DamageType, AController*, InstigatedBy, AActor*, DamageCauser);


UCLASS( ClassGroup=(TestProject), meta=(BlueprintSpawnableComponent) )
class TESTPROJECT_API UTP_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Health Component")
		bool bDead;

	UPROPERTY(BlueprintReadWrite, Category = "Health Component")
		float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Component", meta = (ClampMIn = 0.0, UIMin = 0.0))
		float MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Health Component")
		AActor* MyOwner;

public:

	//a Delegate Signature
	UPROPERTY(BlueprintAssignable)
		FOnHealtChangeSignature OnHealthChangeDelegate;

public:	
	// Sets default values for this component's properties
	UTP_HealthComponent();
	
	//Indicates if the character health is 0
	UFUNCTION(BlueprintCallable)
		bool IsDead() const { return bDead; };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// A function that evaluate the character's health
	UFUNCTION()
		void TakingDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

		
};
