namespace brk::core::ut
{
	void EventTests();
} // namespace brk::core::ut

namespace brk::ulid::ut
{
	void ULIDTests();
} // namespace brk::ulid::ut

namespace brk::string_view::ut
{
	void Tests();
} // namespace brk::string_view::ut



int main(int argc, char const *argv[])
{
	brk::core::ut::EventTests();
	brk::ulid::ut::ULIDTests();
	brk::string_view::ut::Tests();
	return 0;
}
