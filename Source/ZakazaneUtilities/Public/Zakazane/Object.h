// Copyright ZAKAZANE Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

namespace Zkz
{

/// If the object is an actor and the label is available, returns the actor's label. Otherwise returns the
/// object name.
ZAKAZANEUTILITIES_API FString GetObjectNameOrLabel(const UObject& Object);

#if WITH_EDITOR
namespace Editor
{

ZAKAZANEUTILITIES_API UObject* TryGetEditorCounterpartObject(const UObject& Object);

} // namespace Zkz::Editor
#endif

}  // namespace Zkz
