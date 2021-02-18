
#include "Animations/Character/TP_ANS_EnableMeleeCollision.h"
#include "Character/TestProjectCharacter.h"

//================================================================================================================================================//
void UTP_ANS_EnableMeleeCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	SetCollisionEnabled(MeshComp, ECollisionEnabled::QueryOnly, Animation);
}

//================================================================================================================================================//
void UTP_ANS_EnableMeleeCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	SetCollisionEnabled(MeshComp, ECollisionEnabled::NoCollision, Animation);
}

//================================================================================================================================================//

/// <summary>
/// This function enables or disables the character hands collision taking in the first anim montage
/// </summary>
/// <param name="MeshComp">A Character mesh component</param>
/// <param name="NewCollisionState">Collision Enum that indicates the collision state </param>
/// <param name="Animation">Playing animation</param>
void UTP_ANS_EnableMeleeCollision::SetCollisionEnabled(USkeletalMeshComponent* MeshComp, ECollisionEnabled::Type NewCollisionState, UAnimSequenceBase* Animation)
{
	FString Name = Animation->GetName();
	if (!Name.IsEmpty())
	{
		AActor* CharacterActor = MeshComp->GetOwner();
		if (IsValid(CharacterActor))
		{
			ATestProjectCharacter* Character = Cast<ATestProjectCharacter>(CharacterActor);
			if (IsValid(Character))
			{
				if (Name == "AM_LeftHit")
				{
					Character->SetLeftHandDetectorCollision(NewCollisionState);
				}
				else
				{
					Character->SetRightHandDetectorCollision(NewCollisionState);
				}
			}
		}
	}
}
