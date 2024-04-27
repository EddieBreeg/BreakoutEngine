#include "DebugBreak.hpp"

#ifdef _WIN32
#include <intrin.h>
#define DEBUGBREAK __debugbreak
#else
#include <csignal>
#define DEBUGBREAK() raise(SIGTRAP)
#endif

void brk::dbg::Break()
{
	DEBUGBREAK();
}

#undef DEBUGBREAK