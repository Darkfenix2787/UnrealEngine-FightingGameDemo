
#include "Animations/Character/TP_AN_DoingMeleeFinish.h"
#include "Character/TestProjectCharacter.h"

/// <summary>
/// Notifies when the actor finish a melee attack
/// </summary>
/// <param name="MeshComp">A Character mesh component</param>
/// <param name="Animation">Playing animation</param>
void UTP_AN_DoingMeleeFinish::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor))
	{
		ATestProjectCharacter* Character = Cast<ATestProjectCharacter>(CharacterActor);
		if (IsValid(Character))
		{
			Character->SetDoingMelee(false);
		}
	}
}
