#pragma once

#include <editor/brk_editor_export.h>

namespace brk
{
	struct MeshComponent;
	struct TransformComponent;
} // namespace brk

namespace brk::editor
{
	BRK_EDITOR_API bool TransformWidget(TransformComponent&);
	BRK_EDITOR_API bool MeshComponentWidget(MeshComponent&);
} // namespace brk::editor

