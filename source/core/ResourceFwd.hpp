#pragma once

#include <PCH.hpp>
#include "FieldList.hpp"
#include "IO.hpp"
#include "LoadersFwd.hpp"
#include "MemoryPool.hpp"
#include "ULID.hpp"

#include <string>

namespace brk::editor::ui {
	struct UiData;
}

namespace std::pmr {
	class memory_resource;

	memory_resource* new_delete_resource() noexcept;
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

		[[nodiscard]] virtual ResourceTypeInfo& GetTypeInfo() const = 0;

		/**
		 * \return true if the resource was loaded succesfully, false otherwise.
		 */
		virtual bool DoLoad();
		virtual void DoUnload();

		/**
		 * If the internal ref count is 0, the resource is deleted immediately. Otherwise,
		 * it will get deleted when the last ResourceRef pointing to it gets destroyed.
		 * \warning This should only get called by the resource manager: the resource
		 * should be removed from the resource map.
		 */
		void MarkForDeletion();

		[[nodiscard]] bool IsLoaded() const noexcept;

		[[nodiscard]] bool GetSavingDisabled() const noexcept;
		[[nodiscard]] void SetSavingDisabled(bool disabled = true) noexcept;

		[[nodiscard]] EStateFlags GetLoadingState() const noexcept
		{
			return EStateFlags(m_State & EStateFlags::LoadingStateMask);
		}
		[[nodiscard]] const ULID GetId() const noexcept;
		[[nodiscard]] const std::string& GetName() const noexcept;
		[[nodiscard]] const std::string& GetFile() const noexcept;
		[[nodiscard]] uint32 GetRefCount() const noexcept;

	protected:
		void SetLoadingState(EStateFlags state);

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
		virtual void Init(const Resource&);
		virtual bool CreationUi();
		virtual bool EditionUi(const Resource& res, bool& out_shouldReload);

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

	struct BRK_CORE_API ResourceDeleter
	{
		void operator()(Resource* ptr) const;
	};

	/**
	 * Contains runtime data about a specific type of resource (constructor, memory pool,
	 * type name etc...). This gets instanciated once per resource class, and each
	 * instance must be defined explicitly (see the RES_INFO_IMPL macros below).
	 */
	struct BRK_CORE_API ResourceTypeInfo
	{
		ResourceTypeInfo(const ResourceTypeInfo&) = delete;

		~ResourceTypeInfo();

		StringView m_TypeName;
		MemoryPool m_Pool;
		Resource* (*m_Constructor)(MemoryPool&, const ULID&) = nullptr;
#ifdef BRK_EDITOR
		ResourceUiWidget* m_Widget = nullptr;
#endif
		bool (*m_Load)(Resource&, const nlohmann::json&) = nullptr;
		void (*m_Save)(const Resource&, nlohmann::json&) = nullptr;

		Resource* NewResource(const ULID& id);
		void DestroyResource(Resource*);

		template <class Res, class Widget>
		static ResourceTypeInfo& InitFor(
			const StringView name,
			std::pmr::memory_resource* upstreamAllocator =
				std::pmr::new_delete_resource());
		template <class Res>
		static void ResetFor();

		template <class Res>
		static ResourceTypeInfo& GetFor();

	private:
		template <class R, class W>
		ResourceTypeInfo(
			InPlaceType<R, W>,
			const StringView name,
			std::pmr::memory_resource* allocator);

		template <class R>
		struct Impl
		{
			static std::unique_ptr<ResourceTypeInfo> s_Info;
		};

		ResourceTypeInfo() = default;
	};

/**
 * Implements the type info instance for a given resource type
 * \param Type: The resource type
 * \param Attr: Attributes for the implementation, like dll linkage
 */
#define RES_INFO_IMPL(Type, Attr)                                                        \
	inline ResourceTypeInfo& Type::GetTypeInfo() const                          \
	{                                                                                    \
		return ResourceTypeInfo::GetFor<Type>();                                         \
	}                                                                                    \
                                                                                         \
	template <>                                                                          \
	struct Attr ResourceTypeInfo::Impl<Type>                                             \
	{                                                                                    \
		static inline std::unique_ptr<ResourceTypeInfo> s_Info;                          \
	}

/**
 * Same as RES_INFO_IMPL, but without the attributes. Used in unit tests.
 */
#define RES_INFO_IMPL_NO_ATTR(Type)                                                      \
	inline ResourceTypeInfo& Type::GetTypeInfo() const                          \
	{                                                                                    \
		return ResourceTypeInfo::GetFor<Type>();                                         \
	}                                                                                    \
                                                                                         \
	template <>                                                                          \
	struct ResourceTypeInfo::Impl<Type>                                                  \
	{                                                                                    \
		static inline std::unique_ptr<ResourceTypeInfo> s_Info;                          \
	}

	namespace _internal {
		template <class T, class = void>
		struct ValidResourceType : std::false_type
		{};
		template <class R>
		struct ValidResourceType<
			R,
			std::enable_if_t<
				std::is_base_of_v<Resource, R> && std::is_constructible_v<R, const ULID>>>
			: std::true_type
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