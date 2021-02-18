// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/TestProjectGameMode.h"
#include "Character/TestProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Character/TestProjectCharacter.h"
#include "Engine/Classes/Materials/MaterialInterface.h"
#include "Kismet/GameplayStatics.h"
#include "UI/TP_DemoEndWidget.h"

//================================================================================================================================================//
void ATestProjectGameMode::BeginPlay()
{
	if (IsValid(DemoEndWidgetClass))
	{
		DemoEndWidget = CreateWidget<UTP_DemoEndWidget>(GetWorld(), DemoEndWidgetClass);
		DemoEndWidget->AddToViewport();
		DemoEndWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

//================================================================================================================================================//

/// <summary>
/// When the character is dead this function change the dead character material and execute the DemoEnd function with timer
/// </summary>
/// <param name="DeadCharacter"></param>
/// <param name="CauserCharacter"></param>
void ATestProjectGameMode::GameOver(ATestProjectCharacter* DeadCharacter, ATestProjectCharacter* CauserCharacter)
{
	if (IsValid(DeathMaterial))
	{
		DeadCharacter->GetMesh()->SetMaterial(0, DeathMaterial);
		CauserCharacterRef = CauserCharacter;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_DemoEnd, this, &ATestProjectGameMode::DemoEnd, 3.0f, false);		
	}
}

//================================================================================================================================================//

/// <summary>
/// This function is called by the DemoEnd timer inside the game over function. It disable the causer character input and shows the DemoEndWidget for 3 seconds
/// </summary>
void ATestProjectGameMode::DemoEnd()
{
	if (IsValid(CauserCharacterRef))
	{
		CauserCharacterRef->DisableInput(nullptr);
		DemoEndWidget->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_RestartDemo, this, &ATestProjectGameMode::RestartDemo, 10.0f, false);
	}
}

//================================================================================================================================================//

/// <summary>
/// This function is called by the RestartDemo timer inside the DemoEnd Function. It restart the level after 10 seconds
/// </summary>
void ATestProjectGameMode::RestartDemo()
{
	UGameplayStatics::OpenLevel(GetWorld(), RestartMapName);
}
