// Copyright ZAKAZANE Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Algo/Transform.h"
#include "TypeTraits.h"

struct FCsvParser;
struct FSlowTask;

namespace Zkz::Serialization
{

enum class EImportResult
{
	Error,
	Warning,
	CancelledByUser,
	Success,
};

/// Exports objects of a given type to CSV. Current implementation only handles FText properties in a special
/// manner (extracting the underlying string) and exports other types using the generic ExportText_Direct function.
/// Doesn't support containers at the moment. Add handling other property types (and/or containers) as necessary.
ZAKAZANEUTILITIES_API FString ExportToCSV(const UStruct& Struct, TArrayView<const void*> Objects);

/// @see ExportToCSV(const UStruct& Struct, TArrayView<const void*> Objects)
///
/// Templated version for pointers to objects of known type.
template <class T UE_REQUIRES(TIsUHTUStruct_v<T>)>
FString ExportToCSV(const TArrayView<const T*> Objects)
{
	return ExportToCSV(*T::StaticStruct(), Objects);
}

/// @see ExportToCSV(const UStruct& Struct, TArrayView<const void*> Objects)
///
/// Templated version for objects of known type.
template <class T UE_REQUIRES(TIsUHTUStruct_v<T>)>
FString ExportToCSV(const TArrayView<const T> Objects)
{
	TArray<const void*, TInlineAllocator<128>> Pointers;
	Pointers.Reserve(Objects.Num());
	Algo::Transform(Objects, Pointers, [](const T& Ref) { return &Ref; });

	return ExportToCSV(*T::StaticStruct(), Pointers);
}

/// Imports data of a given type from CSV. Current implementation only handles FText properties in a special
/// manner (replacing the underlying string) and imports other types using the generic ImportText_Direct function.
/// Add handling other property types as necessary.
///
/// @param LineCallback - callback function called for each imported object
/// @param Output - optional output device for gathering error messages
/// @param SlowTask - optional slow task to be ticked as task progresses. All import work sums up to 1.0f.
ZAKAZANEUTILITIES_API EImportResult ImportFromCSV(
	const UStruct& Struct,
	const FString& CSV,
	const TFunction<void(const void*)>& LineCallback,
	FOutputDevice* Output = nullptr,
	FSlowTask* SlowTask = nullptr);

/// @see ImportFromCSV(
/// 	const UStruct& Struct,
/// 	const FString& CSV,
/// 	const TFunction<void(const void*)>& LineCallback,
/// 	FOutputDevice* Output = nullptr,
/// 	FSlowTask* SlowTask = nullptr)
///
/// Templated version for objects of known type.
template <
	class T,
	class LineCallbackType UE_REQUIRES(TIsUHTUStruct_v<T>&& TIsInvocable<LineCallbackType, const T&>::Value)>
EImportResult ImportFromCSV(
	const FString& CSV, LineCallbackType&& LineCallback, FOutputDevice* Output = nullptr, FSlowTask* SlowTask = nullptr)
{
	return ImportFromCSV(
		*T::StaticStruct(),
		CSV,
		[&LineCallback](const void* const Data) { LineCallback(*static_cast<const T*>(Data)); },
		Output,
		SlowTask);
}

}  // namespace Zkz::Serialization
