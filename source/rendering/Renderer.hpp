#pragma

#include <PCH.hpp>
#include <math/Matrix.hpp>

struct SDL_Window;

struct ImGuiContext;

namespace brk::rdr {
	class Buffer;
	class MaterialInstance;

	struct RendererData;

	/** Platform agnostic rendering api */
	class BRK_RENDERING_API Renderer
	{
	public:
		~Renderer();

		void Init(ImGuiContext& context, SDL_Window* window);
		void Shutdown();

#ifdef BRK_DEV
		void NewImGuiFrame();
#endif

		void ResizeFrameBuffers(uint32 width, uint32 height);

		/*
		 * Should be called before rendering anything, to setup the render targets
		 * properly
		 */
		void StartRender();

		void SetModelMatrix(const float4x4& transform);
		void SetMaterial(const MaterialInstance& material);

		void DrawIndexed(
			const Buffer& vertexBuffer,
			const Buffer& indexBuffer,
			uint32 numIndices);

		void RenderUI();

		/** Swaps the back and front buffers */
		void Present();

		static Renderer s_Instance;
		float4 m_ClearColor;

		/* Private API access */
		[[nodiscard]] RendererData* GetData() noexcept { return m_Data; }

	private:
		Renderer() = default;
		Renderer(const Renderer&) = delete;

		SDL_Window* m_Window = nullptr;
		RendererData* m_Data = nullptr;
	};
} // namespace brk::rdr
