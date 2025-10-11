// Copyright ZAKAZANE Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "ZkzNameBPFunctionLibrary.generated.h"

UCLASS()
class ZAKAZANEUTILITIES_API UZkzNameBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "ZakazaneUtilities")
	static FName MakeNameNone();

	UFUNCTION(BlueprintPure, Category = "ZakazaneUtilities")
	static bool IsNameNone(FName Name);
};
