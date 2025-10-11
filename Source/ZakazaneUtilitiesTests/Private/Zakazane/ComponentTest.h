#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "ComponentTest.generated.h"

UCLASS(Abstract, Blueprintable)
class AComponentTestActorSuperclass : public AActor
{
	GENERATED_BODY()

public:
	AComponentTestActorSuperclass();

	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultRootComponent = nullptr;

	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultChildComponent = nullptr;
};

UCLASS(Blueprintable)
class AComponentTestActorSubclass : public AComponentTestActorSuperclass
{
	GENERATED_BODY()

public:
	AComponentTestActorSubclass();

	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultSubclassComponent = nullptr;

	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultSubclassChildComponent = nullptr;
};

UCLASS(Blueprintable)
class AComponentTestActor : public AActor
{
	GENERATED_BODY()

public:
	AComponentTestActor();

	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultComponent = nullptr;

	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultChildComponent = nullptr;

	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultGrandchildComponent = nullptr;
};
