// Copyright ZAKAZANE Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "ZkzMathBPFunctionLibrary.generated.h"

/// Contains utility math functions for blueprints. Implementations should just call functions from Math.h.
UCLASS(Category = "Zakazane|Math")
class ZAKAZANEUTILITIES_API UZkzMathBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/// Creates a translation vector between two points.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FVector MakeFromToVector(const FVector& From, const FVector& To);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static float AngleBetweenVectorsRad(const FVector& Vec1, const FVector& Vec2);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static float AngleBetweenVectorsDeg(const FVector& Vec1, const FVector& Vec2);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static float SignedAngleBetweenVectorsRad(const FVector& Vec1, const FVector& Vec2);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static float SignedAngleBetweenVectorsDeg(const FVector& Vec1, const FVector& Vec2);

	/// Takes a path represented as an array of points and shortens it by a given distance.
	/// @param ShortenBy total length removed from the path
	/// @param bReversed if true, removes elements / shortens segments at the start of the path instead of the end
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static TArray<FVector> GetShortenedPath(const TArray<FVector>& Path, float ShortenBy, bool bReversed = false);

	/// Returns SnapValue if the InputValue is within Threshold of it. Otherwise, returns InputValue.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static float SnapIfNear(float InputValue, float SnapValue, float Threshold);
};
