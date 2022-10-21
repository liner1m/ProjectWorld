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

	// Create Planet Sides
	int32 LastVertex = 0;
	LastVertex = CreatePlanetSide(FVector2D(0, 0), FVector(Radius, 0, 0), MyCharacterPosOnCube, Radius, LODsAmount, 0, LastVertex);
	LastVertex = CreatePlanetSide(FVector2D(0, 0), FVector(-Radius, 0, 0), MyCharacterPosOnCube, Radius, LODsAmount, 0, LastVertex);
	LastVertex = CreatePlanetSide(FVector2D(0, 0), FVector(0, Radius, 0), MyCharacterPosOnCube, Radius, LODsAmount, 0, LastVertex);
	LastVertex = CreatePlanetSide(FVector2D(0, 0), FVector(0, -Radius, 0), MyCharacterPosOnCube, Radius, LODsAmount, 0, LastVertex);
	LastVertex = CreatePlanetSide(FVector2D(0, 0), FVector(0, 0, Radius), MyCharacterPosOnCube, Radius, LODsAmount, 0, LastVertex);
	LastVertex = CreatePlanetSide(FVector2D(0, 0), FVector(0, 0, -Radius), MyCharacterPosOnCube, Radius, LODsAmount, 0, LastVertex);

	ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), false);

	//Debug
	for (auto& el : Vertices)
	{
		DrawDebugPoint(GetWorld(), el, 10, FColor().Red, true, false, 1.0f);
		//GEngine->AddOnScreenDebugMessage(-1, 2000.0f, FColor::Green, FString::Printf(TEXT("Point: %s"), *el.ToString()));
	}
}

int32 APlanet::CreatePlanetSide(const FVector2D SquarePosition2D, const FVector SquareNormal, const FVector PointPosition, const float SquareRadius, const int32 MaxDivide, const int32 CurrentDivide, int32 LastVertex)
{
	if (CurrentDivide < MaxDivide)
	{
		const float HalfSquareRadius = SquareRadius / 2;
		FVector2D ChunkPosition2D;

		FVector2D LocalPointPosition2D = Vector3DTo2DByNormal(PointPosition, SquareNormal);

		// Right - Down
		ChunkPosition2D = FVector2D(SquarePosition2D.X - HalfSquareRadius, SquarePosition2D.Y - HalfSquareRadius);
		if (IsSquaresCollided(LocalPointPosition2D, LoadDistance, ChunkPosition2D, HalfSquareRadius)) // Right - Down
		{
			if (IsSquaresCollided(LocalPointPosition2D, LoadDistance / 8, ChunkPosition2D, HalfSquareRadius)) // Right - Down
			{
				// Recursive Self
				LastVertex = CreatePlanetSide(ChunkPosition2D, SquareNormal, PointPosition, HalfSquareRadius, LODsAmount, CurrentDivide + 1, LastVertex);
			}
			else
			{
				// Recursive Self
				LastVertex = CreatePlanetSide(ChunkPosition2D, SquareNormal, PointPosition, HalfSquareRadius, LODsAmount - 1, CurrentDivide + 1, LastVertex);
			}
		}
		else
		{
			// Create Chunk
			LastVertex = CreateChunk(ChunkPosition2D, SquareNormal, HalfSquareRadius, 999, LastVertex);
		}

		// Left - Down
		ChunkPosition2D = FVector2D(SquarePosition2D.X + HalfSquareRadius, SquarePosition2D.Y - HalfSquareRadius);
		if (IsSquaresCollided(LocalPointPosition2D, LoadDistance, ChunkPosition2D, HalfSquareRadius)) // Left - Down
		{
			if (IsSquaresCollided(LocalPointPosition2D, LoadDistance / 8, ChunkPosition2D, HalfSquareRadius)) // Left - Down
			{
				// Recursive Self
				LastVertex = CreatePlanetSide(ChunkPosition2D, SquareNormal, PointPosition, HalfSquareRadius, LODsAmount, CurrentDivide + 1, LastVertex);
			}
			else
			{
				// Recursive Self
				LastVertex = CreatePlanetSide(ChunkPosition2D, SquareNormal, PointPosition, HalfSquareRadius, LODsAmount - 1, CurrentDivide + 1, LastVertex);
			}
		}
		else
		{
			// Create Chunk
			LastVertex = CreateChunk(ChunkPosition2D, SquareNormal, HalfSquareRadius, 999, LastVertex);
		}

		// Right - Up
		ChunkPosition2D = FVector2D(SquarePosition2D.X - HalfSquareRadius, SquarePosition2D.Y + HalfSquareRadius);
		if (IsSquaresCollided(LocalPointPosition2D, LoadDistance, ChunkPosition2D, HalfSquareRadius)) // Right - Up
		{
			if (IsSquaresCollided(LocalPointPosition2D, LoadDistance / 8, ChunkPosition2D, HalfSquareRadius)) // Right - Up
			{
				// Recursive Self
				LastVertex = CreatePlanetSide(ChunkPosition2D, SquareNormal, PointPosition, HalfSquareRadius, LODsAmount, CurrentDivide + 1, LastVertex);
			}
			else
			{
				// Recursive Self
				LastVertex = CreatePlanetSide(ChunkPosition2D, SquareNormal, PointPosition, HalfSquareRadius, LODsAmount - 1, CurrentDivide + 1, LastVertex);
			}
		}
		else
		{
			// Create Chunk
			LastVertex = CreateChunk(ChunkPosition2D, SquareNormal, HalfSquareRadius, 999, LastVertex);
		}

		// Left - Up
		ChunkPosition2D = FVector2D(SquarePosition2D.X + HalfSquareRadius, SquarePosition2D.Y + HalfSquareRadius);
		if (IsSquaresCollided(LocalPointPosition2D, LoadDistance, ChunkPosition2D, HalfSquareRadius)) // Left - Up
		{
			if (IsSquaresCollided(LocalPointPosition2D, LoadDistance / 8, ChunkPosition2D, HalfSquareRadius)) // Left - Up
			{
				// Recursive Self
				LastVertex = CreatePlanetSide(ChunkPosition2D, SquareNormal, PointPosition, HalfSquareRadius, LODsAmount, CurrentDivide + 1, LastVertex);
			}
			else
			{
				// Recursive Self
				LastVertex = CreatePlanetSide(ChunkPosition2D, SquareNormal, PointPosition, HalfSquareRadius, LODsAmount - 1, CurrentDivide + 1, LastVertex);
			}
		}
		else
		{
			// Create Chunk
			LastVertex = CreateChunk(ChunkPosition2D, SquareNormal, HalfSquareRadius, 999, LastVertex);
		}
	}
	else 
	{
		// Create Chunk
		LastVertex = CreateChunk(SquarePosition2D, SquareNormal, SquareRadius, 999, LastVertex);
	}

	return LastVertex;
}

