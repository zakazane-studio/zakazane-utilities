// Copyright ZAKAZANE Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Bits.h"

namespace Zkz
{

template <class FlagType>
struct TMaxFlag;

template <class FlagType>
constexpr auto TMaxFlagV = TMaxFlag<FlagType>::Value;

/// Simplifies and provides a more secure way of using masks of enum flags without the necessity of using casts.
/// @tparam FlagType - the underlying flag type. Must be an enum. There must exist a specialization of the TMaxFlag
/// 	template struct for this enum, containing a static constexpr Value equal to the maximum available flag.
/// 	E.g.:
/// 	@code
/// 	template <>
/// 	struct Zkz::TMaxFlag<EUEnum>
/// 	{
/// 		static constexpr auto Value = static_cast<uint8>(EUEnum::Flag4);
/// 	};
/// 	@endcode
template <class FlagType>
class TMask
{
public:
	static_assert(TIsEnum<FlagType>::Value);

	using MaskType = std::underlying_type_t<FlagType>;

	/// Zeroes-out all bits not used in the enum flag type. Any existing junk values may incorrectly affect binary
	/// operation results.
	static constexpr MaskType Sanitize(MaskType Value)
	{
		// Flags are allowed to be non-single bits, so can't just use (MaxFlagV - 1) to get all bits to the right.
		constexpr auto MaxFlagV = TMaxFlagV<FlagType>;
		constexpr auto MaxFlagLeftmostBit = Bits::GetLeftmostBit(MaxFlagV);

		// MaxSanitary has all bits set to one until (and including) the MaxFlagV leftmost bit.
		constexpr auto MaxSanitary = MaxFlagV | (MaxFlagLeftmostBit - 1);

		return Value & MaxSanitary;
	}

	constexpr TMask() = default;

	constexpr /*implicit*/ TMask(FlagType InFlag) : Mask{static_cast<MaskType>(InFlag)}
	{
	}

	constexpr explicit TMask(MaskType InMask) : Mask{Sanitize(InMask)}
	{
		ensureMsgf(
			Sanitize(InMask) == InMask,
			TEXT("Provided mask %d has bits set to 1 for non-flag values. These will be cleared."),
			InMask);
	}

	constexpr bool HasAny(TMask InMask) const
	{
		return (*this & InMask) != TMask{0};
	}

	constexpr bool HasAll(TMask InMask) const
	{
		return (*this & InMask) == InMask;
	}

	/// @returns a version of this mask with all given flags cleared.
	[[nodiscard]] constexpr TMask GetCleared(TMask InMask) const
	{
		return (*this & ~InMask);
	}

	constexpr auto Value() const
	{
		return Mask;
	}

	// -- Mask operators

	constexpr explicit operator bool() const
	{
		return Mask != 0;
	}

	constexpr friend bool operator==(TMask Lhs, TMask Rhs)
	{
		return Lhs.Mask == Rhs.Mask;
	}

	constexpr friend bool operator!=(TMask Lhs, TMask Rhs)
	{
		return !(Lhs == Rhs);
	}

	constexpr TMask operator~() const
	{
		return TMask{Sanitize(~Mask)};
	}

	constexpr friend TMask operator|(TMask Lhs, TMask Rhs)
	{
		return TMask{static_cast<MaskType>(Lhs.Mask | Rhs.Mask)};
	}

	constexpr friend TMask operator&(TMask Lhs, TMask Rhs)
	{
		return TMask{static_cast<MaskType>(Lhs.Mask & Rhs.Mask)};
	}

	// -- Mask / Flag operators

	constexpr friend TMask operator|(TMask InMask, FlagType InFlag)
	{
		return InMask | TMask{InFlag};
	}

	constexpr friend TMask operator|(FlagType InFlag, TMask InMask)
	{
		return InMask | InFlag;
	}

	constexpr friend TMask operator&(TMask InMask, FlagType InFlag)
	{
		return InMask & TMask{InFlag};
	}

	constexpr friend TMask operator&(FlagType InFlag, TMask InMask)
	{
		return InMask & InFlag;
	}

private:
	MaskType Mask = 0;
};

}  // namespace Zkz
