#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cube.generated.h"



UCLASS()
class CUBESHOOTER_API ACube : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Setters
	void InitialiseCubeProperties(FVector color, int health, int score);
	void ApplyDamage(int Damage, AController* InstigatorController);
	


private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Properties")
	FVector Color;

	UPROPERTY(VisibleAnywhere, Category = "Properties")
	int Health;

	UPROPERTY(VisibleAnywhere, Category = "Properties")
	int Score;

	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	class UMaterialInterface* ParentMaterial;

	class UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	FName ColorControlVectorParam;

	void UpdateMaterial();
};