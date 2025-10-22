// Copyright ZAKAZANE Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include <optional>
#include <type_traits>
#include <utility>
#include <variant>

#include "InPlace.h"

namespace Zkz
{

/// Type tag used to construct error TResults in-place
struct FUnexpect
{
	constexpr FUnexpect() = default;
};
constexpr FUnexpect Unexpect = {};

/// Unreal-style port of std::unexpected. Note that ErrorType can't be void. Use FMonostate instead.
template <class ErrorType UE_REQUIRES(!std::is_void_v<ErrorType>)>
class TUnexpected
{
public:
	template <
		class ArgErrorType = ErrorType UE_REQUIRES(
			!std::is_same_v<std::remove_cvref_t<ArgErrorType>, TUnexpected>
			&& !std::is_same_v<std::remove_cvref_t<ArgErrorType>, FInPlace>
			&& std::is_constructible_v<ErrorType, ArgErrorType>)>
	constexpr explicit TUnexpected(ArgErrorType&& InValue) : Value{Forward<ArgErrorType>(InValue)}
	{
	}

	template <class... ArgTypes UE_REQUIRES(std::is_constructible_v<ErrorType, ArgTypes...>)>
	constexpr explicit TUnexpected(FInPlace, ArgTypes&&... Args) : Value{Forward<ArgTypes>(Args)...}
	{
	}

	template <
		class InitializerListElementType,
		class... ArgTypes UE_REQUIRES(std::is_constructible_v<ErrorType, ArgTypes...>)>
	constexpr explicit TUnexpected(
		FInPlace, std::initializer_list<InitializerListElementType> InitializerList, ArgTypes&&... Args)
		: Value{InitializerList, Forward<ArgTypes>(Args)...}
	{
	}

	constexpr ErrorType& GetError() &
	{
		return Value;
	}

	constexpr const ErrorType& GetError() const&
	{
		return const_cast<TUnexpected&>(*this).GetError();
	}

	constexpr ErrorType&& GetError() &&
	{
		return Value;
	}

	constexpr const ErrorType&& GetError() const&&
	{
		return const_cast<TUnexpected&&>(*this).GetError();
	}

private:
	ErrorType Value;

	template <class OtherErrorType>
	friend constexpr bool operator==(TUnexpected& Lhs, TUnexpected<OtherErrorType>& Rhs)
	{
		return Lhs.Value == Rhs.Value;
	}
};

template <class ErrorType>
TUnexpected(ErrorType) -> TUnexpected<ErrorType>;

/// Unreal-style port of std::expected (with small differences)
template <class InValueType, class InErrorType>
class TResult
{
public:
	using ValueType = InValueType;
	using ErrorType = InErrorType;

	constexpr TResult() = default;
	constexpr ~TResult() = default;
	constexpr TResult(const TResult&) = default;
	constexpr TResult(TResult&&) = default;
	constexpr TResult& operator=(const TResult&) = default;
	constexpr TResult& operator=(TResult&&) = default;

	/// In-place constructor for values.
	/// E.g.:
	/// <pre>
	///		TResult<ValueType, ErrorType>{InPlace, Arg1, Arg2}
	/// </pre>
	/// Will construct the result with the value constructed in-place by calling the constructor with arguments
	/// Arg1, Arg2.
	template <class... ArgTypes>
	constexpr explicit TResult(FInPlace, ArgTypes&&... Args)
		: Value{std::in_place_type<ValueType>, Forward<ArgTypes>(Args)...}
	{
	}

	/// Direct constructor for values.
	/// E.g.:
	/// <pre>
	///		TResult<int, ErrorType> RInt{3};		// RInt.GetValue() == 3
	///		TResult<int, ErrorType> RFloat{3.13f};	// RFloat.GetValue() == 3
	/// </pre>
	/// This is deliberately non-explicit to allow e.g. `return 3;` to be returned from  a function returning
	/// `TResult<int, FString>`.
	template <class ValueArgType = std::remove_cv_t<ValueType>>
	// ReSharper disable once CppNonExplicitConvertingConstructor
	constexpr explicit(!std::is_convertible_v<ValueArgType&&, ValueType>) TResult(ValueArgType&& InValue)
		: TResult{InPlace, Forward<ValueArgType>(InValue)}
	{
	}

