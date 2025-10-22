#include "Zakazane/Lazy.h"
#include "Zakazane/Test/Test.h"

namespace Zkz::Test
{

ZKZ_BEGIN_AUTOMATION_TEST(
	FLazyTest,
	"Zakazane.ZakazaneUtilities.Lazy",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

ZKZ_ADD_TEST(EvaluatesOnDemand)
{
	int32 NumEvaluations = 0;
	const TLazy<int32> LazyInt{
		[&NumEvaluations]() -> int32
		{
			++NumEvaluations;
			return 42;
		}};

	TestFalse("Not evaluated prior to GetValue", LazyInt.IsEvaluated());
	TestEqual("Evaluator not called", NumEvaluations, 0);

	const int& GetValue1 = LazyInt.GetValue();
	TestEqual("GetValue returns evaluated value", GetValue1, 42);

	TestTrue("Evaluated post GetValue", LazyInt.IsEvaluated());

	const int& GetValue2 = LazyInt.GetValue();
	TestEqual("GetValue returns value evaluated before", &GetValue1, &GetValue2);
	TestEqual("Evaluator called once", NumEvaluations, 1);
}

ZKZ_END_AUTOMATION_TEST(FLazyTest);

}  // namespace Zkz::Test
