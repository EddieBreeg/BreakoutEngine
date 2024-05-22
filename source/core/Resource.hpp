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
		enum ELoadingState
		{
			Unloaded,
			Loading,
			Loaded,
		};

		Resource(const ULID id = ULID{});
		virtual ~Resource();

		virtual bool DoLoad() { return false; }
		virtual void DoUnload() {}

		[[nodiscard]] ELoadingState GetLoadingState() const noexcept
		{
			return m_LoadingState;
		}
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
		ELoadingState m_LoadingState = Unloaded;

		friend class ResourceLoadingSystem;
		friend class ResourceManager;
		template <class R>
		friend class ResourceRef;
		friend struct JsonLoader<Resource>;
	};

	template <>
	struct JsonLoader<Resource>
	{
		static bool Load(Resource& out_res, const nlohmann::json& json);
	};
} // namespace brk
