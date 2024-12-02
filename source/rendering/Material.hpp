#pragma once

#include <PCH.hpp>
#include <core/EnumFlags.hpp>
#include <core/LoadersFwd.hpp>
#include <core/Resource.hpp>
#include <core/ResourceRef.hpp>
#include "Buffer.hpp"
#include "Shaders.hpp"

namespace brk::rdr {
	class Texture2d;

	struct MaterialSettings
	{
		enum EOptions : uint8
		{
			None = 0,
			// Indicates the parameter buffer in material instances should be
			// created with the Dynamic option
			DynamicBufferParam = BIT(0),
			NOptions
		};

		/* Vertex shader source code. May be empty, in which case the default vertex
		 * shader will be used (see the Shaders.hpp header)*/
		StringView m_VertexSourceCode;
		/* Fragment shader source code. May be empty, in which case the default fragment
		 * shader will be used (see the Shaders.hpp header)*/
		StringView m_FragmentSourceCode;
		EnumFlags<EOptions> m_Options;
	};

	/**
	 * Generic Material class. The material contains all the information to define the
	 * rendering pipeline, i.e. shaders, rasterizer settings etc...
	 */
	class BRK_RENDERING_API Material : public Resource
	{
	public:
		Material(const ULID& id);
		/**
		 * Manually creates a shader
		 * \param settings: The material's settings. If source code is provided, the
		 * constructor will attempt to compile it. Otherwise, the default implementations
		 * will be used (see Shaders.hpp)
		 * \param id: The specific ULID to use
		 * \param name: The resource's name
		 * \param file: An optional hlsl file to load source code from. This will only get
		 * used if shader compilation fails, or if the material gets unloaded and reloaded
		 * at a later point.
		 */
		Material(
			const MaterialSettings& settings,
			const ULID& id = ULID::Generate(),
			std::string name = {},
			std::string filePath = {});

		bool DoLoad() noexcept override;
		void DoUnload() noexcept override;

		static constexpr StringView Name = "material";

		~Material() = default;

	private:
		friend class MaterialInstance;
		friend struct brk::JsonLoader<Material, void>;

		VertexShader m_VertexShader;
		FragmentShader m_FragmentShader;
		EnumFlags<MaterialSettings::EOptions> m_Options;
		/** Only used when loading the material */
		bool m_UseDefaultVertexShader : 1;
		bool m_UseDefaultFragmentShader : 1;
	};

	/**
	 * A specific instance of a material, with the associated parameters.
	 * This is meant to be subclassed, because it is unaware of the actual type of
	 * parameters to upload to the GPU, and therefore can't be loaded from a file
	 */
	class BRK_RENDERING_API MaterialInstance : public Resource
	{
	public:
		/**
		 * Initializes the material instance, without a null param buffer
		 * \param baseMat: The material this instance is based on
		 * \param id: The resource id (passed to the Resource constructor)
		 * \param name: The resource name (passed to the Resource constructor)
		 */
		MaterialInstance(
			ResourceRef<Material> baseMat,
			const ULID& id = ULID::Generate(),
			std::string name = {});
		/**
		 * Initializes the material instance and uploads parameters to the GPU
		 * \param baseMat: The material this instance is based on
		 * \param parameters: The data to upload to the param buffer
		 * \param id: The resource id (passed to the Resource constructor)
		 * \param name: The resource name (passed to the Resource constructor)
		 */
		template <class P>
		MaterialInstance(
			ResourceRef<Material> baseMat,
			P&& parameters,
			const ULID& id = ULID::Generate(),
			std::string name = {});

		~MaterialInstance();

		static constexpr uint32 s_MaxTextureCount = 8;

		void SetTexture(uint32 slot, ResourceRef<Texture2d> texture);
		void SetTextures(
			const ResourceRef<Texture2d>* textures,
			uint32 numTextures,
			uint32 startSlot = 0);
		template <uint32 N>
		void SetTextures(
			const ResourceRef<Texture2d> (&textures)[N],
			uint32 startSlot = 0)
		{
			SetTextures(textures, N, startSlot);
		}

		[[nodiscard]] const ResourceRef<Texture2d>* GetTextures() const noexcept
		{
			return m_Textures;
		}

		bool DoLoad() override;
		void DoUnload() override;

		[[nodiscard]] const VertexShader& GetVertexShader() const
		{
			return m_BaseMat->m_VertexShader;
		}

		[[nodiscard]] const FragmentShader& GetFragmentShader() const
		{
			return m_BaseMat->m_FragmentShader;
		}

		[[nodiscard]] const Buffer& GetParamBuffer() const noexcept
		{
			return m_ParamBuffer;
		}

		[[nodiscard]] MaterialSettings::EOptions GetOptions() const
		{
			return m_BaseMat->m_Options.Get();
		}

		/**
		 * Updates the parameter buffer with the provided object
		 * \param params: This object will be directly uploaded to the GPU, and made
		 * accessible in the shaders as a cbuffer.
		 */
		template <class P>
		void SetParameters(P&& params);

	private:
		static EBufferOptions GetBufferOptions(MaterialSettings::EOptions matOptions);

		ResourceRef<Material> m_BaseMat;
		Buffer m_ParamBuffer;
		ResourceRef<Texture2d> m_Textures[s_MaxTextureCount];
		ULID m_ResourceIds[1 + s_MaxTextureCount] = {};
	};
} // namespace brk::rdr

namespace brk {
	template <>
	struct BRK_RENDERING_API JsonLoader<rdr::Material, void>
	{
		static bool Load(rdr::Material& out_mat, const nlohmann::json& json);
		static void Save(const rdr::Material& mat, nlohmann::json& out_json);
	};
} // namespace brk

#include "Material.inl"