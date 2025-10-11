#include "Zakazane/Math.h"

namespace Zkz::Math
{

TArray<FVector> GetShortenedPath(const TArrayView<const FVector> Path, float ShortenBy, const bool bReversed)
{
	// #TODO making a copy of the original path and then removing elements is not necessarily super effective
	// especially when bReversed is true. We could do this better by just iterating over elements to be removed
	// and then only copying the elements that are supposed to stay.
	TArray<FVector> Result{Path};

	while (ShortenBy > 0 && Result.Num() >= 2)
	{
		const int LastSegmentEndIndex = bReversed ? 0 : Result.Num() - 1;
		const int LastSegmentStartIndex = bReversed ? 1 : Result.Num() - 2;

		const float LastSegmentLength = FVector::Distance(Result[LastSegmentStartIndex], Result[LastSegmentEndIndex]);
		if (LastSegmentLength <= ShortenBy)
		{
			Result.RemoveAt(LastSegmentEndIndex);
			ShortenBy -= LastSegmentLength;
		}
		else
		{
			const FVector Direction = (Result[LastSegmentEndIndex] - Result[LastSegmentStartIndex]).GetSafeNormal();

			Result[LastSegmentEndIndex] = Result[LastSegmentStartIndex] + Direction * (LastSegmentLength - ShortenBy);
			return Result;
		}
	}

	if (Result.Num() == 1)
	{
		Result.Empty();
	}

	return Result;
}

}  // namespace Zkz::Math
