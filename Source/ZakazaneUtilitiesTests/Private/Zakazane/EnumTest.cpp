#include "EnumTest.h"

#include "Zakazane/Enum.h"
#include "Zakazane/Test/Test.h"

namespace Zkz::Enum::Test
{

ZKZ_BEGIN_AUTOMATION_TEST(
	FEnumTest,
	"Zakazane.ZakazaneUtilities.Enum",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

ZKZ_ADD_TEST(AsStringTest)
{
	TestEqual("GetCppNameAsString(One)", GetShortNameAsString(EZkzGameEnumTest_EnumClass::One), FString{"One"});
	TestEqual("GetCppNameAsString(Two)", GetShortNameAsString(EZkzGameEnumTest_EnumClass::Two), FString{"Two"});

	TestEqual(
		"GetFullNameAsString(One)",
		GetFullNameAsString(EZkzGameEnumTest_EnumClass::One),
		FString{"EZkzGameEnumTest_EnumClass::One"});
	TestEqual(
		"GetFullNameAsString(Two)",
		GetFullNameAsString(EZkzGameEnumTest_EnumClass::Two),
		FString{"EZkzGameEnumTest_EnumClass::Two"});

	TestEqual(
		"GetDisplayNameAsText(One)",
		GetDisplayNameAsText(EZkzGameEnumTest_EnumClass::One).ToString(),
		FString{"First"});
	TestEqual(
		"GetDisplayNameAsText(Two)",
		GetDisplayNameAsText(EZkzGameEnumTest_EnumClass::Two).ToString(),
		FString{"Second"});

	TestEqual("GetDisplayNameAsString(One)", GetDisplayNameAsString(EZkzGameEnumTest_EnumClass::One), FString{"First"});
	TestEqual(
		"GetDisplayNameAsString(Two)", GetDisplayNameAsString(EZkzGameEnumTest_EnumClass::Two), FString{"Second"});
}

ZKZ_ADD_TEST(AsStringTestTEnumAsByte)
{
	const TEnumAsByte<EZkzGameEnumTest_CStyleEnum> One = EZkzGameEnumTest_CStyleEnum::One;
	const TEnumAsByte<EZkzGameEnumTest_CStyleEnum> Two = EZkzGameEnumTest_CStyleEnum::Two;

	TestEqual("GetCppNameAsString(One)", GetShortNameAsString(One), FString{"One"});
	TestEqual("GetCppNameAsString(Two)", GetShortNameAsString(Two), FString{"Two"});

	TestEqual("GetFullNameAsString(One)", GetFullNameAsString(One), FString{"One"});
	TestEqual("GetFullNameAsString(Two)", GetFullNameAsString(Two), FString{"Two"});

	TestEqual("GetDisplayNameAsText(One)", GetDisplayNameAsText(One).ToString(), FString{"First"});
	TestEqual("GetDisplayNameAsText(Two)", GetDisplayNameAsText(Two).ToString(), FString{"Second"});

	TestEqual("GetDisplayNameAsString(One)", GetDisplayNameAsString(One), FString{"First"});
	TestEqual("GetDisplayNameAsString(Two)", GetDisplayNameAsString(Two), FString{"Second"});
}

ZKZ_END_AUTOMATION_TEST(FEnumTest);

}  // namespace Zkz::Enum::Test
