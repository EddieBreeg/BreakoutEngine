#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <new>

#include <core/MetaLists.hpp>
#include <core/TypeTraits.hpp>
#include <core/Unassigned.hpp>
#include <core/Utility.hpp>
#include <math/Constants.hpp>
#include <math/Quaternion.hpp>
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

namespace brk {
	using byte = uint8;
}

#ifdef BRK_DEBUG
#define DEBUG_CHECK(test) if (!(test))
#else
#define DEBUG_CHECK(expr)                                                                \
	MARK_UNUSED(expr);                                                                   \
	if constexpr (false)
#endif

#ifndef BRK_MAX_PATH
#define BRK_MAX_PATH 1024
#endif

#define MARK_UNUSED(expr) (void)(expr)

#ifndef BIT
#define BIT(n) (1ull << (n))
#endif

#include "PlatformMacros.hpp"

#include <core/brk_core_export.h>
#include <imgui/brk_dev_ui_export.h>
#include <ecs/brk_ecs_export.h>
#include <editor/brk_editor_export.h>
#include <managers/brk_managers_export.h>
#include <rendering/brk_rendering_export.h>
#include <systems/brk_systems_export.h>