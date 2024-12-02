#include <PCH.hpp>
#include "TypeDefs.hpp"
#include <core/StringView.hpp>

namespace brk::rdr::d3d {
	BRK_RENDERING_API ID3DBlob* CompileShader(
		StringView source,
		const char* target,
		const char* entryPoint);
} // namespace brk::rdr::d3d