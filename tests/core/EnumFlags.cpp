#include <core/EnumFlags.hpp>

namespace brk::enums::ut {
	enum class E0 : uint8
	{
		None = 0,
		F1 = BIT(0),
		F2 = BIT(1),
		F3 = BIT(2),
	};

	void Tests()
	{
		{
			constexpr EnumFlags<E0> flags;
			static_assert(flags.Get() == E0::None);
			static_assert(!flags.HasAny(E0::F1));
			static_assert(!flags.HasAny(E0::F2));

			static_assert(!flags.HasAll(E0::F1));
			static_assert(!flags.HasAll(E0::F2));
			static_assert(!flags.HasAll(E0::F1 | E0::F2));
		}

		{
			constexpr EnumFlags<E0> flags{ E0::F1 | E0::F2 };

			static_assert(flags.HasAny(E0::F1));
			static_assert(flags.HasAny(E0::F2));

			static_assert(flags.HasAll(E0::F1));
			static_assert(flags.HasAll(E0::F2));

			static_assert(flags.HasAll(E0::F1 | E0::F2));
		}

		{
			constexpr EnumFlags<E0> flags = []()
			{
				EnumFlags<E0> res;
				res.Set(E0::F1);
				return res;
			}();
			static_assert(flags.HasAny(E0::F1));
			static_assert(flags.HasAny(E0::F1 | E0::F2));

			static_assert(!flags.HasAny(E0::F2));
			static_assert(!flags.HasAll(E0::F1 | E0::F2));
		}
		{
			constexpr EnumFlags flags = []()
			{
				EnumFlags<E0> flags;
				flags.SetAll();
				return flags;
			}();
			static_assert(flags.HasAll(E0::F1 | E0::F2 | E0::F3));
		}
		{
			constexpr EnumFlags flags = []()
			{
				EnumFlags<E0> flags;
				flags.SetAll();
				flags.ClearAll();
				return flags;
			}();
			static_assert(!flags.HasAny(E0::F1 | E0::F2 | E0::F3));
		}
		{
			constexpr EnumFlags flags = []()
			{
				EnumFlags<E0> flags;
				flags.SetAll();
				flags.Clear(E0::F1 | E0::F3);
				return flags;
			}();
			static_assert(flags.HasAny(E0::F2));
			static_assert(!flags.HasAny(E0::F1 | E0::F3));
		}
	}
} // namespace brk::enums::ut
