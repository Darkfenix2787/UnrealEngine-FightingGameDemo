
#include "Animations/Character/TP_ANS_SecondHitEnabled.h"
#include "Character/TestProjectCharacter.h"

//================================================================================================================================================//
void UTP_ANS_SecondHitEnabled::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	SetSecondHitEnabled(MeshComp, true);
}

//================================================================================================================================================//
void UTP_ANS_SecondHitEnabled::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	SetSecondHitEnabled(MeshComp, false);
}

//================================================================================================================================================//

/// <summary>
/// This function set a boolean in the character that indicates if the character can make a second hit 
/// </summary>
/// <param name="MeshComp">A Character mesh component</param>
/// <param name="NewState">Character bSecondHit new state</param>
void UTP_ANS_SecondHitEnabled::SetSecondHitEnabled(USkeletalMeshComponent* MeshComp, bool NewState)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor))
	{
		ATestProjectCharacter* Character = Cast<ATestProjectCharacter>(CharacterActor);
		if (IsValid(Character))
		{		
			Character->SetSecondHitEnabled(NewState);			
		}
	}
}
