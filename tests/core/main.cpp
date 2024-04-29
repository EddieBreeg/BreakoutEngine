namespace brk::core::ut
{
	void EventTests();
} // namespace brk::core::ut

namespace brk::utf8::ut
{
	void Utf8Tests();
} // namespace brk::utf8::ut

namespace brk::ulid::ut
{
	void ULIDTests();
} // namespace brk::ulid::ut


int main(int argc, char const *argv[])
{
	brk::core::ut::EventTests();
	return 0;
}
