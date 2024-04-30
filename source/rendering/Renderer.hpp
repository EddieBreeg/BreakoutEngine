#pragma

#include <PCH.hpp>

struct SDL_Window;

namespace brk::rdr
{
	struct RendererData;

	class Renderer
	{
	public:
		~Renderer();

		void Init(SDL_Window* window);
		static Renderer s_Instance;

	private:
		Renderer() = default;
		Renderer(const Renderer&) = delete;

		SDL_Window* m_Window = nullptr;
		RendererData* m_Data = nullptr;
	};
} // namespace brk::rdr
