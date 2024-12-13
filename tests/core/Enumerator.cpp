#include <core/Utility.hpp>
#include <cassert>

namespace brk::enumerator::ut {
	void Tests()
	{
		constexpr char values[] = { 'a', 'b', 'c' };
		for (auto&& [character, index] : Enumerate(values))
		{
			assert(values[index] == character);
		}
	}
} // namespace brk::enumerator::ut