#pragma

#include <PCH.hpp>

namespace brk::rdr
{
	class Renderer
	{
	public:
		~Renderer();
	
		static Renderer s_Instance;

	private:
		Renderer() = default;
		Renderer(const Renderer&) = delete;
	};
} // namespace brk::rdr
