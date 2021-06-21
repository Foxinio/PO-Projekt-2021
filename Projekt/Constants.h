#pragma once

#include <chrono>

namespace Time {

	using deltaTime = std::chrono::milliseconds;

	using timePoint = std::chrono::time_point<std::chrono::high_resolution_clock, deltaTime>;

	class clock : public std::chrono::high_resolution_clock {
		using internalClock = std::chrono::high_resolution_clock;
	public:

		static inline timePoint now() {
			return std::chrono::time_point_cast<timePoint::duration>(internalClock::now());
		}

	};
}

namespace Units {

	using floor = int;

	using weight = int;

	enum class direction {
		Up,
		Down
	};
}