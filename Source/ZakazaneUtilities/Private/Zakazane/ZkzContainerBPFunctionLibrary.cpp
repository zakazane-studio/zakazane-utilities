// Copyright ZAKAZANE Studio. All Rights Reserved.

#include "Zakazane/ZkzContainerBPFunctionLibrary.h"

#include "Zakazane/Container.h"

int32 UZkzContainerBPFunctionLibrary::GetNextCircularBufferIndex(const int32 IndexCurrent, const int32 CollectionNum)
{
	return Zkz::Container::GetNextCircularBufferIndex(IndexCurrent, CollectionNum);
}

int32 UZkzContainerBPFunctionLibrary::GetPreviousCircularBufferIndex(
	const int32 IndexCurrent, const int32 CollectionNum)
{
	return Zkz::Container::GetPreviousCircularBufferIndex(IndexCurrent, CollectionNum);
}
