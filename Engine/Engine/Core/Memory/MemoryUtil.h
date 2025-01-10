//
// Created by liamb on 7/01/2025.
//

#pragma once

#include <stdexcept>

#include "Core/EngineExport.h"
#include "Util/Util.h"

namespace Engine::Memory {

	static constexpr u8 ALIGNMENT = 16;

	inline auto API operator""_KB(size const x) { return 1024 * x; }
	inline auto API operator""_MB(size const x) { return 1024 * 1024 * x; }
	inline auto API operator""_GB(size const x) { return 1024 * 1024 * 1024 * x; }

	inline void CheckAlignment(const u8 alignment) {
		if (const bool isValid = alignment >= 8 && alignment <= 128 && (alignment & (alignment - 1)) == 0; !isValid) {
			throw std::invalid_argument("Alignment must be a power of 2 and be in range: 8 >= alignment >= 128");
		}
	}

}