	/// In-place constructor for errors.
	/// E.g.:
	/// <pre>
	///		TResult<ValueType, ErrorType>{Unexpect, Arg1, Arg2}
	/// </pre>
	/// Will construct the error with the error value constructed in-place by calling the constructor with arguments
	/// Arg1, Arg2.
	template <class... ArgTypes>
	constexpr explicit TResult(FUnexpect, ArgTypes&&... Args)
		: Value{std::in_place_type<TUnexpected<ErrorType>>, InPlace, Forward<ArgTypes>(Args)...}
	{
	}

	/// Direct constructor for errors.
	/// E.g.:
	/// <pre>
	///		TResult<ValueType, int> EInt{TUnexpected{3}};
	///		// -> EInt.GetValue() == 3
	///		TResult<ValueType, int> EFloat{TUnexpected{3.13f}};
	///		// -> EFloat.GetValue() == 3
	/// </pre>
	/// This is deliberately non-explicit to allow e.g. `return TUnexpected{"Error"};` to be returned from
	/// a function returning `TResult<int, FString>`.
	template <
		class ErrorArgType = std::remove_cv_t<ErrorType> UE_REQUIRES(std::is_convertible_v<ErrorArgType&&, ErrorType>)>
	// ReSharper disable once CppNonExplicitConvertingConstructor
	constexpr explicit(!std::is_convertible_v<ErrorArgType&&, ErrorType>) TResult(TUnexpected<ErrorArgType>&& InError)
		: TResult{Unexpect, Forward<ErrorArgType>(InError.GetError())}
	{
	}

	constexpr bool HasValue() const
	{
		return std::holds_alternative<ValueType>(Value);
	}

	constexpr bool HasError() const
	{
		return !HasValue();
	}

	constexpr ValueType& GetValue() &
	{
		return std::get<ValueType>(Value);
	}

	constexpr const ValueType& GetValue() const&
	{
		return const_cast<TResult&>(*this).GetValue();
	}

	constexpr ValueType&& GetValue() &&
	{
		return MoveTemp(std::get<ValueType>(Value));
	}

	constexpr const ValueType&& GetValue() const&&
	{
		return std::move(std::get<ValueType>(Value));
	}

	constexpr ErrorType& GetError() &
	{
		return std::get<TUnexpected<ErrorType>>(Value).GetError();
	}

	constexpr const ErrorType& GetError() const&
	{
		return const_cast<TResult&>(*this).GetError();
	}

	constexpr ErrorType&& GetError() &&
	{
		return MoveTemp(std::get<TUnexpected<ErrorType>>(Value).GetError());
	}

	constexpr const ErrorType&& GetError() const&&
	{
		return std::move(std::get<TUnexpected<ErrorType>>(Value).GetError());
	}

	/// If the result contains a:
	///
	/// * value - Function is invoked given the value stored, returns TResult containing the return value of the function
	/// * error - the error result is returned
	///
	/// Functor provided must accept the value type as argument and return a TResult with an error type constructible
	/// from the original error type.
	///
	/// In other words, the function provided is used to continue work with the value contained in the result as long as
	/// the result is valid. E.g.:
	/// <pre>
	///   TResult<AActor*, FString> FindAttackedActor();
	///   TResult<FDamageInfo, FString> DamageActor(AActor* Actor);
	///
	///   const TResult<FDamageInfo, FString> Result = FindAttackedActor().AndThen([](AActor* AttackedActor) {
	///     return DamageActor(AttackedActor);
	///   });
	/// </pre>
	template <class FunctionType>
	constexpr auto AndThen(FunctionType&& Function) &;

	template <class FunctionType>
	constexpr auto AndThen(FunctionType&& Function) &&;

	template <class FunctionType>
	constexpr auto AndThen(FunctionType&& Function) const&;

	template <class FunctionType>
	constexpr auto AndThen(FunctionType&& Function) const&&;

	/// If the result contains a:
	///
	/// * value - the value result is returned
	/// * error - Function is invoked given the error stored, returns TResult containing the return value of the function
	///
	/// Functor provided must accept the error type as argument and return a TResult with a value type constructible
	/// from the original value type.
	///
	/// In other words, the function provided is used to process the error contained in the result as long as
	/// the result is invalid. Useful to for instance provide a fallback.
	template <class FunctionType>
	constexpr auto OrElse(FunctionType&& Function) &;

	template <class FunctionType>
	constexpr auto OrElse(FunctionType&& Function) &&;

	template <class FunctionType>
	constexpr auto OrElse(FunctionType&& Function) const&;

	template <class FunctionType>
	constexpr auto OrElse(FunctionType&& Function) const&&;

