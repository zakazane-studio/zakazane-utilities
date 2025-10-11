// Copyright ZAKAZANE Studio. All Rights Reserved.


#include "Zakazane/ZkzEditorUtilityFunctionLibrary.h"

#include "PropertyEditorClipboard.h"

void UZkzEditorUtilityFunctionLibrary::CopyTextToClipboard(const FString& TextToCopy)
{
	FPropertyEditorClipboard::ClipboardCopy(*TextToCopy);
}