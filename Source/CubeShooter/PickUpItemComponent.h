#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "PickUpItemComponent.generated.h"



UENUM(BlueprintType)
enum class EPickupType : uint8 {
	Item UMETA(DisplayName = "Item"),
    Gun UMETA(DisplayName = "Gun"),
    Ammo UMETA(DisplayName = "Ammo")
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CUBESHOOTER_API UPickUpItemComponent : public USphereComponent {
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	UPickUpItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Type")
	EPickupType PickupType;

	UFUNCTION()
	void OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

private:
	UPROPERTY(EditAnywhere, Category = "Pickup")
	bool bIsPickupCompActive;

	class ACubeShooterCharacter* PlayerCharacter;

	void EquipGun();
};