	constexpr ValueType& operator*() &
	{
		return GetValue();
	}

	constexpr const ValueType& operator*() const&
	{
		return GetValue();
	}

	constexpr ValueType&& operator*() &&
	{
		return MoveTemp(GetValue());
	}

	constexpr const ValueType&& operator*() const&&
	{
		return MoveTemp(GetValue());
	}

	constexpr ValueType* operator->() &
	{
		return std::addressof(GetValue());
	}

	constexpr const ValueType* operator->() const&
	{
		return const_cast<TResult&>(*this).operator->();
	}

	constexpr explicit operator bool() const
	{
		return HasValue();
	}

private:
	// using std::variant because it's constexpr, unlike TVariant, hopefully all platforms implement this. If not,
	// can switch to TVariant and strip constexpr from TResult ops.
	std::variant<ValueType, TUnexpected<ErrorType>> Value;
};

template <class InErrorType>
class TResult<void, InErrorType>
{
public:
	using ValueType = void;
	using ErrorType = InErrorType;

	constexpr TResult() = default;
	constexpr ~TResult() = default;
	constexpr TResult(const TResult&) = default;
	constexpr TResult(TResult&&) = default;
	constexpr TResult& operator=(const TResult&) = default;
	constexpr TResult& operator=(TResult&&) = default;

	/// In-place constructor for errors.
	/// E.g.:
	/// <pre>
	///		TResult<void, ErrorType>{Unexpect, Arg1, Arg2}
	/// </pre>
	/// Will construct the error with the error value constructed in-place by calling the constructor with arguments
	/// Arg1, Arg2.
	template <class... ArgTypes>
	constexpr explicit TResult(FUnexpect, ArgTypes&&... Args) : Error{std::in_place, Forward<ArgTypes>(Args)...}
	{
	}

	/// Direct constructor for errors.
	/// E.g.:
	/// <pre>
	///		TResult<void, int> EInt{TUnexpected{3}};
	///		// -> EInt.GetValue() == 3
	///		TResult<void, int> EFloat{TUnexpected{3.13f}};
	///		// -> EFloat.GetValue() == 3
	/// </pre>
	/// This is deliberately non-explicit to allow e.g. `return TUnexpected{"Error"};` to be returned from
	/// a function returning `TResult<int, FString>`.
	template <
		class ErrorArgType = std::remove_cv_t<ErrorType> UE_REQUIRES(std::is_convertible_v<ErrorArgType&&, ErrorType>)>
	// ReSharper disable once CppNonExplicitConvertingConstructor
	constexpr explicit(!std::is_convertible_v<ErrorArgType&&, ErrorType>) TResult(TUnexpected<ErrorArgType>&& InError)
		: TResult{Unexpect, Forward<ErrorArgType>(InError.GetError())}
	{
	}

	constexpr bool HasValue() const
	{
		return !Error.has_value();
	}

	constexpr bool HasError() const
	{
		return !HasValue();
	}

	constexpr ErrorType& GetError() &
	{
		return Error.value();
	}

	constexpr const ErrorType& GetError() const&
	{
		return const_cast<TResult&>(*this).GetError();
	}

	constexpr ErrorType&& GetError() &&
	{
		return MoveTemp(Error.value());
	}

	constexpr const ErrorType&& GetError() const&&
	{
		return std::move(Error.value());
	}

	/// If the result contains a:
	///
	/// * value - Function is invoked given the value stored, returns TResult containing the return value of the function
	/// * error - the error result is returned
	///
	/// Functor provided must accept the value type as argument and return a TResult with an error type constructible
	/// from the original error type.
	///
	/// In other words, the function provided is used to continue work with the value contained in the result as long as
	/// the result is valid.
	template <class FunctionType>
	constexpr auto AndThen(FunctionType&& Function) &;

	template <class FunctionType>
	constexpr auto AndThen(FunctionType&& Function) &&;

	template <class FunctionType>
	constexpr auto AndThen(FunctionType&& Function) const&;

	template <class FunctionType>
	constexpr auto AndThen(FunctionType&& Function) const&&;

	/// If the result contains a:
	///
	/// * value - the value result is returned
	/// * error - Function is invoked given the error stored, returns TResult containing the return value of the function
	///
	/// Functor provided must accept the error type as argument and return a TResult with a value type constructible
	/// from the original value type.
	///
	/// In other words, the function provided is used to process the error contained in the result as long as
	/// the result is invalid. Useful to for instance provide a fallback.
	template <class FunctionType>
	constexpr auto OrElse(FunctionType&& Function) &;

