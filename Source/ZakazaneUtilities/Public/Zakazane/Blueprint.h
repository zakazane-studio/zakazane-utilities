#pragma once

#include "CoreMinimal.h"

#include "Templates/SubclassOf.h"

#if WITH_EDITOR

class UBlueprintGeneratedClass;
class USCS_Node;
class AActor;
class UBlueprint;
class USceneComponent;

namespace Zkz
{

ZAKAZANEUTILITIES_API void ForEachNodeInInheritanceTreeOf(
	const TSubclassOf<AActor>& ActorClass,
	const TFunction<void(const USCS_Node&, UBlueprintGeneratedClass& /* InheritanceTreeClass */)>& Func);

ZAKAZANEUTILITIES_API bool IsBlueprintComponent(const USceneComponent& Component);
ZAKAZANEUTILITIES_API UBlueprint* GetBlueprint(const USceneComponent& Component);

ZAKAZANEUTILITIES_API UBlueprint* GetBlueprint(const AActor& Actor);

}  // namespace Zkz

#endif