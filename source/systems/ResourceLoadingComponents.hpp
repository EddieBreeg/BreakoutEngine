#pragma once

#include <PCH.hpp>

namespace brk {
	class Resource;

	struct ResourceLoadRequestComponent
	{
		Resource* m_Res;
	};

	struct ResourceUnloadRequestComponent
	{
		Resource* m_Res;
	};
} // namespace brk
