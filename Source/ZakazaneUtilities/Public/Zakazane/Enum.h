// Copyright ZAKAZANE Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "ReturnIfMacros.h"
#include "Templates/IsUEnumClass.h"
#include "UObject/Class.h"

namespace Zkz::Enum
{

/// @code
/// UENUM()
/// enum class EEnum
/// {
///     One UMETA(DisplayName="First"),
///     Two UMETA(DisplayName="Second"),
/// }
///
/// GetCppNameAsString(EEnum::One) -> FString{"One"}
/// GetCppNameAsString(EEnum::Two) -> FString{"Two"}
/// @endcode
template <class EnumType UE_REQUIRES(TIsEnum<EnumType>::Value)>
FString GetShortNameAsString(EnumType Value)
{
	const UEnum* Enum = StaticEnum<EnumType>();
	ZKZ_RETURN_IF_INVALID(Enum, FString{});

	return Enum->GetNameStringByValue(static_cast<int64>(Value));
}

/// @see GetShortNameAsString
///
/// @code
/// GetCppNameAsString(EEnum::One) -> FString{"One"}
/// GetCppNameAsString(EEnum::Two) -> FString{"Two"}
/// @endcode
template <class EnumType>
FString GetShortNameAsString(TEnumAsByte<EnumType> Value)
{
	return GetShortNameAsString(Value.GetValue());
}

/// @see GetShortNameAsString
///
/// @code
/// GetFullNameAsString(EEnum::One) -> FString{"EEnum::One"}
/// GetFullNameAsString(EEnum::Two) -> FString{"EEnum::Two"}
/// @endcode
template <class EnumType UE_REQUIRES(TIsEnum<EnumType>::Value)>
FString GetFullNameAsString(EnumType Value)
{
	return UEnum::GetValueAsString(Value);
}

/// @see GetShortNameAsString
///
/// @code
/// GetFullNameAsString(EEnum::One) -> FString{"One"}
/// GetFullNameAsString(EEnum::Two) -> FString{"Two"}
/// @endcode
template <class EnumType>
FString GetFullNameAsString(TEnumAsByte<EnumType> Value)
{
	return GetFullNameAsString(Value.GetValue());
}

/// @see GetShortNameAsString
///
/// GetDisplayNameAsString(EEnum::One) -> FText{"First"}
/// GetDisplayNameAsString(EEnum::Two) -> FText{"Second"}
template <class EnumType UE_REQUIRES(TIsEnum<EnumType>::Value)>
FText GetDisplayNameAsText(EnumType Value)
{
	return UEnum::GetDisplayValueAsText(Value);
}

/// @see GetShortNameAsString
///
/// GetDisplayNameAsString(EEnum::One) -> FText{"First"}
/// GetDisplayNameAsString(EEnum::Two) -> FText{"Second"}
template <class EnumType>
FText GetDisplayNameAsText(TEnumAsByte<EnumType> Value)
{
	return GetDisplayNameAsText(Value.GetValue());
}

/// @see GetShortNameAsString
///
/// GetDisplayNameAsString(EEnum::One) -> FString{"First"}
/// GetDisplayNameAsString(EEnum::Two) -> FString{"Second"}
template <class EnumType UE_REQUIRES(TIsEnum<EnumType>::Value)>
FString GetDisplayNameAsString(EnumType Value)
{
	return GetDisplayNameAsText(Value).ToString();
}

/// @see GetShortNameAsString
///
/// GetDisplayNameAsString(EEnum::One) -> FString{"First"}
/// GetDisplayNameAsString(EEnum::Two) -> FString{"Second"}
template <class EnumType>
FString GetDisplayNameAsString(TEnumAsByte<EnumType> Value)
{
	return GetDisplayNameAsString(Value.GetValue());
}

}  // namespace Zkz::Enum
