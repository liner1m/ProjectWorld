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
	CreateAllChunks();
}

void APlanet::CreateAllChunks()
{
	// (FVector(1, MyCharacterPos.Y / MyCharacterPos.X, MyCharacterPos.Z / MyCharacterPos.X) * Radius).GridSnap(LOD0ChunkSize); ////????

	ClearPlanet();

	////////////////

	
	const FVector MyCharacterPosOnCube = MyCharacterPos.BoundToCube(Radius);

	// Debug
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::Printf(TEXT("Point: %s"), *MyCharacterPosOnCube.ToString()));

	//FVector::Dist(MyCharacterPosOnCube, FVector(Radius, -Radius + Radius * 2 * i, -Radius + Radius * 2 * j))

	CreateSquare(FVector(Radius, 0, 0), MyCharacterPosOnCube, Radius * 2, 0, 0);
	
	

	//Debug
	for (auto& el : Vertices)
	{
		DrawDebugPoint(GetWorld(), el, 10, FColor().Red, true, false, 1.0f);
		//GEngine->AddOnScreenDebugMessage(-1, 2000.0f, FColor::Green, FString::Printf(TEXT("Point: %s"), *el.ToString()));
	}

	////////////////


	// LOD0 Chunk
	//CreateChunk(1);

	// LOD1 Chunk
	//CreateChunk(MyCharacterWorldPosOnGrid + FVector(0, LOD0ChunkSize * 1, 0), LOD0ChunkSize * 1, LOD0ChunkSquareSize * 3, ChunkSquaresAmount / 3);
	// LOD2 Chunk
	//CreateChunk(MyCharacterWorldPosOnGrid + FVector(0, LOD0ChunkSize * 3, 0), LOD0ChunkSize * 3, LOD0ChunkSquareSize * 9, ChunkSquaresAmount / 9);
	// LOD3 Chunk
	//CreateChunk(MyCharacterWorldPosOnGrid + FVector(0, LOD0ChunkSize * 9, 0), LOD0ChunkSize * 9, LOD0ChunkSquareSize * 27, ChunkSquaresAmount / 27);
	// LOD4 Chunk
	//CreateChunk(MyCharacterWorldPosOnGrid + FVector(0, LOD0ChunkSize * 27, 0), LOD0ChunkSize * 27, LOD0ChunkSquareSize * 81, ChunkSquaresAmount / 81);

	// Create Mesh
	ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), false);

	// Debug
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::Printf(TEXT("PosSnap: %s"), *MyCharacterWorldPosOnGrid.ToString()));
}

