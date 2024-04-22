#include <core/App.hpp>
#include <iostream>

int main(int argc, const char** argv)
{
	auto& app = bre::App::Init(argc, argv);

	const int retCode = app.Run();

	exit(retCode);
}
