namespace brk::core::ut
{
	void EventTests();
} // namespace brk::core::ut

namespace brk::ulid::ut
{
	void ULIDTests();
} // namespace brk::ulid::ut


int main(int argc, char const *argv[])
{
	brk::core::ut::EventTests();
	brk::ulid::ut::ULIDTests();
	return 0;
}
