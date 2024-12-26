#pragma once

#include <PCH.hpp>
#include <core/ResourceFwd.hpp>

namespace brk::rdr {
	class MeshWidget : public ResourceUiWidget
	{
	public:
		MeshWidget() = default;
		BRK_DEV_UI_API void Init(const Resource& res) override;
		BRK_DEV_UI_API bool CreationUi() override;
		BRK_DEV_UI_API bool EditionUi(const Resource& res, bool& out_shouldReload)
			override;
	};
} // namespace brk::rdr
