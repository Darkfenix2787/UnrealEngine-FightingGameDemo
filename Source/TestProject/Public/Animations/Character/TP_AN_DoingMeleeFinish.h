// An Animation Notify that indicates when the character finish the animation to do a melee hit

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "TP_AN_DoingMeleeFinish.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UTP_AN_DoingMeleeFinish : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
