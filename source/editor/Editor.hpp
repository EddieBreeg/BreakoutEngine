#pragma once

#include <PCH.hpp>
#include <core/Singleton.hpp>

#ifdef BRK_EDITOR

namespace brk {
	class Editor : public Singleton<Editor>
	{
	public:
		~Editor() = default;

		void ShowUI();

	private:
		friend class Singleton<Editor>;
		Editor(int argc, const char** argv);
	};
} // namespace brk

#endif