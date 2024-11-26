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
	enum class EMaterialOptions : uint8
	{
		None = 0,
	};

	/**
	 * Generic Material class. This is mostly meant to be subclassed, as it can't be
	 * loaded from a file.
	 */
	class Material : public Resource
	{
	public:
		using Resource::Resource;
		/**
		 * Creates a material from shader sources
		 * \param vShaderCode: Vertex shader source. May be empty
		 * \param fShaderCode: Fragment shader source. May be empty
		 * \param options: See EMaterialOptions
		 * \details If the provided source code is empty, a default implementation will be
		 * provided by the renderer. The default code can be found in
		 * [Shaders.hpp](Shaders.hpp)
		 */
		Material(
			StringView vShaderCode = {},
			StringView fShaderCode = {},
			EMaterialOptions options = EMaterialOptions::None);

		/**
		 * Same thing as the first constructor, except for the fact the parameter buffer
		 * is also initialized with the provided object
		 * \param parameters: This object will be directly uploaded to the GPU, and made
		 * accessible in the shaders as a cbuffer.
		 * \param vShaderCode: Vertex shader source. May be empty
		 * \param fShaderCode: Fragment shader source. May be empty
		 * \param options: See EMaterialOptions
		 * \details The parameter buffer, if valid, is made accessible in both the vertex
		 * and fragment shaders through a cbuffer. Said cbuffer is bound to register 0
		 */
		template <class P>
		Material(
			const P& parameters,
			StringView vShaderCode = {},
			StringView fShaderCode = {},
			EMaterialOptions options = EMaterialOptions::None);

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

		[[nodiscard]] EnumFlags<EMaterialOptions> GetOptions() const noexcept
		{
			return m_Options;
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
		EnumFlags<EMaterialOptions> m_Options;
	};
} // namespace brk::rdr

#include "Material.inl"