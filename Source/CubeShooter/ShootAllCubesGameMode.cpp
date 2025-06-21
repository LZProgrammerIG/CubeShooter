#include "ShootAllCubesGameMode.h"
#include "Json.h"
#include "Cube.h"
#include "Kismet/GameplayStatics.h"
#include "CubeShooterPlayerController.h"



void AShootAllCubesGameMode::StartPlay() {
    Super::StartPlay();

    TotalCubesInGame = 0;

    FHttpRequestRef RequestRef = FHttpModule::Get().CreateRequest();
    RequestRef -> OnProcessRequestComplete().BindUObject(this, &AShootAllCubesGameMode::OnResponseReceived);

    RequestRef -> SetURL("https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json");
    RequestRef -> SetVerb("GET");
    RequestRef -> ProcessRequest();
}


// Function invoked after the request has been processed.
void AShootAllCubesGameMode::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully) {
    if (!bProcessedSuccessfully || !Response || Response -> GetStatus() != EHttpRequestStatus::Succeeded) {
        UE_LOG(LogTemp, Error, TEXT("Request was not successfull"));
        return;
    }

    TSharedPtr<FJsonObject> ResponseObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response -> GetContentAsString() );
    FJsonSerializer::Deserialize(Reader, ResponseObject);

    if (!ResponseObject) {
        UE_LOG(LogTemp, Error, TEXT("Response Object is invalid"));
        return;
    }

    ExtractCubeTypes(ResponseObject);
    ExtractCubeObjects(ResponseObject);

    SpawnCubes();
}



void AShootAllCubesGameMode::ExtractCubeTypes(TSharedPtr<FJsonObject> &ResponseObject) {
    // The value corresponding to key "types" is an array of objects.
    const TArray<TSharedPtr<FJsonValue>>* OutTypesResponseArray;
    ResponseObject -> TryGetArrayField(TEXT("types"), OutTypesResponseArray);

    for (TSharedPtr<FJsonValue> Value : *OutTypesResponseArray) {
        const TSharedPtr<FJsonObject>* ValueObject;
        if (!Value -> TryGetObject(ValueObject) ) {
            continue;
        }

        FString Name = TEXT("None");
        FVector ColorVector = FVector::ZeroVector;
        int Health = 0;
        int Score = 0;


        ValueObject -> Get() -> TryGetStringField(TEXT("name"), Name);

        // Color key has an array of integers as its value.
        ExtractVectorFromJsonObject(ValueObject, TEXT("color"), ColorVector);

        ValueObject -> Get() -> TryGetNumberField(TEXT("health"), Health);
        ValueObject -> Get() -> TryGetNumberField(TEXT("score"), Score);

        Type.Add(Name, FCubeDescription(Name, ColorVector, Health, Score));
    }
}


void AShootAllCubesGameMode::ExtractVectorFromJsonObject(const TSharedPtr<FJsonObject> *&JsonObject, const FStringView FieldName, FVector &OutVector) {
    const TArray<TSharedPtr<FJsonValue>>* OutJsonValueArray;
    if (JsonObject -> Get() -> TryGetArrayField(FieldName, OutJsonValueArray) == false) {
        return;
    }

    TArray<double> TempArray;
    double OutTemp = 0;

    for (TSharedPtr<FJsonValue> JsonValue : *OutJsonValueArray) {
        JsonValue -> TryGetNumber(OutTemp);
        TempArray.Add(OutTemp);
    }

    if (TempArray.Num() == 3) {
        OutVector = FVector(TempArray[0], TempArray[1], TempArray[2] );
    }
}


void AShootAllCubesGameMode::ExtractCubeObjects(TSharedPtr<FJsonObject> &ResponseObject) {
    // The value corresponding to key "objects" is also an array of objects.
    const TArray<TSharedPtr<FJsonValue>>* CubeObjectsResponseArray;
    if (!ResponseObject -> TryGetArrayField(TEXT("objects"), CubeObjectsResponseArray) ) {
        return;
    } 


    for (TSharedPtr<FJsonValue> Value : *CubeObjectsResponseArray) {
        TSharedPtr<FJsonObject>* ValueObject;
        if (!Value -> TryGetObject(ValueObject) ) {
            continue;
        }

        FString CubeType = TEXT("None");
        FVector LocationVector = FVector::ZeroVector;
        FVector RotationVector = FVector::ZeroVector;
        FVector ScaleVector = FVector::ZeroVector;


        ValueObject -> Get() -> TryGetStringField(TEXT("type"), CubeType);

        // The field "transform" contains an object which contains 3 vectors.
        const TSharedPtr<FJsonObject>* TransformObject;
        if (ValueObject -> Get() -> TryGetObjectField(TEXT("transform"), TransformObject) ) {
            ExtractVectorFromJsonObject(TransformObject, TEXT("location"), LocationVector);
            ExtractVectorFromJsonObject(TransformObject, TEXT("rotation"), RotationVector);
            ExtractVectorFromJsonObject(TransformObject, TEXT("scale"), ScaleVector);
        }

        SpawnableCubes.Add(FCubeSpawnInfo(CubeType, LocationVector, RotationVector, ScaleVector));
    }
}



void AShootAllCubesGameMode::SpawnCubes() {
    if (SpawnableCubes.IsEmpty() || !CubeClass) {
        return;
    }
    
    
    for (FCubeSpawnInfo& CubeInfo : SpawnableCubes) {
        const FTransform CubeTransform(CubeInfo.Rotation, CubeInfo.Location, CubeInfo.Scale);
        ACube* Cube = GetWorld() -> SpawnActor<ACube> (CubeClass, CubeTransform);
        
        FCubeDescription* Desc = Type.Find(CubeInfo.CubeType);
        if (Desc) {
            Cube -> InitialiseCubeProperties(Desc -> Color, Desc -> Health, Desc -> Score);
        }

        TotalCubesInGame++;
    }
}


void AShootAllCubesGameMode::DecrementTotalCubesInGame() {
    TotalCubesInGame--;
    if (TotalCubesInGame <= 0) {
        ACubeShooterPlayerController* PlayerController = Cast<ACubeShooterPlayerController> (
            UGameplayStatics::GetPlayerController(GetWorld(), 0) 
        );

        if (PlayerController) {
            PlayerController -> GameOver();
        }
    }
}