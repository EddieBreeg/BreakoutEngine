#include "UiData.hpp"
#if BRK_EDITOR
#include <core/StringViewFormatter.hpp>
#include <core/ULIDFormatter.hpp>
#include <ecs/ComponentRegistry.hpp>
#include <ecs/World.hpp>
#include <imgui.h>
#include <imgui/MiscWidgets.hpp>
#include <imgui_macros.hpp>
#include <imgui_stdlib.h>
#include <imgui/GameObjectInfoComponent.hpp>
#include <managers/SceneManager.hpp>

namespace {
	using TComponentTypeMap = brk::ecs::ComponentRegistry::TMap;

	static const char* s_StrDeleteObject = "Delete Object?";

	bool DisplayComponentSelector(
		brk::editor::ui::UiData::ComponentTypeSelector& inout_selector,
		const brk::ecs::GameObject& object,
		const TComponentTypeMap& typeMap)
	{
		bool confirm = false;
		bool cancel = false;

		ImGui::SeparatorText("Add Component");
		using TComponent = brk::ecs::GameObject::Component;
		const std::vector<TComponent>& components = object.m_Components;

		ImGui::BeginChild(1);

		inout_selector.m_NameFilter.Draw();
		for (auto&& [key, info] : typeMap)
		{
			{
				auto const it = std::find_if(
					components.begin(),
					components.end(),
					[=](const TComponent& comp)
					{
						return comp.m_Info == &info;
					});
				if (it != components.end())
					continue;
			}

			if (!inout_selector.m_NameFilter.PassFilter(
					info.m_Name.GetPtr(),
					info.m_Name.GetPtr() + info.m_Name.GetLen()))
				continue;

			if (ImGui::Selectable(
					info.m_Name.GetPtr(),
					&info == inout_selector.m_Selection,
					ImGuiSelectableFlags_AllowDoubleClick))
			{
				inout_selector.m_Selection = &info;
				if ((confirm = ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)))
					goto COMPONENT_SELECTOR_END;

				break;
			}
		}

		ImGui::BeginDisabled(!inout_selector.m_Selection);
		confirm = ImGui::Button("Add");
		ImGui::EndDisabled();

		ImGui::SameLine();

		cancel = ImGui::Button("Cancel");

	COMPONENT_SELECTOR_END:
		inout_selector.m_Show = !(cancel || confirm);
		ImGui::EndChild();
		return confirm;
	}
} // namespace

bool brk::editor::ui::UiData::Inspector(
	ecs::EntityWorld& entityWorld,
	const ecs::ComponentRegistry& componentRegistry)
{
	ImGui::Begin(s_StrInspector);

	if (!m_InspectorData.m_SelectedObject)
	{
		ImGui::Text("No object selected");
		ImGui::End();
		return false;
	}

	ecs::GameObject* const object = m_InspectorData.m_SelectedObject;

	ImGui::SeparatorText("Object Info");
	dev_ui::ULIDWidget("ULID", object->m_Id);
	IMGUI_LEFT_LABEL(ImGui::InputText, "Name", &object->m_Name);

	if (ImGui::Button("Delete Object"))
	{
		m_ModalPopup.Open(
			s_StrDeleteObject,
			fmt::format("Delete {}?", object->m_Name),
			&m_InspectorData.m_DeleteObjectRequested);
	}

	ImGui::SeparatorText("Components");

	uint32 stackId = 0;

	bool result = false;
	for (auto&& [component, index] : Enumerate(object->m_Components))
	{
		if (!component.m_Info->m_WidgetInfo.m_Display)
			continue;

		if (!ImGui::TreeNode(component.m_Info->m_Name.GetPtr()))
			continue;

		result |= component.m_Info->m_WidgetInfo.m_Display(
			component.m_Widget,
			entityWorld,
			object->m_Entity);

		ImGui::PushID(stackId++);
		if (ImGui::Button("Delete"))
		{
			m_ModalPopup.Open(
				s_StrDeleteComponent,
				fmt::format("Delete {} component?", component.m_Info->m_Name),
				&m_InspectorData.m_DeleteComponentRequested);
			m_InspectorData.m_DeletedComponent = index;
			ImGui::PopID();
			ImGui::TreePop();
			break;
		}

		ImGui::PopID();
		ImGui::TreePop();
	}

	if (m_InspectorData.m_TypeSelector.m_Show || ImGui::Button("Add Component"))
	{
		m_InspectorData.m_AddComponentRequested = DisplayComponentSelector(
			m_InspectorData.m_TypeSelector,
			*object,
			componentRegistry.GetTypeMap());
	}
	ImGui::End();

	return result;
}
#endif