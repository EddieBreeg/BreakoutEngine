#pragma once

#include <PCH.hpp>
#include <memory_resource>

namespace brk {
	class BRK_CORE_API AllocTracker : public std::pmr::memory_resource
	{
	public:
		AllocTracker() = default;
		AllocTracker(std::pmr::memory_resource& upstream);

		AllocTracker(const AllocTracker&) = delete;
		AllocTracker(AllocTracker&& other);

		AllocTracker& AllocTracker::operator=(const AllocTracker&) = delete;
		AllocTracker& AllocTracker::operator=(AllocTracker&& other);

		~AllocTracker() = default;

		struct Info
		{
			uint64 m_NumAllocs = 0;
			uint64 m_TotalSize = 0;
		};

		[[nodiscard]] const Info& GetInfo() const noexcept { return m_Info; }

	protected:
		bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override
		{
			return this == &other;
		}

		void* do_allocate(size_t n, size_t alignment) override;
		void do_deallocate(void* ptr, size_t n, size_t alignment) override;

	private:
		std::pmr::memory_resource* m_Upstream = std::pmr::new_delete_resource();
		Info m_Info;
	};
} // namespace brk
