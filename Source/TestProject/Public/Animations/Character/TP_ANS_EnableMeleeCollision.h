// An animation Notify State that enable or disable the character hands collisions for make damage

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "TP_ANS_EnableMeleeCollision.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UTP_ANS_EnableMeleeCollision : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	void SetCollisionEnabled(USkeletalMeshComponent* MeshComp, ECollisionEnabled::Type NewCollisionState, UAnimSequenceBase* Animation);
	
};
