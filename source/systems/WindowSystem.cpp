#include <systems/WindowSystem.hpp>

#include <core/App.hpp>
#include <core/Assert.hpp>

#include <SDL2/SDL.h>

namespace {
	template <brk::inputs::EEventType Evt, class... Args>
	inline entt::entity CreateInputEventEntity(brk::WindowSystem::World& world,
											   Args&&... args)
	{
		entt::entity e = world.CreateEntity();
		world.AddComponent<brk::inputs::EventOneFrameComponent>(
			e,
			brk::TInputEvent::Create<Evt>(std::forward<Args>(args)...));
		return e;
	}
} // namespace

void brk::WindowSystem::Terminate()
{
	if (!m_WinPtr)
		return;
	SDL_DestroyWindow(m_WinPtr);
	SDL_Quit();
}

brk::WindowSystem::~WindowSystem()
{
	Terminate();
}

void brk::WindowSystem::ProcessEvents(World& world)
{
	SDL_Event evt;

	while (SDL_PollEvent(&evt))
	{
		switch (evt.type)
		{
		case SDL_QUIT:
		case SDL_WINDOWEVENT_CLOSE: App::GetInstance().Terminate(); break;
		case SDL_MOUSEMOTION:
		{
			const int2 pos{ evt.motion.x, evt.motion.y };
			const int2 oldPos = pos - int2{ evt.motion.xrel, evt.motion.yrel };
			CreateInputEventEntity<inputs::MouseMove>(world, oldPos, pos);
			break;
		}
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN:
			CreateInputEventEntity<inputs::MouseButton>(
				world,
				evt.button.button,
				evt.button.state == SDL_PRESSED,
				evt.button.clicks,
				int2{ evt.button.x, evt.button.y });
			break;
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			CreateInputEventEntity<inputs::Key>(world,
												evt.key.keysym.sym,
												evt.key.keysym.mod,
												evt.key.state == SDL_PRESSED,
												evt.key.repeat);
		default: break;
		}
	}
}

brk::WindowSystem::WindowSystem(const brk::WindowSystemSettings& settings)
{
	const int initCode = SDL_Init(SDL_INIT_VIDEO);
	BRK_ASSERT(!initCode, "Failed to initialize SDL: {}", SDL_GetError());

	m_WinPtr = SDL_CreateWindow(settings.m_Title,
								SDL_WINDOWPOS_CENTERED,
								SDL_WINDOWPOS_CENTERED,
								settings.m_Width,
								settings.m_Height,
								settings.m_Flags);
	BRK_ASSERT(m_WinPtr, "Failed to create window: {}", SDL_GetError());
}

void brk::WindowSystem::Update(World& world, const brk::TimeInfo& timeInfo)
{
	using TEventQuery = ecs::query::Include<const inputs::EventOneFrameComponent>;
	// remove events from previous frame
	for (entt::entity event : world.Query<TEventQuery>())
		world.DestroyEntity(event);

	ProcessEvents(world);
}
