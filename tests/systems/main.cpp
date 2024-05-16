
namespace brk::ecs::ut::manager {
	void DoNothingTest();
	void AddComponentTest();
} // namespace brk::ecs::ut::manager

namespace brk::resource_loading::ut {
	void ResourceLoadingTests();
} // namespace brk::resource_loading::ut

int main(int argc, char const* argv[])
{
	brk::ecs::ut::manager::AddComponentTest();
	brk::ecs::ut::manager::DoNothingTest();
	brk::resource_loading::ut::ResourceLoadingTests();
	return 0;
}
