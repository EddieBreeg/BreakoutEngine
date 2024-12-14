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

	struct ResourceTypeInfo;

	/**
	 * Represents an generic resource. This class is meant to be subclassed for specific
	 * types of resources.
	 * A subclass should meet the following criteria:
	 * - Be constructible from a ULID object
	 */
	class BRK_CORE_API Resource
	{
	public:
		enum EStateFlags : uint8
		{
			Unloaded = 0,
			Loading = 1,
			Unloading = 2,
			Reloading = 3,
			Loaded = 4,
			MarkedForDeletion = 5,
			LoadingStateMask = 7,

			SavingDisabled = BIT(3),
		};

		Resource(const ULID id = ULID{});
		Resource(const ULID id, std::string&& name, std::string&& file);
		/**
		 * Unloads the resource.
		 */
		virtual ~Resource();

		[[nodiscard]] virtual const ResourceTypeInfo& GetTypeInfo() const noexcept = 0;

		/**
		 * \return true if the resource was loaded succesfully, false otherwise.
		 */
		virtual bool DoLoad() { return false; }
		virtual void DoUnload() { SetLoadingState(Unloaded); }

		/**
		 * If the internal ref count is 0, the resource is deleted immediately. Otherwise,
		 * it will get deleted when the last ResourceRef pointing to it gets destroyed.
		 * \warning This should only get called by the resource manager: the resource
		 * should be removed from the resource map.
		 */
		void MarkForDeletion();

		[[nodiscard]] bool IsLoaded() const noexcept
		{
			return GetLoadingState() == EStateFlags::Loaded;
		}

		[[nodiscard]] bool GetSavingDisabled() const noexcept
		{
			return (m_State & EStateFlags::SavingDisabled);
		}
		[[nodiscard]] void SetSavingDisabled(bool disabled = true) noexcept
		{
			const EStateFlags flagBit = EStateFlags(uint8(disabled) << 3);
			m_State = EStateFlags((m_State & ~SavingDisabled) | flagBit);
		}

		[[nodiscard]] EStateFlags GetLoadingState() const noexcept
		{
			return EStateFlags(m_State & EStateFlags::LoadingStateMask);
		}
		[[nodiscard]] const ULID GetId() const noexcept { return m_Id; }
		[[nodiscard]] const std::string& GetName() const noexcept { return m_Name; }
		[[nodiscard]] const std::string& GetFile() const noexcept { return m_FilePath; }
		[[nodiscard]] uint32 GetRefCount() const noexcept { return m_RefCount; }

	protected:
		void SetLoadingState(EStateFlags state)
		{
			m_State = EStateFlags((m_State & ~LoadingStateMask) | state);
		}

		[[nodiscard]] DynamicArrayStream LoadFileContents();

		ULID m_Id;
		std::string m_Name;
		std::string m_FilePath;
#ifndef BRK_DEV
		uint32 m_Offset;
#endif
		std::atomic<uint32> m_RefCount = 0;
		std::atomic<EStateFlags> m_State = EStateFlags::Unloaded;

		friend class ResourceLoader;
		friend struct JsonLoader<Resource>;
		friend struct RetainTraits<Resource>;
	};

#ifdef BRK_EDITOR
	struct ResourceUiWidget
	{
		ResourceUiWidget() = default;
		virtual void Init(const Resource&) {}
		virtual bool CreationUi() { return false; }
		virtual bool EditionUi(const Resource& res, bool& out_shouldReload)
		{
			return false;
		}
		virtual void Commit(Resource& inout_res) const {}
		virtual ~ResourceUiWidget() = default;

	protected:
		ULID m_Id;
		std::string m_Name, m_FilePath;
	};
#endif

	template <>
	struct BRK_CORE_API JsonLoader<Resource>
	{
		static bool Load(Resource& out_res, const nlohmann::json& json);
		static void Save(const Resource& res, nlohmann::json& out_json);
	};

	struct ResourceTypeInfo
	{
		StringView m_TypeName;
		Resource* (*m_Constructor)(const ULID) = nullptr;
#ifdef BRK_EDITOR
		ResourceUiWidget* m_Widget = nullptr;
#endif
		bool (*m_Load)(Resource&, const nlohmann::json&) = nullptr;
		void (*m_Save)(const Resource&, nlohmann::json&) = nullptr;

		template <class R, class Widget = void>
		static ResourceTypeInfo Create(StringView name);
	};

	namespace _internal {
		template <class T, class = void>
		struct ValidResourceType : std::false_type
		{};
		template <class R>
		struct ValidResourceType<
			R,
			std::enable_if_t<
				std::is_base_of_v<Resource, R> &&
				std::is_same_v<decltype(R::Info), const ResourceTypeInfo> &&
				std::is_constructible_v<R, const ULID>>> : std::true_type
		{};
	} // namespace _internal

	namespace meta {

		/**
		 * Determines whether R is a valid resource type, that is:
		 * - R inherits from Resource
		 * - R is constructible from a ULID object
		 * - R::Info is an accessible const ResourceTypeInfo object, containing relevant
		 * runtime information about R
		 */
		template <class R>
		static constexpr bool IsResourceType =
			brk::_internal::ValidResourceType<R>::value;
	} // namespace meta
} // namespace brk

#include "Resource.inl"