#include "Algo/Transform.h"
#include "Zakazane/Functional.h"
#include "Zakazane/Future.h"
#include "Zakazane/Test/Test.h"

namespace Zkz::Test
{

ZKZ_BEGIN_AUTOMATION_TEST(
	FFutureTest,
	"Zakazane.ZakazaneUtilities.Future",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

ZKZ_ADD_TEST(ScopedPromiseSetsOnDeleteIfNotExecutedOrMovedFrom)
{
	{
		const TFuture F = []
		{
			TScopedPromise<bool> P{TLiteralFunction<false>{}};
			TFuture F = P.GetFuture();
			P.SetValue(true);
			return F;
		}();

		TestEqual("SetValuePresentInFuture", F.Get(), true);
	}

	{
		const TFuture F = []
		{
			TScopedPromise<bool> P{TLiteralFunction<false>{}};
			TFuture F = P.GetFuture();
			return F;
		}();

		TestEqual("CancelledValuePresentInFuture", F.Get(), false);
	}

	{
		const TFuture F = []
		{
			TScopedPromise<bool> MovedFrom{TLiteralFunction<false>{}};
			TFuture F = MovedFrom.GetFuture();

			{
				TScopedPromise MovedTo{MoveTemp(MovedFrom)};
				MovedTo.SetValue(true);
			}

			return F;
		}();

		TestEqual("SetValuePresentInFutureMoved", F.Get(), true);
	}

	{
		const TFuture F = []
		{
			TScopedPromise<bool> MovedFrom{TLiteralFunction<false>{}};
			TFuture F = MovedFrom.GetFuture();

			{
				TScopedPromise MovedTo{MoveTemp(MovedFrom)};
			}

			return F;
		}();

		TestEqual("CancelledValuePresentInFutureMoved", F.Get(), false);
	}

	{
		const TFuture F = []
		{
			TScopedPromise<int> MovedFrom{TLiteralFunction<0>{}};
			TFuture F = MovedFrom.GetFuture();

			{
				TScopedPromise<int> MovedTo{TLiteralFunction<1>{}};
				MovedTo = MoveTemp(MovedFrom);
				MovedTo.SetValue(2);
			}

			return F;
		}();

		TestEqual("SetValuePresentInFutureMoveAssigned", F.Get(), 2);
	}

	{
		const TFuture F = []
		{
			TScopedPromise<int> MovedFrom{TLiteralFunction<0>{}};
			TFuture F = MovedFrom.GetFuture();

			{
				// ReSharper disable once CppEntityAssignedButNoRead
				TScopedPromise<int> MovedTo{TLiteralFunction<1>{}};
				MovedTo = MoveTemp(MovedFrom);
			}

			return F;
		}();

		TestEqual("CancelledValuePresentInFutureMoveAssigned", F.Get(), 0);
	}
}

ZKZ_ADD_TEST(AggregateFuturesAccumulatesResults)
{
	TArray<TPromise<int>> Promises;
	Promises.SetNum(10);

	TArray<TFuture<int>> Futures;
	for (TPromise<int>& Promise : Promises)
	{
		Futures.Emplace(Promise.GetFuture());
	}

	TArray<int> InitialResult;
	InitialResult.Reserve(Promises.Num());	// Reserve ensures initial result contains enough space to hold all
		// results. Thanks to this, the array should not be reallocated and we can verify the array is moved correctly
		// by checking that the data address is the same.
	const int* OrigData = InitialResult.GetData();

	const TFuture<TArray<int>> AggregatedFuture = AggregateFutures(
		MoveTemp(Futures),
		MoveTemp(InitialResult),
		[](TArray<int>&& Results, const int Result) -> TArray<int>
		{
			Results.Emplace(Result);
			return MoveTemp(Results);
		});

	for (int Idx = 0; Idx < Promises.Num(); ++Idx)
	{
		Promises[Idx].SetValue(Idx + 1);
	}

	const TArray<int>& Result = AggregatedFuture.Get();
	TestEqual("ResultsGivenInOrder", Result, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
	TestEqual("AggregateMovesResult", Result.GetData(), OrigData);
}

ZKZ_ADD_TEST(AggregateFuturesCanUseScopedPromise)
{
	const TFuture<int> AggregatedFuture = []
	{
		TArray<TScopedPromise<int>> Promises;
		// Scoped promise will return -1 if unfulfilled
		for (int Idx = 0; Idx < 10; ++Idx)
		{
			Promises.Emplace(TLiteralFunction<-1>{});
		}

		TArray<TFuture<int>> Futures;
		for (TScopedPromise<int>& Promise : Promises)
		{
			Futures.Emplace(Promise.GetFuture());
		}

		// Set value for odd numbers only
		for (int Idx = 1; Idx < Promises.Num(); Idx += 2)
		{
			Promises[Idx].SetValue(Idx);
		}

		return AggregateFutures(MoveTemp(Futures), 0, FSum{});
	}();

	// Result should be 1 + 3 + ... + 9 - 5 (for the even numbers) = 20
	TestEqual("AggregateFuturesAccumulatesResults", AggregatedFuture.Get(), 20);
}

ZKZ_END_AUTOMATION_TEST(FFutureTest);

}  // namespace Zkz::Test
