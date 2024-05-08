#pragma once

#include <PCH.hpp>
#include <core/Singleton.hpp>
#include <core/Scene.hpp>
#include "GameObject.hpp"

#ifdef BRK_EDITOR
#include <nlohmann/json_fwd.hpp>
#endif

#include <unordered_map>

namespace brk::ecs {
	class SceneManager : public Singleton<SceneManager>
	{
	public:
		~SceneManager() = default;

#ifdef BRK_EDITOR
		void LoadSceneDescriptions(const nlohmann::json& descriptions);
#endif
		void LoadScene(const ULID sceneId);

		const GameObject* GetObject(const ULID id) const;

		[[nodiscard]] const TULIDMap<SceneDescription>& GetObjects() const noexcept
		{
			return m_Descriptions;
		}

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		TULIDMap<SceneDescription> m_Descriptions;
		TULIDMap<GameObject> m_Objects;
	};
} // namespace brk
