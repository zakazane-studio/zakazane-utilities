// Copyright ZAKAZANE Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "InterpolatedValue.generated.h"

USTRUCT(BlueprintType)
struct ZAKAZANEUTILITIES_API FZkzInterpolatedFloatValue
{
	GENERATED_BODY()

	FZkzInterpolatedFloatValue() = default;

	FZkzInterpolatedFloatValue(float InValue, bool bInEnableInterpolation, float InInterpSpeed);

	FZkzInterpolatedFloatValue(bool bInEnableInterpolation, float InInterpSpeed);

	void Reset(float InValue);

	void Update(float DeltaTime);

	float GetCurrent() const;

	float GetTarget() const;

	void SetTarget(float InTarget);

	void SetEnableInterpolation(bool InEnable);

	void SetInterpSpeed(float InInterpSpeed);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	bool bEnableInterpolation = false;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess, EditCondition = "bEnableInterpolation", EditConditionHides))
	float InterpSpeed = -1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float Current = 0.0f;

	UPROPERTY(Transient, VisibleAnywhere)
	float Target = 0.0f;
};

USTRUCT(BlueprintType)
struct ZAKAZANEUTILITIES_API FZkzInterpolatedVectorValue
{
	GENERATED_BODY()

	FZkzInterpolatedVectorValue() = default;

	FZkzInterpolatedVectorValue(const FVector& InValue, bool bInEnableInterpolation, float InInterpSpeed);

	FZkzInterpolatedVectorValue(bool bInEnableInterpolation, float InInterpSpeed);

	void Reset(const FVector& InValue);

	void Update(float DeltaTime);

	const FVector& GetCurrent() const;

	const FVector& GetTarget() const;

	void SetTarget(const FVector& InTarget);

	void SetEnableInterpolation(bool InEnable);

	void SetInterpSpeed(float InInterpSpeed);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	bool bEnableInterpolation = false;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess, EditCondition = "bEnableInterpolation", EditConditionHides))
	float InterpSpeed = -1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	FVector Current = FVector::ZeroVector;

	UPROPERTY(Transient, VisibleAnywhere)
	FVector Target = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct ZAKAZANEUTILITIES_API FZkzInterpolatedQuatValue
{
	GENERATED_BODY()

	FZkzInterpolatedQuatValue() = default;

	FZkzInterpolatedQuatValue(const FQuat& InValue, bool bInEnableInterpolation, float InInterpSpeed);

	FZkzInterpolatedQuatValue(bool bInEnableInterpolation, float InInterpSpeed);

	void Reset(const FQuat& InValue);

	void Update(float DeltaTime);

	const FQuat& GetCurrent() const;

	const FQuat& GetTarget() const;

	void SetTarget(const FQuat& InTarget);

	void SetEnableInterpolation(bool InEnable);

	void SetInterpSpeed(float InInterpSpeed);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	bool bEnableInterpolation = false;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess, EditCondition = "bEnableInterpolation", EditConditionHides))
	float InterpSpeed = -1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	FQuat Current = FQuat::Identity;

	UPROPERTY(Transient, VisibleAnywhere)
	FQuat Target = FQuat::Identity;
};

UCLASS()
class ZAKAZANEUTILITIES_API UZkzInterpolatedValueBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static void ResetFloat(FZkzInterpolatedFloatValue& InterpolatedFloat, float Value);

	UFUNCTION(BlueprintCallable)
	static void UpdateFloat(FZkzInterpolatedFloatValue& InterpolatedFloat, float DeltaTime);

	UFUNCTION(BlueprintCallable)
	static void SetTargetFloat(FZkzInterpolatedFloatValue& InterpolatedFloat, float Target);

	UFUNCTION(BlueprintCallable)
	static void ResetVector(FZkzInterpolatedVectorValue& InterpolatedVector, const FVector& Value);

	UFUNCTION(BlueprintCallable)
	static void UpdateVector(FZkzInterpolatedVectorValue& InterpolatedVector, float DeltaTime);

	UFUNCTION(BlueprintCallable)
	static void SetTargetVector(FZkzInterpolatedVectorValue& InterpolatedVector, const FVector& Target);

	UFUNCTION(BlueprintCallable)
	static void ResetQuat(FZkzInterpolatedQuatValue& InterpolatedQuat, const FQuat& Value);

	UFUNCTION(BlueprintCallable)
	static void UpdateQuat(FZkzInterpolatedQuatValue& InterpolatedQuat, float DeltaTime);

	UFUNCTION(BlueprintCallable)
	static void SetTargetQuat(FZkzInterpolatedQuatValue& InterpolatedQuat, const FQuat& Target);
};
