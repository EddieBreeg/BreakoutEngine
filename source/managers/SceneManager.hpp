#pragma once

#include <PCH.hpp>
#include <core/Singleton.hpp>
#include <core/Scene.hpp>
#include <ecs/GameObject.hpp>

#ifdef BRK_EDITOR
#include <nlohmann/json_fwd.hpp>
#endif

#include <unordered_map>

namespace brk {
	class SceneManager : public Singleton<SceneManager>
	{
	public:
		~SceneManager() = default;

#ifdef BRK_DEV
		void LoadSceneDescriptions(const nlohmann::json& descriptions);
		const SceneDescription& CreateNewScene(std::string name, std::string path);
#endif
		void LoadScene(const ULID sceneId);

		[[nodiscard]] const ecs::GameObject* GetObject(const ULID id) const;
		[[nodiscard]] const TULIDMap<ecs::GameObject>& GetGameObjects() const
		{
			return m_Objects;
		}

		[[nodiscard]] const TULIDMap<SceneDescription>& GetSceneDesriptions()
			const noexcept
		{
			return m_Descriptions;
		}

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		TULIDMap<SceneDescription> m_Descriptions;
		TULIDMap<ecs::GameObject> m_Objects;
		ULID m_CurrentSceneId;
	};
} // namespace brk
