// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestProjectGameMode.generated.h"


class ATestProjectCharacter;
class UMaterialInterface;
class UTP_DemoEndWidget;

UCLASS(minimalapi)
class ATestProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:	

	void DemoEnd();

	void RestartDemo();

	UFUNCTION()
		void GameOver(ATestProjectCharacter* DeadCharacter, ATestProjectCharacter* CauserCharacter);

protected:

	virtual void BeginPlay() override;


public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DemoEnd")
		UMaterialInterface* DeathMaterial;

protected:
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameFlow")
		FName RestartMapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DemoEnd")
		TSubclassOf<UTP_DemoEndWidget> DemoEndWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "DemoEnd")
		UTP_DemoEndWidget* DemoEndWidget;
	
	ATestProjectCharacter* CauserCharacterRef;
	
	FTimerHandle TimerHandle_DemoEnd;

	FTimerHandle TimerHandle_RestartDemo;


};



