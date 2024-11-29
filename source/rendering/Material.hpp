#pragma once

#include <PCH.hpp>
#include <core/EnumFlags.hpp>
#include <core/LoadersFwd.hpp>
#include <core/Resource.hpp>
#include <core/ResourceRef.hpp>
#include "Buffer.hpp"
#include "Shaders.hpp"

namespace brk::rdr {
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
	class Material : public Resource
	{
	public:
		Material(const ULID& id);
		Material(
			const MaterialSettings& settings,
			const ULID& id = ULID::Generate(),
			std::string name = {});

		bool DoLoad() noexcept override;

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
	class MaterialInstance : public Resource
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

		bool DoLoad() override;

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
	};
} // namespace brk::rdr

namespace brk {
	template <>
	struct JsonLoader<rdr::Material, void>
	{
		static bool Load(rdr::Material& out_mat, const nlohmann::json& json);
		static void Save(const rdr::Material& mat, nlohmann::json& out_json);
	};
} // namespace brk

#include "Material.inl"