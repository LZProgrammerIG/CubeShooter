#include "CubeShooterPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "CubeShooterCameraManager.h"
#include "PlayerHUDWidget.h"



ACubeShooterPlayerController::ACubeShooterPlayerController() {
	// set the player camera manager class
	PlayerCameraManagerClass = ACubeShooterCameraManager::StaticClass();

	Score = 0;
}


void ACubeShooterPlayerController::BeginPlay() {
	Super::BeginPlay();
	
	if (PlayerHudWidgetClass) {
		PlayerHUD = CreateWidget<UPlayerHUDWidget>(this, PlayerHudWidgetClass);
		
		if (PlayerHUD) {
			PlayerHUD -> AddToViewport();
			UpdatePlayerScoreUI();
		}
	}
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


void ACubeShooterPlayerController::UpdatePlayerScoreUI() {
	if (PlayerHUD) {
		PlayerHUD -> UpdateScoreCount(Score);
	}
}


void ACubeShooterPlayerController::UpdatePlayerAmmoUI(int Ammo) {
	if (PlayerHUD) {
		PlayerHUD -> UpdateAmmoCount(Ammo);
	}
}


void ACubeShooterPlayerController::UpdatePlayerMagazineCountUI(int MagCount) {
	if (PlayerHUD) {
		PlayerHUD -> UpdateMagazineCount(MagCount);
	}
}


void ACubeShooterPlayerController::GameOver() {
	if (PlayerHUD) {
		PlayerHUD -> DisplayWinScreen(Score);
	}

	GetPawn() -> DisableInput(this);
    bShowMouseCursor = true;
}