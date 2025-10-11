#include "Zakazane/MultisourceValue.h"
#include "Zakazane/Test.h"

namespace Zkz::Test
{

ZKZ_BEGIN_AUTOMATION_TEST(
	FPriorityBasedMultisourceValueTest,
	"Zakazane.ZakazaneUtilities.MultisourceValue.PriorityBased",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

ZKZ_ADD_TEST(NoSourcesYieldDefaultValue)
{
	TPriorityBasedMultisourceValue<int32> Value{42};

	TestEqual("Empty gets default value", Value.GetValue(), 42);

	const auto SourceId = Value.PushValue(666, 0);
	Value.PopValue(SourceId);

	TestEqual("Emptied gets default value", Value.GetValue(), 42);
}

ZKZ_ADD_TEST(MultiplesSourcesYieldsLatestHighestPriorityDefaultValue)
{
	TPriorityBasedMultisourceValue<int32> Value{42};

	const auto Source0Id = Value.PushValue(0, 0);
	// (id: 0, prio: 0, val: 0)
	TestEqual("1 source", Value.GetValue(), 0);

	const auto Source1Id = Value.PushValue(1, 1);
	// (id: 0, prio: 0, val: 0), (id: 1, prio: 1, val: 1)
	TestEqual("2 sources", Value.GetValue(), 1);

	const auto Source2Id = Value.PushValue(2, 0);
	// (id: 0, prio: 0, val: 0), (id: 1, prio: 1, val: 1), (id: 2, prio: 0, val: 2)
	TestEqual("3 sources", Value.GetValue(), 1);

	const auto Source3Id = Value.PushValue(3, 2);
	// (id: 0, prio: 0, val: 0), (id: 1, prio: 1, val: 1), (id: 2, prio: 0, val: 2), (id: 3, prio: 2, val: 3)
	TestEqual("4 sources", Value.GetValue(), 3);

	Value.PopValue(Source3Id);
	// (id: 0, prio: 0, val: 0), (id: 1, prio: 1, val: 1), (id: 2, prio: 0, val: 2)
	TestEqual("3 sources again", Value.GetValue(), 1);

	Value.PopValue(Source1Id);
	// (id: 0, prio: 0, val: 0), (id: 2, prio: 0, val: 2)
	TestEqual("2 sources again", Value.GetValue(), 2);
}

ZKZ_END_AUTOMATION_TEST(FPriorityBasedMultisourceValueTest);

ZKZ_BEGIN_AUTOMATION_TEST(
	FIfAnyMultisourceValueTest,
	"Zakazane.ZakazaneUtilities.MultisourceValue.IfAny",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

ZKZ_ADD_TEST(NoSourcesYieldDefaultValue)
{
	FIfAnyMultisourceValue DefaultTrue{true};
	TestEqual("Defaults to true", DefaultTrue.GetValue(), true);

	FIfAnyMultisourceValue DefaultFalse{false};
	TestEqual("Defaults to false", DefaultFalse.GetValue(), false);
}

ZKZ_ADD_TEST(VotersYieldNegation)
{
	int Voter1, Voter2;

	FIfAnyMultisourceValue DefaultTrue{true};
	DefaultTrue.PushValue(&Voter1);
	TestEqual("1 voter yields false", DefaultTrue.GetValue(), false);
	DefaultTrue.PushValue(&Voter2);
	TestEqual("2 voters yields false", DefaultTrue.GetValue(), false);
	DefaultTrue.PopValue(&Voter1);
	TestEqual("1 voter again yields false", DefaultTrue.GetValue(), false);
	DefaultTrue.PopValue(&Voter2);
	TestEqual("0 voters yields true", DefaultTrue.GetValue(), true);

	FIfAnyMultisourceValue DefaultFalse{false};
	DefaultFalse.PushValue(&Voter1);
	TestEqual("1 voter yields true", DefaultFalse.GetValue(), true);
	DefaultFalse.PushValue(&Voter2);
	TestEqual("2 voters yields true", DefaultFalse.GetValue(), true);
	DefaultFalse.PopValue(&Voter1);
	TestEqual("1 voter again yields true", DefaultFalse.GetValue(), true);
	DefaultFalse.PopValue(&Voter2);
	TestEqual("0 voters yields false", DefaultFalse.GetValue(), false);
}

ZKZ_END_AUTOMATION_TEST(FIfAnyMultisourceValueTest)

}  // namespace Zkz::Test
