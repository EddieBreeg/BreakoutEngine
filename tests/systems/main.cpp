
namespace brk::ecs::ut::manager {
	void DoNothingTest();
	void AddComponentTest();
} // namespace brk::ecs::ut::manager

int main(int argc, char const* argv[])
{
	brk::ecs::ut::manager::AddComponentTest();
	brk::ecs::ut::manager::DoNothingTest();
	return 0;
}
