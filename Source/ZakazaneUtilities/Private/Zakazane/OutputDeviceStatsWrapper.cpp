// Copyright ZAKAZANE Studio. All Rights Reserved.

#include "Zakazane/OutputDeviceStatsWrapper.h"

FOutputDeviceStatsWrapper::FOutputDeviceStatsWrapper(FOutputDevice* InOutputDevice) : OutputDevice{InOutputDevice}
{
}

void FOutputDeviceStatsWrapper::Serialize(const TCHAR* V, const ELogVerbosity::Type Verbosity, const FName& Category)
{
	IncrementNumMessages(Verbosity);

	if (OutputDevice != nullptr)
	{
		OutputDevice->Serialize(V, Verbosity, Category);
	}
}

void FOutputDeviceStatsWrapper::Serialize(
	const TCHAR* V, const ELogVerbosity::Type Verbosity, const FName& Category, const double Time)
{
	IncrementNumMessages(Verbosity);

	if (OutputDevice != nullptr)
	{
		OutputDevice->Serialize(V, Verbosity, Category, Time);
	}
}

int32 FOutputDeviceStatsWrapper::GetNumMessages(const ELogVerbosity::Type Verbosity) const
{
	const int32* const FoundNumMessages = NumMessagesByVerbosity.Find(Verbosity);
	return FoundNumMessages == nullptr ? 0 : *FoundNumMessages;
}

int32 FOutputDeviceStatsWrapper::GetNumMessagesWorseThan(const ELogVerbosity::Type Verbosity) const
{
	int32 Result = 0;

	for (int32 VerbosityIdx = Verbosity; VerbosityIdx > 0; --VerbosityIdx)
	{
		Result += GetNumMessages(static_cast<ELogVerbosity::Type>(VerbosityIdx));
	}

	return Result;
}

void FOutputDeviceStatsWrapper::IncrementNumMessages(const ELogVerbosity::Type Verbosity)
{
	++NumMessagesByVerbosity.FindOrAdd(Verbosity, 0);
}