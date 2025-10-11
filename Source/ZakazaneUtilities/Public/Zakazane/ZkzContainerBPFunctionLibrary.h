// Copyright ZAKAZANE Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "ZkzContainerBPFunctionLibrary.generated.h"

/// Contains utility container functions for blueprints. Implementations should just call functions from Container.h.
UCLASS(Category = "Zakazane|Container")
class ZAKAZANEUTILITIES_API UZkzContainerBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static int32 GetNextCircularBufferIndex(const int32 IndexCurrent, const int32 CollectionNum);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static int32 GetPreviousCircularBufferIndex(const int32 IndexCurrent, const int32 CollectionNum);
};
