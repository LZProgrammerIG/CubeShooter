#include "CubeHealthWidget.h"
#include "Components/ProgressBar.h"



void UCubeHealthWidget::UpdateHealth(int MaxHealth, int CurrentHealth) {
    if (!HealthBar) {
        return;
    }
    
    float HealthPercent = ((float) CurrentHealth) / MaxHealth;
    HealthBar -> SetPercent(HealthPercent);
}