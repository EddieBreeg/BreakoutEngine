#include "Renderer.hpp"

brk::rdr::Renderer brk::rdr::Renderer::s_Instance;

brk::rdr::Renderer::~Renderer()
{
	Shutdown();
}
