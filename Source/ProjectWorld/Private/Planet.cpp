// Fill out your copyright notice in the Description page of Project Settings.


#include "Planet.h"
#include "ProceduralMeshComponent.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"

// Sets default values
APlanet::APlanet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
	RootComponent = ProceduralMesh;

}

// Called when the game starts or when spawned
void APlanet::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void APlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	MyCharacterPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	CreatePlanet();
}

void APlanet::CreatePlanet()
{	
	MyCharacterPos = MyCharacterPos.BoundToCube(PlanetRadius);

	ClearPlanet();

	// Create Planet
	// FMath::Clamp LoadDistance
	float X2PlanetRadius = PlanetRadius * 2;
	int32 VertexCount = 0;
	VertexCount = RecursiveQuadTreePlanetMeshGeneration(FVector2D().ZeroVector, PlanetRadius, LODsAmount, 0, VertexCount); // Front
	VertexCount = RecursiveQuadTreePlanetMeshGeneration(FVector2D(-X2PlanetRadius, 0), PlanetRadius, LODsAmount, 0, VertexCount); // Right
	VertexCount = RecursiveQuadTreePlanetMeshGeneration(FVector2D(X2PlanetRadius, 0), PlanetRadius, LODsAmount, 0, VertexCount); // Left
	VertexCount = RecursiveQuadTreePlanetMeshGeneration(FVector2D(0, X2PlanetRadius), PlanetRadius, LODsAmount, 0, VertexCount); // Top
	VertexCount = RecursiveQuadTreePlanetMeshGeneration(FVector2D(0, -X2PlanetRadius), PlanetRadius, LODsAmount, 0, VertexCount); // Down

	ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), false);

	//Debug
	for (auto& el : Vertices)
	{
		DrawDebugPoint(GetWorld(), el, 10, FColor().Red, true, false, 1.0f);
		//GEngine->AddOnScreenDebugMessage(-1, 2000.0f, FColor::Green, FString::Printf(TEXT("Point: %s"), *el.ToString()));
	}
}

int32 APlanet::RecursiveQuadTreePlanetMeshGeneration(const FVector2D QuadPosition, const float QuadRadius, int32 MaxSubdivision, const int32 CurentSubdivision, int32 VertexCount)
{
	if (CurentSubdivision <= MaxSubdivision)
	{
		const float HalfQuadRadius = QuadRadius / 2;

		FVector2D SubdividedQuadPosition;

		// Right Down
		SubdividedQuadPosition = FVector2D(QuadPosition.X - HalfQuadRadius, QuadPosition.Y - HalfQuadRadius);
		if (IsSquaresCollided(SubdividedQuadPosition, HalfQuadRadius, FVector2D(MyCharacterPos.Y, MyCharacterPos.Z), LoadDistance))
		{
			if (IsSquaresCollided(SubdividedQuadPosition, HalfQuadRadius, FVector2D(MyCharacterPos.Y, MyCharacterPos.Z), LoadDistance / 8))
			{
				VertexCount = RecursiveQuadTreePlanetMeshGeneration(SubdividedQuadPosition, HalfQuadRadius, LODsAmount, CurentSubdivision + 1, VertexCount);
			}
			else
			{
				VertexCount = RecursiveQuadTreePlanetMeshGeneration(SubdividedQuadPosition, HalfQuadRadius, LODsAmount - 1, CurentSubdivision + 1, VertexCount);
			}
		}
		// Left Down
		SubdividedQuadPosition = FVector2D(QuadPosition.X + HalfQuadRadius, QuadPosition.Y - HalfQuadRadius);
		if (IsSquaresCollided(SubdividedQuadPosition, HalfQuadRadius, FVector2D(MyCharacterPos.Y, MyCharacterPos.Z), LoadDistance))
		{
			if (IsSquaresCollided(SubdividedQuadPosition, HalfQuadRadius, FVector2D(MyCharacterPos.Y, MyCharacterPos.Z), LoadDistance / 8))
			{
				VertexCount = RecursiveQuadTreePlanetMeshGeneration(SubdividedQuadPosition, HalfQuadRadius, LODsAmount, CurentSubdivision + 1, VertexCount);
			}
			else
			{
				VertexCount = RecursiveQuadTreePlanetMeshGeneration(SubdividedQuadPosition, HalfQuadRadius, LODsAmount - 1, CurentSubdivision + 1, VertexCount);
			}
		}
		// Right Up
		SubdividedQuadPosition = FVector2D(QuadPosition.X - HalfQuadRadius, QuadPosition.Y + HalfQuadRadius);
		if (IsSquaresCollided(SubdividedQuadPosition, HalfQuadRadius, FVector2D(MyCharacterPos.Y, MyCharacterPos.Z), LoadDistance))
		{
			if (IsSquaresCollided(SubdividedQuadPosition, HalfQuadRadius, FVector2D(MyCharacterPos.Y, MyCharacterPos.Z), LoadDistance / 8))
			{
				VertexCount = RecursiveQuadTreePlanetMeshGeneration(SubdividedQuadPosition, HalfQuadRadius, LODsAmount, CurentSubdivision + 1, VertexCount);
			}
			else
			{
				VertexCount = RecursiveQuadTreePlanetMeshGeneration(SubdividedQuadPosition, HalfQuadRadius, LODsAmount - 1, CurentSubdivision + 1, VertexCount);
			}
		}
		// Left Up
		SubdividedQuadPosition = FVector2D(QuadPosition.X + HalfQuadRadius, QuadPosition.Y + HalfQuadRadius);
		if (IsSquaresCollided(SubdividedQuadPosition, HalfQuadRadius, FVector2D(MyCharacterPos.Y, MyCharacterPos.Z), LoadDistance))
		{
			if (IsSquaresCollided(SubdividedQuadPosition, HalfQuadRadius, FVector2D(MyCharacterPos.Y, MyCharacterPos.Z), LoadDistance / 8))
			{
				VertexCount = RecursiveQuadTreePlanetMeshGeneration(SubdividedQuadPosition, HalfQuadRadius, LODsAmount, CurentSubdivision + 1, VertexCount);
			}
			else
			{
				VertexCount = RecursiveQuadTreePlanetMeshGeneration(SubdividedQuadPosition, HalfQuadRadius, LODsAmount - 1, CurentSubdivision + 1, VertexCount);
			}
		}		
	}
	else
	{
		VertexCount = CreateChunk(QuadPosition, QuadRadius, 999, VertexCount);
	}

	return VertexCount;
}

