#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>
#include <new>

#include <core/Utility.hpp>
#include <math/Vector.hpp>

#ifndef NDEBUG
#define BRK_DEBUG
#define BRK_EDITOR
#endif

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
#define DEBUG_CHECK(test) if constexpr (false)
#endif