int32 APlanet::CreateChunk(const FVector2D ChunkPosition2D, const FVector ChunkNormal, const float ChunkRadius, const int32 Lod, const int32 StartVertex)
{
	// Create vertices
	Vertices.Add(Vector2DTo3DByNormal(FVector2D(ChunkPosition2D.X + ChunkRadius, ChunkPosition2D.Y - ChunkRadius), ChunkNormal));
	Vertices.Add(Vector2DTo3DByNormal(FVector2D(ChunkPosition2D.X - ChunkRadius, ChunkPosition2D.Y - ChunkRadius), ChunkNormal));
	Vertices.Add(Vector2DTo3DByNormal(FVector2D(ChunkPosition2D.X + ChunkRadius, ChunkPosition2D.Y + ChunkRadius), ChunkNormal));
	Vertices.Add(Vector2DTo3DByNormal(FVector2D(ChunkPosition2D.X - ChunkRadius, ChunkPosition2D.Y + ChunkRadius), ChunkNormal));

	// Create Triangles
	Triangles.Add(StartVertex + 0);
	Triangles.Add(StartVertex + 1);
	Triangles.Add(StartVertex + 2);

	Triangles.Add(StartVertex + 1);
	Triangles.Add(StartVertex + 3);
	Triangles.Add(StartVertex + 2);

	return StartVertex + 4;
}

FVector APlanet::Vector2DTo3DByNormal(const FVector2D Vector2D, const FVector Normal) ///////?????????
{
	return Normal.Rotation().RotateVector(FVector(Radius, Vector2D.X, Vector2D.Y));
}

FVector2D APlanet::Vector3DTo2DByNormal(const FVector Vector3D, const FVector Normal)
{
	FVector2D Vector2D = (0, 0, 0);
	if (Normal.X > 0)
	{
		Vector2D = FVector2D(Vector3D.Y, Vector3D.Z);
	}
	if (Normal.X < 0)
	{
		Vector2D = FVector2D(-Vector3D.Y, Vector3D.Z);
	}
	if (Normal.Y > 0)
	{
		Vector2D = FVector2D(-Vector3D.X, Vector3D.Z);
	}
	if (Normal.Y < 0)
	{
		Vector2D = FVector2D(Vector3D.X, Vector3D.Z);
	}
	if (Normal.Z > 0)
	{
		Vector2D = FVector2D(Vector3D.Y, -Vector3D.X);
	}
	if (Normal.Z < 0)
	{
		Vector2D = FVector2D(Vector3D.Y, Vector3D.X);
	}
	return Vector2D;
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