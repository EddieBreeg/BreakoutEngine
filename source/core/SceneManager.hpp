#pragma once

#include <PCH.hpp>
#include "Singleton.hpp"
#include "Scene.hpp"

#ifdef BRK_EDITOR
#include <nlohmann/json_fwd.hpp>
#endif

#include <unordered_map>

namespace brk {
	class SceneManager : public Singleton<SceneManager>
	{
	public:
		~SceneManager() = default;

#ifdef BRK_EDITOR
		void LoadSceneDescriptions(const nlohmann::json& descriptions);
#endif

		[[nodiscard]] const TULIDMap<SceneDescription>& GetObjects() const noexcept
		{
			return m_Descriptions;
		}

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		TULIDMap<SceneDescription> m_Descriptions;
	};
} // namespace brk
