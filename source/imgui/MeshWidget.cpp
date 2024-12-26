#include "MeshWidget.hpp"
#include "MiscWidgets.hpp"

#include <core/Resource.hpp>
#include <rendering/Mesh.hpp>

namespace brk::rdr {
	void MeshWidget::Init(const Resource& res)
	{
		const auto& mesh = static_cast<const Mesh&>(res);
		m_Id = mesh.m_Id;
		m_Name = mesh.m_Name;
		m_FilePath = mesh.m_FilePath;
	}

	bool MeshWidget::CreationUi()
	{
		dev_ui::ULIDWidget("ULID", m_Id);
		dev_ui::StdStringInput("Mesh Name", m_Name);
		dev_ui::FilePathInput("File", m_FilePath, false);
		return false;
	}

	bool MeshWidget::EditionUi(const Resource& res, bool& out_shouldReload)
	{
		if (!CreationUi())
			return false;
		out_shouldReload = m_FilePath != res.GetFile();
		return out_shouldReload || m_Name != res.GetName();
	}
} // namespace brk::rdr
