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
		virtual ~Resource() = default;

		virtual bool DoLoad() { return false; }

	private:
		ULID m_Id;
		std::string m_Name;
		std::string m_FilePath;
#ifndef BRK_DEV
		const uint32 m_Offset;
#endif
		uint32 m_RefCount = 0;

		template <class R>
		friend class ResourceRef;
		friend class ResourceManager;
		friend struct JsonLoader<Resource>;
	};

	template<>
	struct JsonLoader<Resource>
	{
		static bool Load(Resource& out_res, const nlohmann::json& json);
	};
} // namespace brk
