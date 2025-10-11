// Copyright ZAKAZANE Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/// Wrapper for FOutputDevice implementations that collects the number of messages by verbosity. Provided wrapped output
/// device may be null in which case the log message will not be passed on.
class ZAKAZANEUTILITIES_API FOutputDeviceStatsWrapper : public FOutputDevice
{
public:
	explicit FOutputDeviceStatsWrapper(FOutputDevice* InOutputDevice);

	virtual void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category) override;
	virtual void Serialize(
		const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category, const double Time) override;

	int32 GetNumMessages(ELogVerbosity::Type Verbosity) const;
	int32 GetNumMessagesWorseThan(ELogVerbosity::Type Verbosity) const;

private:
	FOutputDevice* const OutputDevice = nullptr;

	TMap<ELogVerbosity::Type, int32, TInlineSetAllocator<ELogVerbosity::NumVerbosity>> NumMessagesByVerbosity;

	void IncrementNumMessages(ELogVerbosity::Type Verbosity);
};
