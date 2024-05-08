namespace brk::loading::ut {
	void ComponentInfoTests();
} // namespace brk::loading::ut

namespace brk::scene_loading::ut {
	void Tests();
} // namespace brk::loading::ut

int main(int argc, char const* argv[])
{
	brk::loading::ut::ComponentInfoTests();
	brk::scene_loading::ut::Tests();
	return 0;
}
