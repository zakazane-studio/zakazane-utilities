#pragma once

#include "EnumTest.generated.h"

UENUM()
enum class EZkzGameEnumTest_EnumClass
{
	One UMETA(DisplayName = "First"),
	Two UMETA(DisplayName = "Second"),
};

UENUM()
enum EZkzGameEnumTest_CStyleEnum
{
	One UMETA(DisplayName = "First"),
	Two UMETA(DisplayName = "Second"),
};
