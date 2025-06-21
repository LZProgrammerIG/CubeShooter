// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CubeShooterPlayerController.generated.h"

class UInputMappingContext;

/**
 *  Simple first person Player Controller
 *  Manages the input mapping context.
 *  Overrides the Player Camera Manager class.
 */
UCLASS(abstract)
class CUBESHOOTER_API ACubeShooterPlayerController : public APlayerController {
	GENERATED_BODY()
	
public:
	/** Constructor */
	ACubeShooterPlayerController();

protected:
	virtual void BeginPlay() override;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input", meta = (AllowPrivateAccess = "true"))
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

public:
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<class UPlayerHUDWidget> PlayerHudWidgetClass;

	UPlayerHUDWidget* PlayerHUD;

	int Score;
	void UpdatePlayerScoreUI();
	void UpdatePlayerAmmoUI(int Ammo);
	void UpdatePlayerMagazineCountUI(int MagCount);
	void GameOver();
};