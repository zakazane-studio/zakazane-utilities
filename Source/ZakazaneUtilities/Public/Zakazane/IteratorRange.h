// Copyright ZAKAZANE Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

namespace Zkz
{

namespace Private
{

class FNullIterator
{
};

template <class IteratorType>
class TIteratorRangeAdapter
{
public:
	explicit TIteratorRangeAdapter(IteratorType InIterator) : Iterator{MoveTemp(InIterator)}
	{
	}

	decltype(auto) operator*() const
	{
		return *Iterator;
	}

	decltype(auto) operator->() const
	{
		return Iterator;
	}

	decltype(auto) operator++()
	{
		return ++Iterator;
	}

	explicit operator bool() const
	{
		return !!Iterator;
	}

	friend bool operator==(const TIteratorRangeAdapter& Lhs, const FNullIterator& Rhs)
	{
		return !Lhs;
	}

	friend bool operator!=(const TIteratorRangeAdapter& Lhs, const FNullIterator& Rhs)
	{
		return !(Lhs == Rhs);
	}

	friend bool operator==(const FNullIterator& Lhs, const TIteratorRangeAdapter& Rhs)
	{
		return !Rhs;
	}

	friend bool operator!=(const FNullIterator& Lhs, const TIteratorRangeAdapter& Rhs)
	{
		return !(Lhs == Rhs);
	}

private:
	IteratorType Iterator;
};

}  // namespace Private

/// Allows range-based for iteration for any type that exposes an unreal-style iterator.
template <class IteratorType>
class TIteratorRange
{
public:
	explicit TIteratorRange(IteratorType InIterator) : Iterator{MoveTemp(InIterator)}
	{
	}

private:
	IteratorType Iterator;

	// Note: must be called `begin` for c++ to pick-up for range-based for
	friend Private::TIteratorRangeAdapter<IteratorType> begin(const TIteratorRange& Range)
	{
		return Private::TIteratorRangeAdapter{Range.Iterator};
	}

	// Note: must be called `end` for c++ to pick-up for range-based for
	friend Private::FNullIterator end(const TIteratorRange& Range)
	{
		return Private::FNullIterator{};
	}
};

}  // namespace Zkz
