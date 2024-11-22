#pragma

#include <PCH.hpp>

struct SDL_Window;

namespace brk::rdr {
	struct RendererData;

	/** Platform agnostic rendering api */
	class Renderer
	{
	public:
		~Renderer();

		void Init(SDL_Window* window);
		void Shutdown();

#ifdef BRK_DEV
		void NewImGuiFrame();
#endif

		void ResizeFrameBuffers(uint32 width, uint32 height);

		/* Should be called before rendering anything, to setup the render targets
		 * properly*/
		void StartRender();
		void RenderUI();

		/** Swaps the back and front buffers */
		void Present();

		static Renderer s_Instance;
		float4 m_ClearColor;

	private:
		Renderer() = default;
		Renderer(const Renderer&) = delete;

		SDL_Window* m_Window = nullptr;
		RendererData* m_Data = nullptr;
	};
} // namespace brk::rdr
