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
	// Clamping Global Value
	MyCharacterPos = MyCharacterPos.BoundToCube(PlanetRadius);
	LoadDistance = FMath::Clamp(LoadDistance, 10000, PlanetRadius * 2);

	// Clear Planet
	ClearPlanet();

	// Create Planet
	float X2PlanetRadius = PlanetRadius * 2;
	int32 VertexCount = 0;

	VertexCount = RecursiveQuadTreePlanetMeshGeneration(FVector2D().ZeroVector, 3, PlanetRadius, PlanetSubdivision, 0, VertexCount); // Front
	VertexCount = RecursiveQuadTreePlanetMeshGeneration(FVector2D(-X2PlanetRadius, 0), 4, PlanetRadius, PlanetSubdivision, 0, VertexCount); // Right
	VertexCount = RecursiveQuadTreePlanetMeshGeneration(FVector2D(X2PlanetRadius, 0), 2, PlanetRadius, PlanetSubdivision, 0, VertexCount); // Left
	VertexCount = RecursiveQuadTreePlanetMeshGeneration(FVector2D(0, X2PlanetRadius), 1, PlanetRadius, PlanetSubdivision, 0, VertexCount); // Top
	VertexCount = RecursiveQuadTreePlanetMeshGeneration(FVector2D(0, -X2PlanetRadius), 5, PlanetRadius, PlanetSubdivision, 0, VertexCount); // Down

	ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), false);

	//Debug
	//for (auto& el : Vertices)
	//{
	//	DrawDebugPoint(GetWorld(), el, 10, FColor().Red, true, false, 1.0f);
	//	//GEngine->AddOnScreenDebugMessage(-1, 2000.0f, FColor::Green, FString::Printf(TEXT("Point: %s"), *el.ToString()));
	//}
}

int32 APlanet::RecursiveQuadTreePlanetMeshGeneration(const FVector2D QuadPosition, const int32 PlanetSideIndex, const float QuadRadius, int32 MaxSubdivision, const int32 CurentSubdivision, int32 VertexCount)
{
	if (CurentSubdivision <= MaxSubdivision)
	{
		const float HalfQuadRadius = QuadRadius / 2;

		FVector2D SubdividedQuadPosition;
		int32 MaxPlanetSubdivisionByLoadDistance;

		// Right Down
		SubdividedQuadPosition = FVector2D(QuadPosition.X - HalfQuadRadius, QuadPosition.Y - HalfQuadRadius);
		MaxPlanetSubdivisionByLoadDistance = GetMaxPlanetSubdivisionByLoadDistance(SubdividedQuadPosition, HalfQuadRadius);
		if (MaxPlanetSubdivisionByLoadDistance)
		{
			VertexCount = RecursiveQuadTreePlanetMeshGeneration(SubdividedQuadPosition, PlanetSideIndex, HalfQuadRadius, MaxPlanetSubdivisionByLoadDistance, CurentSubdivision + 1, VertexCount);
		}

		// Left Down
		SubdividedQuadPosition = FVector2D(QuadPosition.X + HalfQuadRadius, QuadPosition.Y - HalfQuadRadius);
		MaxPlanetSubdivisionByLoadDistance = GetMaxPlanetSubdivisionByLoadDistance(SubdividedQuadPosition, HalfQuadRadius);
		if (MaxPlanetSubdivisionByLoadDistance)
		{
			VertexCount = RecursiveQuadTreePlanetMeshGeneration(SubdividedQuadPosition, PlanetSideIndex, HalfQuadRadius, MaxPlanetSubdivisionByLoadDistance, CurentSubdivision + 1, VertexCount);
		}

		// Right Up
		SubdividedQuadPosition = FVector2D(QuadPosition.X - HalfQuadRadius, QuadPosition.Y + HalfQuadRadius);
		MaxPlanetSubdivisionByLoadDistance = GetMaxPlanetSubdivisionByLoadDistance(SubdividedQuadPosition, HalfQuadRadius);
		if (MaxPlanetSubdivisionByLoadDistance)
		{
			VertexCount = RecursiveQuadTreePlanetMeshGeneration(SubdividedQuadPosition, PlanetSideIndex, HalfQuadRadius, MaxPlanetSubdivisionByLoadDistance, CurentSubdivision + 1, VertexCount);
		}

		// Left Up
		SubdividedQuadPosition = FVector2D(QuadPosition.X + HalfQuadRadius, QuadPosition.Y + HalfQuadRadius);
		MaxPlanetSubdivisionByLoadDistance = GetMaxPlanetSubdivisionByLoadDistance(SubdividedQuadPosition, HalfQuadRadius);
		if (MaxPlanetSubdivisionByLoadDistance)
		{
			VertexCount = RecursiveQuadTreePlanetMeshGeneration(SubdividedQuadPosition, PlanetSideIndex, HalfQuadRadius, MaxPlanetSubdivisionByLoadDistance, CurentSubdivision + 1, VertexCount);
		}
	}
	else
	{
		VertexCount = CreateChunk(QuadPosition, PlanetSideIndex, QuadRadius, 999, VertexCount);
	}

	return VertexCount;
}

