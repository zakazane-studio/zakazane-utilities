#include "Zakazane/Blueprint.h"

#if WITH_EDITOR

#include "ComponentUtils.h"
#include "Engine/Blueprint.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/SimpleConstructionScript.h"
#include "Zakazane/ReturnIfMacros.h"

namespace Zkz
{

void ForEachNodeInInheritanceTreeOf(
	const TSubclassOf<AActor>& ActorClass, const TFunction<void(const USCS_Node&, UBlueprintGeneratedClass&)>& Func)
{
	// Source: https://forums.unrealengine.com/t/how-to-get-a-component-from-a-classdefaultobject/383881

	TSubclassOf<AActor> CurrentActorClass = ActorClass;

	// Go down the inheritance tree to find nodes
	// that were added to the blueprint or its parents.
	do
	{
		UBlueprintGeneratedClass* const BlueprintGeneratedClass = Cast<UBlueprintGeneratedClass>(CurrentActorClass);
		if (BlueprintGeneratedClass && BlueprintGeneratedClass->SimpleConstructionScript)
		{
			const TArray<USCS_Node*>& BlueprintNodes = BlueprintGeneratedClass->SimpleConstructionScript->GetAllNodes();

			for (const USCS_Node* Node : BlueprintNodes)
			{
				Func(*Node, *BlueprintGeneratedClass);
			}

			CurrentActorClass = Cast<UClass>(CurrentActorClass->GetSuperStruct());
		}
		else
		{
			break;
		}
	} while (CurrentActorClass != AActor::StaticClass());
}

bool IsBlueprintComponent(const USceneComponent& Component)
{
	return IsValid(GetBlueprint(Component));
}

UBlueprint* GetBlueprint(const USceneComponent& Component)
{
	const USimpleConstructionScript* SCS = ComponentUtils::GetSimpleConstructionScript(&Component);
	return (IsValid(SCS) ? SCS->GetBlueprint() : nullptr);
}

UBlueprint* GetBlueprint(const AActor& Actor)
{
	const UClass* const BuildingClass = Actor.GetClass();
	ZKZ_RETURN_IF_INVALID(BuildingClass, nullptr);
	UBlueprint* const BuildingBP = Cast<UBlueprint>(BuildingClass->ClassGeneratedBy);
	ZKZ_RETURN_IF_INVALID(BuildingBP, nullptr);

	return BuildingBP;
}

}  // namespace Zkz

#endif
