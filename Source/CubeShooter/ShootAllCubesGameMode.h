#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Http.h"
#include "ShootAllCubesGameMode.generated.h"



class ACube;

USTRUCT()
struct FCubeDescription {
	GENERATED_BODY()

	FString Name;
	FVector Color;
	int Health;
	int Score;

	FCubeDescription() : Name(TEXT("None")), Color(FVector::ZeroVector), Health(0), Score(0) {}

	FCubeDescription(FString Name, FVector Color, int Health, int Score) {
		this -> Name = Name;
		this -> Color = Color;
		this -> Health = Health;
		this -> Score = Score;
	}

	void PrintValue() {
		UE_LOG(LogTemp, Warning, TEXT("Name = %s"), *Name);
		UE_LOG(LogTemp, Warning, TEXT("Color = %lf, %lf, %lf"), Color.X, Color.Y, Color.Z);
		UE_LOG(LogTemp, Warning, TEXT("Health = %d"), Health);
		UE_LOG(LogTemp, Warning, TEXT("Score = %d"), Score);
		UE_LOG(LogTemp, Display, TEXT("----------------------"));
	}
};



USTRUCT()
struct FCubeSpawnInfo {
	GENERATED_BODY()

	FString CubeType;
	FVector Location;
	FRotator Rotation;
	FVector Scale;	

	FCubeSpawnInfo() : CubeType(TEXT("None")), Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator), Scale(FVector::ZeroVector) {}

	FCubeSpawnInfo(FString CubeType, FVector Location, FVector RotationVector, FVector Scale) {
		this -> CubeType = CubeType;
		this -> Location = Location;
		Rotation.Roll = RotationVector.X;
		Rotation.Pitch = RotationVector.Y;
		Rotation.Yaw = RotationVector.Z;
		this -> Scale = Scale;
	}

	void PrintValue() {
		UE_LOG(LogTemp, Warning, TEXT("Type of Cube = %s"), *CubeType);
		UE_LOG(LogTemp, Warning, TEXT("Location = %lf, %lf, %lf"), Location.X, Location.Y, Location.Z);
		UE_LOG(LogTemp, Warning, TEXT("Rotation = %lf, %lf, %lf"), Rotation.Roll, Rotation.Pitch, Rotation.Yaw);
		UE_LOG(LogTemp, Warning, TEXT("Scale = %lf, %lf, %lf"), Scale.X, Scale.Y, Scale.Z);
		UE_LOG(LogTemp, Display, TEXT("----------------------"));
	}
};



UCLASS()
class CUBESHOOTER_API AShootAllCubesGameMode : public AGameModeBase {
	GENERATED_BODY()
	
public:
	virtual void StartPlay() override;

	TMap<FString, FCubeDescription> Type;
	TArray<FCubeSpawnInfo> SpawnableCubes;
	
	void DecrementTotalCubesInGame();

private:
	UPROPERTY(EditDefaultsOnly, Category = SpawnableActors)
	TSubclassOf<ACube> CubeClass;

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully);

	void ExtractCubeTypes(TSharedPtr<FJsonObject> &ResponseObject);
	void ExtractVectorFromJsonObject(const TSharedPtr<FJsonObject>* &JsonObject, const FStringView FieldName, FVector &OutVector);
	void ExtractCubeObjects(TSharedPtr<FJsonObject> &ResponseObject);

	void SpawnCubes();

	int TotalCubesInGame;
};