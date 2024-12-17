#include <core/Bitset.hpp>

namespace brk::bitset::ut {
	void Tests()
	{
		{
			const BitsetView v;
			assert(v.GetSize() == 0);
			assert(v.GetPtr() == nullptr);
		}
		{
			byte val = 0xfe;
			const BitsetView v{ &val, 8 };
			assert(v.GetPtr() == &val);
			assert(v.GetSize() == 8);
			assert(v[0] == false);
			assert(v[1] == true);
		}
		{
			byte val = 0;
			BitsetView v{ &val, 8 };
			v.Set(0);
			assert(val == 1);
			v.Set(0, 7);
			assert(val == 0x7f);
			assert(v.Find(true) == 0);
			assert(v.Find(false) == 7);
			v.Set(7);
			assert(v.Find(false) == BitsetView::NPos);
		}
		{
			byte val = 0b11111001;
			const BitsetView v{ &val, 8 };
			assert(v.Find(false, 2) == 1);
			assert(v.Find(false, 3) == BitsetView::NPos);
			assert(v.Find(true, 3) == 3);
		}
	}
} // namespace brk::bitset::ut
