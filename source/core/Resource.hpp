#pragma once

#include <PCH.hpp>
#include "FieldList.hpp"
#include "LoadersFwd.hpp"
#include "ULID.hpp"
#include <string>

namespace brk {
	class Resource
	{
	public:
		Resource() = default;
		virtual ~Resource();

		virtual bool DoLoad() { return false; }
		virtual void DoUnload() {}

		[[nodiscard]] bool IsLoaded() const noexcept { return m_IsLoaded; }
		[[nodiscard]] const ULID GetId() const noexcept { return m_Id; }
		[[nodiscard]] const std::string& GetName() const noexcept { return m_Name; }
		[[nodiscard]] const std::string& GetFile() const noexcept { return m_FilePath; }

	private:
		ULID m_Id;
		std::string m_Name;
		std::string m_FilePath;
#ifndef BRK_DEV
		const uint32 m_Offset;
#endif
		uint32 m_RefCount = 0;
		bool m_IsLoaded = false;

		template <class R>
		friend class ResourceRef;
		friend class ResourceLoadingSystem;
		friend struct JsonLoader<Resource>;
	};

	template <>
	struct JsonLoader<Resource>
	{
		static bool Load(Resource& out_res, const nlohmann::json& json);
	};
} // namespace brk
