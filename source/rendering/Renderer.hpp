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
		void EndImGuiFrame();
#endif

		void StartRender(const float4 clearColor);
		void DoRender();

		static Renderer s_Instance;

	private:
		Renderer() = default;
		Renderer(const Renderer&) = delete;

		SDL_Window* m_Window = nullptr;
		RendererData* m_Data = nullptr;
	};
} // namespace brk::rdr
