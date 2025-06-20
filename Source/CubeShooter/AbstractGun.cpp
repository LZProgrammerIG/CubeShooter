#include "AbstractGun.h"
#include "PickUpItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CubeShooterCharacter.h"



// Sets default values
AAbstractGun::AAbstractGun() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent> (TEXT("Root Component"));
	RootComponent = Root;

	GunBody = CreateDefaultSubobject<UStaticMeshComponent> (TEXT("Gun Body"));
	GunBody -> SetupAttachment(Root);

	PickUpSphere = CreateDefaultSubobject<UPickUpItemComponent> (TEXT("PickUpItemComp"));
	PickUpSphere -> SetupAttachment(GunBody);
	PickUpSphere -> PickupType = EPickupType::Gun;

	bIsFireReady = true;
	bPrimaryFire = true;
}

// Called when the game starts or when spawned
void AAbstractGun::BeginPlay() {
	Super::BeginPlay();
	
	PlayerCharacter = Cast<ACubeShooterCharacter> (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) );
	if (PlayerCharacter) {
		PlayerMesh = Cast<USkeletalMeshComponent> (PlayerCharacter -> GetComponentByClass(USkeletalMeshComponent::StaticClass() ) );
	}
}

// Called every frame
void AAbstractGun::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}


void AAbstractGun::Reload() {
	if (Ammo == 0 || ClipSize == MaxClipSize) {
		return;
	}
	
	DelayFire(ReloadTime);

	PlayReloadAnimation();
	
	int BulletsNeeded = MaxClipSize - ClipSize;
	int BulletsAvailable = FMath::Min(BulletsNeeded, Ammo);
	ClipSize += BulletsAvailable;
	Ammo -= BulletsAvailable;
}


void AAbstractGun::DelayFire(float DelayTime) {
	SetIsFireReady(false);
	
	FTimerDelegate DelayTimerDelegate;
	DelayTimerDelegate.BindUObject(this, &AAbstractGun::SetIsFireReady, true);
	GetWorldTimerManager().SetTimer(DelayTimerHandle, DelayTimerDelegate, DelayTime, false);
}



int AAbstractGun::GetAmmoCount() {
	return Ammo + ClipSize;
}

void AAbstractGun::SetAmmoCount(int AmmoCount) {
	Ammo += AmmoCount;
	Ammo = FMath::Min(MaxAmmo, Ammo);
}


bool AAbstractGun::GetIsFireReady() {
	return bIsFireReady;
}

void AAbstractGun::SetIsFireReady(bool bIsReadyToFire) {
	bIsFireReady = bIsReadyToFire;
}