int32 APlanet::CreateChunk(const FVector2D ChunkPosition, const float ChunkRadius, const float ChunkSubdivision, const int32 StartVertexIndex)
{
	// Create Vertices
	Vertices.Add(FVector(0, ChunkPosition.X + ChunkRadius, ChunkPosition.Y - ChunkRadius));
	Vertices.Add(FVector(0, ChunkPosition.X - ChunkRadius, ChunkPosition.Y - ChunkRadius));
	Vertices.Add(FVector(0, ChunkPosition.X + ChunkRadius, ChunkPosition.Y + ChunkRadius));
	Vertices.Add(FVector(0, ChunkPosition.X - ChunkRadius, ChunkPosition.Y + ChunkRadius));

	// Create Triangles
	Triangles.Add(StartVertexIndex + 0); //
	Triangles.Add(StartVertexIndex + 1); // Left
	Triangles.Add(StartVertexIndex + 2); //

	Triangles.Add(StartVertexIndex + 1); //
	Triangles.Add(StartVertexIndex + 3); // Right
	Triangles.Add(StartVertexIndex + 2); //

	return StartVertexIndex + 4;
}

void APlanet::ClearPlanet()
{
	ProceduralMesh->ClearAllMeshSections();
	Vertices.Empty();
	Triangles.Empty();
	UVs.Empty();
}


bool APlanet::IsSquaresCollided(const FVector2D Square1Center, const float Square1Radius, const FVector2D Square2Center, const float Square2Radius)
{
	bool IsCollided = false;

	if ((FMath::Abs(Square1Center.X - Square2Center.X) <= Square1Radius + Square2Radius) &&
		(FMath::Abs(Square1Center.Y - Square2Center.Y) <= Square1Radius + Square2Radius))
	{
		IsCollided = true;
	}

	return IsCollided;
}

FVector APlanet::ProjectToCubeTowardsCenter(const FVector Vector, const int32 CubeRadius)
{
	if (Vector.X >= FMath::Abs(Vector.Y) && Vector.X >= FMath::Abs(Vector.Z))
	{
		return (FVector(1, Vector.Y / Vector.X, Vector.Z / Vector.X) * CubeRadius);
	}
	else
	{
		return FVector().ZeroVector;
	}
}