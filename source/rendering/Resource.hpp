#pragma once

#include <PCH.hpp>

namespace brk::rdr {
	/**  default resource deleter, which does nothing
	 * should be specialized
	 * */
	template <class Handle>
	struct ResourceDeleter
	{
		void operator()(Handle) const noexcept {};
	};
} // namespace brk::rdr

#ifdef BRK_DX11
#include "dx11/ResourceHandles.hpp"
#endif

namespace brk::rdr {
	/** CRTP based rendering resource, provides basic functionnality
	 * \tparam R: The underlying resource type, which should inherit from BasicResource<R>
	 * \details ResourceHandle<R>::Type must be a valid type. It is also recommended to
	 * specialize ResourceDeleter for ResourceHandle<R>::Type, to make deletion automatic
	 */
	template <class R>
	class BasicResource
	{
	public:
		using HandleType = typename ResourceHandle<R>::Type;
		BasicResource() = default;

		BasicResource(const BasicResource&) = delete;
		BasicResource(BasicResource&& other)
			: m_Handle{ other.m_Handle }
		{
			other.m_Handle = Unassigned<HandleType>;
		}

		BasicResource& operator=(const BasicResource&) = delete;
		BasicResource& operator=(BasicResource&)
		{
			Swap(other);
			return *this;
		}

		[[nodiscard]] operator bool() const noexcept
		{
			return m_Handle != Unassigned<HandleType>;
		}

		~BasicResource()
		{
			if (m_Handle != Unassigned<HandleType>)
				ResourceDeleter<HandleType>{}(m_Handle);
		}

		void Swap(BasicResource& other) { std::swap(m_Handle, other.m_Handle); }

		[[nodiscard]] HandleType GetHandle() noexcept { return m_Handle; }

	protected:
		HandleType m_Handle = Unassigned<HandleType>;
	};
} // namespace brk::rdr
