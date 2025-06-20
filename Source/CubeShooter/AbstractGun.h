#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbstractGun.generated.h"



UCLASS(Abstract)
class CUBESHOOTER_API AAbstractGun : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbstractGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* GunBody;

	UPROPERTY(VisibleAnywhere)
	class UPickUpItemComponent* PickUpSphere;

	UPROPERTY(EditAnywhere, Category = "Gun Base Stats")
	float Damage;


	/* Ammo in Pocket and Ammo in Clip is independent of each other.
	It's possible to have 5 bullets in clip (ClipSize) and 10 in pocket (Ammo),
	making a total of 15 bullets available to shoot while the MaxAmmo count is
	10 only. 
	However, the player will not gain bullets more than MaxAmmo count from pickups.
	*/
	UPROPERTY(EditAnywhere, Category = "Gun Base Stats")
	int MaxAmmo;

	UPROPERTY(EditAnywhere, Category = "Gun Base Stats")
	int Ammo;

	UPROPERTY(EditAnywhere, Category = "Gun Base Stats")
	int MaxClipSize;

	UPROPERTY(EditAnywhere, Category = "Gun Base Stats")
	int ClipSize;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun Base Stats", meta = (AllowPrivateAccess = true))
	float ReloadTime;

	// The time between two successive shots.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun Base Stats", meta = (AllowPrivateAccess = true))
	float FiringRate;

	UPROPERTY(EditAnywhere, Category = "Gun Base Stats")
	float Range;


public:
	UPROPERTY(BlueprintReadWrite)
	bool bPrimaryFire;

	UPROPERTY(BlueprintReadWrite)
	class ACubeShooterCharacter* PlayerCharacter;

	UFUNCTION(BlueprintCallable)
	virtual void Fire() PURE_VIRTUAL(AAbstractGun::Fire);

	UFUNCTION(BlueprintCallable)
	virtual void Reload();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayEquipAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayReloadAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayFireAnimation();

	int GetAmmoCount();
	void SetAmmoCount(int AmmoCount);

	bool GetIsFireReady();
	void SetIsFireReady(bool bIsReadyToFire);
	void DelayFire(float DelayTime);

private:
	USkeletalMeshComponent* PlayerMesh;
	FTimerHandle DelayTimerHandle;
	
	bool bIsFireReady;
};