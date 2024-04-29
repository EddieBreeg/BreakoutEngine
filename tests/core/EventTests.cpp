#include <core/Event.hpp>
#include <cassert>

namespace brk::core::ut {
	enum Events
	{
		E0,
		E1,
		E2,
	};
}

template <>
struct brk::EventData<brk::core::ut::E0>
{
	static int count;
	EventData() { ++count; }
};
int brk::EventData<brk::core::ut::E0>::count = 0;

template <>
struct brk::EventData<brk::core::ut::E1>
{
	int val = 0;
};

template <>
struct brk::EventData<brk::core::ut::E2>
{
	static int count;
	EventData() { ++count; }
	~EventData() { --count; }
};
int brk::EventData<brk::core::ut::E2>::count = 0;

namespace brk::core::ut {
	void EventTests()
	{
		{
			using TEvent = brk::Event<E0, E1>;
			const auto e = TEvent::Create<E0>();
			assert(brk::EventData<E0>::count == 1);
		}
		assert(brk::EventData<E0>::count == 1);
		brk::EventData<E0>::count = 0;
		{
			using TEvent = brk::Event<E0, E1, E2>;
			const auto e = TEvent::Create<E2>();
			assert(brk::EventData<E0>::count == 0);
			assert(brk::EventData<E2>::count == 1);
		}
		assert(brk::EventData<E2>::count == 0);
		{
			using TEvent = brk::Event<E0, E1, E2>;
			auto e = TEvent::Create<E1>(666);
			assert(e.GetType() == E1);
			const auto data = e.GetData<E1>();
			assert(data.val == 666);
		}
	}
} // namespace brk::core::ut
