#pragma once

#include <PCH.hpp>
#include <core/Singleton.hpp>
#include <core/Scene.hpp>
#include <ecs/GameObject.hpp>
#include <ecs/WorldFwd.hpp>

#include <nlohmann/json_fwd.hpp>

#include <unordered_map>

namespace brk {
	/*
	 * Manages scenes
	 */
	class SceneManager : public Singleton<SceneManager>
	{
	public:
		~SceneManager() = default;

		/**
		 * Loads a list of scene descriptions, read from a project file
		 */
		BRK_MANAGERS_API void LoadSceneDescriptions(const nlohmann::json& descriptions);
		BRK_MANAGERS_API const SceneDescription& CreateNewScene(
			std::string name,
			std::string path);
		/**
		 * Attemps to get a scene description and load all the corresponding game objects
		 */
		BRK_MANAGERS_API void LoadScene(const ULID sceneId);
		BRK_MANAGERS_API void ClearCurrentScene();

		BRK_MANAGERS_API [[nodiscard]] const ecs::GameObject* GetObject(
			const ULID id) const;
		BRK_MANAGERS_API [[nodiscard]] ecs::GameObject* GetObject(const ULID id);
		[[nodiscard]] const TULIDMap<ecs::GameObject>& GetGameObjects() const
		{
			return m_Objects;
		}
		BRK_MANAGERS_API ecs::GameObject& CreateObject(ecs::EntityWorld& world);
		BRK_MANAGERS_API void DeleteObject(const ULID id);
		[[nodiscard]] TULIDMap<ecs::GameObject>& GetGameObjects() { return m_Objects; }

		[[nodiscard]] const TULIDMap<SceneDescription>& GetSceneDesriptions()
			const noexcept
		{
			return m_Descriptions;
		}

		BRK_MANAGERS_API void SaveCurrentSceneToFile(const ecs::EntityWorld& world) const;

	private:
		BRK_MANAGERS_API static inline std::unique_ptr<SceneManager> s_Instance;
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		TULIDMap<SceneDescription> m_Descriptions;
		TULIDMap<ecs::GameObject> m_Objects;
		ULID m_CurrentSceneId;
	};
} // namespace brk
