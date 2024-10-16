#pragma once

#include <PCH.hpp>

#ifdef BRK_DEV

#include <core/StringView.hpp>
#include <core/ULID.hpp>

#include <string>

namespace brk::dbg
{
	struct GameObjectInfoComponent
	{
		std::string m_Name;
		ULID m_Id;
	};

	bool GameObjectInfoWidget(GameObjectInfoComponent& comp);
} // namespace brk
#endif