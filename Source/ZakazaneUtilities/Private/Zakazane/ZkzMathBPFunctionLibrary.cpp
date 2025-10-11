// Copyright ZAKAZANE Studio. All Rights Reserved.

#include "Zakazane/ZkzMathBPFunctionLibrary.h"

#include "Zakazane/Math.h"

FVector UZkzMathBPFunctionLibrary::MakeFromToVector(const FVector& From, const FVector& To)
{
	return Zkz::Math::MakeFromToVector(From, To);
}

float UZkzMathBPFunctionLibrary::AngleBetweenVectorsRad(const FVector& Vec1, const FVector& Vec2)
{
	return Zkz::Math::AngleBetweenVectorsRad(Vec1, Vec2);
}

float UZkzMathBPFunctionLibrary::AngleBetweenVectorsDeg(const FVector& Vec1, const FVector& Vec2)
{
	return Zkz::Math::AngleBetweenVectorsDeg(Vec1, Vec2);
}

float UZkzMathBPFunctionLibrary::SignedAngleBetweenVectorsRad(const FVector& Vec1, const FVector& Vec2)
{
	return Zkz::Math::SignedAngleBetweenVectorsRad(Vec1, Vec2);
}

float UZkzMathBPFunctionLibrary::SignedAngleBetweenVectorsDeg(const FVector& Vec1, const FVector& Vec2)
{
	return Zkz::Math::SignedAngleBetweenVectorsDeg(Vec1, Vec2);
}

TArray<FVector> UZkzMathBPFunctionLibrary::GetShortenedPath(
	const TArray<FVector>& Path, const float ShortenBy, const bool bReversed)
{
	return Zkz::Math::GetShortenedPath(Path, ShortenBy, bReversed);
}

float UZkzMathBPFunctionLibrary::SnapIfNear(const float InputValue, const float SnapValue, const float Threshold)
{
	return Zkz::Math::SnapIfNear(InputValue, SnapValue, Threshold);
}
