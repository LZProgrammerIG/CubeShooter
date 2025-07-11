#include "HitscanGun.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "CubeShooterPlayerController.h"
#include "Cube.h"



// Sets default values
AHitscanGun::AHitscanGun() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


// Called when the game starts or when spawned
void AHitscanGun::BeginPlay() {
	Super::BeginPlay();
	
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn) {
        PlayerCameraComp = Cast<UCameraComponent> (PlayerPawn -> GetComponentByClass(UCameraComponent::StaticClass() ));
    }
}


// Called every frame
void AHitscanGun::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
    
}



void AHitscanGun::Fire() {    
    if (!PlayerCameraComp || !GetIsFireReady() ) {
        return;
    }
    
    if (ClipSize == 0) {
        Reload();
        return;
    }

    // TODO: VFX and SFX at the gun's muzzle location.
    if (MuzzleFlash) {
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GunBody, TEXT("MuzzleFlashSocket"));
	}
	
	if (MuzzleSound) {
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, GunBody, TEXT("MuzzleFlashSocket"));
	}


    ClipSize--;

    if (PlayerController) {
        PlayerController -> UpdatePlayerMagazineCountUI(ClipSize);
    }

    DelayFire(FiringRate);

    // TODO: Implement Fire Animation.
    PlayFireAnimation();

    DoHitscanTrace();
}


void AHitscanGun::DoHitscanTrace() {
    if (!PlayerCameraComp) {
        return;
    }
    
    FVector StartLocation = PlayerCameraComp -> GetComponentLocation();
    FVector EndLocation = StartLocation + (PlayerCameraComp -> GetForwardVector() * Range);

    bool HasHit = GetWorld() -> SweepSingleByChannel(
        OutHitResult, 
        StartLocation, 
        EndLocation, 
        FQuat::Identity, 
        ECollisionChannel::ECC_Visibility, 
        FCollisionShape::MakeSphere(5.0f) 
    );
 
    if (HasHit) {
        // TODO: VFX and SFX at the Impact Point.
        if (BulletImpact) {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletImpact, OutHitResult.ImpactPoint);
		}
        
        if (BulletImpactSound) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), BulletImpactSound, OutHitResult.ImpactPoint);
		}

        ACube* EnemyCube = Cast<ACube> (OutHitResult.GetActor() );
        if (EnemyCube) {
            EnemyCube -> ApplyDamage(Damage, PlayerController);
        }
        //DrawDebugSphere(GetWorld(), OutHitResult.ImpactPoint, 5.0f, 10, FColor::Red, false, 3.0f);
    }
}