int32 APlanet::CreateSquare(const FVector SquarePosition, const FVector PointPosition, const float SquareSize, const int32 CurrentDivide, int32 LastVertex)
{
	if (CurrentDivide < LODsAmount)
	{
		const float HalfSquareSize = SquareSize / 2;

		if (SquarePosition.Y < PointPosition.Y || SquarePosition.Z < PointPosition.Z) // Right - Down
		{
			// Create Vertices
			Vertices.Add(FVector(SquarePosition.X, SquarePosition.Y, SquarePosition.Z - HalfSquareSize));
			Vertices.Add(FVector(SquarePosition.X, SquarePosition.Y - HalfSquareSize, SquarePosition.Z - HalfSquareSize));
			Vertices.Add(FVector(SquarePosition.X, SquarePosition.Y, SquarePosition.Z));
			Vertices.Add(FVector(SquarePosition.X, SquarePosition.Y - HalfSquareSize, SquarePosition.Z));

			// Create Triangles
			Triangles.Add(LastVertex + 0);
			Triangles.Add(LastVertex + 1);
			Triangles.Add(LastVertex + 2);

			Triangles.Add(LastVertex + 1);
			Triangles.Add(LastVertex + 3);
			Triangles.Add(LastVertex + 2);

			// Increase LastVertex
			LastVertex += 4;
		}
		else
		{
			// Recursive Self
			LastVertex = CreateSquare(FVector(Radius, SquarePosition.Y - HalfSquareSize / 2, SquarePosition.Z - HalfSquareSize / 2), PointPosition, HalfSquareSize, CurrentDivide + 1, LastVertex);
		}

		if (SquarePosition.Y > PointPosition.Y || SquarePosition.Z < PointPosition.Z) // Left - Down
		{
			// Create Vertices
			Vertices.Add(FVector(SquarePosition.X, SquarePosition.Y + HalfSquareSize, SquarePosition.Z - HalfSquareSize));
			Vertices.Add(FVector(SquarePosition.X, SquarePosition.Y, SquarePosition.Z - HalfSquareSize));
			Vertices.Add(FVector(SquarePosition.X, SquarePosition.Y + HalfSquareSize, SquarePosition.Z));
			Vertices.Add(FVector(SquarePosition.X, SquarePosition.Y, SquarePosition.Z));

			// Create Triangles
			Triangles.Add(LastVertex + 0);
			Triangles.Add(LastVertex + 1);
			Triangles.Add(LastVertex + 2);

			Triangles.Add(LastVertex + 1);
			Triangles.Add(LastVertex + 3);
			Triangles.Add(LastVertex + 2);

			// Increase LastVertex
			LastVertex += 4;
		}
		else
		{
			// Recursive Self
			LastVertex = CreateSquare(FVector(Radius, SquarePosition.Y + HalfSquareSize / 2, SquarePosition.Z - HalfSquareSize / 2), PointPosition, HalfSquareSize, CurrentDivide + 1, LastVertex);
		}

		if (SquarePosition.Y <= PointPosition.Y || SquarePosition.Z >= PointPosition.Z) // Right - Up
		{
			// Create Vertices
			Vertices.Add(FVector(SquarePosition.X, SquarePosition.Y, SquarePosition.Z));
			Vertices.Add(FVector(SquarePosition.X, SquarePosition.Y - HalfSquareSize, SquarePosition.Z));
			Vertices.Add(FVector(SquarePosition.X, SquarePosition.Y, SquarePosition.Z + HalfSquareSize));
			Vertices.Add(FVector(SquarePosition.X, SquarePosition.Y - HalfSquareSize, SquarePosition.Z + HalfSquareSize));

			// Create Triangles
			Triangles.Add(LastVertex + 0);
			Triangles.Add(LastVertex + 1);
			Triangles.Add(LastVertex + 2);

			Triangles.Add(LastVertex + 1);
			Triangles.Add(LastVertex + 3);
			Triangles.Add(LastVertex + 2);

			// Increase LastVertex
			LastVertex += 4;
		}
		else
		{
			// Recursive Self
			LastVertex = CreateSquare(FVector(Radius, SquarePosition.Y - HalfSquareSize / 2, SquarePosition.Z + HalfSquareSize / 2), PointPosition, HalfSquareSize, CurrentDivide + 1, LastVertex);
		}

		if (SquarePosition.Y >= PointPosition.Y || SquarePosition.Z >= PointPosition.Z) // Left - Up
		{
			// Create Vertices
			Vertices.Add(FVector(SquarePosition.X, SquarePosition.Y + HalfSquareSize, SquarePosition.Z));
			Vertices.Add(FVector(SquarePosition.X, SquarePosition.Y, SquarePosition.Z));
			Vertices.Add(FVector(SquarePosition.X, SquarePosition.Y + HalfSquareSize, SquarePosition.Z + HalfSquareSize));
			Vertices.Add(FVector(SquarePosition.X, SquarePosition.Y, SquarePosition.Z + HalfSquareSize));

			// Create Triangles
			Triangles.Add(LastVertex + 0);
			Triangles.Add(LastVertex + 1);
			Triangles.Add(LastVertex + 2);

			Triangles.Add(LastVertex + 1);
			Triangles.Add(LastVertex + 3);
			Triangles.Add(LastVertex + 2);

			// Increase LastVertex
			LastVertex += 4;
		}
		else
		{
			// Recursive Self
			LastVertex = CreateSquare(FVector(Radius, SquarePosition.Y + HalfSquareSize / 2, SquarePosition.Z + HalfSquareSize / 2), PointPosition, HalfSquareSize, CurrentDivide + 1, LastVertex);
		}
	}

	return LastVertex;
}

