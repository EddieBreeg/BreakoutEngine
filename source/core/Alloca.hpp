#pragma once

#include <cstddef>

#ifdef _WIN32
extern "C" void* _alloca(size_t);
#else
extern "C" void* alloca(size_t);
#define _alloca alloca
#endif

#define Alloca(type, n) static_cast<type*>(_alloca(n * sizeof(type)))