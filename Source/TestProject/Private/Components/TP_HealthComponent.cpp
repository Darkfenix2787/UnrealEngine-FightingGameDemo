
#include "Components/TP_HealthComponent.h"

//================================================================================================================================================//
// Sets default values for this component's properties
UTP_HealthComponent::UTP_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.0f;
	bDead = false;
}

//================================================================================================================================================//
// Called when the game starts
void UTP_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	MyOwner = GetOwner();
	if (IsValid(MyOwner))
	{
		//Use the Actors OnTakeAnyDamage
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UTP_HealthComponent::TakingDamage);
	}
}

//================================================================================================================================================//
/// <summary>
/// Evaluate the Character Health and Broadcast the custom delegate.
/// </summary>
/// <param name="DamagedActor">Actor who takes the damage</param>
/// <param name="Damage">Quantity of damage taken</param>
/// <param name="DamageType">Type of the damage taken</param>
/// <param name="InstigatedBy">Instigate controller</param>
/// <param name="DamageCauser">Actor who cause the damage</param>
void UTP_HealthComponent::TakingDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || bDead)
	{
		return;
	}	

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);

	if (CurrentHealth == 0.0f)
	{
		bDead = true;
	}

	OnHealthChangeDelegate.Broadcast(this, DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);	
}
