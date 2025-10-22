#include "Zakazane/ArrayView.h"
#include "Zakazane/Test/Test.h"
#include "Zakazane/Variant.h"

namespace Zkz::Test
{

namespace VariantPrivate
{

struct FSuper
{
	bool bVoidCalled = false;
	bool bIntCalled = false;

	void VoidFoo()
	{
		bVoidCalled = true;
	}

	int IntFoo()
	{
		bIntCalled = true;
		return 42;
	}
};

struct FSub1 : FSuper
{
};

struct FSub2 : FSuper
{
};

}  // namespace VariantPrivate

ZKZ_BEGIN_AUTOMATION_TEST(
	FVariantTest,
	"Zakazane.ZakazaneUtilities.Variant",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

ZKZ_ADD_TEST(ForEachVariantTypeCallsForEachType)
{
	bool bIntCalled = false;
	bool bDoubleCalled = false;
	bool bCharCalled = false;

	ForEachVariantType<TVariant<int, double, char>>(
		[&]<class T>(TTypeTag<T>)
		{
			if constexpr (std::is_same_v<T, int>)
			{
				bIntCalled = true;
			}
			else if constexpr (std::is_same_v<T, double>)
			{
				bDoubleCalled = true;
			}
			else if constexpr (std::is_same_v<T, char>)
			{
				bCharCalled = true;
			}
			else
			{
				AddError(TEXT("Unexpected type tag called by ForEachVariantType"));
			}
		});

	TestTrue("Int called", bIntCalled);
	TestTrue("Double called", bDoubleCalled);
	TestTrue("Char called", bCharCalled);
}

ZKZ_ADD_TEST(VariantInvokeCallsGivenFunction)
{
	using namespace VariantPrivate;

	FSub1 Sub1;
	const TVariant<FSub1*, FSub2*> Variant{TInPlaceType<FSub1*>{}, &Sub1};
	VariantInvoke(Variant, &FSuper::VoidFoo);
	const int IntResult = VariantInvoke(Variant, &FSuper::IntFoo);

	TestTrue("Void called", Sub1.bVoidCalled);
	TestTrue("Int called", Sub1.bIntCalled);
	TestEqual("Int result", IntResult, 42);
}

ZKZ_END_AUTOMATION_TEST(FVariantTest);

}  // namespace Zkz::Test
