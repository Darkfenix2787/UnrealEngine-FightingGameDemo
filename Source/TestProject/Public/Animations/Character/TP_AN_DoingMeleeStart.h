// An Animation Notify that indicates when the character start the animation to do a melee hit

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "TP_AN_DoingMeleeStart.generated.h"

UCLASS()
class TESTPROJECT_API UTP_AN_DoingMeleeStart : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
