// A animation notify state that enable or disable when the character can make the second hit.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "TP_ANS_SecondHitEnabled.generated.h"

UCLASS()
class TESTPROJECT_API UTP_ANS_SecondHitEnabled : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	void SetSecondHitEnabled(USkeletalMeshComponent* MeshComp, bool NewState);
	
};
