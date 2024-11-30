#pragma once

namespace brk
{
	struct MeshComponent;
	struct TransformComponent;
} // namespace brk

namespace brk::editor
{
	bool TransformWidget(TransformComponent&);
	bool MeshComponentWidget(MeshComponent&);
} // namespace brk::editor

