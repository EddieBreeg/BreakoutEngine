#pragma once

#include <PCH.hpp>
#include <core/ResourceFwd.hpp>
#include <rendering/Texture.hpp>

namespace brk::rdr {
	class Texture2dWidget : public ResourceUiWidget
	{
	public:
		Texture2dWidget() = default;
		BRK_DEV_UI_API virtual void Init(const Resource& res) override;
		BRK_DEV_UI_API virtual bool CreationUi() override;
		BRK_DEV_UI_API virtual bool EditionUi(const Resource&, bool& out_shouldReload)
			override;
		BRK_DEV_UI_API virtual void Commit(Resource& out_res) const override;

	private:
		Texture2dSettings m_Settings;
	};
} // namespace brk::rdr
