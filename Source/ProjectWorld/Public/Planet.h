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
	int32 RecursiveQuadTreePlanetMeshGeneration(const FVector2D QuadPosition, const int32 PlanetSideIndex, const float QuadRadius, const int32 MaxSubdivision, const int32 CurentSubdivision, int32 VertexCount);
	int32 CreateChunk(const FVector2D ChunkPosition, const int32 PlanetSideIndex, const float ChunkRadius, const int32 ChunkSubdivision, const int32 StartVertexIndex);
	void ClearPlanet();

	int32 GetMaxPlanetSubdivisionByLoadDistance(const FVector2D QuadPosition, const float QuadRadius);
	bool IsSquaresCollided(const FVector2D Square1Center, const float Square1Radius, const FVector2D Square2Center, const float Square2Radius);

	FVector2D MyCharacterPos2D();
	FRotator MyCharacterPosToPlanetSideRotation();

	FVector ProjectToCubeTowardsCenter(const FVector Vector, const int32 CubeRadius);

protected:
	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere)
	int32 PlanetRadius;

	UPROPERTY(EditAnywhere)
	int32 LoadDistance;

	UPROPERTY(EditAnywhere)
	int32 PlanetSubdivision;

	UPROPERTY(EditAnywhere)
	int32 LODsAmount;

};
