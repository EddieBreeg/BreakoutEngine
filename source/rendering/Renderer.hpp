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
		void Shutdown();

#ifdef BRK_DEV
		void NewImGuiFrame();
#endif

		void StartRender();
		void DoRender();

		static Renderer s_Instance;
		float4 m_ClearColor;

	private:
		Renderer() = default;
		Renderer(const Renderer&) = delete;

		SDL_Window* m_Window = nullptr;
		RendererData* m_Data = nullptr;
	};
} // namespace brk::rdr
