#include <core/ULID.hpp>
#include <iostream>

int main(int argc, char const* argv[])
{
	char str[27] = { 0 };
	brk::ULID::Generate().ToChars(str);
	std::cout << str << '\n';
	return 0;
}
