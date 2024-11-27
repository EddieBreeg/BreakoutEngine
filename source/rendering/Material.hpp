#pragma once

#include <PCH.hpp>
#include <core/EnumFlags.hpp>
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
		Material(
			const MaterialSettings& settings,
			ULID id = ULID::Generate(),
			std::string name = {});

		/**
		 * \warning This does NOT actually load anything, as this class doesn't have any
		 * concrete notion of what parameters should be passed to the shaders.
		 * It simply returns true if all shaders are valid, false otherwise
		 */
		bool DoLoad() noexcept override { return m_VertexShader && m_FragmentShader; }

		~Material() = default;

	private:
		friend class MaterialInstance;

		VertexShader m_VertexShader;
		FragmentShader m_FragmentShader;
		EnumFlags<MaterialSettings::EOptions> m_Options;
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

		[[nodiscard]] VertexShader& GetVertexShader() noexcept
		{
			return m_BaseMat->m_VertexShader;
		}

		[[nodiscard]] FragmentShader& GetFragmentShader() noexcept
		{
			return m_BaseMat->m_FragmentShader;
		}

		[[nodiscard]] Buffer& GetParamBuffer() { return m_ParamBuffer; }

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

#include "Material.inl"