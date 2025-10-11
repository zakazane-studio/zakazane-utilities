#include "Zakazane/Mask.h"
#include "Zakazane/Test.h"

namespace Zkz::Test
{

enum class EMaskTest
{
	First = 1 << 0,
	Second = 1 << 1,
	Third = 1 << 2,

	MaxFlag = Third,
};

}  // namespace Zkz::Test

template <>
struct Zkz::TMaxFlag<Zkz::Test::EMaskTest>
{
	static constexpr auto Value = static_cast<uint8>(Test::EMaskTest::MaxFlag);
};

namespace Zkz::Test
{

ZKZ_BEGIN_AUTOMATION_TEST(
	FMaskTest,
	"Zakazane.ZakazaneUtilities.Mask",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

ZKZ_ADD_TEST(GetVectorAbsMaxComponentIndexTest)
{
	constexpr TMask<EMaskTest> FirstAndThird = TMask<EMaskTest>{} | EMaskTest::First | EMaskTest::Third;
	constexpr TMask<EMaskTest> FirstAndSecond = TMask<EMaskTest>{} | EMaskTest::First | EMaskTest::Second;
	constexpr TMask<EMaskTest> FirstSecondAndThird =
		TMask<EMaskTest>{} | EMaskTest::First | EMaskTest::Second | EMaskTest::Third;

	static_assert(FirstAndThird.HasAny(EMaskTest::First), "Has First (Any)");
	static_assert(FirstAndThird & EMaskTest::First, "Has First (Any)");
	static_assert(FirstAndThird.HasAll(EMaskTest::First), "Has First (All)");
	static_assert(FirstAndThird.HasAny(FirstAndThird), "Has First and Third (Any)");
	static_assert((FirstAndThird & FirstAndThird) == FirstAndThird, "Has First and Third (Any)");
	static_assert(FirstAndThird.HasAll(FirstAndThird), "Has First and Third (All)");
	static_assert(FirstAndThird.HasAny(FirstAndSecond), "Has First and Second (Any)");
	static_assert((FirstAndThird & FirstAndSecond) == EMaskTest::First, "Has First and Second (Any)");
	static_assert(!FirstAndThird.HasAll(FirstAndSecond), "!Has First and Second (All)");
	static_assert(FirstAndThird.HasAny(FirstSecondAndThird), "Has First, Second and Third (Any)");
	static_assert((FirstAndThird & FirstSecondAndThird) == FirstAndThird, "Has First, Second and Third (Any)");
	static_assert(!FirstAndThird.HasAll(FirstSecondAndThird), "!Has First, Second and Third (All)");
	static_assert(~FirstAndThird == EMaskTest::Second, "Not First and Third is Second");
}

ZKZ_END_AUTOMATION_TEST(FMaskTest);

}  // namespace Zkz::Test
