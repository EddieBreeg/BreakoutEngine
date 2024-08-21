#pragma once

#include <PCH.hpp>

namespace brk::rdr::d3d {
	template <class T>
	struct ObjectDeleter
	{
		void operator()(T* obj) const { obj->Release(); }
	};

	template<class T, class D = ObjectDeleter<T>>
	using ObjectRef = std::unique_ptr<T, D>;
} // namespace brk::rdr::d3d
