#include <core/App.hpp>
#include <iostream>

int main(int argc, const char** argv)
{
	auto& app = brk::App::Init(argc, argv);

	return app.Run();
}
