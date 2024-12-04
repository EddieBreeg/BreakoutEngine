namespace brk::threadpool::ut {
	void Tests();
} // namespace brk::threadpool::ut

namespace brk::resource_loader::ut {
	void Tests();
}

int main(int argc, char const* argv[])
{
	brk::threadpool::ut::Tests();
	brk::resource_loader::ut::Tests();
}
