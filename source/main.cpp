#include <core/App.hpp>
#include <core/Event.hpp>
#include <iostream>

int main(int argc, const char** argv)
{
	auto& app = breakout::App::Init(argc, argv);

	const int retCode = app.Run();

	exit(retCode);
}
