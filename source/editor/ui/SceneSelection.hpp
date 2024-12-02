#pragma once

#include <PCH.hpp>
#ifdef BRK_EDITOR

#include <core/ULID.hpp>

namespace brk
{
	class SceneDescription;
} // namespace brk


namespace brk::editor
{
	BRK_EDITOR_API void SceneSelectionWindow(const TULIDMap<SceneDescription>& scenes);
} // namespace brk::editor

#endif