#include "Zakazane/InterpolatedValue.h"

#include "Zakazane/ReturnIfMacros.h"

FZkzInterpolatedFloatValue::FZkzInterpolatedFloatValue(
	const float InValue, const bool bInEnableInterpolation, const float InInterpSpeed)
	: bEnableInterpolation{bInEnableInterpolation}, InterpSpeed{InInterpSpeed}, Current{InValue}, Target{InValue}
{
}

FZkzInterpolatedFloatValue::FZkzInterpolatedFloatValue(bool bInEnableInterpolation, float InInterpSpeed)
	: FZkzInterpolatedFloatValue{0.0f, bInEnableInterpolation, InInterpSpeed}
{
}

void FZkzInterpolatedFloatValue::Reset(const float InValue)
{
	Target = InValue;
	Current = InValue;
}

void FZkzInterpolatedFloatValue::Update(const float DeltaTime)
{
	if (!bEnableInterpolation)
	{
		Current = Target;
	}

	Current = FMath::FInterpTo(Current, Target, DeltaTime, InterpSpeed);
}

float FZkzInterpolatedFloatValue::GetCurrent() const
{
	return Current;
}

float FZkzInterpolatedFloatValue::GetTarget() const
{
	return Target;
}

void FZkzInterpolatedFloatValue::SetTarget(const float InTarget)
{
	Target = InTarget;
}

void FZkzInterpolatedFloatValue::SetEnableInterpolation(const bool InEnable)
{
	bEnableInterpolation = InEnable;
}

void FZkzInterpolatedFloatValue::SetInterpSpeed(const float InInterpSpeed)
{
	ZKZ_RETURN_IF_ENSUREALWAYS(InInterpSpeed < UE_KINDA_SMALL_NUMBER);
	bEnableInterpolation = true;
	InterpSpeed = InInterpSpeed;
}

FZkzInterpolatedVectorValue::FZkzInterpolatedVectorValue(
	const FVector& InValue, const bool bInEnableInterpolation, const float InInterpSpeed)
	: bEnableInterpolation{bInEnableInterpolation}, InterpSpeed{InInterpSpeed}, Current{InValue}, Target{InValue}
{
}
FZkzInterpolatedVectorValue::FZkzInterpolatedVectorValue(const bool bInEnableInterpolation, const float InInterpSpeed)
	: FZkzInterpolatedVectorValue{FVector::ZeroVector, bInEnableInterpolation, InInterpSpeed}
{
}

void FZkzInterpolatedVectorValue::Reset(const FVector& InValue)
{
	Target = InValue;
	Current = InValue;
}

void FZkzInterpolatedVectorValue::Update(const float DeltaTime)
{
	if (!bEnableInterpolation)
	{
		Current = Target;
	}

	Current = FMath::VInterpTo(Current, Target, DeltaTime, InterpSpeed);
}

const FVector& FZkzInterpolatedVectorValue::GetCurrent() const
{
	return Current;
}

const FVector& FZkzInterpolatedVectorValue::GetTarget() const
{
	return Target;
}

void FZkzInterpolatedVectorValue::SetTarget(const FVector& InTarget)
{
	Target = InTarget;
}

void FZkzInterpolatedVectorValue::SetEnableInterpolation(const bool InEnable)
{
	bEnableInterpolation = InEnable;
}

void FZkzInterpolatedVectorValue::SetInterpSpeed(const float InInterpSpeed)
{
	ZKZ_RETURN_IF_ENSUREALWAYS(InInterpSpeed < UE_KINDA_SMALL_NUMBER);
	bEnableInterpolation = true;
	InterpSpeed = InInterpSpeed;
}

FZkzInterpolatedQuatValue::FZkzInterpolatedQuatValue(
	const FQuat& InValue, const bool bInEnableInterpolation, const float InInterpSpeed)
	: bEnableInterpolation{bInEnableInterpolation}, InterpSpeed{InInterpSpeed}, Current{InValue}, Target{InValue}
{
}

FZkzInterpolatedQuatValue::FZkzInterpolatedQuatValue(const bool bInEnableInterpolation, const float InInterpSpeed)
	: FZkzInterpolatedQuatValue{FQuat::Identity, bInEnableInterpolation, InInterpSpeed}
{
}

void FZkzInterpolatedQuatValue::Reset(const FQuat& InValue)
{
	Target = InValue;
	Current = InValue;
}

void FZkzInterpolatedQuatValue::Update(const float DeltaTime)
{
	if (!bEnableInterpolation)
	{
		Current = Target;
	}

	Current = FMath::QInterpTo(Current, Target, DeltaTime, InterpSpeed);
}

const FQuat& FZkzInterpolatedQuatValue::GetCurrent() const
{
	return Current;
}

const FQuat& FZkzInterpolatedQuatValue::GetTarget() const
{
	return Target;
}

void FZkzInterpolatedQuatValue::SetTarget(const FQuat& InTarget)
{
	Target = InTarget;
}

void FZkzInterpolatedQuatValue::SetEnableInterpolation(const bool InEnable)
{
	bEnableInterpolation = InEnable;
}

void FZkzInterpolatedQuatValue::SetInterpSpeed(const float InInterpSpeed)
{
	ZKZ_RETURN_IF_ENSUREALWAYS(InInterpSpeed < UE_KINDA_SMALL_NUMBER);
	bEnableInterpolation = true;
	InterpSpeed = InInterpSpeed;
}

void UZkzInterpolatedValueBlueprintFunctionLibrary::ResetFloat(
	FZkzInterpolatedFloatValue& InterpolatedFloat, const float Value)
{
	InterpolatedFloat.Reset(Value);
}

void UZkzInterpolatedValueBlueprintFunctionLibrary::UpdateFloat(
	FZkzInterpolatedFloatValue& InterpolatedFloat, const float DeltaTime)
{
	InterpolatedFloat.Update(DeltaTime);
}

void UZkzInterpolatedValueBlueprintFunctionLibrary::SetTargetFloat(
	FZkzInterpolatedFloatValue& InterpolatedFloat, const float Target)
{
	InterpolatedFloat.SetTarget(Target);
}

void UZkzInterpolatedValueBlueprintFunctionLibrary::ResetVector(
	FZkzInterpolatedVectorValue& InterpolatedVector, const FVector& Value)
{
	InterpolatedVector.Reset(Value);
}

void UZkzInterpolatedValueBlueprintFunctionLibrary::UpdateVector(
	FZkzInterpolatedVectorValue& InterpolatedVector, const float DeltaTime)
{
	InterpolatedVector.Update(DeltaTime);
}

void UZkzInterpolatedValueBlueprintFunctionLibrary::SetTargetVector(
	FZkzInterpolatedVectorValue& InterpolatedVector, const FVector& Target)
{
	InterpolatedVector.SetTarget(Target);
}

void UZkzInterpolatedValueBlueprintFunctionLibrary::ResetQuat(
	FZkzInterpolatedQuatValue& InterpolatedQuat, const FQuat& Value)
{
	InterpolatedQuat.Reset(Value);
}

void UZkzInterpolatedValueBlueprintFunctionLibrary::UpdateQuat(
	FZkzInterpolatedQuatValue& InterpolatedQuat, const float DeltaTime)
{
	InterpolatedQuat.Update(DeltaTime);
}

void UZkzInterpolatedValueBlueprintFunctionLibrary::SetTargetQuat(
	FZkzInterpolatedQuatValue& InterpolatedQuat, const FQuat& Target)
{
	InterpolatedQuat.SetTarget(Target);
}
