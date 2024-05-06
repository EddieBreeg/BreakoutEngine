namespace brk::core::ut {
	void EventTests();
} // namespace brk::core::ut

namespace brk::ulid::ut {
	void ULIDTests();
} // namespace brk::ulid::ut

namespace brk::string_view::ut {
	void Tests();
} // namespace brk::string_view::ut

namespace brk::unique_function::ut {
	void Tests();
} // namespace brk::unique_function::ut

namespace brk::signal::ut {
	void Tests();
} // namespace brk::signal::ut

int main(int argc, char const* argv[])
{
	brk::core::ut::EventTests();
	brk::ulid::ut::ULIDTests();
	brk::string_view::ut::Tests();
	brk::unique_function::ut::Tests();
	brk::signal::ut::Tests();
	return 0;
}