void APlanet::CreateChunk(const int32 LODNumber)
{
	const float MaxChunkSize = Radius * 2;
	const int32 WorldSideChunksAmount = FMath::Pow(3, Density);
	const float WorldGridSize = MaxChunkSize / WorldSideChunksAmount;

	const int32 ChunkSquaresAmount = WorldSideChunksAmount / FMath::Pow(3, LODNumber);
	const float ChunkSize = WorldGridSize;
	const float ChunkSquareSide = (ChunkSize / ChunkSquaresAmount);

	// MyCharacterWorldPosOnGrid
	const FVector ChunkPosition = ProjectToCubeTowardsCenter(MyCharacterPos, Radius).GridSnap(WorldGridSize);

	const int32 ChunkSideVertexAmount = ChunkSquaresAmount + 1;
	const float HalfChunkSize = ChunkSize / 2 + 0.001; //////!!!! + 0.001


	if (Density >= 1 && Density <= MaxDensity && LoadDistance > 0 && Density >= LODNumber)
	{
		//  Create Cube Vertices
		for (float j = -HalfChunkSize; j <= HalfChunkSize; j += ChunkSquareSide)
		{
			for (float i = -HalfChunkSize; i <= HalfChunkSize; i += ChunkSquareSide)
			{
				Vertices.Add(FVector(Radius, i + ChunkPosition.Y, j + ChunkPosition.Z).GetSafeNormal() * Radius);
			}
		}/*
		for (int32 j = -Radius; j <= Radius; j += ChunkSquareSide)
		{
			for (int32 i = -Radius; i <= Radius; i += ChunkSquareSide)
			{
				Vertices.Add(FVector(i, Radius, j).GetSafeNormal() * Radius);
			}
		}*/
	}
	

	// Debug
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("Point: %i"), Vertices.Num()));

	//Debug
	for (auto& el : Vertices)
	{
		DrawDebugPoint(GetWorld(), el, 10, FColor().Red, true, false, 1.0f);
		//GEngine->AddOnScreenDebugMessage(-1, 2000.0f, FColor::Green, FString::Printf(TEXT("Point: %s"), *el.ToString()));
	}

	// Create Triangles
	//for (int32 i = 0; i < Vertices.Num(); ++i)
	//{
	//	if (true)
	//	{
	//		//Left Triangle
	//		Triangles.Add(i + 1);
	//		Triangles.Add(i);
	//		Triangles.Add(i + ChunkSideVertexAmount + 1);

	//		//Right Triangle
	//		Triangles.Add(i);
	//		Triangles.Add(i + ChunkSideVertexAmount);
	//		Triangles.Add(i + ChunkSideVertexAmount + 1);
	//	}
	//}

	// Create UVs
	/*UVs.Add(FVector2D(1, 1));
	UVs.Add(FVector2D(0, 1));
	UVs.Add(FVector2D(1, 0));
	UVs.Add(FVector2D(0, 0));*/

	// Create Mesh
	//ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), false);

	// Set Material
	/*if (Material)
	{
		ProceduralMesh->SetMaterial(0, Material);
	}*/
}

void APlanet::ClearPlanet()
{
	ProceduralMesh->ClearAllMeshSections();
	Vertices.Empty();
	Triangles.Empty();
	UVs.Empty();
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




//const float MaxChunkSize = Radius * 2;
//const int32 GlobalChunkSquaresAmount = (1 + (Density - 1) * 2);
//
//const int32 ChunkSquaresAmount = GlobalChunkSquaresAmount;
//const float ChunkSize = MaxChunkSize / (GlobalChunkSquaresAmount * GlobalChunkSquaresAmount);
//const float ChunkSquareSide = ChunkSize / ChunkSquaresAmount;





//const float MinAngle = 15; //Have to be multiple of 5
	//const int32 PlanetSideVertexAmount = 90 / MinAngle + 1;

	////Create Cube Vertices
	//for (int32 j = -45; j <= 45; j += MinAngle)
	//{
	//	for (int32 i = -45; i <= 45; i += MinAngle)
	//	{
	//		Vertices.Add(FVector(FRotator(j, i, 0).RotateVector(FVector().ForwardVector) * Radius));
	//	}
	//}

	////Debug
	//for (auto &el : Vertices)
	//{
	//	DrawDebugPoint(GetWorld(), el, 10, FColor().Red, true);
	//	//GEngine->AddOnScreenDebugMessage(-1, 2000.0f, FColor::Green, FString::Printf(TEXT("Point: %s"), *el.ToString()));
	//}

	////Create Triangles
	//for (int32 i = 0; i < Vertices.Num(); ++i)
	//{
	//	if (i % PlanetSideVertexAmount != 6 && i < PlanetSideVertexAmount * (PlanetSideVertexAmount - 1))
	//	{
	//		//Left Triangle
	//		Triangles.Add(i + 1);
	//		Triangles.Add(i);
	//		Triangles.Add(i + PlanetSideVertexAmount + 1);

	//		//Right Triangle
	//		Triangles.Add(i);
	//		Triangles.Add(i + PlanetSideVertexAmount);
	//		Triangles.Add(i + PlanetSideVertexAmount + 1);
	//	}
	//}

	//Debug
	//GEngine->AddOnScreenDebugMessage(-1, 2000.0f, FColor::Red, FString::Printf(TEXT("Point: %i"), Vertices.Num()));

	//Create UVs
	/*UVs.Add(FVector2D(1, 1));
	UVs.Add(FVector2D(0, 1));
	UVs.Add(FVector2D(1, 0));
	UVs.Add(FVector2D(0, 0));*/

	// Create Mesh
	//ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), false);

	// Set Material
	/*if (Material)
	{
		ProceduralMesh->SetMaterial(0, Material);
	}*/