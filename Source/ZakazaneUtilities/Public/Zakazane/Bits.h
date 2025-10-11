// Copyright ZAKAZANE Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

namespace Zkz::Bits
{

namespace Private
{

template <int Bits>
constexpr uint64 GetLeftmostBitImpl(uint64 Value)
{
	// This is actually doable by a single intrinsic operation, but then we have to check if it's available and
	// probably not worth the hassle.

	if constexpr (Bits == 1)
	{
		return Value & 1;
	}
	else
	{
		constexpr int HalfBits = Bits / 2;

		if (const auto Lhs = GetLeftmostBitImpl<HalfBits>(Value >> HalfBits); Lhs != 0)
		{
			return Lhs << HalfBits;
		}
		// Technically this could be optimized to a single & operation, but I wholeheartedly believe the
		// optimizer will do it on it's own. Plus it's hard to imagine this could ever be a bottleneck ;)
		return GetLeftmostBitImpl<HalfBits>((Value << HalfBits) >> HalfBits);
	}
}

}  // namespace Private

// @returns the leftmost set bit of the given value. E.g. for 0b00010101 returns 0b00010000.
template <class IntType>
static constexpr IntType GetLeftmostBit(IntType Value)
{
	return static_cast<IntType>(Private::GetLeftmostBitImpl<sizeof(IntType) * 8>(static_cast<uint64>(Value)));
}

}  // namespace Zkz::Bits
