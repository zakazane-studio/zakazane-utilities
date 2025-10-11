// Copyright ZAKAZANE Studio. All Rights Reserved.

#pragma once

#include <type_traits>

namespace Zkz
{

template <typename T, typename = void>
constexpr bool TIsUHTUStruct_v = false;

template <typename T>
constexpr bool TIsUHTUStruct_v<T, std::void_t<decltype(&T::StaticStruct)>> = true;

template <typename T, typename = void>
constexpr bool TIsUHTUClass_v = false;

template <typename T>
constexpr bool TIsUHTUClass_v<T, std::void_t<decltype(&T::StaticClass)>> = true;

template <class T>
constexpr bool TValueTypeIsConst_v = std::is_const_v<std::remove_reference_t<T>>;

template <class From, class To>
using TCopyConstType = std::conditional_t<TValueTypeIsConst_v<From>, std::add_const_t<To>, To>;

static_assert(std::is_same_v<TCopyConstType<const float, int32>, const int32>);
static_assert(std::is_same_v<TCopyConstType<float, int32>, int32>);
static_assert(std::is_same_v<TCopyConstType<const float&, int32>, const int32>);
static_assert(std::is_same_v<TCopyConstType<float&, int32>, int32>);

}  // namespace Zkz
