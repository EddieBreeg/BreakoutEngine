#pragma once

#include <PCH.hpp>

#include <core/StringView.hpp>
#include <core/ULID.hpp>

#include <string>

namespace brk::dbg {
	/**
	 * Contains debug info for an object, used for the inspector
	 */
	struct GameObjectInfoComponent
	{
		std::string m_Name;
		ULID m_Id;
	};

	BRK_DEV_UI_API bool GameObjectInfoWidget(GameObjectInfoComponent& comp);
} // namespace brk::dbg
