#include "Zakazane/Math.h"
#include "Zakazane/Test/Test.h"

#include <array>

namespace Zkz::Math::Test
{

ZKZ_BEGIN_AUTOMATION_TEST(
	FMathTest,
	"Zakazane.ZakazaneUtilities.Math",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

ZKZ_ADD_TEST(GetVectorAbsMaxComponentIndexTest)
{
	TestEqual("Doc example 1", GetVectorAbsMaxComponentIndex({1, 5, 2}), 1);
	TestEqual("Doc example 2", GetVectorAbsMaxComponentIndex({1, -5, 2}), 1);
	TestEqual("Doc example 3", GetVectorAbsMaxComponentIndex({-3, 3, 2}), 0);
}

ZKZ_ADD_TEST(VectorSetComponentTest)
{
	FVector V{0, 1, 2};
	VectorSetComponent(V, 1, 3);

	TestEqual("Doc example 1", V, {0, 3, 2});
}

ZKZ_ADD_TEST(MakeVectorSetComponentTest)
{
	TestEqual("Doc example 1", MakeVectorSetComponent({0, 1, 2}, 1, 3), {0, 3, 2});
	TestEqual("Doc example 2", MakeVectorSetComponent(1, 3), {0, 3, 0});
}

ZKZ_ADD_TEST(SnapIfNearTest)
{
	const FVector OneTwoThree{1.0, 2.0, 3.0};
	const FVector AlmostOneTwoThree{1.1, 2.1, 3.1};
	const FVector AwayFromOneTwoThree{2.0, 2.0, 2.0};
	constexpr double Threshold = 0.2;

	TestEqual("Snaps near points", SnapIfNear(AlmostOneTwoThree, OneTwoThree, Threshold), OneTwoThree);
	TestEqual("Doesn't snap away points", SnapIfNear(AwayFromOneTwoThree, OneTwoThree, Threshold), AwayFromOneTwoThree);
}

ZKZ_ADD_TEST(ShortenPathTest)
{
	// Each segment has length 1, except for last which has length 3.
	const std::array Path =
		{FVector{0.0, 0.0, 0.0},
		 FVector{1.0, 0.0, 0.0},
		 FVector{1.0, 1.0, 0.0},
		 FVector{1.0, 2.0, 0.0},
		 FVector{1.0, 2.0, 1.0},
		 FVector{1.0, 2.0, 4.0}};

	{
		const TArray<FVector> Shortened = GetShortenedPath(MakeArrayView(Path.data(), Path.size()), 0.5);
		const TArray<FVector> Expected = {Path[0], Path[1], Path[2], Path[3], Path[4], FVector{1.0, 2.0, 3.5}};
		TestEqual("Shortens last segment", Shortened, Expected);
	}

	{
		const TArray<FVector> Shortened = GetShortenedPath(MakeArrayView(Path.data(), Path.size()), 3.0);
		const TArray<FVector> Expected = {Path[0], Path[1], Path[2], Path[3], Path[4]};
		TestEqual("Cuts last segment", Shortened, Expected);
	}

	{
		const TArray<FVector> Shortened = GetShortenedPath(MakeArrayView(Path.data(), Path.size()), 3.5);
		const TArray<FVector> Expected = {Path[0], Path[1], Path[2], Path[3], FVector{1.0, 2.0, 0.5}};
		TestEqual("Cuts last segment and shortens", Shortened, Expected);
	}

	{
		const TArray<FVector> Shortened = GetShortenedPath(MakeArrayView(Path.data(), Path.size()), 10.0);
		const TArray<FVector> Expected = {};
		TestEqual("Cuts all segments", Shortened, Expected);
	}

	{
		const TArray<FVector> Shortened = GetShortenedPath(MakeArrayView(Path.data(), Path.size()), 0.5, true);
		const TArray<FVector> Expected = {FVector{0.5, 0.0, 0.0}, Path[1], Path[2], Path[3], Path[4], Path[5]};
		TestEqual("Shortens first segment", Shortened, Expected);
	}

	{
		const TArray<FVector> Shortened = GetShortenedPath(MakeArrayView(Path.data(), Path.size()), 1.0, true);
		const TArray<FVector> Expected = {Path[1], Path[2], Path[3], Path[4], Path[5]};
		TestEqual("Cuts first segment", Shortened, Expected);
	}

	{
		const TArray<FVector> Shortened = GetShortenedPath(MakeArrayView(Path.data(), Path.size()), 1.5, true);
		const TArray<FVector> Expected = {FVector{1.0, 0.5, 0.0}, Path[2], Path[3], Path[4], Path[5]};
		TestEqual("Cuts first segment and shortens", Shortened, Expected);
	}

	{
		const TArray<FVector> Shortened = GetShortenedPath(MakeArrayView(Path.data(), Path.size()), 10.0, true);
		const TArray<FVector> Expected = {};
		TestEqual("Cuts all segments reversed", Shortened, Expected);
	}
}

ZKZ_END_AUTOMATION_TEST(FMathTest);

}  // namespace Zkz::Math::Test
