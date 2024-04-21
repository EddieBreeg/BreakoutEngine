#include <systems/WindowSystem.hpp>

#include <core/App.hpp>
#include <core/Assert.hpp>

#include <SDL2/SDL.h>

namespace {
	template <bre::inputs::EEventType Evt, class... Args>
	inline entt::entity CreateInputEventEntity(entt::registry& world, Args&&... args)
	{
		entt::entity e = world.create();
		world.emplace<bre::inputs::EventOneFrameComponent>(
			e,
			bre::TInputEvent::Create<Evt>(std::forward<Args>(args)...));
		return e;
	}
} // namespace

void bre::WindowSystem::Terminate()
{
	if (!m_WinPtr)
		return;
	SDL_DestroyWindow(m_WinPtr);
	SDL_Quit();
}

bre::WindowSystem::~WindowSystem()
{
	Terminate();
}

void bre::WindowSystem::ProcessEvents(entt::registry& world)
{
	SDL_Event evt;

	while (SDL_PollEvent(&evt))
	{
		switch (evt.type)
		{
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

bre::WindowSystem::WindowSystem(const bre::WindowSystemSettings& settings)
{
	const int initCode = SDL_Init(SDL_INIT_VIDEO);
	BREAKOUT_ASSERT(!initCode, "Failed to initialize SDL: {}", SDL_GetError());

	m_WinPtr = SDL_CreateWindow(settings.m_Title,
								SDL_WINDOWPOS_CENTERED,
								SDL_WINDOWPOS_CENTERED,
								settings.m_Width,
								settings.m_Height,
								settings.m_Flags);
	BREAKOUT_ASSERT(m_WinPtr, "Failed to create window: {}", SDL_GetError());
}

void bre::WindowSystem::Update(World& world, const bre::TimeInfo& timeInfo)
{
	// remove events from previous frame
	for (entt::entity event : world)
		world.GetEntityWorld().destroy(event);

	ProcessEvents(world.GetEntityWorld());
}
