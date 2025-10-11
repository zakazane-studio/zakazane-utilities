// // Copyright ZAKAZANE Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "ZkzSegment.generated.h"

USTRUCT(BlueprintType, Category = "Zakazane", DisplayName = "Segment")
struct ZAKAZANEUTILITIES_API FZkzSegment
{
	GENERATED_BODY()

	FZkzSegment() = default;

	FZkzSegment(const FVector& InStartPoint, const FVector& InEndPoint) : Start(InStartPoint), End(InEndPoint)
	{
	}

	/// Returns a new segment with its ends moved inwards by a given offset. The segment keeps its direction,
	/// only it's length changes.
	/// Note that offsetting by more than the length of the segment will not yield a zero-length segment, but
	/// rather flip the ends.
	[[nodiscard]] FZkzSegment OffsetEndsInward(float StartOffset, float EndOffset) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Start = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector End = FVector::ZeroVector;
};

UCLASS(Category = "Zakazane")
class ZAKAZANEUTILITIES_API UZkzSegmentFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/// Creates a segment with its ends moved inwards by a given offset. The segment keeps its direction,
	/// only it's length changes.
	/// Note that offsetting by more than the length of the segment will not yield a zero-length segment, but
	/// rather flip the ends.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	[[nodiscard]] static FZkzSegment OffsetSegmentEndsInward(
		const FZkzSegment& Segment, float StartOffset, float EndOffset);
};
