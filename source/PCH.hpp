#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <new>

#include <core/MetaLists.hpp>
#include <math/Quaternion.hpp>
#include <core/TypeTraits.hpp>
#include <core/Utility.hpp>
#include <math/Vector.hpp>

#if _BRK_DEBUG_ENABLED
#define BRK_DEBUG
#endif
#undef _BRK_DEBUG_ENABLED

#if _BRK_EDITOR_ENABLED
#define BRK_EDITOR 1
#endif
#undef _BRK_EDITOR_ENABLED

#if defined(BRK_DEBUG) || defined(BRK_EDITOR)
#define BRK_DEV
#endif

using int8 = int8_t;
using uint8 = uint8_t;
using int16 = int16_t;
using uint16 = uint16_t;
using int32 = int32_t;
using uint32 = uint32_t;
using int64 = int64_t;
using uint64 = uint64_t;

#ifdef BRK_DEBUG
#define DEBUG_CHECK(test) if (!(test))
#else
#define DEBUG_CHECK(expr)                                                                \
	MARK_UNUSED(expr);                                                                   \
	if constexpr (false)
#endif

#define MARK_UNUSED(expr) (void)(expr)