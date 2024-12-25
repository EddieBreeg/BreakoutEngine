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
	class Renderer
	{
	public:
		BRK_RENDERING_API ~Renderer();

		BRK_RENDERING_API void Init(SDL_Window* window);
		BRK_RENDERING_API void Shutdown();

		BRK_RENDERING_API void ResizeFrameBuffers(uint32 width, uint32 height);

		/*
		 * Should be called before rendering anything, to setup the render targets
		 * properly
		 */
		BRK_RENDERING_API void StartRender();

		BRK_RENDERING_API void SetModelMatrix(const float4x4& transform);
		BRK_RENDERING_API void SetMaterial(const MaterialInstance& material);

		BRK_RENDERING_API void DrawIndexed(
			const Buffer& vertexBuffer,
			const Buffer& indexBuffer,
			uint32 numIndices);

		BRK_RENDERING_API void RenderUI();

		/** Swaps the back and front buffers */
		BRK_RENDERING_API void Present();

		BRK_RENDERING_API static Renderer s_Instance;
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
