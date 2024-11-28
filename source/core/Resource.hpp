#pragma once

#include <PCH.hpp>
#include "FieldList.hpp"
#include "InputFile.hpp"
#include "LoadersFwd.hpp"
#include "ULID.hpp"

#include <string>

namespace brk {
	template<class T>
	struct RetainTraits;

	/**
	 * Represents an generic resource. This class is meant to be subclassed for specific
	 * types of resources.
	 * A subclass should meet the following criteria:
	 * - Be constructible from a ULID object
	 */
	class Resource
	{
	public:
		enum ELoadingState
		{
			Unloaded,
			Loading,
			Loaded,
			Unloading,
		};

		Resource(const ULID id = ULID{});
		Resource(const ULID id, std::string&& name, std::string&& file);
		/**
		 * Unloads the resource.
		 */
		virtual ~Resource();

		/**
		 * \return true if the resource was loaded succesfully, false otherwise.
		 */
		virtual bool DoLoad() { return false; }
		virtual void DoUnload() { m_LoadingState = ELoadingState::Unloaded; }

		[[nodiscard]] ELoadingState GetLoadingState() const noexcept
		{
			return m_LoadingState;
		}
		[[nodiscard]] const ULID GetId() const noexcept { return m_Id; }
		[[nodiscard]] const std::string& GetName() const noexcept { return m_Name; }
		[[nodiscard]] const std::string& GetFile() const noexcept { return m_FilePath; }
		[[nodiscard]] uint32 GetRefCount() const noexcept { return m_RefCount; }

	protected:
		[[nodiscard]] InputByteStream LoadFileContents();

		ULID m_Id;
		std::string m_Name;
		std::string m_FilePath;
#ifndef BRK_DEV
		uint32 m_Offset;
#endif
		uint32 m_RefCount = 0;
		ELoadingState m_LoadingState = Unloaded;

		friend class ResourceLoadingSystem;
		friend class ResourceManager;
		friend struct JsonLoader<Resource>;
		friend struct RetainTraits<Resource>;
	};

	template <>
	struct JsonLoader<Resource>
	{
		static bool Load(Resource& out_res, const nlohmann::json& json);
	};
} // namespace brk
