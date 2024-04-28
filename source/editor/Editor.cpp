#include "Editor.hpp"

#ifdef BRK_DEV
#include "ui/Menubar.hpp"

void brk::Editor::ShowUI()
{
	MenuBar();
}

brk::Editor::Editor(int argc, const char** argv) {}

#endif