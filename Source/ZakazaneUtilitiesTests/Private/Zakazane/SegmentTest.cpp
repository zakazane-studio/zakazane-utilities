#include "Zakazane/Test/Test.h"
#include "Zakazane/ZkzSegment.h"

namespace Zkz::Segment::Test
{

ZKZ_BEGIN_AUTOMATION_TEST(
	FSegmentTest,
	"Zakazane.ZakazaneUtilities.Segment",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

ZKZ_ADD_TEST(OffsetEndsInwardTest)
{
	const FZkzSegment Segment{FVector{0.0, 1.0, 1.0}, FVector{5.0, 1.0, 1.0}};
	const FZkzSegment SegmentOff_05_1{FVector{0.5, 1.0, 1.0}, FVector{4.0, 1.0, 1.0}};
	const FZkzSegment SegmentOff_5_1{FVector{5.0, 1.0, 1.0}, FVector{4.0, 1.0, 1.0}};
	const FZkzSegment SegmentOff_n05_n1{FVector{-0.5, 1.0, 1.0}, FVector{6.0, 1.0, 1.0}};

	TestEqual("Offsets ends", Segment.OffsetEndsInward(0.5, 1).Start, SegmentOff_05_1.Start);
	TestEqual("Offsets ends", Segment.OffsetEndsInward(0.5, 1).End, SegmentOff_05_1.End);
	TestEqual("Offsets ends", Segment.OffsetEndsInward(5, 1).Start, SegmentOff_5_1.Start);
	TestEqual("Offsets ends", Segment.OffsetEndsInward(5, 1).End, SegmentOff_5_1.End);
	TestEqual("Offsets ends", Segment.OffsetEndsInward(-0.5, -1).Start, SegmentOff_n05_n1.Start);
	TestEqual("Offsets ends", Segment.OffsetEndsInward(-0.5, -1).End, SegmentOff_n05_n1.End);
}

ZKZ_END_AUTOMATION_TEST(FSegmentTest);

}  // namespace Zkz::Segment::Test
