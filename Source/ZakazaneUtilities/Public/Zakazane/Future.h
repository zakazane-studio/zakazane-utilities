// Copyright ZAKAZANE Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Async/Future.h"
#include "ReturnIfMacros.h"

namespace Zkz
{

// #TODO #Promise: Review usages of TPromise and potentially replace with TScopedPromise
/// Wrapper for TPromise gracefully handling destruction prior to being fulfilled. In case this happens it sets the
/// promise value to the result of the CancelledValueFunc function passed to the constructor.
template <class T UE_REQUIRES(!std::is_void_v<T>)>
class TScopedPromise
{
public:
	explicit TScopedPromise(TFunction<T()> InCancelledValueFunc) : CancelledValueFunc{MoveTemp(InCancelledValueFunc)}
	{
	}

	TScopedPromise(TUniqueFunction<void()>&& CompletionCallback, TFunction<T()> InCancelledValueFunc)
		: Promise{MoveTemp(CompletionCallback)}, CancelledValueFunc{MoveTemp(InCancelledValueFunc)}
	{
	}

	TScopedPromise(TScopedPromise&& Other)
		: Promise{MoveTemp(Other.Promise)}
		, CancelledValueFunc{MoveTemp(Other.CancelledValueFunc)}
		, bFulfilled{MoveTemp(Other.bFulfilled)}
	{
		Other.bFulfilled = true;
	}

	TScopedPromise& operator=(TScopedPromise&& Other)
	{
		ZKZ_RETURN_IF(this == &Other, *this);

		Promise = MoveTemp(Other.Promise);
		CancelledValueFunc = MoveTemp(Other.CancelledValueFunc);
		bFulfilled = Other.bFulfilled;

		Other.bFulfilled = true;
		return *this;
	}

	~TScopedPromise()
	{
		// #TODO #Promise: Should have IsFulfilled in TPromise (push request)
		if (!bFulfilled)
		{
			SetValue(CancelledValueFunc());
		}
	}

	template <class... ArgTypes>
	void EmplaceValue(ArgTypes&&... Args)
	{
		bFulfilled = true;
		Promise.EmplaceValue(Forward<ArgTypes>(Args)...);
	}

	template <class... ArgTypes>
	void SetValue(ArgTypes&&... Args)
	{
		bFulfilled = true;
		Promise.SetValue(Forward<ArgTypes>(Args)...);
	}

	TFuture<T> GetFuture()
	{
		return Promise.GetFuture();
	}

private:
	TPromise<T> Promise;

	TFunction<T()> CancelledValueFunc;

	bool bFulfilled = false;
};

namespace AggregateFuturesPrivate
{

template <class FutureType, class ResultType, class AggregateFuncType>
auto DoAggregateFutures(
	const TArrayView<TFuture<FutureType>> Futures, ResultType&& Initial, AggregateFuncType&& AggregateFunc)
{
	// The example work done for (Initial, {Fut0, Fut1, Fut2}) is:
	// * creates Promise {Fut0, Fut1, Fut2}
	// * set Next handler for Fut0 - the handler:
	// ** calls the aggregate function for (Initial, Fut0)
	// ** calls aggregate futures for (result from line above, {Fut1, Fut2}) recursively
	// ** sets a Next handler for that call to fulfil the result promise

	TPromise<ResultType> Promise;
	auto AggregatedFuture = Promise.GetFuture();

	if (Futures.IsEmpty())
	{
		Promise.SetValue(Forward<ResultType>(Initial));
		return AggregatedFuture;
	}

	auto& Head = Futures[0];

	Head.Next(
		[Tail = Futures.RightChop(1),
		 Promise = MoveTemp(Promise),
		 Initial = Forward<ResultType>(Initial),
		 AggregateFunc = MoveTemp(AggregateFunc)]<class FutureResultType>(FutureResultType&& FutureResult) mutable
		{
			DoAggregateFutures(
				MoveTemp(Tail),
				::Invoke(AggregateFunc, Forward<ResultType>(Initial), Forward<FutureResultType>(FutureResult)),
				MoveTemp(AggregateFunc))
				.Next([Promise = MoveTemp(Promise)]<class FinalResultType>(FinalResultType&& FinalResult) mutable
					  { Promise.SetValue(Forward<FinalResultType>(FinalResult)); });
		});

	return AggregatedFuture;
}

}  // namespace AggregateFuturesPrivate

/// Creates a single future from multiple futures. The result value is built by calling the given aggregate func.
/// The aggregate func is a binary function taking the accumulated result (or the Initial value) and a future
/// result. The futures are aggregated in order passed to the Futures argument.
template <class FutureType, class ResultType, class AggregateFuncType>
auto AggregateFutures(TArray<TFuture<FutureType>> Futures, ResultType&& Initial, AggregateFuncType&& AggregateFunc)
{
	using namespace AggregateFuturesPrivate;

	// This is a recursive function calling a helper DoAggregateFutures. This is so that the futures are passed by value
	// here (need to be kept alive in the Next handler until the whole set is ready) but we can offset into it using
	// array view. The initial Futures array is kept alive by that last Next handler

	TPromise<ResultType> Promise;
	auto AggregatedFuture = Promise.GetFuture();

	DoAggregateFutures(MakeArrayView(Futures), Forward<ResultType>(Initial), Forward<AggregateFuncType>(AggregateFunc))
		.Next([Futures = MoveTemp(Futures),
			   Promise = MoveTemp(Promise),
			   Initial = Forward<ResultType>(Initial),
			   AggregateFunc = MoveTemp(AggregateFunc)]<class AggregatedResultType>(
				  AggregatedResultType&& Result) mutable { Promise.SetValue(Forward<AggregatedResultType>(Result)); });

	return AggregatedFuture;
}

}  // namespace Zkz
