#include "Zakazane/String.h"
#include "Zakazane/Test/Test.h"

namespace Zkz::String::Test
{

ZKZ_BEGIN_AUTOMATION_TEST(
	FStringTest,
	"Zakazane.ZakazaneUtilities.String",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

ZKZ_ADD_TEST(GetRightmostSegmentsTest)
{
	TestEqual("Empty", GetRightmostSegments(FStringView{}, TCHAR{'.'}, 3), FStringView{});

	TestEqual("0", GetRightmostSegments(TEXT("abc.def.ghi"), TCHAR{'.'}, 0), FStringView{});
	TestEqual("1", GetRightmostSegments(TEXT("abc.def.ghi"), TCHAR{'.'}, 1), FStringView{TEXT("ghi")});
	TestEqual("2", GetRightmostSegments(TEXT("abc.def.ghi"), TCHAR{'.'}, 2), FStringView{TEXT("def.ghi")});
	TestEqual("3", GetRightmostSegments(TEXT("abc.def.ghi"), TCHAR{'.'}, 3), FStringView{TEXT("abc.def.ghi")});
	TestEqual("4", GetRightmostSegments(TEXT("abc.def.ghi"), TCHAR{'.'}, 4), FStringView{TEXT("abc.def.ghi")});
}

ZKZ_END_AUTOMATION_TEST(FStringTest);

}  // namespace Zkz::String::Test
