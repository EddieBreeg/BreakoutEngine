#include <core/Event.hpp>
#include <cassert>

namespace bre::tests::events {
	enum Events
	{
		E0,
		E1,
		E2,
	};

} // namespace bre::tests::events

namespace bre {
	template <>
	struct EventData<tests::events::E0>
	{
		static int count;
		EventData() { ++count; }
	};
	int EventData<tests::events::E0>::count = 0;

	template <>
	struct EventData<tests::events::E1>
	{
		int val = 0;
	};

	template <>
	struct EventData<tests::events::E2>
	{
		static int count;
		EventData() { ++count; }
		~EventData() { --count; }
	};
	int EventData<tests::events::E2>::count = 0;

} // namespace bre

int main(int argc, char const* argv[])
{
	using namespace bre::tests;
	{
		using TEvent = bre::Event<events::E0, events::E1>;
		const auto e = TEvent::Create<events::E0>();
		assert(bre::EventData<events::E0>::count == 1);
	}
	assert(bre::EventData<events::E0>::count == 1);
	bre::EventData<events::E0>::count = 0;
	{
		using TEvent = bre::Event<events::E0, events::E1, events::E2>;
		const auto e = TEvent::Create<events::E2>();
		assert(bre::EventData<events::E0>::count == 0);
		assert(bre::EventData<events::E2>::count == 1);
	}
	assert(bre::EventData<events::E2>::count == 0);
	{
		using TEvent = bre::Event<events::E0, events::E1, events::E2>;
		auto e = TEvent::Create<events::E1>(666);
		assert(e.GetType() == events::E1);
		const auto data = e.GetData<events::E1>();
		assert(data.val == 666);
	}
}
