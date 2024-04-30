#pragma once

#include <PCH.hpp>

/** A fast and reliable PRNG, using xoshiro256ss.
 * This class is a singleton: just access the s_Instance static member to use it.
 * The instance is marked as thread_local, this generator is therefore thread safe and
 * will give you a different sequence per thread.
 * \note The initial seed can be selected by defining the RNG_INIT_SEED macro
 */
class RNG
{
public:
	using result_type = uint64;

	// The minimum value returned by the generator
	static constexpr result_type min() { return 0; }

	// The maximum value returned by the generator
	static constexpr result_type max() { return ~static_cast<result_type>(0); }

	// Produces the next value in the sequence
	result_type operator()();

	// Reseeds the generator
	void Seed(uint64 s);

	RNG(const RNG&) = delete;
	RNG& operator=(const RNG&) = delete;

	/**
	 * The global instance of the generator.
	 */
	static thread_local RNG s_Instance;

	~RNG() = default;

private:
	RNG();
	RNG(const uint64 seed);

	uint64_t m_State[4];
};