// Copyright ZAKAZANE Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ZkzEditorUtilityFunctionLibrary.generated.h"

UCLASS()
class ZAKAZANEUTILITIESEDITOR_API UZkzEditorUtilityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintCallable, Category = "ZakazaneUtilities")
	static void CopyTextToClipboard(const FString& TextToCopy);
};
