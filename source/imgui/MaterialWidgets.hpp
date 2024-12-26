#include <core/EnumFlags.hpp>
#include <core/ResourceFwd.hpp>
#include <core/ULID.hpp>
#include <rendering/Material.hpp>

namespace brk::rdr {
	class Material;

	class MaterialWidget : public ResourceUiWidget
	{
	public:
		MaterialWidget() = default;
		BRK_DEV_UI_API void Init(const Resource&) override;
		BRK_DEV_UI_API bool CreationUi() override;
		BRK_DEV_UI_API bool EditionUi(const Resource& resource, bool& out_shouldReload)
			override;

		BRK_DEV_UI_API void Commit(Resource& out_resource) const override;

	private:
		bool m_UseDefaultVertexShader = false;
		bool m_UseDefaultFragmentShader = false;
		EnumFlags<MaterialSettings::EOptions> m_Options;
	};

	class MaterialInstanceWidget : public ResourceUiWidget
	{
	public:
		MaterialInstanceWidget() = default;
		BRK_DEV_UI_API void Init(const Resource&) override;
		BRK_DEV_UI_API bool CreationUi() override;
		BRK_DEV_UI_API bool EditionUi(const Resource&, bool& out_shouldReload);

		BRK_DEV_UI_API void Commit(Resource& out_res) const override;

	private:
		bool ResourceSelector(
			const char* label,
			ULID& current,
			const ResourceTypeInfo& type,
			bool allowReset,
			int32& stackId);

		ULID m_MatId;
		ULID m_TexIds[8];
		const TULIDMap<Resource*>* m_Resources = nullptr;
		bool m_ShouldReload = false;
	};
} // namespace brk::rdr
