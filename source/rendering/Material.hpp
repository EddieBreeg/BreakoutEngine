#pragma once

#include <PCH.hpp>
#include <core/Resource.hpp>
#include <core/EnumFlags.hpp>
#include "Buffer.hpp"
#include "Shaders.hpp"

namespace brk::rdr {
	/**
	 * TODO: Add material options
	 */

	struct MaterialSettings
	{
		enum EOptions : uint8
		{
			None = 0,
			DynamicBufferParam = BIT(0),
			NOptions
		};

		StringView m_VertexSourceCode;
		StringView m_FragmentSourceCode;
		EnumFlags<EOptions> m_Options;
	};

	/**
	 * Generic Material class. This is mostly meant to be subclassed, as it can't be
	 * loaded from a file.
	 */
	class Material : public Resource
	{
	public:
		using Resource::Resource;

		Material(const MaterialSettings& settings);

		/**
		 * Same thing as the first constructor, but additionally initializes the parameter
		 * buffer with the provided object
		 * \param parameters: The object which will be uploaded to the parameter buffer
		 */
		template <class P>
		Material(P&& parameters, const MaterialSettings& settings);

		[[nodiscard]] VertexShader& GetVertexShader() noexcept { return m_VertexShader; }
		[[nodiscard]] const VertexShader& GetVertexShader() const noexcept
		{
			return m_VertexShader;
		}

		[[nodiscard]] FragmentShader& GetFragmentShader() noexcept
		{
			return m_FragmentShader;
		}

		[[nodiscard]] Buffer& GetParamBuffer() noexcept { return m_ParamBuffer; }
		[[nodiscard]] const Buffer& GetParamBuffer() const noexcept
		{
			return m_ParamBuffer;
		}

		[[nodiscard]] MaterialSettings::EOptions GetOptions() const noexcept
		{
			return m_Options.Get();
		}

		/**
		 * Updates or create the parameter buffer with the provided object
		 * \param parameters: This object will be directly uploaded to the GPU, and made
		 * accessible in the shaders as a cbuffer.
		 */
		template <class P>
		void SetParameters(P&& params);

		~Material() = default;

	private:
		VertexShader m_VertexShader;
		FragmentShader m_FragmentShader;
		Buffer m_ParamBuffer;
		EnumFlags<MaterialSettings::EOptions> m_Options;
	};
} // namespace brk::rdr

#include "Material.inl"