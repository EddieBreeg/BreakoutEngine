#include <core/ResourceLoader.hpp>
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
		RAIIHelper(int argc = 0, const char** argv = nullptr)
			: m_ResLoader{ ResourceLoader::Init() }
			, m_ECSManager{ ecs::Manager::Init() }
			, m_ResManager{ ResourceManager::Init(m_ECSManager.GetWorld()) }
			, m_SceneManager{ SceneManager::Init() }
			, m_Editor{ editor::Editor::Init(
				  *ImGui::CreateContext(),
				  m_ECSManager,
				  m_ResManager,
				  m_SceneManager,
				  argc,
				  argv) }
		{
			m_ResManager.RegisterResourceType<Res1>();
		}

		ResourceLoader& m_ResLoader;
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
			ResourceLoader::Reset();
			ImGui::DestroyContext();
		}
	};

	void Tests()
	{
		{
			const char* args[2] = { nullptr, DATA_DIR "/testProj.brk" };
			RAIIHelper helper{ 2, args };
			helper.m_Editor.Update();
			helper.m_ResLoader.ProcessBatch();
			helper.m_ResLoader.Wait();

			RetainPtr ref = helper.m_ResManager.GetRef<Res1>(s_ResId1);
			assert(ref);
			assert(ref->Value == 666);
		}
	}
} // namespace brk::project_loading::ut