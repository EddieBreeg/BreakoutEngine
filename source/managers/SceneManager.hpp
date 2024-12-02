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
	/*
	 * Manages scenes
	 */
	class BRK_MANAGERS_API SceneManager : public Singleton<SceneManager>
	{
	public:
		~SceneManager() = default;

#ifdef BRK_DEV
		/**
		 * Loads a list of scene descriptions, read from a project file
		 */
		void LoadSceneDescriptions(const nlohmann::json& descriptions);
		const SceneDescription& CreateNewScene(std::string name, std::string path);
#endif
		/**
		 * Attemps to get a scene description and load all the corresponding game objects
		 */
		void LoadScene(const ULID sceneId);

		[[nodiscard]] const ecs::GameObject* GetObject(const ULID id) const;
		[[nodiscard]] ecs::GameObject* GetObject(const ULID id);
		[[nodiscard]] const TULIDMap<ecs::GameObject>& GetGameObjects() const
		{
			return m_Objects;
		}
		[[nodiscard]] TULIDMap<ecs::GameObject>& GetGameObjects() { return m_Objects; }

		[[nodiscard]] const TULIDMap<SceneDescription>& GetSceneDesriptions()
			const noexcept
		{
			return m_Descriptions;
		}

	private:
		static std::unique_ptr<SceneManager> s_Instance;
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		TULIDMap<SceneDescription> m_Descriptions;
		TULIDMap<ecs::GameObject> m_Objects;
		ULID m_CurrentSceneId;
	};
} // namespace brk
