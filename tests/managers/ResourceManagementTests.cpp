#include <managers/ResourceManager.hpp>
#include <cassert>

namespace brk::resource_ref::ut
{
	struct Res1 : public Resource
	{
		int& count;
		int value = 0;

		Res1(int& n)
			: Resource{ {}, "test", "" }
			, count{ ++n }
		{}
		~Res1() { --count; }
	};

	class RAIIHelper
	{
	public:
		RAIIHelper() { ResourceManager::Init(); }
		~RAIIHelper() { ResourceManager::Reset(); }

	private:
	};

	void ResourceRefTests()
	{
		RAIIHelper helper;
		int n = 0;
		{
			ResourceRef<const Res1> ref{ *(new Res1{ n }) };
			assert(n == 1);
		}
		assert(n == 0);
		{
			ResourceRef<const Res1> ref1{ *(new Res1{ n }) };
			{
				ResourceRef ref2{ ref1 };
			}
			assert(n == 1);
		}
		assert(n == 0);
		{
			ResourceRef<const Res1> ref1{ *(new Res1{ n }) };
			{
				ResourceRef ref2{ std::move(ref1) };
			}
			assert(n == 0);
		}
		{
			Res1* ptr = new Res1{ n };
			ptr->value = 1;
			ResourceRef<const Res1> ref1{ *ptr };
			ResourceRef ref2{ ref1 };
			assert(ref1->value == 1);
			assert(ref2->value == 1);
		}
	}
} // namespace brk::resource_ref::ut
