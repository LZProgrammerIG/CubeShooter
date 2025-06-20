// Copyright Epic Games, Inc. All Rights Reserved.


#include "CubeShooterPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "CubeShooterCameraManager.h"

ACubeShooterPlayerController::ACubeShooterPlayerController() {
	// set the player camera manager class
	PlayerCameraManagerClass = ACubeShooterCameraManager::StaticClass();

	Score = 0;
}


void ACubeShooterPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}
