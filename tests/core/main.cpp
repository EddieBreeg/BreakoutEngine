namespace brk::core::ut {
	void EventTests();
} // namespace brk::core::ut

namespace brk::retain_ptr::ut {
	void Tests();
} // namespace brk::retain_ptr::ut

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

namespace brk::enums::ut {
	void Tests();
} // namespace brk::enums::ut

namespace brk::io::ut {
	void InputFileTests();
} // namespace brk::io::ut

int main(int argc, char const* argv[])
{
	brk::core::ut::EventTests();
	brk::ulid::ut::ULIDTests();
	brk::string_view::ut::Tests();
	brk::unique_function::ut::Tests();
	brk::signal::ut::Tests();
	brk::retain_ptr::ut::Tests();
	brk::enums::ut::Tests();
	brk::io::ut::InputFileTests();
	return 0;
}
