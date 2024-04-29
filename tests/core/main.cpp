namespace brk::core::ut
{
	void EventTests();
} // namespace brk::core::ut

namespace brk::utf8::ut
{
	void Utf8Tests();
} // namespace brk::utf8::ut

int main(int argc, char const *argv[])
{
	brk::core::ut::EventTests();
	return 0;
}
