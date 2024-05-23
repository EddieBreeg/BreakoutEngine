#include <core/Hash.hpp>
#include <core/ULID.hpp>

namespace brk::hash::ut {
	void Tests()
	{
		static_assert(IsNoThrowHashable<int>);
		static_assert(IsNoThrowHashable<ULID>);
	}
} // namespace brk::hash::ut
