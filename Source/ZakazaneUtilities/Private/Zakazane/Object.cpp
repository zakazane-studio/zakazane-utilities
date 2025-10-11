#include "Zakazane/Object.h"

#include "GameFramework/Actor.h"
#include "Algo/Find.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

namespace Zkz
{

FString GetObjectNameOrLabel(const UObject& Object)
{
	if (const AActor* const Actor = Cast<AActor>(&Object); IsValid(Actor))
	{
		return Actor->GetActorNameOrLabel();
	}

	return Object.GetName();
}

#if WITH_EDITOR
namespace Editor
{

// TODO: #Log Maybe we should introduce an interface that we could implement in objects that require custom logic for acquiring their editor counterparts?
// For example all characters are spawned dynamically in world so maybe their editor counterpart should really be their Character Definition asset
UObject* TryGetEditorCounterpartObject(const UObject& Object)
{
	if (const AActor* ActorObject = Cast<AActor>(&Object))
	{
		return EditorUtilities::GetEditorWorldCounterpartActor(const_cast<AActor*>(ActorObject));
	}

	if (const UActorComponent* ComponentObject = Cast<UActorComponent>(&Object))
	{
		AActor* const ComponentOwnerActor = ComponentObject->GetOwner();
		if (IsValid(ComponentOwnerActor))
		{
			const AActor* const EditorCounterpartOwnerActor = EditorUtilities::GetEditorWorldCounterpartActor(
				ComponentOwnerActor);
			if (IsValid(EditorCounterpartOwnerActor))
			{
				TArray<UActorComponent*> ComponentArray;
				EditorCounterpartOwnerActor->GetComponents(ComponentObject->GetClass(), ComponentArray);
				UActorComponent** EditorCounterpartComponent = Algo::FindByPredicate(
					ComponentArray,
					[ComponentObject](const UActorComponent* const Other)
					{
						return ComponentObject->GetFName() == Other->GetFName();
					});

				if (EditorCounterpartComponent != nullptr)
				{
					return *EditorCounterpartComponent;
				}
			}
		}
	}

	UObject* BlueprintClassSource = Object.GetClass()->ClassGeneratedBy;
	if (IsValid(BlueprintClassSource))
	{
		return BlueprintClassSource;
	}

	return nullptr;
}

} // namespace Zkz::Editor
#endif

}  // namespace Zkz
