#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"



UCLASS()
class CUBESHOOTER_API UPlayerHUDWidget : public UUserWidget {
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* Panel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Crosshair;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MagazineCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* AmmoCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Score;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* WinScreenDialog;

	UFUNCTION(BlueprintCallable)
	void UpdateMagazineCount(int MagCount);

	UFUNCTION(BlueprintCallable)
	void UpdateAmmoCount(int Ammo);

	UFUNCTION(BlueprintCallable)
	void UpdateScoreCount(int NewScore);

	UFUNCTION(BlueprintCallable)
	void DisplayWinScreen(int FinalScore);
};