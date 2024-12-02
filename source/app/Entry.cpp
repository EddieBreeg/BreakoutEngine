#include "App.hpp"
#include "Entry.hpp"

int main(int argc, const char* argv[])
{
	brk::App& app = brk::App::Init(brk::CreateEntryPoint(), argc, argv);
	app.Run();
}