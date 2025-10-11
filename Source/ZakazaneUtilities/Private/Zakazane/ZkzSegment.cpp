#include "Zakazane/ZkzSegment.h"

#include "Zakazane/Math.h"

FZkzSegment FZkzSegment::OffsetEndsInward(const float StartOffset, const float EndOffset) const
{
	FZkzSegment Result;

	const FVector Direction = Zkz::Math::MakeFromToVector(Start, End).GetSafeNormal();

	Result.Start = Start + Direction * StartOffset;
	Result.End = End - Direction * EndOffset;
	return Result;
}

FZkzSegment UZkzSegmentFunctionLibrary::OffsetSegmentEndsInward(
	const FZkzSegment& Segment, const float StartOffset, const float EndOffset)
{
	return Segment.OffsetEndsInward(StartOffset, EndOffset);
}
