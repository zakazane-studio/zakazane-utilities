// Copyright ZAKAZANE Studio. All Rights Reserved.

namespace Zkz::Container
{

/// Calculates a correct index in a circular buffer. E.g.
///
/// @code
/// GetCircularBufferIndex(0, 3) == 0
/// GetCircularBufferIndex(1, 3) == 1
/// GetCircularBufferIndex(2, 3) == 2
/// GetCircularBufferIndex(3, 3) == 0
/// GetCircularBufferIndex(4, 3) == 1
/// GetCircularBufferIndex(5, 3) == 2
/// ...
///
/// GetCircularBufferIndex(0, 3) == 0
/// GetCircularBufferIndex(-1, 3) == 2
/// GetCircularBufferIndex(-2, 3) == 1
/// GetCircularBufferIndex(-3, 3) == 0
/// GetCircularBufferIndex(-4, 3) == 2
/// GetCircularBufferIndex(-5, 3) == 1
/// @endcode
constexpr int32 GetCircularBufferIndex(const int32 Index, const int32 CollectionNum)
{
	const int32 Mod = Index % CollectionNum;
	return Mod >= 0 ? Mod : CollectionNum + Mod;
}

static_assert(GetCircularBufferIndex(0, 3) == 0);
static_assert(GetCircularBufferIndex(1, 3) == 1);
static_assert(GetCircularBufferIndex(2, 3) == 2);
static_assert(GetCircularBufferIndex(3, 3) == 0);
static_assert(GetCircularBufferIndex(4, 3) == 1);
static_assert(GetCircularBufferIndex(5, 3) == 2);
static_assert(GetCircularBufferIndex(0, 3) == 0);
static_assert(GetCircularBufferIndex(-1, 3) == 2);
static_assert(GetCircularBufferIndex(-2, 3) == 1);
static_assert(GetCircularBufferIndex(-3, 3) == 0);
static_assert(GetCircularBufferIndex(-4, 3) == 2);
static_assert(GetCircularBufferIndex(-5, 3) == 1);

/// Returns the next index in a circular buffer. E.g.:
/// @code
/// GetNextCircularBufferIndex(0, 3) == 1
/// GetNextCircularBufferIndex(1, 3) == 2
/// GetNextCircularBufferIndex(2, 3) == 0
/// @endcode
constexpr int32 GetNextCircularBufferIndex(const int32 Index, const int32 CollectionNum)
{
	return GetCircularBufferIndex(Index + 1, CollectionNum);
}

static_assert(GetNextCircularBufferIndex(0, 3) == 1);
static_assert(GetNextCircularBufferIndex(1, 3) == 2);
static_assert(GetNextCircularBufferIndex(2, 3) == 0);

/// Returns the previous index in a circular buffer. E.g.:
/// @code
/// GetNextCircularBufferIndex(0, 3) == 1
/// GetNextCircularBufferIndex(1, 3) == 2
/// GetNextCircularBufferIndex(2, 3) == 0
/// @endcode
constexpr int32 GetPreviousCircularBufferIndex(const int32 Index, const int32 CollectionNum)
{
	return GetCircularBufferIndex(Index - 1, CollectionNum);
}

static_assert(GetNextCircularBufferIndex(0, 3) == 1);
static_assert(GetNextCircularBufferIndex(1, 3) == 2);
static_assert(GetNextCircularBufferIndex(2, 3) == 0);

}  // namespace Zkz::Container
