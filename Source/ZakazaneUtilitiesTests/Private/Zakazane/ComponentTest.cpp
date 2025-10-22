#include "ComponentTest.h"

#include "Zakazane/Component.h"
#include "Zakazane/Test/Test.h"

AComponentTestActorSuperclass::AComponentTestActorSuperclass()
{
	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	RootComponent = DefaultRootComponent;

	DefaultChildComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultChildComponent"));
	DefaultChildComponent->SetupAttachment(DefaultRootComponent);
}

AComponentTestActorSubclass::AComponentTestActorSubclass()
{
	DefaultSubclassComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSubclassComponent"));
	DefaultSubclassComponent->SetupAttachment(DefaultRootComponent);

	DefaultSubclassChildComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSubclassChildComponent"));
	DefaultSubclassChildComponent->SetupAttachment(DefaultSubclassComponent);
}

AComponentTestActor::AComponentTestActor()
{
	DefaultComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultComponent"));
	RootComponent = DefaultComponent;

	DefaultChildComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultChildComponent"));
	DefaultChildComponent->SetupAttachment(DefaultComponent);

	DefaultGrandchildComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultGrandchildComponent"));
	DefaultGrandchildComponent->SetupAttachment(DefaultChildComponent);
}

namespace Zkz::Component::Test
{
ZKZ_BEGIN_AUTOMATION_TEST(
	FComponentTest,
	"Zakazane.ZakazaneUtilities.Component",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

ZKZ_ADD_TEST(ForEachComponentInHierarchyOverPureCppHierarchy)
{
	const auto ComponentVisitor =
		[](const UActorComponent& Component, TArray<const UActorComponent*>& VisitedComponents)
	{ VisitedComponents.Emplace(&Component); };

	const AComponentTestActor* const DefaultActor = GetDefault<AComponentTestActor>();
	ZKZ_RETURN_IF_INVALID_ENSUREALWAYS(DefaultActor);

	const FComponentHierarchy ComponentHierarchy{*DefaultActor};

	{
		TArray<const UActorComponent*> VisitedComponents;

		ComponentHierarchy.ForEachComponentInSubtree<EForEachComponentRecursionType::NotRecursive>(
			*DefaultActor->DefaultComponent, ComponentVisitor, VisitedComponents);
		TestEqual("SimpleHierarchyRootNotRecursive", VisitedComponents, {DefaultActor->DefaultComponent.Get()});
	}

	{
		TArray<const UActorComponent*> VisitedComponents;

		ComponentHierarchy.ForEachComponentInSubtree<EForEachComponentRecursionType::Prefix>(
			*DefaultActor->DefaultComponent, ComponentVisitor, VisitedComponents);
		TestEqual(
			"SimpleHierarchyRootPrefix",
			VisitedComponents,
			{DefaultActor->DefaultComponent.Get(),
			 DefaultActor->DefaultChildComponent.Get(),
			 DefaultActor->DefaultGrandchildComponent.Get()});
	}

	{
		const auto CondVisitor =
			[DefaultActor](const UActorComponent& Component, TArray<const UActorComponent*>& VisitedComponents)
		{
			VisitedComponents.Emplace(&Component);
			return &Component != DefaultActor->DefaultChildComponent;
		};

		TArray<const UActorComponent*> VisitedComponents;

		ComponentHierarchy.ForEachComponentInSubtree<EForEachComponentRecursionType::PrefixCond>(
			*DefaultActor->DefaultComponent, CondVisitor, VisitedComponents);
		TestEqual(
			"SimpleHierarchyRootSuffixCond",
			VisitedComponents,
			{DefaultActor->DefaultComponent.Get(), DefaultActor->DefaultChildComponent.Get()});
	}

	{
		TArray<const UActorComponent*> VisitedComponents;

		ComponentHierarchy.ForEachComponentInSubtree<EForEachComponentRecursionType::Suffix>(
			*DefaultActor->DefaultComponent, ComponentVisitor, VisitedComponents);
		TestEqual(
			"SimpleHierarchyRootSuffix",
			VisitedComponents,
			{DefaultActor->DefaultGrandchildComponent.Get(),
			 DefaultActor->DefaultChildComponent.Get(),
			 DefaultActor->DefaultComponent.Get()});
	}
}

// #TODO #Components: this works, but it still debug-breaks at ensure, which is irritating, can we fix it?
// ZKZ_ADD_TEST(NonConstOpsEnsureInPureCppHierarchy)
// {
// 	const AComponentTestActor* const DefaultActor = GetDefault<AComponentTestActor>();
// 	ZKZ_RETURN_IF_INVALID_ENSUREALWAYS(DefaultActor);
//
// 	FComponentHierarchy ComponentHierarchy{*DefaultActor};
//
// 	{
// 		AddExpectedError("!bComponentsMutable", EAutomationExpectedErrorFlags::Contains, 1, false);
// 		ComponentHierarchy.AddNewSubobject(
// 			*UStaticMeshComponent::StaticClass(),
// 			nullptr,
// 			"StaticMeshComponent",
// 			Editor::EMarkBlueprintAsStructurallyModified::Enabled);
// 		TestTrue("AddNewSubobjectEnsured", HasMetExpectedErrors());
// 	}
// }

ZKZ_ADD_TEST(ForEachComponentInHierarchyOverPureBluprintHierarchy)
{
	const auto ComponentVisitor =
		[](const UActorComponent& Component, TArray<const UActorComponent*>& VisitedComponents)
	{ VisitedComponents.Emplace(&Component); };

	const auto FindVisitedComponent = [](const TArray<const UActorComponent*>& VisitedComponents, const FString& Name)
	{
		const UActorComponent* const* const CompPtrPtr = Algo::FindBy(
			VisitedComponents,
			Name,
			[](const UActorComponent* const Comp)
			{ return Comp == nullptr ? FString{} : GetComponentNameNoSuffix(*Comp); });
		return CompPtrPtr == nullptr ? nullptr : *CompPtrPtr;
	};

	{
		const TSubclassOf<AActor> LoadedClass = LoadClass<AActor>(
			nullptr, TEXT("/ZakazaneUtilities/BP_ZkzComponentTestSimpleActor.BP_ZkzComponentTestSimpleActor_C"));
		ZKZ_RETURN_IF_INVALID_ENSUREALWAYS(LoadedClass);

		const AActor* const DefaultActor = LoadedClass->GetDefaultObject<AActor>();
		ZKZ_RETURN_IF_INVALID_ENSUREALWAYS(DefaultActor);

		const FComponentHierarchy ComponentHierarchy{*DefaultActor};

		TArray<const UActorComponent*> VisitedComponents;
		ComponentHierarchy.ForEachComponent(ComponentVisitor, VisitedComponents);

		const UActorComponent* const DefaultSceneRoot = FindVisitedComponent(VisitedComponents, "DefaultSceneRoot");
		if (TestTrue("VisitedDefaultSceneRoot", DefaultSceneRoot != nullptr))
		{
			TestTrue("DefaultSceneRootIsRoot", ComponentHierarchy.FindParent(*DefaultSceneRoot) == nullptr);
		}

		const UActorComponent* const ParentCube = FindVisitedComponent(VisitedComponents, "ParentCube");
		if (TestTrue("VisitedParentCube", ParentCube != nullptr))
		{
			TestTrue(
				"ParentCubesParentIsDefaultSceneRoot", ComponentHierarchy.FindParent(*ParentCube) == DefaultSceneRoot);
		}

		const UActorComponent* const ChildSphere = FindVisitedComponent(VisitedComponents, "ChildSphere");
		if (TestTrue("VisitedChildSphere", ChildSphere != nullptr))
		{
			TestTrue("ChildSpheresParentIsParentCube", ComponentHierarchy.FindParent(*ChildSphere) == ParentCube);
		}

		const UActorComponent* const ActorComponent = FindVisitedComponent(VisitedComponents, "ActorComponent");
		if (TestTrue("VisitedActorComponent", ActorComponent != nullptr))
		{
			TestTrue("ActorComponentHasNoParent", ComponentHierarchy.FindParent(*ActorComponent) == nullptr);
		}
	}

	// #TODO #Components: Add test for inherited blueprint hierarchies
}

// #TODO #Components: Add test for mixed cpp / blueprint hierarchy
// #TODO #Components: Add test for add / remove subobject
// #TODO #Components: Add tests for hierarchy traversal

ZKZ_END_AUTOMATION_TEST(FComponentTest);

}  // namespace Zkz::Component::Test