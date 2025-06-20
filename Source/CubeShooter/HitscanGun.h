#pragma once

#include "CoreMinimal.h"
#include "AbstractGun.h"
#include "HitscanGun.generated.h"



UCLASS()
class CUBESHOOTER_API AHitscanGun : public AAbstractGun {
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AHitscanGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Fire() override;

private:
	class UCameraComponent* PlayerCameraComp;
	FHitResult OutHitResult;
};