#include "PickUpItemComponent.h"
#include "CubeShooterCharacter.h"
#include "AbstractGun.h"
#include "CubeShooterPlayerController.h"



// Sets default values for this component's properties
UPickUpItemComponent::UPickUpItemComponent() {
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bIsPickupCompActive = false;
}


// Called when the game starts
void UPickUpItemComponent::BeginPlay() {
    Super::BeginPlay();

	if (bIsPickupCompActive) {
    	OnComponentBeginOverlap.AddDynamic(this, &UPickUpItemComponent::OnPlayerOverlap);
	}
}


// Called every frame
void UPickUpItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}



void UPickUpItemComponent::OnPlayerOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, 
                                                            int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
    // Called when the player overlaps with the component's owner actor.
	if (!OtherActor || OverlappedComponent != this) {
		return;
	}

	PlayerCharacter = Cast<ACubeShooterCharacter> (OtherActor);
	if (!PlayerCharacter) {
		return;
	}

	switch (PickupType) {
	case EPickupType::Gun:
		EquipGun();
		break;

	default:
		GetOwner() -> Destroy();
		break;
	}

}


void UPickUpItemComponent::EquipGun() {
	if (!PlayerCharacter) {
		return;
	}

	if (PlayerCharacter -> bDoesPlayerHasGun) {
		// Add ammo of this gun to the gun the player currently has.
		AAbstractGun* PickedGun = GetOwner<AAbstractGun> () ;
		if (!PickedGun) {
			return;
		}

		int PickedAmmo = PickedGun -> GetAmmoCount();

		AAbstractGun* PlayerGun = PlayerCharacter -> Gun;
		if (PlayerGun) {
			PlayerGun -> IncreaseAmmoCount(PickedAmmo);
		}

		GetOwner() -> Destroy();
		return;
	}
	
	USkeletalMeshComponent* PlayerMesh = PlayerCharacter -> GetFirstPersonMesh();

	if(PlayerMesh) {
		OnComponentBeginOverlap.RemoveDynamic(this, &UPickUpItemComponent::OnPlayerOverlap);

		GetOwner() -> AttachToComponent(PlayerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("HandGrip_R") );
		PlayerCharacter -> bDoesPlayerHasGun = true;

		AAbstractGun* Gun = GetOwner<AAbstractGun> ();	
		PlayerCharacter -> Gun = Gun;
		if (PlayerCharacter -> Gun) {
			PlayerCharacter -> Gun -> PlayEquipAnimation();

			ACubeShooterPlayerController* PlayerController = Gun -> PlayerController;
			if (PlayerController) {
				PlayerController -> UpdatePlayerAmmoUI(Gun -> GetAmmoCount() );
				PlayerController -> UpdatePlayerMagazineCountUI(Gun -> GetMagCount() );
			}

		}
		

		GetOwner() -> SetActorEnableCollision(false);
	}
}