#pragma once

#include <PCH.hpp>

namespace brk {
	struct MeshComponent;
	struct TransformComponent;
} // namespace brk

namespace brk::ecs {
	template <class Component>
	struct ComponentUiWidget;

	template <>
	struct ComponentUiWidget<brk::TransformComponent>
	{
		BRK_DEV_UI_API ComponentUiWidget(const TransformComponent& transform);
		BRK_DEV_UI_API bool operator()(TransformComponent& comp);

		float3 m_Translate, m_Scale, m_EulerRotation;
	};

	template <>
	struct ComponentUiWidget<brk::MeshComponent>
	{
		BRK_DEV_UI_API ComponentUiWidget(const MeshComponent&);
		BRK_DEV_UI_API bool operator()(MeshComponent& mesh);
	};
} // namespace brk::ecs
