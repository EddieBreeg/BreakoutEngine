#pragma once

#include <PCH.hpp>
#include "ULID.hpp"
#include <string>

namespace brk
{
	class Resource
	{
	public:
		Resource(
			const ULID id,
			const std::string& name,
			const std::string& filePath
#ifndef BRK_DEV
			,
			const uint32 offset
#endif
			)
			: m_Id{ id }
			, m_Name{ name }
			, m_FilePath{ filePath }
#ifndef BRK_DEV
			, m_Offset{ offset }
#endif
		{}
		virtual ~Resource();

		virtual bool DoLoad() { return false; }

	protected:
		const ULID m_Id;
		const std::string m_Name;
		const std::string m_FilePath;
#ifndef BRK_DEV
		const uint32 m_Offset;
#endif
	private:
		template <class R>
		friend class ResourceRef;

		uint32 m_RefCount = 0;
	};
} // namespace brk
