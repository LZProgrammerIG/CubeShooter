#include "Cube.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "CubeShooterPlayerController.h"
#include "ShootAllCubesGameMode.h"
#include "Components/WidgetComponent.h"
#include "CubeHealthWidget.h"



// Sets default values
ACube::ACube() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComp = CreateDefaultSubobject<USceneComponent> (TEXT("Root"));
	RootComponent = RootComp;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent> (TEXT("Mesh"));
	Mesh -> SetupAttachment(RootComp);

	WidgetComp = CreateDefaultSubobject<UWidgetComponent> (TEXT("Health Widget"));
	WidgetComp -> SetupAttachment(RootComp);

	Color = FVector::ZeroVector;
	Health = 0;
	CurrentHealth = Health;
	Score = 0;

	ColorControlVectorParam = TEXT("ColorControl");
}


// Called when the game starts or when spawned
void ACube::BeginPlay() {
	Super::BeginPlay();

	if (ParentMaterial) {
		DynamicMaterial = UMaterialInstanceDynamic::Create(ParentMaterial, this, TEXT("M_CubeDynamic"));
		if (DynamicMaterial) {
			Mesh -> SetMaterial(0, DynamicMaterial);
		}
	}

	GameMode = Cast<AShootAllCubesGameMode> (GetWorld() -> GetAuthGameMode() );

	if (CubeHealthWidgetClass) {
		WidgetComp -> SetWidgetClass(CubeHealthWidgetClass);
		//WidgetComp -> SetTwoSided(true);

		HealthWidget = Cast<UCubeHealthWidget> (WidgetComp -> GetWidget() );
		if (HealthWidget) {
			HealthWidget -> UpdateHealth(Health, CurrentHealth);
		}
	}
}


// Called every frame
void ACube::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}



void ACube::InitialiseCubeProperties(FVector color, int health, int score) {
	Color = color;
	Health = health;
	CurrentHealth = Health;
	Score = score;

	UpdateMaterial();
}


void ACube::UpdateMaterial() {
	if (!DynamicMaterial) {
		return;
	}

	DynamicMaterial -> SetVectorParameterValue(ColorControlVectorParam, Color);
}


void ACube::ApplyDamage(int Damage, AController* InstigatorController) {
	CurrentHealth -= Damage;
	if (HealthWidget) {
		HealthWidget -> UpdateHealth(Health, CurrentHealth);
	}

	if (CurrentHealth <= 0) {
		ACubeShooterPlayerController* PlayerController = Cast<ACubeShooterPlayerController> (InstigatorController);

		if (PlayerController) {
			PlayerController -> Score += Score;
			PlayerController -> UpdatePlayerScoreUI();
		}

		if (GameMode) {
			GameMode -> DecrementTotalCubesInGame();
		}
		
		Destroy();
	}
}