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
	const FVector MyCharacterPosOnCube = MyCharacterPos.BoundToCube(Radius);

	ClearPlanet();

	CreateAllChunks(FVector(Radius, 0, 0), MyCharacterPosOnCube, Radius, LODsAmount, 0, 0);
	ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), false);

	//Debug
	for (auto& el : Vertices)
	{
		DrawDebugPoint(GetWorld(), el, 10, FColor().Red, true, false, 1.0f);
		//GEngine->AddOnScreenDebugMessage(-1, 2000.0f, FColor::Green, FString::Printf(TEXT("Point: %s"), *el.ToString()));
	}
}

int32 APlanet::CreateAllChunks(const FVector SquarePosition, const FVector PointPosition, const float SquareRadius, const int32 MaxDivide, const int32 CurrentDivide, int32 LastVertex)
{
	if (CurrentDivide < MaxDivide)
	{
		const float HalfSquareRadius = SquareRadius / 2;
		FVector ChunkPosition;

		// Right - Down
		ChunkPosition = FVector(Radius, SquarePosition.Y - HalfSquareRadius, SquarePosition.Z - HalfSquareRadius);
		if (IsSquaresCollided(FVector2D(PointPosition.Y, PointPosition.Z), LoadDistance,
			FVector2D(ChunkPosition.Y, ChunkPosition.Z), HalfSquareRadius)) // Right - Down
		{
			if (IsSquaresCollided(FVector2D(PointPosition.Y, PointPosition.Z), LoadDistance / 8,
				FVector2D(ChunkPosition.Y, ChunkPosition.Z), HalfSquareRadius)) // Right - Down
			{
				// Recursive Self
				LastVertex = CreateAllChunks(ChunkPosition, PointPosition, HalfSquareRadius, LODsAmount, CurrentDivide + 1, LastVertex);
			}
			else
			{
				// Recursive Self
				LastVertex = CreateAllChunks(ChunkPosition, PointPosition, HalfSquareRadius, LODsAmount - 1, CurrentDivide + 1, LastVertex);
			}
		}
		else
		{
			// Create Chunk
			LastVertex = CreateChunk(ChunkPosition, FVector(1, 0, 0), HalfSquareRadius, 999, LastVertex);
		}

		// Left - Down
		ChunkPosition = FVector(Radius, SquarePosition.Y + HalfSquareRadius, SquarePosition.Z - HalfSquareRadius);
		if (IsSquaresCollided(FVector2D(PointPosition.Y, PointPosition.Z), LoadDistance,
			FVector2D(ChunkPosition.Y, ChunkPosition.Z), HalfSquareRadius)) // Left - Down
		{
			if (IsSquaresCollided(FVector2D(PointPosition.Y, PointPosition.Z), LoadDistance / 8,
				FVector2D(ChunkPosition.Y, ChunkPosition.Z), HalfSquareRadius)) // Left - Down
			{
				// Recursive Self
				LastVertex = CreateAllChunks(ChunkPosition, PointPosition, HalfSquareRadius, LODsAmount, CurrentDivide + 1, LastVertex);
			}
			else
			{
				// Recursive Self
				LastVertex = CreateAllChunks(ChunkPosition, PointPosition, HalfSquareRadius, LODsAmount - 1, CurrentDivide + 1, LastVertex);
			}
		}
		else
		{
			// Create Chunk
			LastVertex = CreateChunk(ChunkPosition, FVector(1, 0, 0), HalfSquareRadius, 999, LastVertex);
		}

		// Right - Up
		ChunkPosition = FVector(Radius, SquarePosition.Y - HalfSquareRadius, SquarePosition.Z + HalfSquareRadius);
		if (IsSquaresCollided(FVector2D(PointPosition.Y, PointPosition.Z), LoadDistance,
			FVector2D(ChunkPosition.Y, ChunkPosition.Z), HalfSquareRadius)) // Right - Up
		{
			if (IsSquaresCollided(FVector2D(PointPosition.Y, PointPosition.Z), LoadDistance / 8,
				FVector2D(ChunkPosition.Y, ChunkPosition.Z), HalfSquareRadius)) // Right - Up
			{
				// Recursive Self
				LastVertex = CreateAllChunks(ChunkPosition, PointPosition, HalfSquareRadius, LODsAmount, CurrentDivide + 1, LastVertex);
			}
			else
			{
				// Recursive Self
				LastVertex = CreateAllChunks(ChunkPosition, PointPosition, HalfSquareRadius, LODsAmount - 1, CurrentDivide + 1, LastVertex);
			}
		}
		else
		{
			// Create Chunk
			LastVertex = CreateChunk(ChunkPosition, FVector(1, 0, 0), HalfSquareRadius, 999, LastVertex);
		}

		// Left - Up
		ChunkPosition = FVector(SquarePosition.X, SquarePosition.Y + HalfSquareRadius, SquarePosition.Z + HalfSquareRadius);
		if (IsSquaresCollided(FVector2D(PointPosition.Y, PointPosition.Z), LoadDistance,
			FVector2D(ChunkPosition.Y, ChunkPosition.Z), HalfSquareRadius)) // Left - Up
		{
			if (IsSquaresCollided(FVector2D(PointPosition.Y, PointPosition.Z), LoadDistance / 8,
				FVector2D(ChunkPosition.Y, ChunkPosition.Z), HalfSquareRadius)) // Left - Up
			{
				// Recursive Self
				LastVertex = CreateAllChunks(ChunkPosition, PointPosition, HalfSquareRadius, LODsAmount, CurrentDivide + 1, LastVertex);
			}
			else
			{
				// Recursive Self
				LastVertex = CreateAllChunks(ChunkPosition, PointPosition, HalfSquareRadius, LODsAmount - 1, CurrentDivide + 1, LastVertex);
			}
		}
		else
		{
			// Create Chunk
			LastVertex = CreateChunk(ChunkPosition, FVector(1, 0, 0), HalfSquareRadius, 999, LastVertex);
		}
	}
	else 
	{
		// Create Chunk
		LastVertex = CreateChunk(SquarePosition, FVector(1, 0, 0), SquareRadius, 999, LastVertex);
	}

	return LastVertex;
}

int32 APlanet::CreateChunk(const FVector ChunkPosition, const FVector ChunkNormal, const float ChunkRadius, const int32 Lod, const int32 StartVertex)
{
	// Create vertices
	Vertices.Add(FVector(ChunkPosition.X, ChunkPosition.Y + ChunkRadius, ChunkPosition.Z - ChunkRadius));
	Vertices.Add(FVector(ChunkPosition.X, ChunkPosition.Y - ChunkRadius, ChunkPosition.Z - ChunkRadius));
	Vertices.Add(FVector(ChunkPosition.X, ChunkPosition.Y + ChunkRadius, ChunkPosition.Z + ChunkRadius));
	Vertices.Add(FVector(ChunkPosition.X, ChunkPosition.Y - ChunkRadius, ChunkPosition.Z + ChunkRadius));

	// Create Triangles
	Triangles.Add(StartVertex + 0);
	Triangles.Add(StartVertex + 1);
	Triangles.Add(StartVertex + 2);

	Triangles.Add(StartVertex + 1);
	Triangles.Add(StartVertex + 3);
	Triangles.Add(StartVertex + 2);

	return StartVertex + 4;
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