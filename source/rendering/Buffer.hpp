#pragma once

#include <PCH.hpp>
#include "Resource.hpp"
#include <core/EnumFlags.hpp>
#include <core/Resource.hpp>

namespace brk::rdr {
	enum class EBufferOptions : uint8
	{
		None,
		CpuWrite = BIT(0),
		CpuRead = BIT(1),
		Dynamic = BIT(2), /* Specifies the contents of the buffer will be frequently
							 updated. Imples CpuWrite */
	};

	class Buffer : public BasicResource<Buffer>
	{
	public:
		enum EType : int8
		{
			Invalid = -1,
			VertexBuffer,
			IndexBuffer,
			ParamBuffer,
			NumTypes
		};

		static constexpr EnumFlags s_DefaultOptions{ EBufferOptions::CpuWrite };

		using BasicResource<Buffer>::BasicResource;

		Buffer(
			EType type,
			uint32 size,
			const void* data = nullptr,
			EnumFlags<EBufferOptions> options = s_DefaultOptions);

		template <class T>
		Buffer(
			EType type,
			const T* data,
			uint32 n,
			EnumFlags<EBufferOptions> options = s_DefaultOptions)
			: Buffer{ type, n * sizeof(T), data, options }
		{}

		template <class T>
		Buffer(
			EType type,
			const T& data,
			EnumFlags<EBufferOptions> options = s_DefaultOptions)
			: Buffer{ type, sizeof(data), &data, options }
		{}

		Buffer(Buffer&& other) noexcept
			: m_Type{ other.m_Type }
			, m_Options{ other.m_Options }
		{
			m_Handle = other.m_Handle;
			other.m_Handle = nullptr;
			other.m_Type = Invalid;
		}

		Buffer& operator=(Buffer&& other) noexcept
		{
			std::swap(m_Handle, other.m_Handle);
			m_Type = other.m_Type;
			other.m_Type = Invalid;
			m_Options = other.m_Options;
			return *this;
		}

		void SetData(const void* data, uint32 size);
		template <class T>
		void SetData(const T* data, uint32 n)
		{
			SetData(data, n * sizeof(T));
		}

		template <class T>
		void SetData(const T& data)
		{
			SetData((const void*)&data, sizeof(T));
		}

		~Buffer();

	private:
		EType m_Type = EType::Invalid;
		EnumFlags<EBufferOptions> m_Options = s_DefaultOptions;
	};
} // namespace brk::rdr