	template <class FunctionType>
	constexpr auto OrElse(FunctionType&& Function) &&;

	template <class FunctionType>
	constexpr auto OrElse(FunctionType&& Function) const&;

	template <class FunctionType>
	constexpr auto OrElse(FunctionType&& Function) const&&;

	constexpr explicit operator bool() const
	{
		return HasValue();
	}

private:
	// using std::optional because it's constexpr, unlike TOptional, hopefully all platforms implement this. If not,
	// can switch to TOptional and strip constexpr from TResult ops.
	std::optional<ErrorType> Error;
};

template <class T>
constexpr bool IsResult = false;

template <class ValueType, class ErrorType>
constexpr bool IsResult<TResult<ValueType, ErrorType>> = true;

namespace ResultPrivate
{

template <class ResultType, class FunctionType UE_REQUIRES(!std::is_void_v<typename ResultType::ValueType>)>
constexpr auto AndThenImpl(ResultType&& Result, FunctionType&& Function)
{
	static_assert(
		TIsInvocable<FunctionType, decltype(*DeclVal<ResultType>())>::Value,
		"Expected Function to be invocable with result value");

	using FunctionResultType = decltype(Invoke(DeclVal<FunctionType>(), *DeclVal<ResultType>()));

	static_assert(IsResult<FunctionResultType>, "Expected Function to return a TResult");
	static_assert(
		TIsConstructible<FunctionResultType, FUnexpect, decltype(Forward<ResultType>(Result).GetError())>::Value,
		"Expected Function result error type to be constructible from original result error type");

	return Result.HasValue() ? Invoke(Forward<FunctionType>(Function), *Forward<ResultType>(Result))
							 : FunctionResultType{Unexpect, Forward<ResultType>(Result).GetError()};
}

template <class ResultType, class FunctionType UE_REQUIRES(std::is_void_v<typename ResultType::ValueType>)>
constexpr auto AndThenImpl(ResultType&& Result, FunctionType&& Function)
{
	static_assert(TIsInvocable<FunctionType>::Value, "Expected Function to be invocable without params");

	using FunctionResultType = decltype(Invoke(DeclVal<FunctionType>()));

	static_assert(IsResult<FunctionResultType>, "Expected Function to return a TResult");
	static_assert(
		TIsConstructible<FunctionResultType, FUnexpect, decltype(Forward<ResultType>(Result).GetError())>::Value,
		"Expected Function result error type to be constructible from original result error type");

	return Result.HasValue() ? Invoke(Forward<FunctionType>(Function))
							 : FunctionResultType{Unexpect, Forward<ResultType>(Result).GetError()};
}

template <class ResultType, class FunctionType UE_REQUIRES(!std::is_void_v<typename ResultType::ValueType>)>
constexpr auto OrElseImpl(ResultType&& Result, FunctionType&& Function)
{
	static_assert(
		TIsInvocable<FunctionType, decltype(*DeclVal<ResultType>())>::Value,
		"Expected Function to be invocable with result value");

	using FunctionResultType = decltype(Invoke(DeclVal<FunctionType>(), *DeclVal<ResultType>()));

	static_assert(IsResult<FunctionResultType>, "Expected Function to return a TResult");
	static_assert(
		TIsConstructible<FunctionResultType, FInPlace, decltype(Forward<ResultType>(Result).GetValue())>::Value,
		"Expected Function result value type to be constructible from original result value type");

	return Result.HasError() ? Invoke(Forward<FunctionType>(Function), Forward<ResultType>(Result).GetError())
							 : FunctionResultType{InPlace, *Forward<ResultType>(Result)};
}

template <class ResultType, class FunctionType UE_REQUIRES(std::is_void_v<typename ResultType::ValueType>)>
constexpr auto OrElseImpl(ResultType&& Result, FunctionType&& Function)
{
	static_assert(
		TIsInvocable<FunctionType, decltype(DeclVal<ResultType>().GetError())>::Value,
		"Expected Function to be invocable with error type");

	using FunctionResultType = decltype(Invoke(DeclVal<FunctionType>(), DeclVal<ResultType>().GetError()));

	static_assert(IsResult<FunctionResultType>, "Expected Function to return a TResult");
	static_assert(
		TIsConstructible<FunctionResultType, FInPlace>::Value,
		"Expected Function result value type to be constructible from void");

	return Result.HasError() ? Invoke(Forward<FunctionType>(Function), Forward<ResultType>(Result).GetError())
							 : FunctionResultType{InPlace};
}

}  // namespace ResultPrivate

// -- AndThen (non void)

template <class InValueType, class InErrorType>
template <class FunctionType>
constexpr auto TResult<InValueType, InErrorType>::AndThen(FunctionType&& Function) &
{
	return ResultPrivate::AndThenImpl(*this, Forward<FunctionType>(Function));
}

template <class InValueType, class InErrorType>
template <class FunctionType>
constexpr auto TResult<InValueType, InErrorType>::AndThen(FunctionType&& Function) &&
{
	return ResultPrivate::AndThenImpl(MoveTemp(*this), Forward<FunctionType>(Function));
}

template <class InValueType, class InErrorType>
template <class FunctionType>
constexpr auto TResult<InValueType, InErrorType>::AndThen(FunctionType&& Function) const&
{
	return ResultPrivate::AndThenImpl(*this, Forward<FunctionType>(Function));
}

template <class InValueType, class InErrorType>
template <class FunctionType>
constexpr auto TResult<InValueType, InErrorType>::AndThen(FunctionType&& Function) const&&
{
	return ResultPrivate::AndThenImpl(MoveTemp(*this), Forward<FunctionType>(Function));
}

// -- AndThen (void)

template <class InErrorType>
template <class FunctionType>
constexpr auto TResult<void, InErrorType>::AndThen(FunctionType&& Function) &
{
	return ResultPrivate::AndThenImpl(*this, Forward<FunctionType>(Function));
}

template <class InErrorType>
template <class FunctionType>
constexpr auto TResult<void, InErrorType>::AndThen(FunctionType&& Function) &&
{
	return ResultPrivate::AndThenImpl(MoveTemp(*this), Forward<FunctionType>(Function));
}

template <class InErrorType>
template <class FunctionType>
constexpr auto TResult<void, InErrorType>::AndThen(FunctionType&& Function) const&
{
	return ResultPrivate::AndThenImpl(*this, Forward<FunctionType>(Function));
}

template <class InErrorType>
template <class FunctionType>
constexpr auto TResult<void, InErrorType>::AndThen(FunctionType&& Function) const&&
{
	return ResultPrivate::AndThenImpl(MoveTemp(*this), Forward<FunctionType>(Function));
}

// -- OrElse (non void)

template <class InValueType, class InErrorType>
template <class FunctionType>
constexpr auto TResult<InValueType, InErrorType>::OrElse(FunctionType&& Function) &
{
	return ResultPrivate::OrElseImpl(*this, Forward<FunctionType>(Function));
}

template <class InValueType, class InErrorType>
template <class FunctionType>
constexpr auto TResult<InValueType, InErrorType>::OrElse(FunctionType&& Function) &&
{
	return ResultPrivate::OrElseImpl(MoveTemp(*this), Forward<FunctionType>(Function));
}

template <class InValueType, class InErrorType>
template <class FunctionType>
constexpr auto TResult<InValueType, InErrorType>::OrElse(FunctionType&& Function) const&
{
	return ResultPrivate::OrElseImpl(*this, Forward<FunctionType>(Function));
}

template <class InValueType, class InErrorType>
template <class FunctionType>
constexpr auto TResult<InValueType, InErrorType>::OrElse(FunctionType&& Function) const&&
{
	return ResultPrivate::OrElseImpl(MoveTemp(*this), Forward<FunctionType>(Function));
}

// -- OrElse (void)

template <class InErrorType>
template <class FunctionType>
constexpr auto TResult<void, InErrorType>::OrElse(FunctionType&& Function) &
{
	return ResultPrivate::OrElseImpl(*this, Forward<FunctionType>(Function));
}

template <class InErrorType>
template <class FunctionType>
constexpr auto TResult<void, InErrorType>::OrElse(FunctionType&& Function) &&
{
	return ResultPrivate::OrElseImpl(MoveTemp(*this), Forward<FunctionType>(Function));
}

template <class InErrorType>
template <class FunctionType>
constexpr auto TResult<void, InErrorType>::OrElse(FunctionType&& Function) const&
{
	return ResultPrivate::OrElseImpl(*this, Forward<FunctionType>(Function));
}

template <class InErrorType>
template <class FunctionType>
constexpr auto TResult<void, InErrorType>::OrElse(FunctionType&& Function) const&&
{
	return ResultPrivate::OrElseImpl(MoveTemp(*this), Forward<FunctionType>(Function));
}

}  // namespace Zkz
