#pragma once

#include <PCH.hpp>
#include "FieldList.hpp"
#include "ULID.hpp"

#include <string>

namespace brk {
	template <class T, class>
	struct JsonLoader;

	class SceneDescription
	{
	public:
		SceneDescription() = default;
		SceneDescription(const SceneDescription&) = default;
		SceneDescription(SceneDescription&&) = default;

		SceneDescription& operator=(const SceneDescription&) = default;

		[[nodiscard]] ULID GetId() const noexcept { return m_Id; }
		[[nodiscard]] const std::string& GetPath() const noexcept { return m_File; }
		[[nodiscard]] const std::string& GetName() const noexcept { return m_Name; }

	private:
		ULID m_Id;
		std::string m_Name;
#ifdef BRK_EDITOR
		std::string m_File;
		friend struct JsonLoader<SceneDescription, void>;

	public:
		static constexpr meta::FieldList<
			&SceneDescription::m_Id,
			&SceneDescription::m_Name,
			&SceneDescription::m_File>
			Fields{ "id", "name", "file" };
#else
		uint32 m_Offset = 0;
		friend struct BinaryLoader<SceneDescription, void>;

	public:
		static constexpr meta::FieldList<
			&SceneDescription::m_Id,
			&SceneDescription::m_Name,
			&SceneDescription::m_Offset>
			Fields{};
#endif
	};

} // namespace brk
