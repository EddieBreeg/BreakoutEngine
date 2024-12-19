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

	/**
	 * An STL compliant allocator, which used an AllocTracker as its backend.
	 * This is a convenience API for use with STL containers.
	 */
	template <class T>
	struct TrackerAllocator
	{
		using value_type = T;
		using is_always_equal = std::false_type;
		using propagate_on_container_move_assignment = std::true_type;

		template <class U>
		struct rebind
		{
			using other = TrackerAllocator<U>;
		};

		explicit TrackerAllocator(AllocTracker& upstream) noexcept
			: m_Upstream{ &upstream }
		{}

		template <class U>
		TrackerAllocator(const TrackerAllocator<U>& other) noexcept
			: m_Upstream{ other.m_Upstream }
		{}

		TrackerAllocator(const TrackerAllocator&) noexcept = default;
		TrackerAllocator& operator=(const TrackerAllocator&) noexcept = default;
		~TrackerAllocator() = default;

		[[nodiscard]] bool operator==(const TrackerAllocator& other) const noexcept
		{
			return m_Upstream == other.m_Upstream;
		}
		[[nodiscard]] bool operator!=(const TrackerAllocator& other) const noexcept
		{
			return m_Upstream != other.m_Upstream;
		}

		T* allocate(size_t n)
		{
			return static_cast<T*>(m_Upstream->allocate(n * sizeof(T), alignof(T)));
		}
		void deallocate(T* ptr, size_t n)
		{
			m_Upstream->deallocate(ptr, n * sizeof(T), alignof(T));
		}

		template <class U>
		friend struct TrackerAllocator;

	private:
		AllocTracker* m_Upstream;
	};
} // namespace brk
