#include "PlayerHUDWidget.h"
#include "Components/TextBlock.h"



bool UPlayerHUDWidget::Initialize() {
    Super::Initialize();

    if (MagazineCount) {
        MagazineCount -> SetVisibility(ESlateVisibility::Hidden);
    }

    if (AmmoCount) {
        AmmoCount -> SetVisibility(ESlateVisibility::Hidden);
    }

    if (WinScreenDialog) {
        WinScreenDialog -> SetVisibility(ESlateVisibility::Hidden);
    }

    return true;
}


void UPlayerHUDWidget::UpdateMagazineCount(int MagCount) {
    if (!MagazineCount) {
        return;
    }

    MagazineCount -> SetVisibility(ESlateVisibility::Visible);
    MagazineCount -> SetText(FText::FromString(FString::FromInt(MagCount) ) ); 
}


void UPlayerHUDWidget::UpdateAmmoCount(int Ammo) {
    if (!AmmoCount) {
        return;
    }
    
    AmmoCount -> SetVisibility(ESlateVisibility::Visible);
    AmmoCount -> SetText(FText::FromString(TEXT("/ ") + FString::FromInt(Ammo) ) ); 
}


void UPlayerHUDWidget::UpdateScoreCount(int NewScore) {
    if (!Score) {
        return;
    }
    
    Score -> SetText(FText::FromString(TEXT("Score = ") + FString::FromInt(NewScore) ) ); 
}


void UPlayerHUDWidget::DisplayWinScreen(int FinalScore) {
    if (MagazineCount) {
        MagazineCount -> SetVisibility(ESlateVisibility::Hidden);
    }

    if (AmmoCount) {
        AmmoCount -> SetVisibility(ESlateVisibility::Hidden);
    }

    if (Crosshair) {
        Crosshair -> SetVisibility(ESlateVisibility::Hidden);
    }

    if (Score) {
        Score -> SetVisibility(ESlateVisibility::Hidden);
    }

    if (!WinScreenDialog) {
        return;
    }
    
    FString WinString = WinScreenDialog -> GetText().ToString();
    WinScreenDialog -> SetText(FText::FromString(WinString + FString::FromInt(FinalScore) ) );

    WinScreenDialog -> SetVisibility(ESlateVisibility::Visible);
}