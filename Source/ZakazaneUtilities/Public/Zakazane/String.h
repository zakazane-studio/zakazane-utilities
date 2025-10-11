// Copyright ZAKAZANE Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"

struct FGameplayTag;

namespace Zkz::String
{

/// @return Elems rightmost segments of the given string delimited by the given character. E.g. for x.y.z
/// * Elems == 1 will return "z"
/// * Elems == 2 will return "y.z"
/// * Elems == 3 will return "x.y.z"
/// * Elems == 4 will return "x.y.z" (as there are only 3 segments)
ZAKAZANEUTILITIES_API FStringView GetRightmostSegments(const FStringView& String, TCHAR Delimiter, int32 Elems);

/// @returns Elems rightmost segments of the given tag. E.g. for x.y.z
/// * Elems == 1 will return "z"
/// * Elems == 2 will return "y.z"
/// * Elems == 3 will return "x.y.z"
/// * Elems == 4 will return "x.y.z" (as there are only 3 segments)
ZAKAZANEUTILITIES_API FString GetTagSuffix(const FGameplayTag& Tag, int32 Elems = 2);

/// @returns "True" or "False" depending on bValue
ZAKAZANEUTILITIES_API FString BoolToString(bool bValue);

/// @returns "True" or "False" depending on bValue
ZAKAZANEUTILITIES_API FString IntToString(int32 Value, bool AddSign = false);

}  // namespace Zkz::String
