// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Planet.generated.h"

class UProceduralMeshComponent;

UCLASS()
class PROJECTWORLD_API APlanet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlanet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	const int32 MaxDensity = 5;

	UProceduralMeshComponent* ProceduralMesh;

	FVector MyCharacterPos;

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector2D> UVs;

	void CreatePlanet();
	int32 CreatePlanetSide(const FVector2D SquarePosition2D, const FVector SquareNormal, const FVector PointPosition, const float SquareRadius, const int32 MaxDivide, const int32 CurrentDivide, int32 LastVertex);
	int32 CreateChunk(const FVector2D ChunkPosition2D, const FVector ChunkNormal, const float ChunkRadius, const int32 Lod, const int32 StartVertex);
	FVector Vector2DTo3DByNormal(const FVector2D Vector2D, const FVector Normal);
	FVector2D Vector3DTo2DByNormal(const FVector Vector3D, const FVector Normal);
	void ClearPlanet();

	bool IsSquaresCollided(const FVector2D Square1Center, const float Square1Radius, const FVector2D Square2Center, const float Square2Radius);
	FVector ProjectToCubeTowardsCenter(const FVector Vector, const int32 CubeRadius);

protected:
	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere)
	int32 Radius;

	UPROPERTY(EditAnywhere)
	int32 LoadDistance;

	UPROPERTY(EditAnywhere)
	int32 Density;

	UPROPERTY(EditAnywhere)
	int32 LODsAmount;

};
