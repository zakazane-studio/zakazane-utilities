// Copyright ZAKAZANE Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Misc/TVariant.h"

namespace Zkz
{

/// Represents a lazy value. At construction takes an "evaluator", being a function returning the value of the wanted
/// type. When GetValue is called for the first time, the value is calculated and stored for future access.
template <class InValueType>
class TLazy
{
public:
	using ValueType = InValueType;
	using EvaluatorType = TFunction<ValueType()>;

	explicit TLazy(EvaluatorType Evaluator);

	const ValueType& GetValue() const;

	bool IsEvaluated() const;

private:
	mutable TVariant<ValueType, EvaluatorType> Value;
};

// -- implementation

template <class InValueType>
TLazy<InValueType>::TLazy(EvaluatorType Evaluator) : Value{TInPlaceType<EvaluatorType>{}, MoveTemp(Evaluator)}
{
}

template <class InValueType>
// ReSharper disable once CppRedundantTypenameKeyword : causes compilation error
const typename TLazy<InValueType>::ValueType& TLazy<InValueType>::GetValue() const
{
	if (auto* Evaluator = Value.template TryGet<EvaluatorType>())
	{
		Value.template Emplace<ValueType>(Invoke(*Evaluator));
	}

	return Value.template Get<ValueType>();
}

template <class InValueType>
bool TLazy<InValueType>::IsEvaluated() const
{
	return Value.template IsType<ValueType>();
}

template <class EvaluatorType>
TLazy(EvaluatorType) -> TLazy<decltype(std::declval<EvaluatorType>()())>;

}  // namespace Zkz
