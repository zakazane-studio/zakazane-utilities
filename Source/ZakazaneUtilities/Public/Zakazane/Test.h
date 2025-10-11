// Copyright ZAKAZANE Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Algo/Find.h"
#include "Misc/AutomationTest.h"

// Preprocessor internals begin
#define ZKZ_BEGIN_AUTOMATION_TEST_PRIVATE(TClass, TBaseClass, PrettyName, TFlags, FileName, LineNumber)             \
	class TClass final : public TBaseClass                                                                          \
	{                                                                                                               \
	public:                                                                                                         \
		TClass(const FString& InName) : TBaseClass(InName, true)                                                    \
		{                                                                                                           \
			static_assert(                                                                                          \
				!!((TFlags) & EAutomationTestFlags_ApplicationContextMask),                                         \
				"AutomationTest has no application flag.  It shouldn't run.  See AutomationTest.h.");               \
			static_assert(                                                                                          \
				!!(((TFlags) & EAutomationTestFlags_FilterMask) == EAutomationTestFlags::SmokeFilter)               \
					|| !!(((TFlags) & EAutomationTestFlags_FilterMask) == EAutomationTestFlags::EngineFilter)       \
					|| !!(((TFlags) & EAutomationTestFlags_FilterMask) == EAutomationTestFlags::ProductFilter)      \
					|| !!(((TFlags) & EAutomationTestFlags_FilterMask) == EAutomationTestFlags::PerfFilter)         \
					|| !!(((TFlags) & EAutomationTestFlags_FilterMask) == EAutomationTestFlags::StressFilter)       \
					|| !!(((TFlags) & EAutomationTestFlags_FilterMask) == EAutomationTestFlags::NegativeFilter),    \
				"All AutomationTests must have exactly 1 filter type specified.  See AutomationTest.h.");           \
                                                                                                                    \
			AddTests();                                                                                             \
		}                                                                                                           \
		virtual EAutomationTestFlags GetTestFlags() const override                                                  \
		{                                                                                                           \
			return ((TFlags) & ~(EAutomationTestFlags::SmokeFilter));                                               \
		}                                                                                                           \
		virtual bool IsStressTest() const                                                                           \
		{                                                                                                           \
			return true;                                                                                            \
		}                                                                                                           \
		virtual uint32 GetRequiredDeviceNum() const override                                                        \
		{                                                                                                           \
			return 1;                                                                                               \
		}                                                                                                           \
		virtual FString GetTestSourceFileName() const override                                                      \
		{                                                                                                           \
			return FileName;                                                                                        \
		}                                                                                                           \
		virtual int32 GetTestSourceFileLine() const override                                                        \
		{                                                                                                           \
			return LineNumber;                                                                                      \
		}                                                                                                           \
                                                                                                                    \
	protected:                                                                                                      \
		virtual FString GetBeautifiedTestName() const override                                                      \
		{                                                                                                           \
			return PrettyName;                                                                                      \
		}                                                                                                           \
                                                                                                                    \
		virtual void GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const override \
		{                                                                                                           \
			for (const FTestCaseEntry& TestCase : TestCases)                                                        \
			{                                                                                                       \
				OutTestCommands.Emplace(TestCase.Name);                                                             \
				OutBeautifiedNames.Emplace(TestCase.BeautifiedName);                                                \
			}                                                                                                       \
		}                                                                                                           \
                                                                                                                    \
		virtual bool RunTest(const FString& Parameters) override                                                    \
		{                                                                                                           \
			const FTestCaseEntry* TestCase = Algo::FindBy(TestCases, Parameters, &FTestCaseEntry::Name);            \
			if (TestCase == nullptr)                                                                                \
			{                                                                                                       \
				return false;                                                                                       \
			}                                                                                                       \
                                                                                                                    \
			TestCase->TestCaseFunction();                                                                           \
			return true;                                                                                            \
		}                                                                                                           \
                                                                                                                    \
	private:                                                                                                        \
		using FTestCase = TFunction<void()>;                                                                        \
                                                                                                                    \
		struct FTestCaseEntry                                                                                       \
		{                                                                                                           \
			FString Name;                                                                                           \
			FString BeautifiedName;                                                                                 \
			FTestCase TestCaseFunction;                                                                             \
		};                                                                                                          \
                                                                                                                    \
		TArray<FTestCaseEntry> TestCases;                                                                           \
                                                                                                                    \
		void AddTests()                                                                                             \
		{                                                                                                           \
			void ZkzDummyDecl__(
// --

#define ZKZ_END_AUTOMATION_TEST_PRIVATE(TClass)           \
	)                                                     \
	;                                                     \
	}                                                     \
	}                                                     \
	;                                                     \
                                                          \
	namespace                                             \
	{                                                     \
	TClass TClass##AutomationTestInstance(TEXT(#TClass)); \
	}
// Preprocessor internals end

#define ZKZ_BEGIN_AUTOMATION_TEST(TClass, PrettyName, TFlags) \
	ZKZ_BEGIN_AUTOMATION_TEST_PRIVATE(TClass, FAutomationTestBase, PrettyName, TFlags, __FILE__, __LINE__)

#define ZKZ_ADD_TEST(TestCaseName) \
	)                              \
	;                              \
	TestCases.Emplace(#TestCaseName, #TestCaseName, [this]()
#define ZKZ_END_AUTOMATION_TEST(TClass) ZKZ_END_AUTOMATION_TEST_PRIVATE(TClass)

/**
 * Template for implementing tests:
 *
 * namespace Zkz::ModuleName::Tests
 * {
 *
 * ZKZ_BEGIN_AUTOMATION_TEST(
 *     FTestClassName,
 *     "Zakazane.ModuleName.TestClassName",
 *     EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter) // Use your own flags
 * 
 * ZKZ_ADD_TEST(TestName)
 * {
 *     TestEqual("WHAT", 0, 0);
 * }
 *
 * ZKZ_ADD_TEST(AnotherTestName)
 * {
 *     TestEqual("WHAT", 0, 1);
 * }
 *
 * ZKZ_END_AUTOMATION_TEST(FTestClassName);
 *
 * } // namespace Zkz::ModuleName::Tests
 */