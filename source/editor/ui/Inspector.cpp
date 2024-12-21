#include "UiData.hpp"
#ifdef BRK_EDITOR
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
						return comp.m_Info == info;
					});
				if (it != components.end())
					continue;
			}

			if (!inout_selector.m_NameFilter.PassFilter(
					info->m_Name.GetPtr(),
					info->m_Name.GetPtr() + info->m_Name.GetLen()))
				continue;

			if (ImGui::Selectable(
					info->m_Name.GetPtr(),
					info == inout_selector.m_Selection,
					ImGuiSelectableFlags_AllowDoubleClick))
			{
				inout_selector.m_Selection = info;
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
	SceneManager& sceneManager,
	const ecs::ComponentRegistry& componentRegistry)
{
	ImGui::Begin("Inspector");

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

	if (m_InspectorData.m_ShowObjectDeleteWarning |= ImGui::Button("Delete Object"))
	{
		ImGui::OpenPopup("Delete Game Object?");
	}

	if (ImGui::BeginPopupModal(
			"Delete Game Object?",
			&m_InspectorData.m_ShowObjectDeleteWarning))
	{
		m_InspectorData.m_DeleteObjectRequested = ImGui::Button("Confirm");
		ImGui::SameLine();
		m_InspectorData.m_ShowObjectDeleteWarning =
			!(m_InspectorData.m_DeleteObjectRequested || ImGui::Button("Cancel"));
		ImGui::EndPopup();
	}

	ImGui::SeparatorText("Components");

	bool result = false;
	for (auto&& [info, widget] : object->m_Components)
	{
		if (!ImGui::TreeNode(info->m_Name.GetPtr()))
			continue;

		result |= info->m_WidgetInfo.m_Display(widget, entityWorld, object->m_Entity);
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