#pragma once

#include <PCH.hpp>
#include "FieldList.hpp"
#include "IO.hpp"
#include "LoadersFwd.hpp"
#include "ULID.hpp"

#include <string>

namespace brk::editor::ui {
	struct UiData;
}

namespace brk {
	template <class T>
	struct RetainTraits;

	/**
	 * Represents an generic resource. This class is meant to be subclassed for specific
	 * types of resources.
	 * A subclass should meet the following criteria:
	 * - Be constructible from a ULID object
	 */
	class BRK_CORE_API Resource
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

		[[nodiscard]] bool IsLoaded() const noexcept
		{
			return m_LoadingState == ELoadingState::Loaded;
		}

		[[nodiscard]] ELoadingState GetLoadingState() const noexcept
		{
			return m_LoadingState;
		}
		[[nodiscard]] const ULID GetId() const noexcept { return m_Id; }
		[[nodiscard]] const std::string& GetName() const noexcept { return m_Name; }
		[[nodiscard]] const std::string& GetFile() const noexcept { return m_FilePath; }
		[[nodiscard]] uint32 GetRefCount() const noexcept { return m_RefCount; }

#ifdef BRK_EDITOR
		/**
		 * UI Widget used to create a new resource. Returns true when the resource is
		 * ready to be added to the manager.
		 */
		virtual bool CreationUiWidget() { return false; }
#endif

	protected:
		[[nodiscard]] DynamicArrayStream LoadFileContents();

		ULID m_Id;
		std::string m_Name;
		std::string m_FilePath;
#ifndef BRK_DEV
		uint32 m_Offset;
#endif
		std::atomic<uint32> m_RefCount = 0;
		std::atomic<ELoadingState> m_LoadingState = Unloaded;

		friend class ResourceLoader;
		friend class ResourceLoadingSystem;
		friend struct JsonLoader<Resource>;
		friend struct RetainTraits<Resource>;
	};

	template <>
	struct BRK_CORE_API JsonLoader<Resource>
	{
		static bool Load(Resource& out_res, const nlohmann::json& json);
	};
} // namespace brk