int32 APlanet::CreateChunk(const FVector2D ChunkPosition, const int32 PlanetSideIndex, const float ChunkRadius, const float ChunkSubdivision, const int32 StartVertexIndex)
{
	FVector Vertex0 = FVector(0, ChunkPosition.X + ChunkRadius, ChunkPosition.Y - ChunkRadius);
	FVector Vertex1 = FVector(0, ChunkPosition.X - ChunkRadius, ChunkPosition.Y - ChunkRadius);
	FVector Vertex2 = FVector(0, ChunkPosition.X + ChunkRadius, ChunkPosition.Y + ChunkRadius);
	FVector Vertex3 = FVector(0, ChunkPosition.X - ChunkRadius, ChunkPosition.Y + ChunkRadius);

	float X2PlanetRadius = PlanetRadius * 2;
	switch (PlanetSideIndex)
	{
	case 1: // Up
		Vertex0 = FRotator(90, 0, 0).RotateVector(Vertex0 - FVector(0, 0, X2PlanetRadius)) + FVector(0, 0, PlanetRadius);
		Vertex1 = FRotator(90, 0, 0).RotateVector(Vertex1 - FVector(0, 0, X2PlanetRadius)) + FVector(0, 0, PlanetRadius);
		Vertex2 = FRotator(90, 0, 0).RotateVector(Vertex2 - FVector(0, 0, X2PlanetRadius)) + FVector(0, 0, PlanetRadius);
		Vertex3 = FRotator(90, 0, 0).RotateVector(Vertex3 - FVector(0, 0, X2PlanetRadius)) + FVector(0, 0, PlanetRadius);
		break;
	case 2: // Left
		Vertex0 = FRotator(0, 90, 0).RotateVector(Vertex0 - FVector(0, X2PlanetRadius, 0)) + FVector(0, PlanetRadius, 0);
		Vertex1 = FRotator(0, 90, 0).RotateVector(Vertex1 - FVector(0, X2PlanetRadius, 0)) + FVector(0, PlanetRadius, 0);
		Vertex2 = FRotator(0, 90, 0).RotateVector(Vertex2 - FVector(0, X2PlanetRadius, 0)) + FVector(0, PlanetRadius, 0);
		Vertex3 = FRotator(0, 90, 0).RotateVector(Vertex3 - FVector(0, X2PlanetRadius, 0)) + FVector(0, PlanetRadius, 0);
		break;
	case 3: // Front
		Vertex0 = Vertex0 + FVector(PlanetRadius, 0, 0);
		Vertex1 = Vertex1 + FVector(PlanetRadius, 0, 0);
		Vertex2 = Vertex2 + FVector(PlanetRadius, 0, 0);
		Vertex3 = Vertex3 + FVector(PlanetRadius, 0, 0);
		break;
	case 4: // Right
		Vertex0 = FRotator(0, -90, 0).RotateVector(Vertex0 + FVector(0, X2PlanetRadius, 0)) - FVector(0, PlanetRadius, 0);
		Vertex1 = FRotator(0, -90, 0).RotateVector(Vertex1 + FVector(0, X2PlanetRadius, 0)) - FVector(0, PlanetRadius, 0);
		Vertex2 = FRotator(0, -90, 0).RotateVector(Vertex2 + FVector(0, X2PlanetRadius, 0)) - FVector(0, PlanetRadius, 0);
		Vertex3 = FRotator(0, -90, 0).RotateVector(Vertex3 + FVector(0, X2PlanetRadius, 0)) - FVector(0, PlanetRadius, 0);
		break;
	case 5: // Down
		Vertex0 = FRotator(-90, 0, 0).RotateVector(Vertex0 + FVector(0, 0, X2PlanetRadius)) - FVector(0, 0, PlanetRadius);
		Vertex1 = FRotator(-90, 0, 0).RotateVector(Vertex1 + FVector(0, 0, X2PlanetRadius)) - FVector(0, 0, PlanetRadius);
		Vertex2 = FRotator(-90, 0, 0).RotateVector(Vertex2 + FVector(0, 0, X2PlanetRadius)) - FVector(0, 0, PlanetRadius);
		Vertex3 = FRotator(-90, 0, 0).RotateVector(Vertex3 + FVector(0, 0, X2PlanetRadius)) - FVector(0, 0, PlanetRadius);
		break;
	default:
		break;
	}

	// Rotate to CameraPosition
	Vertex0 = MyCharacterPosToPlanetSideRotation().RotateVector(Vertex0);
	Vertex1 = MyCharacterPosToPlanetSideRotation().RotateVector(Vertex1);
	Vertex2 = MyCharacterPosToPlanetSideRotation().RotateVector(Vertex2);
	Vertex3 = MyCharacterPosToPlanetSideRotation().RotateVector(Vertex3);

	// Convert Cube to Sphere
	Vertex0 = Vertex0.GetSafeNormal() * PlanetRadius;
	Vertex1 = Vertex1.GetSafeNormal() * PlanetRadius;
	Vertex2 = Vertex2.GetSafeNormal() * PlanetRadius;
	Vertex3 = Vertex3.GetSafeNormal() * PlanetRadius;

	// Create Vertices
	Vertices.Add(Vertex0);
	Vertices.Add(Vertex1);
	Vertices.Add(Vertex2);
	Vertices.Add(Vertex3);

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

// Change Chunk Quality by Distance
int32 APlanet::GetMaxPlanetSubdivisionByLoadDistance(const FVector2D QuadPosition, const float QuadRadius)
{
	int32 MaxSubdivision = 0;

	float LoadDistanceDivider = 1;
	for (int32 i = 1; i <= LODsAmount; ++i)
	{
		// Debug
		//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::Printf(TEXT("Point: %f"), LoadDistanceDivider));

		if (IsSquaresCollided(QuadPosition, QuadRadius, MyCharacterPos2D(), LoadDistance / LoadDistanceDivider))
		{
			MaxSubdivision = PlanetSubdivision - LODsAmount + i;
		}

		LoadDistanceDivider = LoadDistanceDivider * FMath::Pow(1.5, i); //Value for Change Chunk Quality by Distance
	}
	
	return MaxSubdivision;
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

FVector2D APlanet::MyCharacterPos2D()
{
	FVector2D Vector2D = FVector2D().ZeroVector;

	if (MyCharacterPos.X >= PlanetRadius) // Front
	{
		Vector2D = FVector2D(MyCharacterPos.Y, MyCharacterPos.Z);
	}
	if (MyCharacterPos.X <= -PlanetRadius) // Back
	{
		Vector2D = FVector2D(-MyCharacterPos.Y, MyCharacterPos.Z);
	}
	if (MyCharacterPos.Y >= PlanetRadius) // Left
	{
		Vector2D = FVector2D(-MyCharacterPos.X, MyCharacterPos.Z);
	}
	if (MyCharacterPos.Y <= -PlanetRadius) // Right
	{
		Vector2D = FVector2D(MyCharacterPos.X, MyCharacterPos.Z);
	}
	if (MyCharacterPos.Z >= PlanetRadius) // Up
	{
		Vector2D = FVector2D(MyCharacterPos.Y, -MyCharacterPos.X);
	}
	if (MyCharacterPos.Z <= -PlanetRadius) // Down
	{
		Vector2D = FVector2D(MyCharacterPos.Y, MyCharacterPos.X);
	}

	return Vector2D;
}

FRotator APlanet::MyCharacterPosToPlanetSideRotation()
{
	FRotator Rotator = FRotator().ZeroRotator;
		
	if (MyCharacterPos.X >= PlanetRadius) // Front
	{
		Rotator = FRotator(0, 0, 0);
	}
	if (MyCharacterPos.X <= -PlanetRadius) // Back
	{
		Rotator = FRotator(0, 180, 0);
	}
	if (MyCharacterPos.Y >= PlanetRadius) // Left
	{
		Rotator = FRotator(0, 90, 0);
	}
	if (MyCharacterPos.Y <= -PlanetRadius) // Right
	{
		Rotator = FRotator(0, -90, 0);
	}
	if (MyCharacterPos.Z >= PlanetRadius) // Up
	{
		Rotator = FRotator(90, 0, 0);
	}
	if (MyCharacterPos.Z <= -PlanetRadius) // Down
	{
		Rotator = FRotator(-90, 0, 0);
	}

	return Rotator;
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