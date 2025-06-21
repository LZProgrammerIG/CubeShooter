#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CubeHealthWidget.generated.h"



UCLASS()
class CUBESHOOTER_API UCubeHealthWidget : public UUserWidget {
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* Panel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HealthBar;

	UFUNCTION(BlueprintCallable)
	void UpdateHealth(int MaxHealth, int CurrentHealth);
};