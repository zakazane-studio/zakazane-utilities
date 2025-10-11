// Copyright ZAKAZANE Studio. All Rights Reserved.

#pragma once

namespace Zkz
{

/// Utility type used to be able to identify a type with an object, for example to be used for function overloading
/// without instantiating a type.
/// See ForEachVariantType for example usage.
template <class T>
struct TTypeTag
{
};

}  // namespace Zkz
