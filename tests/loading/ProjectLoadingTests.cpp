#include <editor/Editor.hpp>
#include <managers/ECSManager.hpp>
#include <managers/ResourceManager.hpp>
#include <managers/SceneManager.hpp>
#include <nlohmann/json.hpp>
#include <imgui.h>

namespace brk::project_loading::ut {
	static constexpr ULID s_ResId1 = ULID::FromString("01JE1M440F9TN7EHDCQARXTXT1");

	struct Res1 : public Resource
	{
		using Resource::Resource;
		uint32 Value = 0;
		static constexpr StringView Name = "Res1";

		static constexpr meta::FieldList<&Res1::Value> Fields = { "value" };
	};
} // namespace brk::project_loading::ut

namespace brk::project_loading::ut {
	struct RAIIHelper
	{
		RAIIHelper()
			: m_ECSManager{ ecs::Manager::Init() }
			, m_ResManager{ ResourceManager::Init(m_ECSManager.GetWorld()) }
			, m_SceneManager{ SceneManager::Init() }
			, m_Editor{ editor::Editor::Init(
				  *ImGui::CreateContext(),
				  m_ECSManager,
				  m_SceneManager,
				  0,
				  nullptr) }
		{
			m_ResManager.RegisterResourceType<Res1>();
		}

		ecs::Manager& m_ECSManager;
		ResourceManager& m_ResManager;
		SceneManager& m_SceneManager;
		editor::Editor& m_Editor;

		~RAIIHelper()
		{
			m_Editor.Reset();
			m_ECSManager.Reset();
			m_SceneManager.Reset();
			m_ResManager.Reset();
			ImGui::DestroyContext();
		}
	};

	void Tests()
	{
		{
			RAIIHelper helper;
			helper.m_Editor.LoadProjectDeferred(DATA_DIR "/testProj.brk");
			helper.m_Editor.Update();

			RetainPtr ref = helper.m_ResManager.GetRef<Res1>(s_ResId1);
			assert(ref);
			assert(ref->Value == 666);
		}
	}
} // namespace brk::project_loading::ut